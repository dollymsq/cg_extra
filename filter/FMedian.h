#ifndef FMEDIAN_H
#define FMEDIAN_H

#include "Filter.h"

class FMedian : public Filter
{
public:
    FMedian();
    void doFilter(BGRA* concol);
};

#endif // FMEDIAN_H
