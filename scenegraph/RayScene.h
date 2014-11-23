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

/**
 * @class RayScene
 *
 *  Students will implement this class as necessary in the Ray project.
 */
#define RECURSION_LIMIT 2

class RayScene : public Scene
{
public:
    RayScene();
    virtual ~RayScene();
    Vector4 generateRay(Vector4 eye, Vector4 filmP, glm::mat4 M);
    void trace(Vector4 eye, Vector4 dir, BGRA &canvascolor);
    REAL calculateIntersection(Vector4 start, Vector4 dir, Vector3& normal, CS123SceneMaterial &tmaterial, Vector2 &textureCo, int &intersectId);
    void setTextureImage();

protected:

    Vector3 transNormalo2w(Vector3 normal,glm::mat4 M);

    CS123SceneColor rayTrace(Vector4 eye, Vector4 dir, int recursiondepth);
    CS123SceneColor illuminatePoint(Vector4 eye, Vector4 dir, REAL t, Vector3 n, CS123SceneMaterial matrl, Vector2 textureCo, int intersectId, int recursiondepth);
    bool isInShadow(Vector3 object, Vector3 raydir, REAL tmin, int intersectId);
    Vector3 diffuseShade(Vector3 lightray, Vector3 n, CS123SceneMaterial matrl, Vector2 textureCo);
    Vector3 specularShade(Vector3 lightray, Vector3 n, Vector3 viewray, CS123SceneMaterial matrl);
    CS123SceneColor textureShade(CS123SceneMaterial matrl, Vector2 textureCo, CS123SceneColor tcolor);

private:
    Vector4 p, d; // eye and direction in object space
    Vector4 pw, dw; // eye and direction in world space
    std::map<std::string, QImage> texImgPair;
};

#endif // RAYSCENE_H
