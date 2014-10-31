#ifndef CONE_H
#define CONE_H

#include "Shape.h"

class Cone : public Shape
{
public:
    Cone(int p1, int p2, double p3, CS123SceneMaterial m);

protected:
    void generatePoints();
    void generateTriangle();
    REAL calculateIntersecP(Vector3 &tnormal, Vector2 &tex);

};

#endif // CONE_H
