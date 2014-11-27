#ifndef RAYSCENE_H
#define RAYSCENE_H

#include "Scene.h"
#include "shape/Shape.h"
#include "shape/Cube.h"
#include "shape/Cone.h"
#include "shape/Sphere.h"
#include "shape/Cylinder.h"
#include "shape/Torus.h"
#include "shape/Fractal.h"

#include "filter/Filter.h"
#include "KdtreeNode.h"

/**
 * @class RayScene
 *
 *  Students will implement this class as necessary in the Ray project.
 */

class RayScene : public Scene
{
public:
    RayScene();
    virtual ~RayScene();
    Vector4 generateRay(Vector4 eye, Vector4 filmP, glm::mat4 M);
    void builKdtree();
    void clear(KdtreeNode *node);
    void setTextureImage();
    KdtreeNode *m_root;
    std::map<std::string, QImage> texImgPair;

protected:
    void calculateAABB(Vector4 v, Vector2& xRange, Vector2& yRange, Vector2& zRange);

private:
    Vector4 p, d; // eye and direction in object space
    Vector4 pw, dw; // eye and direction in world space
};

#endif // RAYSCENE_H
