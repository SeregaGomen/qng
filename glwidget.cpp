#include <QtGui>
#include <QtOpenGL>
#include <QWheelEvent>
#include <math.h>
#include "glwidget.h"

/*******************************************************************/
GLWidget::GLWidget(QWidget *parent) : QGLWidget(QGLFormat(QGL::DoubleBuffer), parent)
{
    initParams();
    setFocusPolicy(Qt::ClickFocus);
}
/*******************************************************************/
GLWidget::~GLWidget(void)
{
    if (xList1)
        glDeleteLists(xList1, 1);
    if (xList2)
        glDeleteLists(xList2, 1);
}
/*******************************************************************/
void GLWidget::initParams(void)
{
    params.init();

    xRot = yRot = zRot = minX = maxX = minY = maxY = minZ = maxZ = xList1 = xList2 = radius = 0;

    diffuse = ambient = 0.8;
    specular = 0.6;

    xTransl = yTransl = zTransl = 0.0;
    scale = 1.0;

    isLeftBtn = isRotate = true;
    isMouseDown = isScale = isTranslate = false;

}
/*******************************************************************/
void GLWidget::restore(void)
{
    xRot = yRot = zRot = 0;

    diffuse = ambient = 0.8;
    specular = 0.6;

    xTransl = yTransl = zTransl = 0.0;
    scale = 1.0;

    updateGL();
}
/*******************************************************************/
void GLWidget::repaint(void)
{
    qglClearColor(params.bkgColor);
    if (xList1)
        glDeleteLists(xList1, 1);
    if (xList2)
        glDeleteLists(xList2, 1);
    xList1 = xList2 = 0;
    updateGL();
}
/*******************************************************************/
QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}
/*******************************************************************/
QSize GLWidget::sizeHint() const
{
    return QSize(400, 400);
}
/*******************************************************************/
void GLWidget::setRotate(void)
{
    isRotate = true;
    isScale = isTranslate = false;
}
/*******************************************************************/
void GLWidget::setScale(void)
{
    isScale = true;
    isRotate = isTranslate = false;
}
/*******************************************************************/
void GLWidget::setTranslate(void)
{
    isTranslate = true;
    isScale = isRotate = false;
}
/*******************************************************************/
/*
static void qNormalizeAngle(int& angle)
{
    while (angle < 0)
        angle += 360*16;
    while (angle > 360*16)
        angle -= 360*16;
}*/
/*******************************************************************/
void GLWidget::initializeGL()
{
    qglClearColor(params.bkgColor);

    glEnable(GL_DEPTH_TEST);
    glShadeModel (GL_SMOOTH);
    glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
    glEnable (GL_NORMALIZE);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
/*******************************************************************/
void GLWidget::setupLightGL(void)
{
    GLfloat vals[3],
            lightdiff = 0.7f,
            lightspec = 1.0f,
            mat_spec_col[] = { 1, 1, 1, 1 };


    vals[0] = vals[1] = vals[2] = 0.3f;
    glLightfv(GL_LIGHT0, GL_AMBIENT, vals);

    vals[0] = vals[1] = vals[2] = lightdiff;
    glLightfv(GL_LIGHT0, GL_DIFFUSE, vals);
    vals[0] = vals[1] = vals[2] = lightspec;
    glLightfv(GL_LIGHT0, GL_SPECULAR, vals);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 50);
    glMaterialfv (GL_FRONT_AND_BACK, GL_SPECULAR, mat_spec_col);

    glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    glEnable (GL_LIGHTING);
    glEnable (GL_LIGHT0);
}
/*******************************************************************/
void GLWidget::resizeGL(int width, int height)
{
    setupCameraGL(width, height);
    if (params.isLight)
        setupLightGL();
    else
        glEnable(GL_COLOR_MATERIAL);
}
/*******************************************************************/
void GLWidget::mousePressEvent(QMouseEvent *event)
{
    isMouseDown = true;
    isIdle = false;
    lastPos = event->pos();
}
/*******************************************************************/
void GLWidget::mouseReleaseEvent(QMouseEvent*)
{
    isMouseDown = false;
    isIdle = true;
//    repaint();
    update();
}
/*******************************************************************/
void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x(),
        dy = event->y() - lastPos.y();

    if ((event->buttons() & Qt::LeftButton) && (QApplication::keyboardModifiers().testFlag(Qt::ShiftModifier) == false))
    {
        if (isRotate)
            setupRotate(dy,dx,0);
        else if (isScale)
            setupScale(dx, dy);
        else if (isTranslate)
            setupTranslate(dx, dy);

    }
    else if ((event->buttons() & Qt::LeftButton) && (QApplication::keyboardModifiers().testFlag(Qt::ShiftModifier) == true))
        if (isRotate)
            setupRotate(dy,0,dx);
    lastPos = event->pos();
}
/*******************************************************************/
void GLWidget::setupRotate(int dx, int dy, int dz)
{
    xRot += 8*dx;
    yRot += 8*dy;
    zRot += 8*dz;
    updateGL();
}
/*******************************************************************/
void GLWidget::setupScale(int dx, int dy)
{
    if (dx > 0 || dy > 0)
        scale *= 1.05;
    else
        scale /= 1.05;
    updateGL();
}
/*******************************************************************/
void GLWidget::setupTranslate(int dx, int dy)
{
    xTransl += dx*radius*0.001;
    yTransl -= dy*radius*0.001;
    updateGL();
}
/*******************************************************************/
void GLWidget::setupCameraGL(int width, int height)
{
    double a = (height == 0) ? 1.0 : double(width)/double(height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    perspective(60.0, a, 0.01*radius, 10*radius);
    lookAt(0, 0, radius, 0, 0, 0, 0, 1, 0);
    glViewport(0, 0, width, height);
    glMatrixMode(GL_MODELVIEW);
}
/*******************************************************************/
void GLWidget::makeMaterial(float r,float g,float b,float a)
{
  GLfloat ambvals[] = { 0.4f, 0.4f, 0.4f, 1.0f };
  GLfloat diffvals[] = { 0.5f, 0.5f, 0.5f, 1.0f };
  GLfloat specvals[] =  { 0.7f, 0.7f, 0.7f, 1.0f };
  GLfloat val[] = { r, g, b, a };

  glLightfv(GL_LIGHT0, GL_AMBIENT, ambvals);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffvals);
  glLightfv(GL_LIGHT0, GL_SPECULAR, specvals);


  val[0] = r*diffuse;
  val[1] = g*diffuse;
  val[2] = b*diffuse;
  val[3] = a;
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, val);

  val[0] = r*ambient;
  val[1] = g*ambient;
  val[2] = b*ambient;
  val[3] = a;
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, val);

  val[0] = specular;
  val[1] = specular;
  val[2] = specular;
  val[3] = a;
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, val);

  val[0] = 0;
  val[1] = 0;
  val[2] = 0;
  val[3] = a;
  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, val);
}
/*******************************************************************/
void GLWidget::drawCoordinateCross(void)
{
    GLint viewport[4];

    glMatrixMode (GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glMatrixMode (GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glGetIntegerv (GL_VIEWPORT, viewport);

    glTranslatef (-1, -1, 0.0);
    glScalef (40.0/viewport[2], 40.0/viewport[3], 1);
    glTranslatef (1.25, 1.25, 0.0);

    glRotatef(xRot/16.0, 1.0, 0.0, 0.0);
    glRotatef(yRot/16.0, 0.0, 1.0, 0.0);
    glRotatef(zRot/16.0, 0.0, 0.0, 1.0);

    if (params.isLight)
    {
        glEnable (GL_COLOR_MATERIAL);
        glDisable (GL_LIGHTING);
    }

    glLineWidth (1.0f);

    glBegin(GL_LINES);
        glColor3f(1,0,0);
        glVertex3f (0.0f, 0.0f, 0.0f);
        glVertex3f (0.9f, 0.0f, 0.0f);
        glColor3f(0,1,0);
        glVertex3f (0.0f, 0.0f, 0.0f);
        glVertex3f (0.0f, 0.9f, 0.0f);
        glColor3f(0,0,1);
        glVertex3f (0.0f, 0.0f, 0.0f);
        glVertex3f (0.0f, 0.0f, 0.9f);
    glEnd ();

    glColor3f(1,0,0);
    renderText(1.1, 0.0, 0.0, (char*)"X");

    glColor3f(0,1,0);
    renderText(0.1, 1.0, 0.0, (char*)"Y");

    glColor3f(0,0,1);
    renderText(0.1, 0.0, 1.0, (char*)"Z");

    if (params.isLight)
    {
        glEnable(GL_LIGHTING);
        glDisable (GL_COLOR_MATERIAL);
    }

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

}
/*******************************************************************/
void GLWidget::setColor(GLdouble r, GLdouble g, GLdouble b, GLdouble a)
{
    if (params.isLight)
        makeMaterial(r,g,b,a);
    else
        glColor4f(r,g,b,a);
}
/*******************************************************************/
void GLWidget::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_F5)
        repaint();
}
/*******************************************************************/
void GLWidget::perspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
    GLdouble m[4][4],
             sine,
             cotangent,
             deltaZ,
             radians = fovy*0.5*M_PI/180.0;

    deltaZ = zFar - zNear;
    sine = sin(radians);
    if ((deltaZ == 0) || (sine == 0) || (aspect == 0)) return;
    cotangent = cos(radians)/sine;

    makeIdentity(&m[0][0]);
    m[0][0] = cotangent/aspect;
    m[1][1] = cotangent;
    m[2][2] = -(zFar + zNear)/deltaZ;
    m[2][3] = -1;
    m[3][2] = -2*zNear*zFar/deltaZ;
    m[3][3] = 0;
    glMultMatrixd(&m[0][0]);
}
/*******************************************************************/
void GLWidget::lookAt(GLdouble eyex,GLdouble eyey,GLdouble eyez,GLdouble centerx,GLdouble centery,GLdouble centerz,GLdouble upx,GLdouble upy,GLdouble upz)
{
    GLdouble forward[3],
             side[3],
             up[3],
             m[4][4];

    forward[0] = centerx - eyex;
    forward[1] = centery - eyey;
    forward[2] = centerz - eyez;

    up[0] = upx;
    up[1] = upy;
    up[2] = upz;

    normalize(forward);

    /* Side = forward x up */
    cross(forward, up, side);
    normalize(side);

    /* Recompute up as: up = side x forward */
    cross(side, forward, up);

    makeIdentity(&m[0][0]);
    m[0][0] = side[0];
    m[1][0] = side[1];
    m[2][0] = side[2];

    m[0][1] = up[0];
    m[1][1] = up[1];
    m[2][1] = up[2];

    m[0][2] = -forward[0];
    m[1][2] = -forward[1];
    m[2][2] = -forward[2];

    glMultMatrixd(&m[0][0]);
    glTranslated(-eyex, -eyey, -eyez);
}
/*******************************************************************/
void GLWidget::normalize(GLdouble* v)
{
    GLdouble r = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);

    if (r)
    {
        v[0] /= r;
        v[1] /= r;
        v[2] /= r;
    }
}
/*******************************************************************/
void GLWidget::cross(GLdouble* v1,GLdouble* v2,GLdouble* result)
{
    result[0] = v1[1]*v2[2] - v1[2]*v2[1];
    result[1] = v1[2]*v2[0] - v1[0]*v2[2];
    result[2] = v1[0]*v2[1] - v1[1]*v2[0];
}
/*******************************************************************/
void GLWidget::makeIdentity(GLdouble* m)
{
    m[0+4*0] = 1; m[0+4*1] = 0; m[0+4*2] = 0; m[0+4*3] = 0;
    m[1+4*0] = 0; m[1+4*1] = 1; m[1+4*2] = 0; m[1+4*3] = 0;
    m[2+4*0] = 0; m[2+4*1] = 0; m[2+4*2] = 1; m[2+4*3] = 0;
    m[3+4*0] = 0; m[3+4*1] = 0; m[3+4*2] = 0; m[3+4*3] = 1;
}
/*******************************************************************/
// Вращение колёсика мыши
void GLWidget::wheelEvent(QWheelEvent* pe)
{
    setupScale(pe->delta(), pe->delta());
    updateGL();
}
/*******************************************************************/
