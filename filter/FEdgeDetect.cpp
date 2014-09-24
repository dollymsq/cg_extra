#include "FEdgeDetect.h"

FEdgeDetect::FEdgeDetect()
    :FGrayScale()
{
    m_numoffilter = 2;
    m_threshold = 0; //waiting for value
    m_radius = 1; //sobel is a 3x3 matrix, so the radius is 1

    m_filter1 = new REAL[3];
    m_filter2 = new REAL[3];
}

void FEdgeDetect::doFilter(BGRA* cancol)
{
    //transform to gray scale
    this->FGrayScale::doFilter(cancol);

    REAL p = 0.9; //before cap the G
    int size = oriHeight*oriWidth;
    Gx = new REAL[size];
    Gy = new REAL[size];
    REAL *tmpcpy = new REAL[size];
    REAL *cancpy = new REAL[size];

    //copy out the one channel of cancol and make it [0,1]
    for(int i = 0; i< size; i ++)
    {
        cancpy[i] = (double)cancol[i].b/255.0;
    }

    //for horizontal edges;
    m_filter1[0] = 1; m_filter1[1] = 2; m_filter1[2] = 1;
    m_filter2[0] = -1; m_filter2[1] = 0; m_filter2[2] = 1;
    convolution(tmpcpy, cancpy, m_filter1, 1, 3);//v: the column vector
    convolution(Gx, tmpcpy, m_filter2, 3, 1);//h: the row vector

    //for vertical edges;
    m_filter1[0] = 1; m_filter1[1] = 0; m_filter1[2] = -1;
    m_filter2[0] = 1; m_filter2[1] = 2; m_filter2[2] = 1;
    convolution(tmpcpy, cancpy, m_filter1, 1, 3);//v
    convolution(Gy, tmpcpy, m_filter2, 3, 1);//h


    //calculate gradient and modify the region
    int cpyidx;
    for(int row = m_minY; row < m_maxY +1; row++)
    {
        for(int col = m_minX; col < m_maxX +1; col++)
        {
            cpyidx = row*oriWidth + col;
            tmpcpy[cpyidx] = p *sqrt(Gx[cpyidx]*Gx[cpyidx] + Gy[cpyidx]*Gy[cpyidx]);
            if(tmpcpy[cpyidx] > 1)
                tmpcpy[cpyidx]=1;
            else
            {}
            cancol[cpyidx].b = cancol[cpyidx].g = cancol[cpyidx].r = tmpcpy[cpyidx]*255* m_threshold;
        }
    }

    delete[] tmpcpy;
    delete[] cancpy;

}

void FEdgeDetect::setThreshold(REAL thd)
{
    //set threshold
    m_threshold = thd;
}

FEdgeDetect::~FEdgeDetect()
{
    delete[] Gx;
    delete[] Gy;
}
