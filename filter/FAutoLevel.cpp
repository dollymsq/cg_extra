#include "FAutoLevel.h"

FAutoLevel::FAutoLevel()
    :Filter()
{
    m_numoffilter = 0;// they are the same
    m_radius = 0; //waiting for value;
}

void FAutoLevel::doFilter(BGRA* cancol)
{
    int bin_R[256], bin_Raft[256], bin_B[256], bin_Baft[256], bin_G[256], bin_Gaft[256];

    memset(bin_B, 0, 256*sizeof(int));
    memset(bin_G, 0, 256*sizeof(int));
    memset(bin_R, 0, 256*sizeof(int));

    memset(bin_Baft, 0, 256*sizeof(int));
    memset(bin_Gaft, 0, 256*sizeof(int));
    memset(bin_Raft, 0, 256*sizeof(int));

    for(int row = m_minY; row < m_maxY +1; row++)
    {
        for(int col = m_minX; col < m_maxX +1; col++)
        {
            bin_R[cancol[row*oriWidth + col].r]++;
            bin_G[cancol[row*oriWidth + col].g]++;
            bin_B[cancol[row*oriWidth + col].b]++;
        }
    }

    for(int i = 1; i<256; i++)
    {
        bin_Baft[i] = bin_Baft[i-1] + bin_B[i];
        bin_Gaft[i] = bin_Gaft[i-1] + bin_G[i];
        bin_Raft[i] = bin_Raft[i-1] + bin_R[i];
    }

    for(int i = 0; i< 256; i++)
    {
        bin_Baft[i] =round((double)bin_Baft[i]/(double)((m_maxY-m_minY+1)*(m_maxX-m_minX+1)) * 256.0);
        bin_Gaft[i] =round((double)bin_Gaft[i]/(double)((m_maxY-m_minY+1)*(m_maxX-m_minX+1)) * 256.0);
        bin_Raft[i] =round((double)bin_Raft[i]/(double)((m_maxY-m_minY+1)*(m_maxX-m_minX+1)) * 256.0);
    }
    for(int row = m_minY; row < m_maxY +1; row++)
    {
        for(int col = m_minX; col < m_maxX +1; col++)
        {
             cancol[row*oriWidth + col].r = bin_Raft[cancol[row*oriWidth + col].r];
             cancol[row*oriWidth + col].g = bin_Gaft[cancol[row*oriWidth + col].g];
             cancol[row*oriWidth + col].b = bin_Baft[cancol[row*oriWidth + col].b];

        }
    }
}

