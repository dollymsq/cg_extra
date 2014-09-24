#include "Filter.h"
#include "Canvas2D.h"

Filter::Filter()
{
    m_filter1 = NULL;
    m_filter2 = NULL;
}

Filter::~Filter()
{
    if(m_numoffilter == 1)
        delete[] m_filter1;
    else if(m_numoffilter == 2)
    {
        delete[] m_filter1;
        delete[] m_filter2;
    }
}

void Filter::setRegionParameter(Canvas2D *canvas)
{
    oriWidth = canvas->width();
    oriHeight = canvas->height();

    QPoint q1 = canvas->marqueeStart();
    QPoint q2 = canvas->marqueeStop();

    if(q1 == q2)
    {
        m_minX = 0;
        m_minY = 0;
        m_maxX = oriWidth - 1;
        m_maxY = oriHeight - 1;
    }
    else
    {
        m_minX = qMin(q1.x(), q2.x());
        m_minY = qMin(q1.y(), q2.y());
        m_maxX = qMax(q1.x(), q2.x());
        m_maxY = qMax(q1.y(), q2.y());

        //make sure it falls into the canvas square
        m_minX = qMax(m_minX, 0);
        m_minY = qMax(m_minY, 0);
        m_maxX = qMin(m_maxX, oriWidth - 1);
        m_maxY = qMin(m_maxY, oriHeight - 1);
    }

}

void Filter::convolution(BGRA *dst, BGRA *src, REAL* filter, int f_width, int f_height)
{
    int widthR = (f_width - 1)/2;
    int heightR = (f_height - 1)/2;

    REAL realb, realg, realr;

    int srowIdx, scolIdx, fIdx;

    for(int row = m_minY; row < m_maxY +1; row++)
    {
        for(int col = m_minX; col < m_maxX +1; col++)
        {
            realb = realg = realr = 0;

            for(int i = -heightR; i < heightR+1; i ++)
            {
                srowIdx = row + i;
                handleEdgeIndex(srowIdx, oriHeight);
                for(int j = -widthR; j < widthR+1; j ++)
                {
                    scolIdx = col + j;
                    handleEdgeIndex(scolIdx, oriWidth);
                    fIdx = (i + heightR)* f_width + j + widthR;
                    realb += (double)src[srowIdx*oriWidth + scolIdx].b*filter[fIdx];
                    realg += (double)src[srowIdx*oriWidth + scolIdx].g*filter[fIdx];
                    realr += (double)src[srowIdx*oriWidth + scolIdx].r*filter[fIdx];
                }
            }
            dst[row*oriWidth + col].b = round(realb);
            dst[row*oriWidth + col].g = round(realg);
            dst[row*oriWidth + col].r = round(realr);
        }
    }
}

void Filter::convolution(REAL *dst, REAL *src, REAL* filter, int f_width, int f_height)
{
    int widthR = (f_width - 1)/2;
    int heightR = (f_height - 1)/2;

    int srowIdx, scolIdx, fIdx;

    for(int row = m_minY; row < m_maxY +1; row++)
    {
        for(int col = m_minX; col < m_maxX +1; col++)
        {
            dst[row*oriWidth + col] = 0;
            for(int i = -heightR; i < heightR+1; i ++)
            {
                srowIdx = row + i;
                handleEdgeIndex(srowIdx, oriHeight);
                for(int j = -widthR; j < widthR+1; j ++)
                {
                    scolIdx = col + j;
                    handleEdgeIndex(scolIdx, oriWidth);
                    fIdx = (i + heightR)* f_width + j + widthR;
                    dst[row*oriWidth + col] += src[srowIdx*oriWidth + scolIdx]*filter[fIdx];
                }
            }
        }
    }
}




