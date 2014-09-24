#ifndef FAUTOLEVEL_H
#define FAUTOLEVEL_H

#include "Filter.h"

class FAutoLevel : public Filter
{
public:
    FAutoLevel();
    void doFilter(BGRA* concol);
};

#endif // FAUTOLEVEL_H
