#ifndef SPHERE_H
#define SPHERE_H

#include "Shape.h"

class Sphere : public Shape
{
public:
    Sphere(int p1, int p2, double p3);
    Sphere(Vector4 pp, Vector4 dd);

protected:
    void generatePoints();
    void generateTriangle();
    REAL calculateIntersecP(Vector3 &tnormal, Vector2 &tex);

};

#endif // SPHERE_H
