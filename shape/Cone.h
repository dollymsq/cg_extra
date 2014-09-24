#ifndef CONE_H
#define CONE_H

#include "Shape.h"

class Cone : public Shape
{
public:
    Cone(int p1, int p2, double p3);
    Cone(Vector4 pp, Vector4 dd);

protected:
    void generatePoints();
    void generateTriangle();
    REAL calculateIntersecP(Vector3 &tnormal, Vector2 &tex);

};

#endif // CONE_H
