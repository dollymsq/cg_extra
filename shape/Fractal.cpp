#include "Fractal.h"

Fractal::Fractal(int p1, int p2, double p3)
    :Shape(p1, p2, p3)
{
    m_p1 = min(7, m_p1/10);
    generatePoints();
}

Fractal::~Fractal()
{
    m_pList.clear();
    m_nList.clear();
}

void Fractal::generatePoints()
{
    Vector4 up(0, 0.5, sqrt(2)/4, 1);
    Vector4 down(0, -0.5, sqrt(2)/4, 1);
    Vector4 left(-0.5, 0, -sqrt(2)/4, 1);
    Vector4 right(0.5, 0, -sqrt(2)/4, 1);

    normal1 = (right-down).cross(up-down).getNormalized();
    normal2 = (left-right).cross(up-right).getNormalized();
    normal3 = (down-left).cross(up-left).getNormalized();
    normal4 = (left-down).cross(right-down).getNormalized();

    generatePoints_helper(up, down, left, right, m_p1);



//    m_pList.push_back(up);m_pList.push_back(down);m_pList.push_back(right);
//    m_pList.push_back(up);m_pList.push_back(right);m_pList.push_back(left);
//    m_pList.push_back(up);m_pList.push_back(left);m_pList.push_back(down);
//    m_pList.push_back(left);m_pList.push_back(right);m_pList.push_back(down);

//    m_nList.push_back(normal1);m_nList.push_back(normal1);m_nList.push_back(normal1);
//    m_nList.push_back(normal2);m_nList.push_back(normal2);m_nList.push_back(normal2);
//    m_nList.push_back(normal3);m_nList.push_back(normal3);m_nList.push_back(normal3);
//    m_nList.push_back(normal4);m_nList.push_back(normal4);m_nList.push_back(normal4);

}

void Fractal::generatePoints_helper(Vector4 up, Vector4 down, Vector4 left, Vector4 right, int level)
{
    if(level == 0)
    {
        m_pList.push_back(up);m_pList.push_back(down);m_pList.push_back(right);
        m_pList.push_back(up);m_pList.push_back(right);m_pList.push_back(left);
        m_pList.push_back(up);m_pList.push_back(left);m_pList.push_back(down);
        m_pList.push_back(left);m_pList.push_back(right);m_pList.push_back(down);

        m_nList.push_back(normal1);m_nList.push_back(normal1);m_nList.push_back(normal1);
        m_nList.push_back(normal2);m_nList.push_back(normal2);m_nList.push_back(normal2);
        m_nList.push_back(normal3);m_nList.push_back(normal3);m_nList.push_back(normal3);
        m_nList.push_back(normal4);m_nList.push_back(normal4);m_nList.push_back(normal4);
    }
    else
    {
        generatePoints_helper(up, (up + down)/2, (up + left)/2, (up + right)/2, level-1);
        generatePoints_helper((up + down)/2, down, (left + down)/2, (right + down)/2, level-1);
        generatePoints_helper((up + left)/2, (left + down)/2, left, (left + right)/2, level-1);
        generatePoints_helper((up + right)/2, (right + down)/2, (right + left)/2, right, level-1);
    }

}

void Fractal::drawShape(GLfloat *vertexData)
{
    int i, j;
    for( i = 0, j = 0; i < m_pList.size(); i = i+3, j++)
    {
        it = m_pList.begin() + i;
        Vector4 v1 = *it;
        it = m_nList.begin() + i;
        Vector4 n1 = *it;

        it = m_pList.begin() + i+1;
        Vector4 v2 = *it;
        it = m_nList.begin() + i+1;
        Vector4 n2 = *it;

        it = m_pList.begin() + i+2;
        Vector4 v3 = *it;
        it = m_nList.begin() + i+2;
        Vector4 n3 = *it;

        vertexData[18*j + 0] = float(v1.x); vertexData[18*j + 1] = float(v1.y); vertexData[18*j + 2] = float(v1.z);
        vertexData[18*j + 3] = float(n1.x); vertexData[18*j + 4] = float(n1.y); vertexData[18*j + 5] = float(n1.z);
        vertexData[18*j + 6] = float(v2.x); vertexData[18*j + 7] = float(v2.y); vertexData[18*j + 8] = float(v2.z);
        vertexData[18*j + 9] = float(n2.x); vertexData[18*j + 10] = float(n2.y); vertexData[18*j + 11] = float(n2.z);
        vertexData[18*j + 12] = float(v3.x); vertexData[18*j + 13] = float(v3.y); vertexData[18*j + 14] = float(v3.z);
        vertexData[18*j + 15] = float(n3.x); vertexData[18*j + 16] = float(n3.y); vertexData[18*j + 17] = float(n3.z);
    }
}

int Fractal::getVerticesNumber()
{
    return m_pList.size();
}
