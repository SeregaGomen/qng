#include <QApplication>
#include "glmodel.h"

namespace nglib
{
    #include "nglib.h"
}
using namespace nglib;
using namespace std;

/*******************************************************************/
void GLSTLWidget::buildScene(void)
{
    xRot = yRot = zRot = 0;
//    radius = Ng_STL_Radius(object);

    minX = Ng_STL_MinX(object);
    minY = Ng_STL_MinY(object);
    minZ = Ng_STL_MinZ(object);
    maxX = Ng_STL_MaxX(object);
    minY = Ng_STL_MaxY(object);
    maxY = Ng_STL_MaxZ(object);
    radius = sqrt((maxX - minX)*(maxX - minX) + (maxY - minY)*(maxY - minY)  + (maxZ - minZ)*(maxZ - minZ));
    setupCameraGL(width(),height());
    updateGL();
}
/*******************************************************************/
void GLSTLWidget::paintGL(void)
{
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
void GLSTLWidget::displaySceleton(void)
{
    if (!xList2)
        createSceleton();
    glCallList(xList2);
}
/*******************************************************************/
void GLSTLWidget::createSceleton(void)
{

    GLfloat x0 = (maxX + minX)*0.5,
            y0 = (maxY + minY)*0.5,
            z0 = (maxZ + minZ)*0.5;
    int numVertex = Ng_STL_NP(object);

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
        glVertex3d(Ng_STL_X(object,i) - x0, Ng_STL_Y(object,i) - y0, Ng_STL_Z(object,i) - z0);
    glEnd();
    if (params.isLight)
    {
        glEnable(GL_LIGHTING);
        glDisable (GL_COLOR_MATERIAL);
    }
    glEndList();
}
/*******************************************************************/
void GLSTLWidget::displayObject(void)
{
    if (!xList1)
        createObject();
    glCallList(xList1);
}
/*******************************************************************/
void GLSTLWidget::createObject(void)
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
void GLSTLWidget::createSTL(void)
{
    GLfloat x0 = (maxX + minX)*0.5,
            y0 = (maxY + minY)*0.5,
            z0 = (maxZ + minZ)*0.5;
    int numTri = Ng_STL_NT(object);

    QApplication::setOverrideCursor(Qt::BusyCursor);

    xList1 = glGenLists(1);
    glNewList (xList1, GL_COMPILE);

    setColor(0,1,0,params.alpha);
    for (int i = 1; i <= numTri; i++)
    {
        glNormal3d(Ng_STL_Normal_X(object,i),Ng_STL_Normal_Y(object,i),Ng_STL_Normal_Z(object,i));
        glBegin(GL_TRIANGLES);
        for (unsigned j = 1; j <= 3; j++)
            glVertex3d(Ng_STL_TRI_X(object,i,j) - x0, Ng_STL_TRI_Y(object,i,j) - y0, Ng_STL_TRI_Z(object,i,j) - z0);
        glEnd();
    }

    glEndList();
    QApplication::restoreOverrideCursor();
}
/*******************************************************************/
void GLSTLWidget::createGEO(void)
{
}
/*******************************************************************/
void GLSTLWidget::createMesh(void)
{
    GLfloat x0 = (maxX + minX)*0.5,
            y0 = (maxY + minY)*0.5,
            z0 = (maxZ + minZ)*0.5;
    int numTri = Ng_STL_NT(object);

    QApplication::setOverrideCursor(Qt::BusyCursor);

    xList1 = glGenLists(1);
    glNewList (xList1, GL_COMPILE);

    setColor(0,1,0,params.alpha);
    for (int i = 1; i <= numTri; i++)
    {
        glNormal3d(Ng_STL_Normal_X(object,i),Ng_STL_Normal_Y(object,i),Ng_STL_Normal_Z(object,i));
        glBegin(GL_TRIANGLES);
        for (unsigned j = 1; j <= 3; j++)
            glVertex3d(Ng_STL_TRI_X(object,i,j) - x0, Ng_STL_TRI_Y(object,i,j) - y0, Ng_STL_TRI_Z(object,i,j) - z0);
        glEnd();
    }

    glEndList();
    QApplication::restoreOverrideCursor();
}
/*******************************************************************/