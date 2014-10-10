#ifndef FBLUR_H
#define FBLUR_H

#include "Filter.h"

class FBlur : public Filter
{
public:
    FBlur();    
    void doFilter(BGRA* concol);
    void setBlurRadius(int r);

protected:
    int m_radius;             // Selected blur radius

};

#endif // FBLUR_H
