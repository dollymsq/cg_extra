#ifndef FINVERT_H
#define FINVERT_H

#include "Filter.h"


class FInvert : public Filter
{
public:
    FInvert();
    void doFilter(BGRA* concol);
};

#endif // FINVERT_H
