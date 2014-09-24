#include "FSharper.h"

FSharper::FSharper()
    :Filter()
{
    m_numoffilter = 1;// a 2 dimensional filter
    m_radius = 3;
    m_filter1 = new REAL[9];
    m_filter1[0] = 0; m_filter1[1] = -1; m_filter1[2] = 0;
    m_filter1[3] = -1; m_filter1[4] = 5; m_filter1[5] = -1;
    m_filter1[6] = 0; m_filter1[7] = -1; m_filter1[8] = 0;
}

void FSharper::doFilter(BGRA* cancol)
{
    tmpcpy = new BGRA[oriWidth*oriHeight];
    memcpy(&tmpcpy[0], &cancol[0], oriWidth*oriHeight*sizeof(BGRA));
    tpb = new REAL[oriWidth*oriHeight];
    tpg = new REAL[oriWidth*oriHeight];
    tpr = new REAL[oriWidth*oriHeight];

    REAL *cpb = new REAL[oriWidth*oriHeight];
    REAL *cpg = new REAL[oriWidth*oriHeight];
    REAL *cpr = new REAL[oriWidth*oriHeight];

    for(int i = 0; i< oriWidth*oriHeight; i ++)
    {
//        tpb[i] = cancol[i].b;
//        tpg[i] = cancol[i].g;
//        tpr[i] = cancol[i].r;

        cpb[i] = cancol[i].b;
        cpg[i] = cancol[i].g;
        cpr[i] = cancol[i].r;
    }
    convolution(tpb,cpb,m_filter1,3,3);
    convolution(tpg,cpg,m_filter1,3,3);
    convolution(tpr,cpr,m_filter1,3,3);

    int cpyidx;
    REAL p = 0.9;
    for(int row = m_minY; row < m_maxY +1; row++)
    {
        for(int col = m_minX; col < m_maxX +1; col++)
        {
            cpyidx = row*oriWidth + col;
            tpb[cpyidx] *= p;
            tpg[cpyidx] *= p;
            tpr[cpyidx] *= p;
            if(tpb[cpyidx]>255)
                cancol[cpyidx].b = 255;
            else
                cancol[cpyidx].b = tpb[cpyidx];

            if(tpg[cpyidx]>255)
                cancol[cpyidx].g = 255;
            else
                cancol[cpyidx].g = tpg[cpyidx];

            if(tpr[cpyidx]>255)
                cancol[cpyidx].r = 255;
            else
                cancol[cpyidx].r = tpr[cpyidx];
        }
    }
//    REAL minb = 1000, maxb = -1000, ming = 1000, maxg = -1000, minr = 1000, maxr = -1000;


//    memcpy(&cancol[0], &tmpcpy[0],oriWidth*oriHeight*sizeof(BGRA));
//    for(int i = 0 ; i<oriWidth*oriHeight; i++)
//    {
//        tpb[i] = (double)(-cancol[i].b + tmpcpy[i].b)*0.1 + (double)tmpcpy[i].b;
//        tpg[i] = (double)(-cancol[i].g + tmpcpy[i].g)*0.1 + (double)tmpcpy[i].g;
//        tpr[i] = (double)(-cancol[i].r + tmpcpy[i].r)*0.1 + (double)tmpcpy[i].r;

//        if(tpb[i]< minb)
//            minb = tpb[i];
//        if(tpb[i]> maxb)
//            maxb = tpb[i];

//        if(tpg[i]< ming)
//            ming = tpg[i];
//        if(tpg[i]> maxg)
//            maxg = tpg[i];

//        if(tpr[i]< minr)
//            minr = tpr[i];
//        if(tpr[i]> maxr)
//            maxr = tpr[i];

////        REAL tpb = (double)(cancol[i].b - tmpcpy[i].b);
////        REAL tpg = (double)(cancol[i].g - tmpcpy[i].g);
////        REAL tpr = (double)(cancol[i].r - tmpcpy[i].r);

////        tpb = round(tpb) + tmpcpy[i].b;
////        tpg = round(tpg) + tmpcpy[i].g;
////        tpr = round(tpr) + tmpcpy[i].r;
////        cancol[i].b = round(tpb) ;
////        cancol[i].g = round(tpg) ;
////        cancol[i].r = round(tpr) ;

//    }

//    for(int i = 0 ; i<oriWidth*oriHeight; i++)
//    {
//        cancol[i].b = round((tpb[i]-minb)/(maxb-minb)*255) ;
//        cancol[i].g = round((tpg[i]-ming)/(maxg-ming)*255) ;
//        cancol[i].r = round((tpr[i]-minr)/(maxr-minr)*255) ;

//                cancol[i].b = round((tpb[i])/(maxb)*255) ;
//                cancol[i].g = round((tpg[i])/(maxg)*255) ;
//                cancol[i].r = round((tpr[i])/(maxr)*255) ;
//    }
//    delete[] tpb ;
//    delete[] tpg ;
//    delete[] tpr ;
}

FSharper::~FSharper()
{
    delete[] tmpcpy;
    delete[] tpb;
    delete[] tpg;
    delete[] tpr;
}


