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
    std::vector<int> m_tbdId;

    // Transformation at this node
    Vector2 xRange, yRange, zRange;

    // Children of this node
    KdtreeNode* leftChild;
    KdtreeNode* rightChild;

    REAL calculatePlaneHit(const Vector3& n, const Vector3& p0, const Vector4 &p, const Vector4 &d);
    bool isInBoundingBox(const REAL& tmpt, const Vector4 &p, const Vector4 &d, char signal);

};

#endif // KDTREENODE_H
