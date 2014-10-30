#include "FScale.h"
#include "Canvas2D.h"

FScale::FScale()
    :Filter()
{
    m_numoffilter = 2;// horizontal and vertical filter
}

void FScale::setScaleXnY(REAL x, REAL y)
{
    m_scaleX = x;
    m_scaleY = y;
}

void FScale::doFilter(BGRA* cancol)
{
    bfWidth = m_maxX - m_minX + 1;
    bfHeight = m_maxY - m_minY + 1;
    aftWidth = m_scaleX * (double)bfWidth;
    aftHeight = m_scaleY * (double)bfHeight;

    midcpy = new BGRA[aftWidth*bfHeight];
    fnlcpy = new BGRA[aftWidth*aftHeight];

    if(m_scaleX != 1)
    {
        if(m_scaleX > 1)
            fxLen = 2;
        else
            fxLen = 2/m_scaleX;
        scaleImageX(midcpy, cancol, aftWidth, bfHeight);
    }
    else
    {    for(int row = 0; row < bfHeight; row++)
            memcpy(&midcpy[row*bfWidth], &cancol[(row+m_minY)*oriWidth+m_minX],bfWidth*sizeof(BGRA));
    }

    if(m_scaleY != 1)
    {
        if(m_scaleY > 1)
            fyLen = 2;
        else
            fyLen = 2/m_scaleY;
        scaleImageY(fnlcpy, midcpy, aftWidth, aftHeight);
    }
    else
        memcpy(&fnlcpy[0], &midcpy[0],aftWidth*aftHeight*sizeof(BGRA));
}

void FScale::scaleImageX(BGRA *dst, BGRA* src, int curWidth, int curHeight)
{

    int start, end, dIdx, sIdx;
    REAL swdg , tmpb, tmpg, tmpr, *w;//the wedge position of source image

    //for each new image point
    for(int row = 0; row < curHeight; row++)
    {
        for(int col = 0; col < curWidth; col++)
        {
            dIdx = row*curWidth + col;
            swdg = (float)col/m_scaleX + (1-m_scaleX)/2/m_scaleX;
            start = ceil(swdg - fxLen/2);//filter start place
            end = floor(swdg + fxLen/2);


            //filter with original image point
            w = new REAL[end - start + 1];
            getWeightNnormalized(w,start,end,swdg,fxLen);

            int t;
            tmpb = tmpg = tmpr = 0;
            for(int i = start; i< end+1; i ++)
            {
                t = i+m_minX;
                t = handleEdgeIndex(t, oriWidth);
                sIdx = (row+m_minY)*oriWidth + t ;

                tmpb += src[sIdx].b* w[i-start];
                tmpg += src[sIdx].g* w[i-start];
                tmpr += src[sIdx].r* w[i-start];
            }
            dst[dIdx].b = round(tmpb);
            dst[dIdx].g = round(tmpg);
            dst[dIdx].r = round(tmpr);

            delete[] w;
        }
    }
}

void FScale::scaleImageY(BGRA *dst, BGRA* src, int curWidth, int curHeight)
{

    int start, end, dIdx, sIdx;
    REAL swdg , tmpb, tmpg, tmpr, *w;//the wedge position of source image

    //for each new image point

    for(int col = 0; col < curWidth; col++)
    {
        for(int row = 0; row < curHeight; row++)
        {
            dIdx = row*curWidth + col;
            swdg = (float)row/m_scaleY + (1-m_scaleY)/2/m_scaleY;
            start = ceil(swdg - fyLen/2);//filter start place
            end = floor(swdg + fyLen/2);

            //filter with original image point
            int t;
            w = new REAL[end - start + 1];
            getWeightNnormalized(w,start,end,swdg,fyLen);

            tmpb = tmpg = tmpr = 0;
            for(int i = start; i< end+1; i ++)
            {
                t = i;
                t = handleEdgeIndex(t, bfHeight);
                sIdx = t*curWidth + col ;

                tmpb += src[sIdx].b* w[i-start];
                tmpg += src[sIdx].g* w[i-start];
                tmpr += src[sIdx].r* w[i-start];
            }
            dst[dIdx].b = round(tmpb);
            dst[dIdx].g = round(tmpg);
            dst[dIdx].r = round(tmpr);

            delete[] w;
        }
    }
}

void FScale::getWeightNnormalized(REAL *w, int start, int end, REAL wedge, REAL len)
{
    double sb, lb, lh, sum = 0;
    for(int i = start; i< end+1; i ++)
    {
        sb = len/2.0 - fabs(wedge-(double)i);
        lb = len/2.0;
        lh = 2.0/len;
        w[i-start] = sb * lh / lb;
        sum += w[i-start];
    }
    if(sum != 1)
        for(int i = 0; i< end - start + 1; i++)
            w[i] /= sum;
}

FScale::~FScale()
{
    delete[] fnlcpy;
    delete[] midcpy;
}

void FScale::setCanvas(Canvas2D *canvas)
{
    canvas->resize(aftWidth,aftHeight);
    memcpy(&canvas->data()[0], &fnlcpy[0],aftWidth*aftHeight*sizeof(BGRA));
}
