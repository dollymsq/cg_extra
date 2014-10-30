#ifndef FAUTOLEVEL_H
#define FAUTOLEVEL_H

#include "Filter.h"

class FAutoLevel : public Filter
{
public:
    FAutoLevel();
    void doFilter(BGRA* concol);
    int m_radius;
};

#endif // FAUTOLEVEL_H
