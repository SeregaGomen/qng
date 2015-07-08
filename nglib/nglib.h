#ifndef NGLIB
#define NGLIB

#include "csg.hpp"
#include "stlgeom.hpp"
#include "nginterface.h"

//---------------------------------------------------------------------------
namespace netgen
{

class NGInterface
{
private:
    bool isCSG = true;
    double facets = 20;
    double detail = 0.001;
    double minX = -1000;
    double maxX = 1000;
    double minY = -1000;
    double maxY = 1000;
    double minZ = -1000;
    double maxZ = 1000;
    double minMeshSize = 0;
    double maxMeshSize = 1000;
    double meshSizeGrading = 0.3;
    double epRadius = 2.0;
    double epEdge = 1.0;
    NetgenGeometry* geometry = NULL;
    Mesh* mesh = NULL;
    Array<STLReadTriangle> readtrias;
    Array<Point<3> > readedges;
    int makeEdgesSTL(void);
    int generateSurfaceMeshSTL(void);
    int generateVolumeMesh(void);
    void addTriangleSTL(double*,double*, double*,double*);
    shared_ptr<Mesh> s_ptr;
public:
    NGInterface(void)
    {
        mesh = new Mesh();
        mesh->AddFaceDescriptor(FaceDescriptor(1, 1, 0, 1));
    }
    ~NGInterface(void)
    {
        if (dynamic_cast<STLGeometry*>(geometry))
            delete mesh;
        if (geometry)
            delete geometry;
    }
    Mesh* getMesh(void)
    {
        return mesh;
    }
    CSGeometry* getGeometryCSG(void)
    {
        return (CSGeometry*)geometry;
    }
    STLGeometry* getGeometrySTL(void)
    {
        return (STLGeometry*)geometry;
    }
    void getMeshPoint(int,double*);
    void getSTLNormal(int,double*);
    void getMeshNormal(int,double*);
    void getTriangleSTL(int,int,double*);
    void refinementMesh(void);
    void saveMesh(string);
    int genMeshSTL(string);
    int genMeshCSG(string);
    int initSTL(void);
    NG_ELEMENT_TYPE getMeshSurfaceElement(int,int*);
    STLGeometry* loadSTL(string data);
    CSGeometry* loadCSG(string data);
};

}
//---------------------------------------------------------------------------
using namespace netgen;


#endif // NGLIB
