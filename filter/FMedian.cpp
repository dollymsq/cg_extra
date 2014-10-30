#include "FMedian.h"

FMedian::FMedian()
    :Filter()
{
    m_numoffilter = 0;
}

void FMedian::doFilter(BGRA* cancol)
{
    int srowIdx, scolIdx;
    int heightR = 2, widthR = 2;

    BGRA* tmcpy = new BGRA[oriHeight*oriWidth];
    memcpy(&tmcpy[0], &cancol[0],oriWidth*oriHeight*sizeof(BGRA));

    for(int row = m_minY; row < m_maxY +1; row++)
    {
        for(int col = m_minX; col < m_maxX +1; col++)
        {
            std::vector<int> tpb;
            std::vector<int> tpg;
            std::vector<int> tpr;
            for(int i = -heightR; i < heightR+1; i ++)
            {
                srowIdx = row + i;
                srowIdx = handleEdgeIndex(srowIdx, oriHeight);
                for(int j = -widthR; j < widthR+1; j ++)
                {
                    scolIdx = col+j;
                    scolIdx = handleEdgeIndex(scolIdx, oriWidth);
                    tpb.push_back(cancol[srowIdx*oriWidth+scolIdx].b);
                    tpg.push_back(cancol[srowIdx*oriWidth+scolIdx].g);
                    tpr.push_back(cancol[srowIdx*oriWidth+scolIdx].r);
                }
            }
            sort(tpb.begin(), tpb.begin()+(2*heightR+1)*(2*widthR+1) );
            sort(tpg.begin(), tpg.begin()+(2*heightR+1)*(2*widthR+1) );
            sort(tpr.begin(), tpr.begin()+(2*heightR+1)*(2*widthR+1) );

            tmcpy[row*oriWidth + col].b = tpb[(2*heightR+1)*(2*widthR+1)/2];
            tmcpy[row*oriWidth + col].g = tpg[(2*heightR+1)*(2*widthR+1)/2];
            tmcpy[row*oriWidth + col].r = tpr[(2*heightR+1)*(2*widthR+1)/2];
        }
    }
    memcpy(&cancol[0], &tmcpy[0],oriWidth*oriHeight*sizeof(BGRA));
    delete[] tmcpy;
}
