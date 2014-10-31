#include "Sphere.h"

Sphere::Sphere(int p1, int p2, double p3, CS123SceneMaterial m)
    :Shape(p1, p2, p3, m)
{
    m_p1 = max(2, m_p1);
    m_p2 = max(3, m_p2);
    generatePoints();
    generateTriangle();
}

void Sphere::generatePoints()
{
    float delta_u = 1.0 / (float)m_p1;
    float delta_v = 1.0 / (float)m_p2;
    float u, v, x, y, z;

    int i, j;

    //bottom of sphere
    m_pList.push_back(Vector3(0, -0.5, 0));
    m_nList.push_back(Vector3(0, -1, 0));

    for(i = 1; i< m_p1; i++)
    {
        u = delta_u * i;
        for(j = 0; j < m_p2; j++)
        {
            v = delta_v * j;
            x = 0.5 * sin(M_PI*u) * cos(2*M_PI*v);
            y = -0.5 * cos(M_PI*u);
            z = -0.5 * sin(M_PI*u) * sin(2*M_PI*v);
            m_pList.push_back(Vector3(x, y, z));
            m_nList.push_back(Vector3(2*x, 2*y, 2*z));
        }
    }

    m_pList.push_back(Vector3(0, 0.5, 0));
    m_nList.push_back(Vector3(0, 1, 0));
}

void Sphere::generateTriangle()
{
    //the most inner circle of bottom
    int i,j;
    for(i = 1; i < m_p2 ; i ++)
    {
        m_tList.push_back(i);
        m_tList.push_back(0);
        m_tList.push_back(i+1);
    }
    m_tList.push_back(i);
    m_tList.push_back(0);
    m_tList.push_back(1);

    //body of sphere
    for(i = 0; i < m_p1-2; i++)
    {
        for(j = 0; j < m_p2-1 ; j ++)
        {
            m_tList.push_back(i*m_p2+j+1);
            m_tList.push_back(i*m_p2+j+2);
            m_tList.push_back(i*m_p2+j+m_p2+1);

            m_tList.push_back((i+1)*m_p2+j+2);
            m_tList.push_back((i+1)*m_p2+j+1);
            m_tList.push_back(i*m_p2+j+2);
        }
        m_tList.push_back(i*m_p2+j+1);
        m_tList.push_back(i*m_p2+1);
        m_tList.push_back(i*m_p2+j+m_p2+1);

        m_tList.push_back((i+1)*m_p2+1);
        m_tList.push_back((i+1)*m_p2+j+1);
        m_tList.push_back(i*m_p2+1);
    }

    //top
    for(i = 0; i < m_p2-1; i++)
    {
        m_tList.push_back((m_p1-2)*m_p2 + 1+i);
        m_tList.push_back((m_p1-2)*m_p2 + 2+i);
        m_tList.push_back((m_p1-2)*m_p2 + 1 +m_p2);
    }
    m_tList.push_back((m_p1-2)*m_p2 + 1+i);
    m_tList.push_back((m_p1-2)*m_p2 + 1);
    m_tList.push_back((m_p1-2)*m_p2 + 1 +m_p2);
}

REAL Sphere::calculateIntersecP(Vector3 &tnormal, Vector2 &tex)
{
        REAL tmin = MAX_LIMIT, t1=-1, t2=-1 ;

        //body
        REAL a = d.x*d.x + d.y*d.y + d.z*d.z;
        REAL b = 2*(p.x*d.x + p.y*d.y + p.z*d.z);
        REAL c = p.x*p.x + p.y*p.y + p.z*p.z - 0.25;

        getEquationRoot(a, b, c, t1, t2);
        if(t1 > 0 && t1 < tmin)
        {
            tmin = t1;
            tnormal.x = p.x + t1*d.x;
            tnormal.y = p.y + t1*d.y;
            tnormal.z = p.z + t1*d.z;
        }
        if(t2 > 0 && t2 < tmin)
        {
            tmin = t2;
            tnormal.x = p.x + t2*d.x;
            tnormal.y = p.y + t2*d.y;
            tnormal.z = p.z + t2*d.z;
        }

        if(settings.useTextureMapping && tmin< MAX_LIMIT)
        {
            REAL x = p.x+tmin*d.x;
            REAL y = p.y+tmin*d.y;
            REAL z = p.z+tmin*d.z;

            REAL theta = atan2(z,x);
            if(theta< 0)
                tex.x = -theta/2/M_PI;
            else
                tex.x = 1 - theta/2/M_PI;

            REAL phi = asin(y/0.5);
            tex.y = -phi/M_PI + 0.5;
        }
        return tmin;
}

