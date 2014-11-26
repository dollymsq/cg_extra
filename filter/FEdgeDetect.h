#ifndef FEDGEDETECT_H
#define FEDGEDETECT_H

#include "FGrayScale.h"

class FEdgeDetect : public FGrayScale
{
public:
    FEdgeDetect();
    ~FEdgeDetect();
    void doFilter(BGRA* concol);
    void setThreshold(REAL thd);

    void blurEdges(BGRA* cancol);

protected:
    REAL *Gx, *Gy;
    REAL m_threshold;
    int m_radius;
};

#endif // FEDGEDETECT_H
