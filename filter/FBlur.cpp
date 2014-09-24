#include "FBlur.h"

FBlur::FBlur()
    :Filter()
{
    m_numoffilter = 1;// it's a seperable filter and the two are same in value.
    m_radius = 0; //waiting for value;
}

void FBlur::doFilter(BGRA* cancol)
{

    int size = oriHeight*oriWidth;

    BGRA *tmpcpy = new BGRA[size];

    memcpy( &tmpcpy[0], &cancol[0], oriHeight*oriWidth * sizeof( BGRA ) );

    convolution(tmpcpy, cancol, m_filter1, 1, 2*m_radius+1);//v: the column vector
    convolution(cancol, tmpcpy, m_filter1, 2*m_radius+1, 1);//h: the row vector

    delete[] tmpcpy;
}

void FBlur::setBlurRadius(int r)
{
    m_radius = r;
    REAL sigma = (double)r/3;
    REAL k = 1/(sqrt(2*M_PI)*sigma*sigma);
    REAL sum = 0;//for normalize

    m_filter1 = new REAL[2*m_radius +1];
    for(int i = -m_radius; i< m_radius+1; i++)
    {
        m_filter1[i+m_radius] = k* exp(-i*i/2/sigma/sigma);
        sum += m_filter1[i+m_radius];
    }
    for(int i = -m_radius; i< m_radius+1; i++)
        m_filter1[i+m_radius] /= sum;
}
