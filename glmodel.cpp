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
    GLdouble coord[3];

    switch (mType)
    {
        case STL_MODEL:
            minX = maxX = ((NGInterface*)object)->geometry_STL->GetPoint(1)(0);
            minY = maxY = ((NGInterface*)object)->geometry_STL->GetPoint(1)(1);
            minZ = maxZ = ((NGInterface*)object)->geometry_STL->GetPoint(1)(2);
            for (int i = 2; i <= ((NGInterface*)object)->geometry_STL->GetNP(); i++)
            {
                minX = (((NGInterface*)object)->geometry_STL->GetPoint(i)(0) < minX) ? ((NGInterface*)object)->geometry_STL->GetPoint(i)(0) : minX;
                minY = (((NGInterface*)object)->geometry_STL->GetPoint(i)(1) < minY) ? ((NGInterface*)object)->geometry_STL->GetPoint(i)(1) : minY;
                minZ = (((NGInterface*)object)->geometry_STL->GetPoint(i)(2) < minZ) ? ((NGInterface*)object)->geometry_STL->GetPoint(i)(2) : minZ;
                maxX = (((NGInterface*)object)->geometry_STL->GetPoint(i)(0) > maxX) ? ((NGInterface*)object)->geometry_STL->GetPoint(i)(0) : maxX;
                maxY = (((NGInterface*)object)->geometry_STL->GetPoint(i)(1) > maxY) ? ((NGInterface*)object)->geometry_STL->GetPoint(i)(1) : maxY;
                maxZ = (((NGInterface*)object)->geometry_STL->GetPoint(i)(2) > maxZ) ? ((NGInterface*)object)->geometry_STL->GetPoint(i)(2) : maxZ;
            }
            break;
        case MESH_MODEL:
            ((NGInterface*)object)->getMeshPoint(1,coord);
            minX = maxX = coord[0];
            minY = maxY = coord[1];
            minZ = maxZ = coord[2];
            for (int i = 2; i <= ((NGInterface*)object)->mesh->GetNP(); i++)
            {
                ((NGInterface*)object)->getMeshPoint(i,coord);
                minX = (coord[0] < minX) ? coord[0] : minX;
                minY = (coord[1] < minY) ? coord[1] : minY;
                minZ = (coord[2] < minZ) ? coord[2] : minZ;
                maxX = (coord[0] > maxX) ? coord[0] : maxX;
                maxY = (coord[1] > maxY) ? coord[1] : maxY;
                maxZ = (coord[2] > maxZ) ? coord[2] : maxZ;
            }
            break;
        case CSG_MODEL:
            for (int i = 1; i <= ((NGInterface*)object)->geometry_CSG->GetNTopLevelObjects(); i++)
            {
                const TriangleApproximation &ta = *((NGInterface*)object)->geometry_CSG->GetTriApprox(i);

                minX = maxX = minY = maxY = minZ = maxZ = 0;
                if (&ta)
                {
                    for (int j = 0; j < 3; j++)
                    {
                        minX = (ta.GetPoint(j)(0) < minX) ? ta.GetPoint(j)(0) : minX;
                        minY = (ta.GetPoint(j)(1) < minY) ? ta.GetPoint(j)(1) : minY;
                        minZ = (ta.GetPoint(j)(2) < minZ) ? ta.GetPoint(j)(2) : minZ;
                        maxX = (ta.GetPoint(j)(0) > maxX) ? ta.GetPoint(j)(0) : maxX;
                        maxY = (ta.GetPoint(j)(1) > maxY) ? ta.GetPoint(j)(1) : maxY;
                        maxZ = (ta.GetPoint(j)(2) > maxZ) ? ta.GetPoint(j)(2) : maxZ;
                    }
                }
            }
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
        case CSG_MODEL:
            createSceletonCSG();
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
void GLModelWidget::createSceletonCSG(void)
{
    GLdouble x0 = (maxX + minX)*0.5,
             y0 = (maxY + minY)*0.5,
             z0 = (maxZ + minZ)*0.5,

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
    for (int i = 1; i <= ((NGInterface*)object)->geometry_CSG->GetNTopLevelObjects(); i++)
    {
        const TriangleApproximation &ta = *((NGInterface*)object)->geometry_CSG->GetTriApprox(i);
        if (&ta)
            glVertex3d(ta.GetPoint(0)(0) - x0, ta.GetPoint(0)(1) - y0, ta.GetPoint(0)(2) - z0);
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
        case CSG_MODEL:
            createCSG();
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
void GLModelWidget::createCSG(void)
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
