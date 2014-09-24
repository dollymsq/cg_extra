#include "Cube.h"

Cube::Cube(int p1, int p2, double p3)
    :Shape(p1, p2, p3)
{
    generatePoints();
    generateTriangle();
}

Cube::Cube(Vector4 pp, Vector4 dd)
    :Shape(pp, dd)
{
}

void Cube::generatePoints()
{
    float delta_u = 1.0 / (float)m_p1;
    float u, x, y, z;

    int i, j, flag;

    //the bottom and top of cube; (m_p1)*(m_p1) points, first z,x
    for(flag = -1; flag < 2; flag += 2)
    {
        for(i = 0; i < m_p1 + 1; i ++)
        {
            u = delta_u * i;
            x = -0.5 + u;
            for(j = 0; j < m_p1 + 1; j++)
            {
                u = delta_u * j;
                z = -0.5 + u;
                y = flag * 0.5;
                m_pList.push_back(Vector4(x, y, z, 1.0));
                m_nList.push_back(Vector4(0, flag, 0, 0));
            }
        }
    }

    //front and back, first y,x
    for(flag = -1; flag < 2; flag += 2)
    {
        for(i = 0; i < m_p1 + 1; i ++)
        {
            u = delta_u * i;
            x = -0.5 + u;
            for(j = 0; j < m_p1 + 1; j++)
            {
                u = delta_u * j;
                y = -0.5 + u;
                z = flag * -0.5;
                m_pList.push_back(Vector4(x, y, z, 1.0));
                m_nList.push_back(Vector4(0, 0, -1*flag, 0));
            }
        }
    }

    //left and right, first y,z
    for(flag = -1; flag < 2; flag += 2)
    {
        for(i = 0; i < m_p1 + 1; i ++)
        {
            u = delta_u * i;
            z = -0.5 + u;
            for(j = 0; j < m_p1 + 1; j++)
            {
                u = delta_u * j;
                y = -0.5 + u;
                x = flag * 0.5;
                m_pList.push_back(Vector4(x, y, z, 1.0));
                m_nList.push_back(Vector4(flag, 0, 0, 0));
            }
        }
    }


}

void Cube::generateTriangle()
{
    int i, j, count, index;

    //store triangle information.
    for(count = 0 ; count < 6; count ++)
    {
        index = count * (m_p1+1) * (m_p1+1);

        for(i = 0; i< m_p1; i ++)
        {
            for(j = 0; j< m_p1; j++)
            {
                if(count%2 == 0)
                {
                    m_tList.push_back(index+i*(m_p1+1)+j+1);
                    m_tList.push_back(index+i*(m_p1+1)+j);
                    m_tList.push_back(index+i*(m_p1+1)+j+m_p1+2);

                    m_tList.push_back(index+i*(m_p1+1)+j+m_p1+1);
                    m_tList.push_back(index+i*(m_p1+1)+j+m_p1+2);
                    m_tList.push_back(index+i*(m_p1+1)+j);
                }
                else
                {
                    m_tList.push_back(index+i*(m_p1+1)+j);
                    m_tList.push_back(index+i*(m_p1+1)+j+1);
                    m_tList.push_back(index+i*(m_p1+1)+j+m_p1+2);

                    m_tList.push_back(index+i*(m_p1+1)+j+m_p1+2);
                    m_tList.push_back(index+i*(m_p1+1)+j+m_p1+1);
                    m_tList.push_back(index+i*(m_p1+1)+j);
                }
            }
        }
    }
}

REAL Cube::calculateIntersecP(Vector3 &tnormal, vec2<REAL> &tex)
{
    //cap
    REAL tmpt, tmin = MAX_LIMIT;
    Vector3 n, p0;

    //6 faces

    n = Vector3(0, 1, 0);
    p0 = Vector3(0, 0.5, 0);
    tmpt = calculateCapItsP(n, p0);
    checkQuadCapBoundary(tmpt, tmin, n, tnormal,'y');

    n = Vector3(0, -1, 0);
    p0 = Vector3(0, -0.5, 0);
    tmpt = calculateCapItsP(n, p0);
    checkQuadCapBoundary(tmpt, tmin, n, tnormal,'y');

    n = Vector3(1, 0, 0);
    p0 = Vector3(0.5, 0, 0);
    tmpt = calculateCapItsP(n, p0);
    checkQuadCapBoundary(tmpt, tmin, n, tnormal,'x');

    n = Vector3(-1, 0, 0);
    p0 = Vector3(-0.5, 0, 0);
    tmpt = calculateCapItsP(n, p0);
    checkQuadCapBoundary(tmpt, tmin, n, tnormal,'x');

    n = Vector3(0, 0, 1);
    p0 = Vector3(0, 0, 0.5);
    tmpt = calculateCapItsP(n, p0);
    checkQuadCapBoundary(tmpt, tmin, n, tnormal,'z');

    n = Vector3(0, 0, -1);
    p0 = Vector3(0, 0, -0.5);
    tmpt = calculateCapItsP(n, p0);
    checkQuadCapBoundary(tmpt, tmin, n, tnormal,'z');

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
        else if(tnormal == Vector3(-1, 0, 0) || tnormal == Vector3(1, 0, 0))
        {
            tex.x = 0.5-z;
            tex.y = 0.5-y;
        }
        else
        {
            tex.x = 0.5+x;
            tex.y = 0.5-y;
        }
    }

    return tmin;
}

void Cube::checkQuadCapBoundary(REAL tmpt, REAL &tmin, Vector3 n, Vector3 &tnormal, char signal)
{
    if(tmpt >=0 )
    {
        REAL x = p.x + tmpt*d.x;
        REAL y = p.y + tmpt*d.y;
        REAL z = p.z + tmpt*d.z;
        switch(signal)
        {
        case 'x':
            if(y < -0.5 || y > 0.5 || z < -0.5 || z > 0.5 || tmpt < 0)
                tmpt = MAX_LIMIT;
            else
            {
                if(tmin > tmpt)
                {
                    tmin = tmpt;
                    tnormal = n;
                }
            }
            break;
        case 'y':
            if(x < -0.5 || x > 0.5 || z < -0.5 || z > 0.5 || tmpt < 0)
                tmpt = MAX_LIMIT;
            else
            {
                if(tmin > tmpt)
                {
                    tmin = tmpt;
                    tnormal = n;
                }
            }
            break;
        case 'z':
            if(x < -0.5 || x > 0.5 || y < -0.5 || y > 0.5 || tmpt < 0)
                tmpt = MAX_LIMIT;
            else
            {
                if(tmin > tmpt)
                {
                    tmin = tmpt;
                    tnormal = n;
                }
            }
            break;
        }
    }
}

