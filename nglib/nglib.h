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
    bool getIsCSG(void)
    {
        return isCSG;
    }
    void setIsCSG(bool p)
    {
        isCSG = p;
    }
    double getFacets(void)
    {
        return facets;
    }
    void setFacets(double p)
    {
        facets = p;
    }
    double getDetail(void)
    {
        return detail;
    }
    void setDetail(double p)
    {
        detail = p;
    }
    double getMinX(void)
    {
        return minX;
    }
    double getMaxX(void)
    {
        return maxX;
    }
    void setMinX(double p)
    {
        minX = p;
    }
    void setMaxX(double p)
    {
        maxX = p;
    }
    double getMinY(void)
    {
        return minY;
    }
    double getMaxY(void)
    {
        return maxY;
    }
    void setMinY(double p)
    {
        minY = p;
    }
    void setMaxY(double p)
    {
        maxY = p;
    }
    double getMinZ(void)
    {
        return minZ;
    }
    double getMaxZ(void)
    {
        return maxZ;
    }
    void setMinZ(double p)
    {
        minZ = p;
    }
    void setMaxZ(double p)
    {
        maxZ = p;
    }
    double getMinMeshSize(void)
    {
        return minMeshSize;
    }
    void setMinMeshSize(double p)
    {
        minMeshSize = p;
    }
    double getMaxMeshSize(void)
    {
        return maxMeshSize;
    }
    void setMaxMeshSize(double p)
    {
        maxMeshSize = p;
    }
    double getMeshSizeGrading(void)
    {
        return meshSizeGrading;
    }
    void setMeshSizeGrading(double p)
    {
        meshSizeGrading = p;
    }
    double getEpRadius(void)
    {
        return epRadius;
    }
    void setEpRadius(double p)
    {
        epRadius = p;
    }
    double getEpEdge(void)
    {
        return epEdge;
    }
    void setEpEdge(double p)
    {
        epEdge = p;
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
