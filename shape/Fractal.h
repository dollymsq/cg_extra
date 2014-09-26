#ifndef FRACTAL_H
#define FRACTAL_H
#include "shape/Shape.h"

class Fractal : public Shape
{
public:
    Fractal(int p1, int p2, double p3);

protected:
    void generatePoints();
    void generatePoints_helper(Vector3 up, Vector3 down, Vector3 left, Vector3 right, int level);

    void drawShape();
    int getVerticesNumber();

    Vector3 normal1, normal2, normal3, normal4;
};

#endif // FRACTAL_H
