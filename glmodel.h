#ifndef GLMODEL_H
#define GLMODEL_H

#include "glwidget.h"

typedef enum { STL_MODEL, GEO_MODEL, MESH_MODEL } ModelType;

class GLSTLWidget : public GLWidget
{
    Q_OBJECT

public:
    GLSTLWidget(void** p,ModelType m,QWidget* parent) : GLWidget(parent)
    {
        object = p;
        mType = m;
        isIdle = isRotate = true;
        isLeftBtn = isScale = isTranslate = false;
        buildScene();
    }
    ~GLSTLWidget() {}
protected:
    void paintGL(void);

private:
    ModelType mType;
    void **object = NULL;
    void displayObject(void);
    void displaySceleton(void);
    void createObject(void);
    void createSceleton(void);
    void createSTL(void);
    void createGEO(void);
    void createMesh(void);
    void buildScene(void);
};


#endif // GLMODEL_H
