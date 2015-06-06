#include "glstl.h"
//#include <mystdlib.h>

//#include <myadt.hpp>
//#include <meshing.hpp>
// #include <csg.hpp>

//#include <stlgeom.hpp>


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
//    STLGeometry* stl_geom = (STLGeometry*)object;
//    GLfloat x0 = (maxX + minX)*0.5,
//            y0 = (maxY + minY)*0.5,
//            z0 = (maxZ + minZ)*0.5;

//    xList2 = glGenLists(1);
//    glNewList (xList2, GL_COMPILE);
//    if (params.isLight)
//    {
//        glEnable (GL_COLOR_MATERIAL);
//        glDisable (GL_LIGHTING);
//    }
//    glColor3d(0,0,0);
//    glPointSize(1);
//    glBegin(GL_POINTS);
//    for (unsigned i = 1; i < stl_geom->getNT(); i++)
//        for (unsigned j = 0; j < surface.size2(); j++)
//            glVertex3d(X[surface[i][j]] - x0, (Y.size()) ? Y[surface[i][j]] - y0 : 0, (Z.size()) ? Z[surface[i][j]] - z0 : 0);
//    glEnd();
//    if (params.isLight)
//    {
//        glEnable(GL_LIGHTING);
//        glDisable (GL_COLOR_MATERIAL);
//    }
//    glEndList();
}
/*******************************************************************/
void GLSTLWidget::setObject(void *o)
{
//    object = o;
//    xRot = yRot = zRot = 0;
//    minX = *min_element(object->getMesh().getX().begin(),object->getMesh().getX().end());
//    maxX = *max_element(object->getMesh().getX().begin(),object->getMesh().getX().end());
//    minY = (object->getMesh().getY().size()) ? *min_element(object->getMesh().getY().begin(),object->getMesh().getY().end()) : 0;
//    maxY = (object->getMesh().getY().size()) ? *max_element(object->getMesh().getY().begin(),object->getMesh().getY().end()) : 0;
//    minZ = (object->getMesh().getZ().size()) ? *min_element(object->getMesh().getZ().begin(),object->getMesh().getZ().end()) : 0;
//    maxZ = (object->getMesh().getZ().size()) ? *max_element(object->getMesh().getZ().begin(),object->getMesh().getZ().end()) : 0;
//    radius = sqrt((maxX - minX) * (maxX - minX) + (maxY - minY) * (maxY - minY)  + (maxZ - minZ) * (maxZ - minZ));
//    setupCameraGL(width(),height());
//    if (object->getMesh().getZ().size())
//        createNormal();
//    if (object->getMesh().getFreedom() == 1)
//        params.isMesh = true;
//    if (params.isLimit || params.isForce)
//    {
//        setSelectedVertex();
//        repaint();
//    }

//    updateGL();
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
//    if (!object->getMesh().getX().size()) return;

//    QApplication::setOverrideCursor(Qt::BusyCursor);

//    xList1 = glGenLists(1);
//    glNewList (xList1, GL_COMPILE);
//    switch (object->getMesh().getFEType())
//    {
//        case FE1D_2:
//            drawTRP_1D();
//            break;
//        case FE2D_3:
//        case FE2D_6:
//        case FE2D_4:
//            drawTRP_2D();
//            break;
//        case FE3D_4:
//        case FE3D_10:
//        case FE3D_8:
//            drawTRP_3D();
//            break;
//        default:
//            break;
//    }
//    glEndList();
//    QApplication::restoreOverrideCursor();
}
/*******************************************************************/
//void GLSTLWidget::drawTRP_3D(void)
//{
//    vector<Float> &X = object->getMesh().getX(),
//                  &Y = object->getMesh().getY(),
//                  &Z = object->getMesh().getZ();
//    matrix<unsigned>& Faces = object->getMesh().getSurface();
//    size_t NumFaces = Faces.size1(),
//        NumVertex = X.size(),
//        SizeFaces = (Faces.size2() > 4) ? 3 : Faces.size2(); // Для обработки квадратичных элементов
//    Float x0 = (maxX + minX) * 0.5,
//          y0 = (maxY + minY) * 0.5,
//          z0 = (maxZ + minZ) * 0.5;
//    GLenum Val = (SizeFaces == 3) ? GL_TRIANGLES : GL_QUADS;

//    for (unsigned i = 0; i < NumFaces; i++)
//    {
//        if (params.isFace)
//        {
//            setColor(0,1,0,params.alpha);
//            glBegin(Val);
//            glNormal3d(Normal[i][0],Normal[i][1],Normal[i][2]);
//            for (unsigned j = 0; j < SizeFaces; j++)
//                glVertex3d(X[Faces[i][j]] - x0, Y[Faces[i][j]] - y0, Z[Faces[i][j]] - z0);
//            glEnd();
//        }

//        if (params.isMesh)
//        {
//            setColor(0,0,0,params.alpha);
//            glBegin(GL_LINE_LOOP);
//            for (unsigned j = 0; j < SizeFaces; j++)
//                glVertex3d(X[Faces[i][j]] - x0, Y[Faces[i][j]] - y0, Z[Faces[i][j]] - z0);
//            glEnd();
//        }

//    }

//    if (params.isVertex)
//    {
//        setColor(0,0,0,params.alpha);
//        glPointSize(2);
//        glBegin(GL_POINTS);
//        for (unsigned i = 0; i < NumVertex; i++)
//            glVertex3d(X[i] - x0, Y[i] - y0, Z[i] - z0);
//        glEnd();
//    }
//    if (params.isLimit)
//        drawLimit();
//    if (params.isForce)
//        drawForce();
//}
/*******************************************************************/
