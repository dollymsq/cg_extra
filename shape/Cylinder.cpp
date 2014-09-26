#include "Cylinder.h"

Cylinder::Cylinder(int p1, int p2, double p3)
    :Shape(p1, p2, p3)
{
    m_p2 = max(3, m_p2);
    generatePoints();
    generateTriangle();

}

void Cylinder::generatePoints()
{
    float delta_u = 1.0 / (float)m_p1;
    float delta_v = 1.0 / (float)m_p2;
    float u, v, x, y, z;

    int i, j;

    //bottom of cone, from inner to outside
    m_pList.push_back(Vector3(0.0, -0.5, 0));
    m_nList.push_back(Vector3(0, -1, 0));
    for(i = 1; i< m_p1+1; i++)
    {
        u = delta_u * i;
        for(j = 0; j < m_p2; j++)
        {
            v = delta_v * j;
            x = 0.5*u * sin(2*M_PI*v);
            z = 0.5*u * cos(2*M_PI*v);
            y = -0.5;
            m_pList.push_back(Vector3(x, y, z));
            m_nList.push_back(Vector3(0, -1, 0));
        }
    }

    //the biggest circle is saved twice, but the second time only the reference is stored
   for(it = m_pList.end()-m_p2,j = 0; j < m_p2; j++, it++)
   {
        v = delta_v * j;
       m_pList.push_back(*it);
       m_nList.push_back(Vector3(sin(2*M_PI*v), 0, cos(2*M_PI*v)));
   }

    //side
    for(i = 1; i < m_p1 + 1; i++)
    {
        u = delta_u * i;
        y = -0.5 + u;
        for(j = 0; j < m_p2; j++)
        {
            v = delta_v * j;
            x = 0.5 * sin(2*M_PI*v);
            z = 0.5 * cos(2*M_PI*v);
            m_pList.push_back(Vector3(x, y, z));
            m_nList.push_back(Vector3(sin(2*M_PI*v), 0, cos(2*M_PI*v)));
        }
    }

    //the biggest circle is saved twice, but the second time only the reference is stored
    for(it = m_pList.end()-m_p2,j = 0; j < m_p2; j++, it++)
    {
         v = delta_v * j;
        m_pList.push_back(*it);
        m_nList.push_back(Vector3(0, 1, 0));
    }

    //top from the second biggest circle
    for(i = 1; i< m_p1; i++)
    {
        u = delta_u * i;
        for(j = 0; j < m_p2; j++)
        {
            v = delta_v * j;
            x = 0.5*(1-u) * sin(2*M_PI*v);
            z = 0.5*(1-u) * cos(2*M_PI*v);
            y = 0.5;
            m_pList.push_back(Vector3(x, y, z));
            m_nList.push_back(Vector3(0, 1, 0));
        }
    }
    m_pList.push_back(Vector3(0, 0.5, 0));
    m_nList.push_back(Vector3(0, 1, 0));

}

void Cylinder::generateTriangle()
{
    //triangles
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

    //the layers in bottom, side, and up

    for(i = 0; i < 3*m_p1; i++)
    {
        if(i == m_p1-1)
            continue;
        if(i == 2*m_p1)
            continue;
        for(j = 1; j < m_p2 ; j ++)
        {
            m_tList.push_back(i*m_p2+j);
            m_tList.push_back(i*m_p2+j+1);
            m_tList.push_back(i*m_p2+j+m_p2+1);

            m_tList.push_back((i+1)*m_p2+j);
            m_tList.push_back(i*m_p2+j);
            m_tList.push_back((i+1)*m_p2+j+1);
        }
        m_tList.push_back(i*m_p2+j);
        m_tList.push_back(i*m_p2+1);
        m_tList.push_back(i*m_p2+m_p2+1);

        m_tList.push_back((i+1)*m_p2+j);
        m_tList.push_back(i*m_p2+j);
        m_tList.push_back((i+1)*m_p2+1);
    }

    //top the most inner part
    for(i = 1; i < m_p2; i++)
    {
        m_tList.push_back(3*m_p1*m_p2+i);
        m_tList.push_back(3*m_p1*m_p2+i+1);
        m_tList.push_back((3*m_p1+1)*m_p2+1);
    }
    m_tList.push_back(3*m_p1*m_p2+i);
    m_tList.push_back(3*m_p1*m_p2+1);
    m_tList.push_back((3*m_p1+1)*m_p2+1);
}

REAL Cylinder::calculateIntersecP(Vector3 &tnormal, Vector2 &tex)
{
        REAL tmpt, tmin = MAX_LIMIT, t1=-1, t2=-1;
        //body
        REAL a = d.x*d.x + d.z*d.z;
        REAL b = 2*(p.x*d.x + p.z*d.z);
        REAL c = p.x*p.x + p.z*p.z - 0.25;
        Vector3 n;

        getEquationRoot(a, b, c, t1, t2);

        n = Vector3(p.x + t1*d.x, 0, p.z + t1*d.z);
        n = glm::normalize(n);
        checkBodyBoundary(t1, tmin, n, tnormal);
        if(t1!=t2)
        {
            n = Vector3(p.x + t2*d.x, 0, p.z + t2*d.z);
            n = glm::normalize(n);
            checkBodyBoundary(t2, tmin, n, tnormal);
        }

        //cap
        Vector3 p0(0, -0.5, 0);
        n = Vector3(0, -1, 0);
        tmpt = calculateCapItsP(n, p0);
        checkCircleCapBoundary(tmpt, tmin, n, tnormal);

        n = Vector3(0, 1, 0);
        p0 = Vector3(0, 0.5, 0);
        tmpt = calculateCapItsP(n, p0);
        checkCircleCapBoundary(tmpt, tmin, n, tnormal);

        if(settings.useTextureMapping && tmin< MAX_LIMIT)
        {
            REAL x = p.x+tmin*d.x;
            REAL y = p.y+tmin*d.y;
            REAL z = p.z+tmin*d.z;
            if(tnormal == Vector3(0, -1, 0) || tnormal == Vector3(0, 1, 0)) // cap
            {
                tex.x = 0.5+x;
                tex.y = 0.5+z;
            }
            else // body
            {
                tex.y = 0.5-y;
                REAL theta = atan2(z,x);
                if(theta< 0)
                    tex.x = -theta/2/M_PI;
                else
                    tex.x = 1 - theta/2/M_PI;
            }
        }
        return tmin;

}


