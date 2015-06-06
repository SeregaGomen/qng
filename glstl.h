#ifndef GLSTL_H
#define GLSTL_H


#include "glwidget.h"



class GLSTLWidget : public GLWidget
{
    Q_OBJECT

public:
    GLSTLWidget(QWidget* parent) : GLWidget(parent)
    {
        isIdle = isRotate = true;
        isLeftBtn = isScale = isTranslate = false;
    }
    ~GLSTLWidget() {}
    void setObject(void *);
protected:
    void paintGL(void);

private:
    void *object = NULL;
    void displayObject(void);
    void displaySceleton(void);
    void createObject(void);
    void createSceleton(void);
};


#endif // GLSTL_H
