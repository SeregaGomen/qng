#include <strstream>
#include "nglib.h"

MeshingParameters mparam;

void NGInterface::createMesh(void)
{
    if (mesh)
        delete mesh;
    mesh = new Mesh();
    mesh->AddFaceDescriptor (FaceDescriptor (1, 1, 0, 1));
}

STLGeometry* NGInterface::loadSTL(std::string data)
{
    strstream in;
    STLGeometry geom;

    in << data;
    if (geometry_STL)
        delete geometry_STL;
    geometry_STL = geom.Load(in);
    return geometry_STL;
}

int NGInterface::genMeshSTL(string data)
{
    int ng_res = 0;

    createMesh();
    if (!loadSTL(data))
    {
        cout << "Error reading in current STL data" << endl;
        return ng_res;
    }
    cout << "Successfully loaded STL data" << endl;


    // Set the Meshing Parameters to be used
    mparam.maxh = 1.0e+6;
//    mparam.fineness = 0.4;
//    mparam.second_order = 0;

    cout << "Initialise the STL Geometry structure...." << endl;
    if (!initSTL())
    {
        cout << "Error Initialising the STL Geometry....Aborting!!" << endl;
        return 0;
    }

    cout << "Start Edge Meshing...." << endl;
    if (!makeEdgesSTL())
    {
        cout << "Error in Edge Meshing....Aborting!!" << endl;
        return 0;
    }

    cout << "Start Surface Meshing...." << endl;
    if (!generateSurfaceMeshSTL())
    {
        cout << "Error in Surface Meshing....Aborting!!" << endl;
        return 0;
    }

    cout << "Start Volume Meshing...." << endl;
    if(!generateVolumeMesh())
    {
        cout << "Error in Volume Meshing....Aborting!!" << endl;
        return 0;
    }

    cout << "Meshing successfully completed....!!" << endl;

    // volume mesh output
    cout << "Points: " << mesh->GetNP() << endl;
    cout << "Elements: " << mesh->GetNE() << endl;
    cout << "Saving Mesh in VOL Format...." << endl;


    // refinement without geomety adaption:
    // Ng_Uniform_Refinement (mesh);

    // refinement with geomety adaption:
    geometry_STL->GetRefinement().Refine (*mesh );
    cout << "elements after refinement: " << mesh->GetNP() << endl;
    cout << "points   after refinement: " << mesh->GetNE() << endl;
    return 1;
}

int NGInterface::initSTL(void)
{
   geometry_STL->InitSTLGeometry(readtrias);
   readtrias.SetSize(0);

   if (readedges.Size() != 0)
      geometry_STL->AddEdges(readedges);

   if (geometry_STL->GetStatus() == STLTopology::STL_GOOD || geometry_STL->GetStatus() == STLTopology::STL_WARNING)
       return 1;
   return 0;
}

NG_ELEMENT_TYPE NGInterface::getMeshSurfaceElement(int num, int * pi)
{
    const Element2d &el = mesh->SurfaceElement(num);
    NG_ELEMENT_TYPE et;

    for (int i = 1; i <= el.GetNP(); i++)
        pi[i-1] = el.PNum(i);
    switch (el.GetNP())
    {
        case 3:
            et = NG_TRIG;
            break;
        case 4:
            et = NG_QUAD;
            break;
        case 6:
            switch (el.GetNV())
            {
                case 3:
                    et = NG_TRIG6;
                    break;
                case 4:
                    et = NG_QUAD6;
                    break;
                default:
                    et = NG_TRIG6;
                    break;
            }
            break;
//        case 8:
//            et = NG_QUAD8;
//            break;
        default:
            et = NG_TRIG;
            break; // for the compiler
        }
    return et;
}

void NGInterface::getMeshPoint(int i, double *x)
{
    const Point3d &p = mesh->Point(i);

    x[0] = p.X();
    x[1] = p.Y();
    x[2] = p.Z();
}

void NGInterface::getSTLNormal(int i, double *x)
{
    const Vec3d &n = geometry_STL->GetTriangle(i).Normal();

    x[0] = n.X();
    x[1] = n.Y();
    x[2] = n.Z();
}

void NGInterface::getTriangleSTL(int i,int j,double *x)
{
    const Point3d &tp = geometry_STL->GetPoint((geometry_STL)->GetTriangle(i).PNum(j));

    x[0] = tp.X();
    x[1] = tp.Y();
    x[2] = tp.Z();
}

void NGInterface::getMeshNormal(int i,double* x)
{
    Element2d &face = ((Mesh*)mesh)->SurfaceElement(i);
    const Point3d &lp1 = ((Mesh*)mesh)->Point (face.PNum(1)),
            &lp2 = ((Mesh*)mesh)->Point (face.PNum(2)),
            &lp3 = ((Mesh*)mesh)->Point (face.PNum(3));
    Vec3d n = Cross (Vec3d (lp1, lp2), Vec3d (lp1, lp3));

    n /= (n.Length()+1e-12);
    x[0] = n.X();
    x[1] = n.Y();
    x[2] = n.Z();
}

int NGInterface::makeEdgesSTL(void)
{
    mesh->SetGlobalH (mparam.maxh);
    mesh->SetLocalH (geometry_STL->GetBoundingBox().PMin() - Vec3d(10, 10, 10),
                     geometry_STL->GetBoundingBox().PMax() + Vec3d(10, 10, 10),
                     0.3);

    STLMeshing (*geometry_STL, *mesh);
    geometry_STL->edgesfound = 1;
    geometry_STL->surfacemeshed = 0;
    geometry_STL->surfaceoptimized = 0;
    geometry_STL->volumemeshed = 0;
    return 1;
}

int NGInterface::generateSurfaceMeshSTL(void)
{
    int retval = STLSurfaceMeshing(*geometry_STL, *mesh);

    if (retval == MESHING3_OK)
    {
        (*mycout) << "Success !!!!" << endl;
        geometry_STL->surfacemeshed = 1;
        geometry_STL->surfaceoptimized = 0;
        geometry_STL->volumemeshed = 0;
    }
    else if (retval == MESHING3_OUTERSTEPSEXCEEDED)
    {
        (*mycout) << "ERROR: Give up because of too many trials. Meshing aborted!" << endl;
    }
    else if (retval == MESHING3_TERMINATE)
    {
        (*mycout) << "Meshing Stopped!" << endl;
    }
    else
    {
        (*mycout) << "ERROR: Surface meshing not successful. Meshing aborted!" << endl;
    }
    STLSurfaceOptimization (*geometry_STL, *mesh, mparam);
    return 1;
}

int NGInterface::generateVolumeMesh(void)
{
    mesh->CalcLocalH(mparam.grading);

    MeshVolume (mparam, *mesh);
    RemoveIllegalElements (*mesh);
    OptimizeVolume (mparam, *mesh);
    return 1;
}

