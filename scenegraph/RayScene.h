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

class KdtreeNode
{
public:
    KdtreeNode() {}
    ~KdtreeNode() {}
    bool isLeaf;
    
    // Primitives at this node
    std::vector<primitiveNmatrix*> m_tbd;
    
    // Transformation at this node
    Vector2 xRange, yRange, zRange;
    
    // Children of this node
    KdtreeNode* leftChild;
    KdtreeNode* rightChild;

    void splitNode();
    void countChild(char axis, REAL threshold, int& left, int & right);
    void addPrimitive2Node(char axis, REAL threshold, KdtreeNode *left, KdtreeNode *right);
//    void isHit(REAL &t, const Vector4 &p, const Vector4 &d); //eye and dir
    REAL calculatePlaneHit(const Vector3& n, const Vector3& p0, const Vector4 &p, const Vector4 &d);
    bool isInBoundingBox(const REAL& tmpt, const Vector4 &p, const Vector4 &d, char signal);


};

class RayScene : public Scene
{
public:
    RayScene();
    virtual ~RayScene();
    Vector4 generateRay(Vector4 eye, Vector4 filmP, glm::mat4 M);
    void trace(Vector4 eye, Vector4 dir, BGRA &canvascolor);
    void calculateIntersection(REAL &tpostmin, std::vector<primitiveNmatrix*> node_tbd, Vector4 start, Vector4 dir, Vector3& normal, CS123SceneMaterial &tmaterial, Vector2 &textureCo, int &intersectId);
    void setTextureImage();
    void builKdtree();
    void clear(KdtreeNode *node);

protected:

    Vector3 transNormalo2w(Vector3 normal,glm::mat4 M);

    CS123SceneColor rayTrace(Vector4 eye, Vector4 dir, int recursiondepth, int intersectId);
    CS123SceneColor illuminatePoint(Vector4 eye, Vector4 dir, REAL t, Vector3 n, CS123SceneMaterial matrl, Vector2 textureCo, int intersectId, int recursiondepth);
    bool isInShadow(Vector3 object, Vector3 raydir, REAL tmin, int intersectId);
    Vector3 diffuseShade(Vector3 lightray, Vector3 n, CS123SceneMaterial matrl, Vector2 textureCo);
    Vector3 specularShade(Vector3 lightray, Vector3 n, Vector3 viewray, CS123SceneMaterial matrl);
    CS123SceneColor textureShade(CS123SceneMaterial matrl, Vector2 textureCo, CS123SceneColor tcolor);
    void calculateAABB(Vector4 v, Vector2& xRange, Vector2& yRange, Vector2& zRange);


private:
    Vector4 p, d; // eye and direction in object space
    Vector4 pw, dw; // eye and direction in world space
    std::map<std::string, QImage> texImgPair;
    KdtreeNode *m_root;

    void traverseTree(KdtreeNode* node, REAL &t, const Vector4 &p, const Vector4 &d, Vector3& normal, CS123SceneMaterial& tmaterial, Vector2 &textureCo, int &intersectId);

//    int xsort_helper(const void* a, const void* b);
//    int ysort_helper(const void* a, const void* b);
//    int zsort_helper(const void* a, const void* b);
};

#endif // RAYSCENE_H
