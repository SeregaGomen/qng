#ifndef GLSTL_H
#define GLSTL_H


#include "glwidget.h"

class GLSTLWidget : public GLWidget
{
    Q_OBJECT

public:
    GLSTLWidget(void** p,QWidget* parent) : GLWidget(parent)
    {
        object = p;
        isIdle = isRotate = true;
        isLeftBtn = isScale = isTranslate = false;
        buildScene();
    }
    ~GLSTLWidget() {}
protected:
    void paintGL(void);

private:
    void **object = NULL;
    void displayObject(void);
    void displaySceleton(void);
    void createObject(void);
    void createSceleton(void);
    void buildScene(void);
};


#endif // GLSTL_H
