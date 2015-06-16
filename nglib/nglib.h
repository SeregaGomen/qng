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
    int genMeshSTL(std::string);
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
