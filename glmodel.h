#ifndef GLMODEL_H
#define GLMODEL_H

#include "glwidget.h"

typedef enum { STL_MODEL, GEO_MODEL, MESH_MODEL } ModelType;

class GLModelWidget : public GLWidget
{
    Q_OBJECT

public:
    GLModelWidget(void** p,ModelType m,QWidget* parent) : GLWidget(parent)
    {
        object = p;
        mType = m;
        isIdle = isRotate = true;
        isLeftBtn = isScale = isTranslate = false;
        buildScene();
    }
    ~GLModelWidget() {}
    void setObject(void** p, ModelType m)
    {
        object = p;
        mType = m;
        if (xList1)
        {
            glDeleteLists(xList1, 1);
            xList1 = 0;
        }
        if (xList2)
        {
            glDeleteLists(xList2, 1);
            xList2 = 0;
        }
        buildScene();
    }

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
    void createSceletonGEO(void);
    void createSceletonMesh(void);
    void createSceletonSTL(void);
    void calcRadius(void);
};


#endif // GLMODEL_H
