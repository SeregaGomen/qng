#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include "imageparams.h"


class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget(void);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    void init(void)
    {
        initParams();
        updateGL();
    }
    void init(const ImageParams &p)
    {
        params = p;
        updateGL();
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
    virtual void paintGL(void) = 0;
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
    QPoint lastPos;
    GLdouble diffuse;
    GLdouble ambient;
    GLdouble specular;
    void setupScale(int,int);
    void setupTranslate(int,int);
    void setupRotate(int,int,int);
    void initParams(void);
    virtual void calcSelection(int,int) = 0;
    void perspective(GLdouble,GLdouble,GLdouble,GLdouble);
    void lookAt(GLdouble,GLdouble,GLdouble,GLdouble,GLdouble,GLdouble,GLdouble,GLdouble,GLdouble);
    void normalize(GLdouble*);
    void cross(GLdouble*,GLdouble*,GLdouble*);
    void makeIdentity(GLdouble*);
};

#endif
