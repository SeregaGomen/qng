#include <strstream>
#include "nglib.h"

MeshingParameters mparam;

int NGInterface::genMeshSTL(string data)
{
    strstream inp;
    STLGeometry geom;
    int ng_res = 0;

    inp << data;
    if (mesh)
        delete mesh;
    if (geometry_STL)
        delete geometry_STL;
    // Actually create the mesh structure
    mesh = new Mesh();
    mesh->AddFaceDescriptor (FaceDescriptor (1, 1, 0, 1));
    geometry_STL = geom.Load(inp);
    if (!geometry_STL)
    {
        cout << "Error reading in current STL data" << endl;
        return ng_res;
    }
    cout << "Successfully loaded STL data" << endl;


    // Set the Meshing Parameters to be used
    mparam.maxh = 1.0e+6;
    mparam.fineness = 0.4;
    mparam.second_order = 0;

    cout << "Initialise the STL Geometry structure...." << endl;
    ng_res = Ng_STL_InitSTLGeometry(stl_geom);
    if (!ng_res)
    {
        cout << "Error Initialising the STL Geometry....Aborting!!" << endl;
        return;
    }

    cout << "Start Edge Meshing...." << endl;
    ng_res = Ng_STL_MakeEdges(stl_geom, mesh, &mp);
    if (!ng_res)
    {
        cout << "Error in Edge Meshing....Aborting!!" << endl;
        return;
    }

    cout << "Start Surface Meshing...." << endl;
    ng_res = Ng_STL_GenerateSurfaceMesh(stl_geom, mesh, &mp);
    if (!ng_res)
    {
        cout << "Error in Surface Meshing....Aborting!!" << endl;
        return;
    }

    cout << "Start Volume Meshing...." << endl;
    ng_res = Ng_GenerateVolumeMesh (mesh, &mp);
    if(!ng_res)
    {
        cout << "Error in Volume Meshing....Aborting!!" << endl;
        return;
    }

    cout << "Meshing successfully completed....!!" << endl;

    // volume mesh output
    cout << "Points: " << mesh->GetNP() << endl;

    cout << "Elements: " << mesh->GetNE() << endl;

    cout << "Saving Mesh in VOL Format...." << endl;


    // refinement without geomety adaption:
    // Ng_Uniform_Refinement (mesh);

    // refinement with geomety adaption:
    Ng_STL_Uniform_Refinement (stl_geom, mesh);

    cout << "elements after refinement: " << mesh->GetNP() << endl;
    cout << "points   after refinement: " << mesh->GetNE() << endl;
    return 1;
}
