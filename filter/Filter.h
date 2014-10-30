#ifndef FILTER_H
#define FILTER_H

#include <CS123Common.h>
#include "Settings.h"
#include <QPoint>

class Canvas2D;

class Filter
{
public:
    Filter();
    virtual ~Filter();
    virtual void doFilter(BGRA* cancol) = 0;
    virtual void setRegionParameter(Canvas2D *canvas);
    virtual void setThreshold(REAL thd) {}
    virtual void setBlurRadius(int r) {}
    virtual void setScaleXnY(REAL x, REAL y) {}
    virtual void setRotateDegree(int d) {}
    virtual void setCanvas(Canvas2D *canvas) {}


protected:

    //for edgedetect
    void convolution(REAL *dst, REAL *src, REAL* filter, int f_width, int f_height);

    //for blur
    void convolution(BGRA *dst, BGRA *src, REAL* filter, int f_width, int f_height);

    int m_numoffilter; //indicating the number of filters in sub class.
    REAL* m_filter1;  // This is a pointer to filter1.
    REAL* m_filter2;  // This is a pointer to filter2.

    int m_minX, m_minY, m_maxX, m_maxY, oriWidth, oriHeight;

};

inline int handleEdgeIndex(int p, int max)
{
    if(p < 0)
        return 0;
    else if(p > max - 1)
        return max - 1;
    else
        return p;
}

#endif // FILTER_H
