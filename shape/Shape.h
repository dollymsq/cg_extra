#ifndef SHAPE_H
#define SHAPE_H

#include <CS123Common.h>
#include <deque>
#include <iterator>
#include "Settings.h"
#include <qgl.h>
#include <SupportCanvas3D.h>
#include <CS123SceneData.h>
#include <QFile>

#define MAX_LIMIT 1E+37

class Shape
{
public:
    Shape(int p1, int p2, double p3);
    Shape(Vector4 pp, Vector4 dd);

    virtual ~ Shape();

    void virtual drawShape();

    REAL virtual calculateIntersecP(Vector3 &tnormal, Vector2 &tex) { return 0.0;}

    int virtual getVerticesNumber();

    GLfloat* getVertexDataPtr() { return vertexData; }
    void clearVertexData() { delete[] vertexData;}
    void virtual packVerticesintoBuffer(GLuint &m_shader);

    std::deque<Vector3> m_pList; //store points
    std::deque<Vector3> m_nList; //store normals
    std::deque<Vector2> m_cList; //store tex coordinates

    std::deque<int> m_tList;//store triangles(index of points)

    GLuint vertexBuffer;
    GLuint m_vaoID; // The vertex array object ID, which is passed to glBindVertexArray.




protected:
    int m_p1, m_p2, m_vsize;
    double m_p3;
    Vector4 p, d;
    GLfloat* vertexData;
    CS123SceneMaterial   m_material;


    std::deque<Vector3>::iterator it;
    std::deque<Vector2>::iterator itc;
    std::deque<int>::iterator iit;
    void virtual generatePoints() {}
    void virtual generateTriangle() {}
    void drawTriangle(int offset1, int offset2, int offset3, int j);
    REAL calculateCapItsP(Vector3 n, Vector3 p0);
    void getEquationRoot(REAL a, REAL b, REAL c, REAL &t1, REAL &t2);
    void checkBodyBoundary(REAL tmpt, REAL &tmin, Vector3 n, Vector3 &tnormal);
    void checkCircleCapBoundary(REAL tmpt, REAL &tmin, Vector3 n, Vector3 &tnormal);
    Vector2 tex;
//    Vector2 calculateTexCoor(Vector2 tex);
    int texImgWidth, texImgHeight;






};

#endif // SHAPE_H
