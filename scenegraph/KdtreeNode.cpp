#include "KdtreeNode.h"

void KdtreeNode::splitNode(int depth)
{
    if(isLeaf)
    {
        return;
    }

    char splitAxis = 'y';
    int tmp;
    REAL xr = xRange.y - xRange.x;
    REAL yr = yRange.y - yRange.x;
    REAL zr = zRange.y - zRange.x;
    if( xr > yr)
    {
        splitAxis = 'x';
        tmp = xr;
    }
    if( zr > tmp)
    {
        splitAxis = 'z';
    }

    std::vector<primitiveNmatrix*>::iterator it;
    int countl, countr;
    REAL costSum = INT_MAX, thresholdt, costt, threshold;
    switch(splitAxis)
    {
    case 'x':
        for(it = m_tbd.begin(); it != m_tbd.end(); it++)
        {
            primitiveNmatrix* pnm = (*it);
            thresholdt = pnm->xRange.y;
            countChild('x',thresholdt,countl,countr);
            costt = countr * (yr*zr + zr* (xRange.y - thresholdt) + yr* (xRange.y - thresholdt))
                    + countl * (yr*zr + zr* (thresholdt - xRange.x) + yr* (thresholdt - xRange.x));
            if(costSum > costt)
            {
                threshold = thresholdt;
                costSum = costt;
            }
        }
        break;
    case 'y':
        for(it = m_tbd.begin(); it != m_tbd.end(); it++)
        {
            primitiveNmatrix* pnm = (*it);
            thresholdt = pnm->yRange.y;
            countChild('y',thresholdt,countl,countr);
            costt = countr * (xr*zr + zr* (yRange.y - thresholdt) + xr* (yRange.y - thresholdt))
                    + countl * (xr*zr + zr* (thresholdt - yRange.x) + xr* (thresholdt - yRange.x));
            if(costSum > costt)
            {
                threshold = thresholdt;
                costSum = costt;
            }
        }
        break;
    case 'z':
        for(it = m_tbd.begin(); it != m_tbd.end(); it++)
        {
            primitiveNmatrix* pnm = (*it);
            thresholdt = pnm->zRange.y;
            countChild('z',thresholdt,countl,countr);
            costt = countr * (xr*yr + yr* (zRange.y - thresholdt) + xr* (zRange.y - thresholdt))
                    + countl * (xr*yr + yr* (thresholdt - zRange.x) + xr* (thresholdt - zRange.x));
            if(costSum > costt)
            {
                threshold = thresholdt;
                costSum = costt;
            }
        }
        break;
    default:
        break;
    }
    leftChild = new KdtreeNode();
    rightChild = new KdtreeNode();

    addPrimitive2Node(splitAxis, threshold, leftChild, rightChild);

    leftChild->splitNode(depth+1);
    rightChild->splitNode(depth+1);
}

void KdtreeNode::countChild(char axis, REAL threshold, int& left, int & right)
{
    std::vector<primitiveNmatrix*>::iterator it;
    left = 0;
    right = 0;
    for(it = m_tbd.begin(); it != m_tbd.end(); it++)
    {
        primitiveNmatrix* pnm = (*it);
        switch(axis)
        {
        case 'x':
            if(pnm->xRange.x < threshold)
                left ++;
            if(pnm->xRange.y > threshold)
                right ++;
            break;
        case 'y':
            if(pnm->yRange.x < threshold)
                left ++;
            if(pnm->yRange.y > threshold)
                right ++;
            break;
        case 'z':
            if(pnm->zRange.x < threshold)
                left ++;
            if(pnm->zRange.y > threshold)
                right ++;
            break;
        default:
            break;
        }
    }
}

void KdtreeNode::addPrimitive2Node(char axis, REAL threshold, KdtreeNode *left, KdtreeNode *right)
{
    std::vector<primitiveNmatrix*>::iterator it;

    switch(axis)
    {
    case 'x':
        left->yRange = yRange;
        left->zRange = zRange;

        right->yRange = yRange;
        right->zRange = zRange;

        left->xRange.x = xRange.x;
        left->xRange.y = threshold;

        right->xRange.x = threshold;
        right->xRange.y = xRange.y;
        for(it = m_tbd.begin(); it != m_tbd.end(); it++)
        {
            primitiveNmatrix* pnm = (*it);
            if(pnm->xRange.x < threshold)
                left->m_tbd.push_back(pnm);
            if(pnm->xRange.y > threshold)
                right->m_tbd.push_back(pnm);
        }
        break;
    case 'y':
        left->xRange = xRange;
        left->zRange = zRange;

        right->xRange = xRange;
        right->zRange = zRange;

        left->yRange.x = yRange.x;
        left->yRange.y = threshold;

        right->yRange.x = threshold;
        right->yRange.y = yRange.y;
        for(it = m_tbd.begin(); it != m_tbd.end(); it++)
        {
            primitiveNmatrix* pnm = (*it);

            if(pnm->yRange.x < threshold)
                left->m_tbd.push_back(pnm);
            if(pnm->yRange.y > threshold)
                right->m_tbd.push_back(pnm);
        }
        break;
    case 'z':
        left->xRange = xRange;
        left->yRange = yRange;

        right->xRange = xRange;
        right->yRange = yRange;

        left->zRange.x = zRange.x;
        left->zRange.y = threshold;

        right->zRange.x = threshold;
        right->zRange.y = zRange.y;
        for(it = m_tbd.begin(); it != m_tbd.end(); it++)
        {
            primitiveNmatrix* pnm = (*it);
            if(pnm->zRange.x < threshold)
                left->m_tbd.push_back(pnm);
            if(pnm->zRange.y > threshold)
                right->m_tbd.push_back(pnm);
        }
        break;
    default:
        break;
    }

    if(left->m_tbd.size() < 6)
        left->isLeaf = true;
    else
        left->isLeaf = false;

    if(right->m_tbd.size() < 6)
        right->isLeaf = true;
    else
        right->isLeaf = false;
}


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

