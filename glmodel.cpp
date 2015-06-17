#include <QApplication>
#include "glmodel.h"
#include "nglib.h"

namespace nglib
{
    #include "nglib.h"
}
using namespace nglib;
using namespace std;

/*******************************************************************/
void GLModelWidget::buildScene(void)
{
    xRot = yRot = zRot = 0;
//    radius = Ng_STL_Radius(object);

    calcRadius();
    setupCameraGL(width(),height());
    updateGL();
}
/*******************************************************************/
void GLModelWidget::calcRadius(void)
{
    Point3d pMin,
            pMax;

    switch (mType)
    {
        case STL_MODEL:
            minX = ((NGInterface*)object)->geometry_STL->GetBoundingBox().PMin()(0);
            minY = ((NGInterface*)object)->geometry_STL->GetBoundingBox().PMin()(1);
            minZ = ((NGInterface*)object)->geometry_STL->GetBoundingBox().PMin()(2);
            maxX = ((NGInterface*)object)->geometry_STL->GetBoundingBox().PMax()(0);
            minY = ((NGInterface*)object)->geometry_STL->GetBoundingBox().PMax()(1);
            maxY = ((NGInterface*)object)->geometry_STL->GetBoundingBox().PMax()(2);
            break;
        case MESH_MODEL:
            ((NGInterface*)object)->mesh->GetBox(pMin,pMax);
            minX = pMin.X();
            minY = pMin.Y();
            minZ = pMin.Z();
            maxX = pMax.X();
            minY = pMax.Y();
            maxY = pMax.Z();
            break;
        case GEO_MODEL:
            break;
    }
    radius = sqrt((maxX - minX)*(maxX - minX) + (maxY - minY)*(maxY - minY)  + (maxZ - minZ)*(maxZ - minZ));
}
/*******************************************************************/
void GLModelWidget::paintGL(void)
{
    setupCameraGL(width(),height());

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    if (params.isQuality && params.isMesh)
    {
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(1.0,1.0);
    }
    else
        glDisable(GL_POLYGON_OFFSET_FILL);
    if (params.isLight)
    {
        glDisable(GL_COLOR_MATERIAL);
        glEnable(GL_LIGHTING);
    }
    else
    {
        glDisable(GL_LIGHTING);
        glEnable(GL_COLOR_MATERIAL);
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    glScalef(scale, scale, scale);
    glTranslatef(xTransl, yTransl, zTransl);
    glRotatef(xRot/16.0, 1.0, 0.0, 0.0);
    glRotatef(yRot/16.0, 0.0, 1.0, 0.0);
    glRotatef(zRot/16.0, 0.0, 0.0, 1.0);

    if (!object)
        return;
    if (isIdle)
        displayObject();
    else
        displaySceleton();
    if (params.isCoord)
        drawCoordinateCross();
}
/*******************************************************************/
void GLModelWidget::displaySceleton(void)
{
    if (!xList2)
        createSceleton();
    glCallList(xList2);
}
/*******************************************************************/
void GLModelWidget::createSceleton(void)
{
    switch (mType)
    {
        case STL_MODEL:
            createSceletonSTL();
            break;
        case GEO_MODEL:
            createSceletonGEO();
            break;
        case MESH_MODEL:
            createSceletonMesh();
            break;
    }
}
/*******************************************************************/
void GLModelWidget::createSceletonSTL(void)
{
    GLdouble x0 = (maxX + minX)*0.5,
             y0 = (maxY + minY)*0.5,
             z0 = (maxZ + minZ)*0.5;
    int numVertex = ((NGInterface*)object)->geometry_STL->GetNP();

    xList2 = glGenLists(1);
    glNewList(xList2, GL_COMPILE);
    if (params.isLight)
    {
        glEnable (GL_COLOR_MATERIAL);
        glDisable (GL_LIGHTING);
    }
    glColor3d(0,0,0);
    glPointSize(1);
    glBegin(GL_POINTS);
    for (int i = 1; i <= numVertex; i++)
        glVertex3d(((NGInterface*)object)->geometry_STL->GetPoint(i)(0) - x0, ((NGInterface*)object)->geometry_STL->GetPoint(i)(1) - y0, ((NGInterface*)object)->geometry_STL->GetPoint(i)(2) - z0);
    glEnd();
    if (params.isLight)
    {
        glEnable(GL_LIGHTING);
        glDisable (GL_COLOR_MATERIAL);
    }
    glEndList();
}
/*******************************************************************/
void GLModelWidget::createSceletonMesh(void)
{
    GLdouble x0 = (maxX + minX)*0.5,
             y0 = (maxY + minY)*0.5,
             z0 = (maxZ + minZ)*0.5,
             coord[3];
    int numTri = ((NGInterface*)object)->mesh->GetNSE(),
        index[3];

    xList2 = glGenLists(1);
    glNewList(xList2, GL_COMPILE);
    if (params.isLight)
    {
        glEnable (GL_COLOR_MATERIAL);
        glDisable (GL_LIGHTING);
    }
    glColor3d(0,0,0);
    glPointSize(1);
    glBegin(GL_POINTS);
    for (int i = 1; i <= numTri; i++)
    {
        ((NGInterface*)object)->getMeshSurfaceElement(i,index);
        for (unsigned j = 1; j <= 3; j++)
        {
            ((NGInterface*)object)->getMeshPoint(index[j - 1],coord);
            glVertex3d(coord[0] - x0, coord[1] - y0, coord[2] - z0);
        }
    }
    glEnd();
    if (params.isLight)
    {
        glEnable(GL_LIGHTING);
        glDisable (GL_COLOR_MATERIAL);
    }
    glEndList();
}
/*******************************************************************/
void GLModelWidget::createSceletonGEO(void)
{

}
/*******************************************************************/
void GLModelWidget::displayObject(void)
{
    if (!xList1)
        createObject();
    glCallList(xList1);
}
/*******************************************************************/
void GLModelWidget::createObject(void)
{
    switch (mType)
    {
        case STL_MODEL:
            createSTL();
            break;
        case GEO_MODEL:
            createGEO();
            break;
        case MESH_MODEL:
            createMesh();
            break;
    }
}
/*******************************************************************/
void GLModelWidget::createSTL(void)
{
    GLdouble x0 = (maxX + minX)*0.5,
             y0 = (maxY + minY)*0.5,
             z0 = (maxZ + minZ)*0.5,
             coord[3];
    int numTri = ((NGInterface*)object)->geometry_STL->GetNT();

    QApplication::setOverrideCursor(Qt::BusyCursor);

    xList1 = glGenLists(1);
    glNewList (xList1, GL_COMPILE);

    setColor(0,1,0,params.alpha);
    for (int i = 1; i <= numTri; i++)
    {
        ((NGInterface*)object)->getSTLNormal(i,coord);
        glNormal3d(coord[0],coord[1],coord[2]);
        glBegin(GL_TRIANGLES);
        for (unsigned j = 1; j <= 3; j++)
        {
            ((NGInterface*)object)->getTriangleSTL(i,j,coord);
            glVertex3d(coord[0] - x0, coord[1] - y0, coord[2] - z0);
        }
        glEnd();
    }

    glEndList();
    QApplication::restoreOverrideCursor();
}
/*******************************************************************/
void GLModelWidget::createGEO(void)
{
}
/*******************************************************************/
void GLModelWidget::createMesh(void)
{
    GLdouble x0 = (maxX + minX)*0.5,
             y0 = (maxY + minY)*0.5,
             z0 = (maxZ + minZ)*0.5,
             coord[3];
    int numTri = ((NGInterface*)object)->mesh->GetNSE(),
        index[3];

    QApplication::setOverrideCursor(Qt::BusyCursor);

    xList1 = glGenLists(1);
    glNewList (xList1, GL_COMPILE);

    for (int i = 1; i <= numTri; i++)
    {
        setColor(0,1,0,params.alpha);
        ((NGInterface*)object)->getMeshSurfaceElement(i,index);
        ((NGInterface*)object)->getMeshNormal(i,coord);
        glNormal3d(coord[0],coord[1],coord[2]);
        glBegin(GL_TRIANGLES);
        for (unsigned j = 1; j <= 3; j++)
        {
            ((NGInterface*)object)->getMeshPoint(index[j - 1],coord);
            glVertex3d(coord[0] - x0, coord[1] - y0, coord[2] - z0);
        }
        glEnd();
        if (params.isMesh)
        {
            setColor(0,0,0,params.alpha);
            glBegin(GL_LINE_LOOP);
            for (unsigned j = 1; j <= 3; j++)
            {
                ((NGInterface*)object)->getMeshPoint(index[j - 1],coord);
                glVertex3d(coord[0] - x0, coord[1] - y0, coord[2] - z0);
            }
            glEnd();

        }
    }

    glEndList();
    QApplication::restoreOverrideCursor();
}
/*******************************************************************/
