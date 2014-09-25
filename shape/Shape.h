#ifndef SHAPE_H
#define SHAPE_H

#include <CS123Common.h>
#include <deque>
#include <iterator>
#include "Settings.h"
#include <qgl.h>
#include <SupportCanvas3D.h>
#include "math/CS123Algebra.h"

#define MAX_LIMIT 1E+37

class Shape
{
public:
    Shape(int p1, int p2, double p3);
    Shape(Vector4 pp, Vector4 dd);

    virtual ~ Shape();

    void virtual drawShape(GLfloat *vertexData);

    REAL virtual calculateIntersecP(Vector3 &tnormal, Vector2 &tex) { return 0.0;}

    int virtual getVerticesNumber();

    std::deque<Vector4> m_pList; //store points
    std::deque<Vector4> m_nList; //store normals
    std::deque<int> m_tList;//store triangles(index of points)

protected:
    int m_p1;
    int m_p2;
    double m_p3;
    Vector4 p, d;
    std::deque<Vector4>::iterator it;
    std::deque<int>::iterator iit;
    void virtual generatePoints() {}
    void virtual generateTriangle() {}
    void drawTriangle(int offset1, int offset2, int offset3, GLfloat *vertexData, int j);
    REAL calculateCapItsP(vec3<REAL> n, vec3<REAL> p0);
    void getEquationRoot(REAL a, REAL b, REAL c, REAL &t1, REAL &t2);
    void checkBodyBoundary(REAL tmpt, REAL &tmin, Vector3 n, Vector3 &tnormal);
    void checkCircleCapBoundary(REAL tmpt, REAL &tmin, Vector3 n, Vector3 &tnormal);


};

#endif // SHAPE_H

