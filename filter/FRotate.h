#ifndef FROTATE_H
#define FROTATE_H

#include "Filter.h"

class FRotate : public Filter
{
public:
    FRotate();
    ~FRotate();
    virtual void setRotateDegree(int d);
    virtual void doFilter(BGRA* cancol);
    virtual void setCanvas(Canvas2D *canvas);
    virtual void setRegionParameter(Canvas2D *canvas);

private:
    BGRA *fnlcpy; // for the final copy
    int bfWidth, bfHeight , aftWidth , aftHeight;
    REAL theta;
    glm::vec2 v1, v2, v3, v4; // topleft, topright, bottomleft, bottomright point after rotation
    glm::mat2 rotMat1, rotMat2;

    bool inRange(glm::vec2 pos);
    void interpoloate(glm::vec2 pos, BGRA *cancol, BGRA &dst);
};

#endif // FROTATE_H
