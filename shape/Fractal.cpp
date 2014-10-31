#include "Fractal.h"

Fractal::Fractal(int p1, int p2, double p3, CS123SceneMaterial m)
    :Shape(p1, p2, p3, m)
{
    m_p1 = min(7, m_p1/10);
    generatePoints();
}

void Fractal::generatePoints()
{
    Vector3 up(0, 0.5, sqrt(2)/4);
    Vector3 down(0, -0.5, sqrt(2)/4);
    Vector3 left(-0.5, 0, -sqrt(2)/4);
    Vector3 right(0.5, 0, -sqrt(2)/4);

    normal1 = glm::cross(right-down, up-down);
    normal2 = glm::cross(left-right, up-right);
    normal3 = glm::cross(down-left, up-left);
    normal4 = glm::cross(left-down, right-down);

    generatePoints_helper(up, down, left, right, m_p1);
}

void Fractal::generatePoints_helper(Vector3 up, Vector3 down, Vector3 left, Vector3 right, int level)
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
        generatePoints_helper(up, (up + down)/2.0f, (up + left)/2.0f, (up + right)/2.0f, level-1);
        generatePoints_helper((up + down)/2.0f, down, (left + down)/2.0f, (right + down)/2.0f, level-1);
        generatePoints_helper((up + left)/2.0f, (left + down)/2.0f, left, (left + right)/2.0f, level-1);
        generatePoints_helper((up + right)/2.0f, (right + down)/2.0f, (right + left)/2.0f, right, level-1);
    }

}

void Fractal::drawShape()
{
    int i, j;
    for( i = 0, j = 0; i < m_pList.size(); i = i+3, j++)
    {
        it = m_pList.begin() + i;
        Vector3 v1 = *it;
        it = m_nList.begin() + i;
        Vector3 n1 = *it;

        it = m_pList.begin() + i+1;
        Vector3 v2 = *it;
        it = m_nList.begin() + i+1;
        Vector3 n2 = *it;

        it = m_pList.begin() + i+2;
        Vector3 v3 = *it;
        it = m_nList.begin() + i+2;
        Vector3 n3 = *it;

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
