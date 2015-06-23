#ifndef NGLIB
#define NGLIB

#include "csg.hpp"
#include "geometry2d.hpp"
#include "stlgeom.hpp"
#include "nginterface.h"
//---------------------------------------------------------------------------
namespace netgen
{

class NGInterface
{
private:
    Array<STLReadTriangle> readtrias;
    Array<Point<3> > readedges;
    int makeEdgesSTL(void);
    int generateSurfaceMeshSTL(void);
    int generateVolumeMesh(void);
    void addTriangleSTL(double*,double*, double*,double*);
    shared_ptr<Mesh> s_ptr;
public:
    Mesh* mesh = NULL;
    CSGeometry* geometry_CSG = NULL;
    STLGeometry* geometry_STL = NULL;
    NGInterface(void)
    {
        mesh = new Mesh();
        mesh->AddFaceDescriptor(FaceDescriptor(1, 1, 0, 1));
    }
    ~NGInterface(void)
    {
        if (geometry_STL)
        {
            delete geometry_STL;
            delete mesh;
        }
        if (geometry_CSG)
            delete geometry_CSG;
    }
    void getMeshPoint(int,double*);
    void getSTLNormal(int,double*);
    void getMeshNormal(int,double*);
    void getTriangleSTL(int,int,double*);
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
