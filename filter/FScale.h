#ifndef FSCALE_H
#define FSCALE_H

#include "Filter.h"

class FScale : public Filter
{
public:
    FScale();
    ~FScale();
    void doFilter(BGRA* concol);
    void setScaleXnY(REAL x, REAL y);
    void scaleImageX(BGRA *dst, BGRA* src, int curWidth, int curHeight);
    void scaleImageY(BGRA *dst, BGRA* src, int curWidth, int curHeight);

protected:
    int bfWidth, bfHeight , aftWidth , aftHeight;
    BGRA *fnlcpy , *midcpy;
    REAL fxLen, fyLen; 
    REAL m_scaleX;            // Horizontal scale factor
    REAL m_scaleY;
    void getWeightNnormalized(REAL *w, int start, int end, REAL wedge, REAL len);
    void setCanvas(Canvas2D *canvas);

};

#endif // FSCALE_H
