#include "FBilateral.h"

FBilateral::FBilateral()
    :Filter()
{
    m_numoffilter = 0;
    m_radius = 8;
}

FBilateral::~FBilateral()
{
    delete[] bfilterB;
    delete[] bfilterG;
    delete[] bfilterR;
}

void FBilateral::bilateralConvolution(BGRA *dst, BGRA *src)
{
    REAL realb, realg, realr;

    int srowIdx, scolIdx, fIdx;

    bfilterB = new REAL[(m_radius*2+1)*(m_radius*2+1)];
    bfilterR = new REAL[(m_radius*2+1)*(m_radius*2+1)];
    bfilterG = new REAL[(m_radius*2+1)*(m_radius*2+1)];

    for(int row = m_minY; row < m_maxY +1; row++)
    {
        for(int col = m_minX; col < m_maxX +1; col++)
        {
            setbfilter(src, row, col);
            realb = realg = realr = 0;
            for(int i = -m_radius; i < m_radius+1; i ++)
            {
                srowIdx = row + i;
                srowIdx = handleEdgeIndex(srowIdx, oriHeight);
                for(int j = -m_radius; j < m_radius+1; j ++)
                {
                    scolIdx = col + j;
                    scolIdx = handleEdgeIndex(scolIdx, oriWidth);
                    fIdx = (i + m_radius)* (m_radius*2+1) + j + m_radius;
                    realb += (double)src[srowIdx*oriWidth + scolIdx].b*bfilterB[fIdx];
                    realg += (double)src[srowIdx*oriWidth + scolIdx].g*bfilterG[fIdx];
                    realr += (double)src[srowIdx*oriWidth + scolIdx].r*bfilterR[fIdx];
                }
            }
            dst[row*oriWidth + col].b = round(realb);
            dst[row*oriWidth + col].g = round(realg);
            dst[row*oriWidth + col].r = round(realr);
        }
    }
}

void FBilateral::doFilter(BGRA* cancol)
{

    int size = oriHeight*oriWidth;

    BGRA *tmpcpy = new BGRA[size];

    memcpy( &tmpcpy[0], &cancol[0], oriHeight*oriWidth * sizeof( BGRA ) );

    bilateralConvolution(tmpcpy, cancol);

    memcpy( &cancol[0], &tmpcpy[0], oriHeight*oriWidth * sizeof( BGRA ) );

    delete[] tmpcpy;
}

void FBilateral::setbfilter(BGRA* src, int row, int col)
{
    REAL sigma1 = (double)m_radius/3;
    REAL sigma2 = (double)m_radius*3;

    REAL k1 = 1/(sqrt(2*M_PI)*sigma1*sigma1);
    REAL k2 = 1/(sqrt(2*M_PI)*sigma2*sigma2);

    REAL sumB = 0, sumG = 0, sumR = 0;//for normalize

    REAL intdifB, intdifG, intdifR;

    int srcidx, fidx;

    for(int i = -m_radius; i< m_radius+1; i++)
    {
        int trow = handleEdgeIndex(row+i,oriHeight) ;
        for(int j = -m_radius; j< m_radius+1; j++)
        {
            int tcol = handleEdgeIndex(col+j,oriHeight) ;
            srcidx = row*oriWidth + col;
            fidx = (i+m_radius)*(2*m_radius+1)+j+m_radius;
            intdifB = src[srcidx].b - src[trow * oriWidth + tcol].b;
            intdifG = src[srcidx].g - src[trow * oriWidth + tcol].g;
            intdifR = src[srcidx].r - src[trow * oriWidth + tcol].r;
            bfilterB[fidx] = k1* exp(-i*i/2.0/sigma1/sigma1)
                    * k2* exp(-intdifB*intdifB/2.0/sigma2/sigma2);
            bfilterG[fidx] = k1* exp(-i*i/2.0/sigma1/sigma1)
                    * k2* exp(-intdifG*intdifG/2.0/sigma2/sigma2);
            bfilterR[fidx] = k1* exp(-i*i/2.0/sigma1/sigma1)
                    * k2* exp(-intdifR*intdifR/2.0/sigma2/sigma2);
            sumB += bfilterB[fidx];
            sumG += bfilterG[fidx];
            sumR += bfilterR[fidx];
        }
    }
    for(int i = -m_radius; i< m_radius+1; i++)
    {
        for(int j = -m_radius; j< m_radius+1; j++)
        {
            fidx = (i+m_radius)*(2*m_radius+1)+j+m_radius;
            bfilterB[fidx] /= sumB;
            bfilterG[fidx] /= sumG;
            bfilterR[fidx] /= sumR;
        }
    }
}
