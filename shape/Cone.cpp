#include "Cone.h"

Cone::Cone(int p1, int p2, double p3)
    :Shape(p1, p2, p3)
{
    m_p2 = max(3, m_p2);
    generatePoints();
    generateTriangle();
}

void Cone::generatePoints()
{

    float delta_u = 1.0 / (float)m_p1;
    float delta_v = 1.0 / (float)m_p2;
    float u, v, x, y, z;

    int i, j;

    //bottom of cone, from inner to outside
    m_pList.push_back(Vector3(0.0, -0.5, 0.0));
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
            {
                tex.x = 0.5+x;
                tex.y = 0.5+z;

                m_cList.push_back(tex);

            }
        }
    }

     //the biggest circle is saved twice, but the second time only the reference is stored
    for(it = m_pList.end()-m_p2,j = 0; j < m_p2; j++, it++)
    {
        v = delta_v * j;
        m_pList.push_back(*it);
        m_nList.push_back(Vector3(2/sqrt(5)*sin(2*M_PI*v), 1/sqrt(5), 2/sqrt(5)*cos(2*M_PI*v)));
        {
            tex.x = 0.5+(*it).x;
            tex.y = 0.5+(*it).z;

            m_cList.push_back(tex);
        }
    }

    //side
    for(i = 1; i < m_p1; i++)
    {
        u = delta_u * i;
        y = -0.5 + u;
        for(j = 0; j < m_p2; j++)
        {
            v = delta_v * j;
            x = (1-u)/2 * sin(2*M_PI*v);
            z = (1-u)/2 * cos(2*M_PI*v);
            m_pList.push_back(Vector3(x, y, z));
            m_nList.push_back(Vector3(2/sqrt(5)*sin(2*M_PI*v), 1/sqrt(5), 2/sqrt(5)*cos(2*M_PI*v)));
            {
                tex.y = 0.5-y;
                REAL theta = atan2(z,x);
                if(theta< 0)
                    tex.x = -theta/2/M_PI;
                else
                    tex.x = 1 - theta/2/M_PI;
                m_cList.push_back(tex);
            }
        }
    }

    //top
    Vector3 Vtip(0, 0.5, 0), v1, v2, Vtipnormal;//to calculate normals on top

    //duplicate of top
    it = m_nList.end()-m_p2;
    for(j = 0 ; j < m_p2; j++)
    {
        v = delta_v * j;
        x = 0.5 * sin(2*M_PI*v);
        z = 0.5 * cos(2*M_PI*v);
        v1 = Vector3(x, -0.5, z);
        v1 = Vtip - v1;

        v = delta_v * (j+1);
        x = 0.5 * sin(2*M_PI*v);
        z = 0.5 * cos(2*M_PI*v);
        v2 = Vector3(x, -0.5, z);
        v2 = Vtip - v2;

            //  cross product
        Vtipnormal.x = v1.y*v2.z - v1.z*v2.y;
        Vtipnormal.y = v1.z*v2.x - v1.x*v2.z;
        Vtipnormal.z = v1.x*v2.y - v1.y*v2.x;

        Vtipnormal = glm::normalize(Vtipnormal);
        m_pList.push_back(Vtip);
        m_nList.push_back(Vtipnormal);
        {
            tex.x = 0.5;
            tex.y = 0.5;

            m_cList.push_back(tex);
        }
    }
}

void Cone::generateTriangle()
{
    //draw triangles
    //the most inner circle of bottom
    int i, j;
    for(i = 1; i < m_p2 ; i ++)
    {
        m_tList.push_back(i);
        m_tList.push_back(0);
        m_tList.push_back(i+1);
    }
    m_tList.push_back(i);
    m_tList.push_back(0);
    m_tList.push_back(1);

    //the layers in bottom and side
    it = m_pList.begin() + 1;
    for(i = 0; i < 2 * (m_p1-1)+1; i++)
    {
        if(i == m_p1-1) i++;
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

    //top of cone
    it = m_pList.begin() + (2*m_p1-1)*m_p2 +1;
    for(i = 0; i < m_p2-1; i++)
    {
        m_tList.push_back((2*m_p1-1)*m_p2 +1+i);
        m_tList.push_back((2*m_p1-1)*m_p2 +1+i+1);
        m_tList.push_back((2*m_p1-1)*m_p2 +1+m_p2+i);
    }
    m_tList.push_back((2*m_p1-1)*m_p2 +1+i);
    m_tList.push_back((2*m_p1-1)*m_p2 +1);
    m_tList.push_back((2*m_p1-1)*m_p2 +1+m_p2+i);
}

REAL Cone::calculateIntersecP(Vector3 &tnormal, Vector2 &tex)
{
        REAL tmpt, tmin = MAX_LIMIT, t1=-1, t2=-1;
        //body
        REAL a = d.x*d.x + d.z*d.z - d.y*d.y/4.0;
        REAL b = 2*p.x*d.x + 2*p.z*d.z - p.y*d.y/2.0 + d.y/4.0;
        REAL c = p.x*p.x + p.z*p.z - p.y*p.y/4.0 + p.y/4.0 - 1.0/16.0;
        REAL x, z;
        Vector3 n;

        getEquationRoot(a, b, c, t1, t2);

        x = p.x + t1*d.x; z = p.z + t1*d.z;
        n.x = x; n.y = sqrt(x*x + z*z)/2; n.z = z;
        n = glm::normalize(n);
        checkBodyBoundary(t1, tmin, n, tnormal);
        if(t1!=t2)
        {
            x = p.x + t2*d.x; z = p.z + t2*d.z;
            n.x = x; n.y = sqrt(x*x + z*z)/2; n.z = z;
            n = glm::normalize(n);
            checkBodyBoundary(t2, tmin, n, tnormal);
        }

        //cap
        Vector3  p0(0, -0.5, 0);
        n = Vector3(0, -1, 0);
        tmpt = calculateCapItsP(n, p0);
        checkCircleCapBoundary(tmpt, tmin, n, tnormal);

        if(settings.useTextureMapping && tmin< MAX_LIMIT)
        {
            REAL x = p.x+tmin*d.x;
            REAL y = p.y+tmin*d.y;
            REAL z = p.z+tmin*d.z;
            if(tnormal == Vector3(0, -1, 0)) // cap
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
