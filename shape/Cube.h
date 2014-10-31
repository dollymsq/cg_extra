#ifndef CUBE_H
#define CUBE_H

#include "Shape.h"

class Cube : public Shape
{
public:
    Cube(int p1, int p2, double p3, CS123SceneMaterial m);

protected:
    void generatePoints();
    void generateTriangle();
    REAL calculateIntersecP(Vector3 &tnormal, Vector2 &tex);
    void checkQuadCapBoundary(REAL tmpt, REAL &tmin, Vector3 n, Vector3 &tnormal, char signal);

};

#endif // CUBE_H
