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
    int medianB = 0, medianG = 0, medianR = 0;
    int mid = (m_maxY +1 - m_minY) * (m_maxX +1 -m_minX)/2;
    for(int i = 1; i<256; i++)
    {
        bin_Baft[i] = bin_Baft[i-1] + bin_B[i];
        bin_Gaft[i] = bin_Gaft[i-1] + bin_G[i];
        bin_Raft[i] = bin_Raft[i-1] + bin_R[i];
        if(bin_Baft[i]>= mid && bin_Baft[i-1] <= mid)
            medianB = i;
        if(bin_Gaft[i]>= mid && bin_Gaft[i-1] <= mid)
            medianG = i;
        if(bin_Raft[i]>= mid && bin_Raft[i-1] <= mid)
            medianR = i;
    }

    for(int i = 0; i< 256; i++)
    {
        bin_Baft[i] =round((double)bin_Baft[i]/(double)((m_maxY-m_minY+1)*(m_maxX-m_minX+1)) * 256.0);
        bin_Gaft[i] =round((double)bin_Gaft[i]/(double)((m_maxY-m_minY+1)*(m_maxX-m_minX+1)) * 256.0);
        bin_Raft[i] =round((double)bin_Raft[i]/(double)((m_maxY-m_minY+1)*(m_maxX-m_minX+1)) * 256.0);
    }

    REAL gammaB = (log(1/2.0f))/log(medianB/256.0f);
    REAL gammaG = (log(1/2.0f))/log(medianG/256.0f);
    REAL gammaR = (log(1/2.0f))/log(medianR/256.0f);
    for(int row = m_minY; row < m_maxY +1; row++)
    {
        for(int col = m_minX; col < m_maxX +1; col++)
        {
            glm::vec3 ori;
            ori.r = bin_Raft[cancol[row*oriWidth + col].r];
            ori.g = bin_Gaft[cancol[row*oriWidth + col].g];
            ori.b = bin_Baft[cancol[row*oriWidth + col].b];
            cancol[row*oriWidth + col].r = ori.r;
            cancol[row*oriWidth + col].g = ori.g;
            cancol[row*oriWidth + col].b = ori.b;
//            cancol[row*oriWidth + col].r = round(pow(ori.r*1.0f, gammaR));
//            cancol[row*oriWidth + col].g = round(pow(ori.g*1.0f, gammaG));
//            cancol[row*oriWidth + col].b = round(pow(ori.b*1.0f, gammaB));
        }
    }
}

