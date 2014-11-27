#ifndef KDTREENODE_H
#define KDTREENODE_H
#include <CS123Common.h>
#include "CS123SceneData.h"
#include "Scene.h"

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

    void splitNode(int depth);
    void countChild(char axis, REAL threshold, int& left, int & right);
    void addPrimitive2Node(char axis, REAL threshold, KdtreeNode *left, KdtreeNode *right);
    REAL calculatePlaneHit(const Vector3& n, const Vector3& p0, const Vector4 &p, const Vector4 &d);
    bool isInBoundingBox(const REAL& tmpt, const Vector4 &p, const Vector4 &d, char signal);

};

#endif // KDTREENODE_H
