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

protected:
    REAL *Gx, *Gy;

};

#endif // FEDGEDETECT_H
