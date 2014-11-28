#include "KdtreeNode.h"

REAL KdtreeNode::calculatePlaneHit(const Vector3& n, const Vector3& p0, const Vector4 &p, const Vector4 &d)
{
    return ((n.x*p0.x + n.y*p0.y+n.z*p0.z) - (n.x*p.x + n.y*p.y + n.z*p.z))/(n.x*d.x + n.y*d.y + n.z*d.z);
}

bool KdtreeNode::isInBoundingBox(const REAL& tmpt, const Vector4 &p, const Vector4 &d, char signal)
{
    if(tmpt >=0 )
    {
        REAL x = p.x + tmpt*d.x;
        REAL y = p.y + tmpt*d.y;
        REAL z = p.z + tmpt*d.z;
        switch(signal)
        {
        case 'x':
            if(y < yRange.x || y > yRange.y || z < zRange.x || z > zRange.y)
                return false;
            else
                return true;
            break;
        case 'y':
            if(x < xRange.x || x > xRange.y || z < zRange.x || z > zRange.y)
                return false;
            else
                return true;
            break;
        case 'z':
            if(x < xRange.x || x > xRange.y || y < yRange.x || y > yRange.y )
                return false;
            else
                return true;
            break;
        default:
            return false;
            break;
        }
    }
    else
        return false;
}

