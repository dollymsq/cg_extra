#ifndef TORUS_H
#define TORUS_H
#include "shape/Shape.h"
class Torus : public Shape
{
public:
    Torus(int p1, int p2, double p3);
    Torus(Vector4 pp, Vector4 dd);

protected:
    void generatePoints();
    void generateTriangle();
};

#endif // TORUS_H
