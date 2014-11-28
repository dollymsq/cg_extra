#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <QThread>
#include <CS123Common.h>
#include "CS123SceneData.h"
#include "Settings.h"

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
#include "RayScene.h"


#define RECURSION_LIMIT 2

class RayTracer : public QThread
{
    Q_OBJECT
public:
    RayTracer();

    CS123SceneColor trace(Vector4 eye, Vector4 dir);
    void calculateIntersection(REAL &tpostmin, std::vector<int> node_tbd, Vector4 start, Vector4 dir, Vector3& normal, CS123SceneMaterial &tmaterial, Vector2 &textureCo, int &intersectId);
    void prepare(Vector4 e, int w, int h, glm::mat4 M, BGRA *d, RayScene *m_s, int odr, int interval);

    void setSceneLight(std::vector<CS123SceneLightData> sl)  {    m_scenelights = sl;}
    void setGlobal(const CS123SceneGlobalData &global) {      m_sceneglobalData = global; }

    void run();

private:
    Vector3 transNormalo2w(Vector3 normal,glm::mat4 M);
    CS123SceneColor rayTrace(Vector4 eye, Vector4 dir, int recursiondepth, int intersectId);
    CS123SceneColor illuminatePoint(Vector4 eye, Vector4 dir, REAL t, Vector3 n, CS123SceneMaterial matrl, Vector2 textureCo, int intersectId, int recursiondepth);
    bool isInShadow(Vector3 object, Vector3 raydir, REAL tmin, int intersectId);
    Vector3 diffuseShade(Vector3 lightray, Vector3 n, CS123SceneMaterial matrl, Vector2 textureCo);
    Vector3 specularShade(Vector3 lightray, Vector3 n, Vector3 viewray, CS123SceneMaterial matrl);
    CS123SceneColor textureShade(CS123SceneMaterial matrl, Vector2 textureCo, CS123SceneColor tcolor);
    void traverseTree(KdtreeNode* node, REAL &t, const Vector4 &p, const Vector4 &d, Vector3& normal, CS123SceneMaterial& tmaterial, Vector2 &textureCo, int &intersectId);
    Vector4 generateRay(Vector4 eye, Vector4 filmP, glm::mat4 M);
    CS123SceneColor superSample(const Vector4 &eye, const glm::mat4 &Mc2w, double xmin, double xmax, double ymin, double ymax, int depth);

    std::map<std::string, QImage> texImgPair;
    std::map<int, primitiveNmatrix> pnmDict;
    KdtreeNode *m_root;
    Vector4 eye;
    int width, height, m_order, m_interval;
    glm::mat4 Mc2w;
    BGRA *data;
    int m_intersectId;

    std::vector<CS123SceneLightData> m_scenelights;
    CS123SceneGlobalData m_sceneglobalData;
};

#endif // RAYTRACER_H

inline float randBtw(float LO, float HI)
{
    return LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
}
