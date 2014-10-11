#ifndef FBILATERAL_H
#define FBILATERAL_H
#include "Filter.h"

class FBilateral : public Filter
{
public:
    FBilateral();
    ~FBilateral();
    void bilateralConvolution(BGRA *dst, BGRA *src);
    void doFilter(BGRA* cancol);

protected:
    int m_radius;             // Selected blur radius
    REAL* bfilterB, *bfilterR, *bfilterG;              //special filter
    void setbfilter(BGRA* src, int row, int col);

};

#endif // FBILATERAL_H
