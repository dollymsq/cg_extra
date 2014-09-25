#ifndef FRACTAL_H
#define FRACTAL_H
#include "shape/Shape.h"

class Fractal : public Shape
{
public:
    Fractal(int p1, int p2, double p3);

protected:
    void generatePoints();
    void generatePoints_helper(Vector4 up, Vector4 down, Vector4 left, Vector4 right, int level);

    void drawShape();
    int getVerticesNumber();

    Vector4 normal1, normal2, normal3, normal4;
};

#endif // FRACTAL_H
