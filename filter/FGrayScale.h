#ifndef FGRAYSCALE_H
#define FGRAYSCALE_H

#include "Filter.h"

class FGrayScale : public Filter
{
public:
    FGrayScale();
    void doFilter(BGRA* concol);
};

#endif // FGRAYSCALE_H
