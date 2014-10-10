#include "FRotate.h"
#include "Canvas2D.h"

FRotate::FRotate()
    :Filter()
{
    m_numoffilter = 0;// no filter
}

FRotate::~FRotate()
{
    delete[] fnlcpy;
}

void FRotate::setRotateDegree(int d)
{
    theta = (float)d / 180.0f * M_PI;
    rotMat1 = glm::mat2(cos(theta), sin(theta), -sin(theta), cos(theta));
    rotMat2 = glm::mat2(cos(theta), -sin(theta), sin(theta), cos(theta)); // the inverse of rotMat1

}

void FRotate::doFilter(BGRA* cancol)
{
    //calculate the size and corresponding relation between two images
    bfWidth = m_maxX - m_minX + 1;
    bfHeight = m_maxY - m_minY + 1;

    v1 = glm::vec2(0.0f, 0.0f) * rotMat1;
    v2 = glm::vec2(bfWidth, 0.0f) * rotMat1;
    v3 = glm::vec2(0.0f, bfHeight) * rotMat1;
    v4 = glm::vec2(bfWidth, bfHeight) * rotMat1;

    float minW = min(v1.x, v2.x);
    minW = min(minW, v3.x);
    minW = min(minW, v4.x);

    float minH = min(v1.y, v2.y);
    minH = min(minH, v3.y);
    minH = min(minH, v4.y);

    float maxW = max(v1.x, v2.x);
    maxW = max(maxW, v3.x);
    maxW = max(maxW, v4.x);

    float maxH = max(v1.y, v2.y);
    maxH = max(maxH, v3.y);
    maxH = max(maxH, v4.y);  // find the range of the new image

    aftWidth = round(maxW - minW);
    aftHeight = round(maxH - minH);

    fnlcpy = new BGRA[aftWidth*aftHeight];
    for(int row = 0; row < aftWidth; row ++)
    {
        for(int col = 0; col < aftHeight; col ++)
        {
            glm::vec2 afterCoordin(row + minW, col + minH);
            glm::vec2 beforePosition = afterCoordin * rotMat2 + glm::vec2(m_minX, m_minY);
            if(inRange(beforePosition))
                interpoloate(beforePosition, cancol, fnlcpy[row* aftWidth + col]);
        }
    }
}

bool FRotate::inRange(glm::vec2 pos)
{
    return pos.x >= 0 && pos.x <= oriWidth -1
            && pos.y >=0 && pos.y <= oriHeight -1;
}

void FRotate::interpoloate(glm::vec2 pos, BGRA *cancol, BGRA &dst)
{
    glm::lowp_ivec2 h, i, e, f;
    h = glm::lowp_ivec2(floor(pos.x), ceil(pos.y));
    i = glm::lowp_ivec2(ceil(pos.x), ceil(pos.y));
    e = glm::lowp_ivec2(ceil(pos.x), floor(pos.y));
    f = glm::lowp_ivec2(floor(pos.x), floor(pos.y));

    REAL wi = (i.x - pos.x) * (i.y - pos.y);
    REAL wh = (pos.x - h.x) * (h.y - pos.y);
    REAL we = (e.x - pos.x) * (pos.y - e.y);
    REAL wf = (pos.x - f.x) * (pos.y - f.y);
    REAL sum = wi + wh + we + wf;
    if( sum < 10e-6)
        dst = cancol[h.x*oriWidth + h.y];
    else
    {
        wi /= sum; wh /= sum; we /= sum; wf /= sum;
        dst.b = round((cancol[h.x*oriWidth + h.y].b) * wh + (cancol[i.x*oriWidth + i.y].b) * wi
                      + (cancol[e.x*oriWidth + e.y].b) * we + (cancol[f.x*oriWidth + f.y].b) * wf);
        dst.g = round((cancol[h.x*oriWidth + h.y].g) * wh + (cancol[i.x*oriWidth + i.y].g) * wi
                      + (cancol[e.x*oriWidth + e.y].g) * we + (cancol[f.x*oriWidth + f.y].g) * wf);
        dst.r = round((cancol[h.x*oriWidth + h.y].r) * wh + (cancol[i.x*oriWidth + i.y].r) * wi
                      + (cancol[e.x*oriWidth + e.y].r) * we + (cancol[f.x*oriWidth + f.y].r) * wf);
    }
}

void FRotate::setCanvas(Canvas2D *canvas)
{
    canvas->resize(aftWidth,aftHeight);
    memcpy(&canvas->data()[0], &fnlcpy[0],aftWidth*aftHeight*sizeof(BGRA));
}

void FRotate::setRegionParameter(Canvas2D *canvas)
{
    oriWidth = canvas->width();
    oriHeight = canvas->height();
    m_minX = 0;
    m_minY = 0;
    m_maxX = oriWidth - 1;
    m_maxY = oriHeight - 1;
}
