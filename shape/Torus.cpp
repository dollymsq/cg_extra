#include "Torus.h"

Torus::Torus(int p1, int p2, double p3)
    :Shape(p1, p2, p3)
{
    m_p1 = max(3, m_p1);
    m_p2 = max(3, m_p2);
    m_p3 = max(0.01, m_p3/100);
    generatePoints();
    generateTriangle();
}

Torus::Torus(Vector4 pp, Vector4 dd)
    :Shape(pp, dd)
{
}

void Torus::generatePoints()
{
    float delta_u = 1.0 / (float)m_p1;
    float delta_v = 1.0 / (float)m_p2;
    float u, v, x, y, z, R, r;
    R = 0.5 * (1 - m_p3);
    r = 0.5 * m_p3;

    int i, j;

    //body of torus
    m_pList.push_back(Vector3(0, -0.5, 0));
    m_nList.push_back(Vector3(0, -1, 0));

    for(i = 0; i< m_p1; i++)
    {
        u = delta_u * i;
        for(j = 0; j < m_p2; j++)
        {
            v = delta_v * j;
            x = (R +  r * cos(2*M_PI*v)) * cos(2*M_PI*u);
            y = (R +  r * cos(2*M_PI*v)) * sin(2*M_PI*u);
            z = r * sin(2 * M_PI*v);
            m_pList.push_back(Vector3(x, y, z));
            m_nList.push_back(Vector3(cos(2*M_PI*v) * cos(2*M_PI*u), cos(2*M_PI*v) * sin(2*M_PI*u), sin(2 * M_PI*v)));
            {
                tex.x = u;
                tex.y = v;
                m_cList.push_back(tex);
            }
        }
    }
}

void Torus::generateTriangle()
{
    //body of torus
    int i, j;
    for(i = 0; i < m_p1-1; i++)
    {
        for(j = 0; j < m_p2-1 ; j ++)
        {
            m_tList.push_back(i*m_p2+j+2);
            m_tList.push_back(i*m_p2+j+1);
            m_tList.push_back(i*m_p2+j+m_p2+1);

            m_tList.push_back((i+1)*m_p2+j+1);
            m_tList.push_back((i+1)*m_p2+j+2);
            m_tList.push_back(i*m_p2+j+2);
        }
        m_tList.push_back(i*m_p2+1);
        m_tList.push_back(i*m_p2+1+j);
        m_tList.push_back(i*m_p2+j+m_p2+1);

        m_tList.push_back((i+1)*m_p2+1+j);
        m_tList.push_back((i+1)*m_p2+1);
        m_tList.push_back(i*m_p2+1);
    }

    //head and tail
    for(i = 0; i < m_p2-1; i++)
    {
        m_tList.push_back((m_p1-1)*m_p2 + 2+i);
        m_tList.push_back((m_p1-1)*m_p2 + 1+i);
        m_tList.push_back( i + 1);

        m_tList.push_back(i+1);
        m_tList.push_back(i+2);
        m_tList.push_back((m_p1-1)*m_p2+i+2);
    }
    m_tList.push_back((m_p1-1)*m_p2+1);
    m_tList.push_back((m_p1-1)*m_p2+1+i);
    m_tList.push_back(i+1);

    m_tList.push_back(i+1);
    m_tList.push_back(1);
    m_tList.push_back((m_p1-1)*m_p2+1);
}
