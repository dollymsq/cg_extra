#ifndef FBLUR_H
#define FBLUR_H

#include "Filter.h"

class FBlur : public Filter
{
public:
    FBlur();    
    void doFilter(BGRA* concol);
    void setBlurRadius(int r);


};

#endif // FBLUR_H
