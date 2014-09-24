#ifndef FSHARPER_H
#define FSHARPER_H

#include "Filter.h"

class FSharper : public Filter
{
public:
    FSharper();
    ~FSharper();
    void doFilter(BGRA* concol);

protected:
    BGRA* tmpcpy;
    REAL *tpg, *tpr, *tpb;
};

#endif // FSHARPER_H
