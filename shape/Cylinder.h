#ifndef CYLINDER_H
#define CYLINDER_H

#include "Shape.h"

class Cylinder : public Shape
{
public:
    Cylinder(int p1, int p2, double p3, CS123SceneMaterial m);

protected:
    void generatePoints();
    void generateTriangle();
    REAL calculateIntersecP(Vector3 &tnormal, Vector2 &tex);

};

#endif // CYLINDER_H
