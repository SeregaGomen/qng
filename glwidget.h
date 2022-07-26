#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QtOpenGLWidgets/QOpenGLWidget>

#include "imageparams.h"

typedef enum { STL_MODEL, CSG_MODEL, MESH_MODEL } ModelType;


class GLWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    GLWidget(void* p,ModelType m,QWidget* parent) : QOpenGLWidget(parent)
    {
        object = p;
        mType = m;
        isIdle = isRotate = true;
        isLeftBtn = isScale = isTranslate = false;
        initParams();
        setFocusPolicy(Qt::ClickFocus);
        buildScene();
    }
    ~GLWidget(void)
    {
        if (xList1)
            glDeleteLists(xList1, 1);
        if (xList2)
            glDeleteLists(xList2, 1);
    }
    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    void init(void)
    {
        initParams();
        update();
    }
    void init(const ImageParams &p)
    {
        params = p;
        update();
    }
    void restore(void);
    void repaint(void);
    void setImageParams(const ImageParams& r)
    {
        params = r;
    }
    void setColor(GLdouble r,GLdouble g,GLdouble b,GLdouble a = 1.0);
    bool getRotate(void)
    {
        return isRotate;
    }
    bool getScale(void)
    {
        return isScale;
    }
    bool getTranslate(void)
    {
        return isTranslate;
    }
    ImageParams& getImageParams(void)
    {
        return params;
    }

public slots:
    void setRotate(void);
    void setScale(void);
    void setTranslate(void);

signals:
    void xRotationChanged(int);
    void yRotationChanged(int);
    void zRotationChanged(int);

protected:
    ImageParams params;
    int xList1;
    int xList2;
    int xRot;
    int yRot;
    int zRot;
    bool isRotate;
    bool isScale;
    bool isTranslate;
    bool isLeftBtn;
    bool isIdle;
    bool isMouseDown;
    GLdouble scale;
    GLdouble xTransl;
    GLdouble yTransl;
    GLdouble zTransl;
    GLdouble maxX;
    GLdouble minX;
    GLdouble maxY;
    GLdouble minY;
    GLdouble maxZ;
    GLdouble minZ;
    GLdouble radius;
    void initializeGL(void);
    virtual void paintGL(void);
    void resizeGL(int,int);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void keyPressEvent(QKeyEvent *);
    void wheelEvent(QWheelEvent *);
    void setupCameraGL(int,int);
    void setupLightGL(void);
    void drawCoordinateCross(void);
    void makeMaterial(float,float,float,float);

private:
    ModelType mType;
    QPoint lastPos;
    GLdouble diffuse;
    GLdouble ambient;
    GLdouble specular;
    void setupScale(int,int);
    void setupTranslate(int,int);
    void setupRotate(int,int,int);
    void initParams(void);
    void perspective(GLdouble,GLdouble,GLdouble,GLdouble);
    void lookAt(GLdouble,GLdouble,GLdouble,GLdouble,GLdouble,GLdouble,GLdouble,GLdouble,GLdouble);
    void normalize(GLdouble*);
    void cross(GLdouble*,GLdouble*,GLdouble*);
    void makeIdentity(GLdouble*);
    void *object = NULL;
    void displayObject(void);
    void displaySceleton(void);
    void createObject(void);
    void createSceleton(void);
    void createSTL(void);
    void createCSG(void);
    void createMesh(void);
    void buildScene(void);
    void createSceletonCSG(void);
    void createSceletonMesh(void);
    void createSceletonSTL(void);
    void calcRadius(void);
    void renderText(double, double, double, QString, QColor, const QFont& = QFont("Helvetica", 8));
    inline GLint project(double, double, double, const double [16], const double [16], const int [4], double*, double*, double*);
    inline void transformPoint(double [4], const double [16], const double [4]);
};

#endif
