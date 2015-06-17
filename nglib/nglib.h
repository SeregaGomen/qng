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
public:
    Mesh* mesh = NULL;
    CSGeometry* geometry_CSG = NULL;
    STLGeometry* geometry_STL = NULL;
    NGInterface(void) {}
    ~NGInterface(void)
    {
        if (mesh)
            delete mesh;
        if (geometry_STL)
            delete geometry_STL;
        if (geometry_STL)
            delete geometry_STL;
    }
    void createMesh(void);
    void getMeshPoint(int,double*);
    void getSTLNormal(int,double*);
    void getMeshNormal(int,double*);
    void getTriangleSTL(int,int,double*);
    int genMeshSTL(std::string);
    int initSTL(void);
    NG_ELEMENT_TYPE getMeshSurfaceElement(int,int*);
    STLGeometry* loadSTL(std::string data);
    CSGeometry* parseGeometry (char* data)
    {
        CSGeometry* ParseCSG (char*);

        return ParseCSG(data);
    }
//    int genMeshCSG(int perfstepsstart,int perfstepsend,const char *optstringcsg)
//    {
//        int GenerateMesh(CSGeometry&,Mesh*&,int,int,const char*);

////        return GenerateMesh(geometry_CSG,mesh,perfstepsstart,perfstepsend,optstringcsg);
//    }
};

}
//---------------------------------------------------------------------------
using namespace netgen;


#endif // NGLIB
