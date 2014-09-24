#ifndef TUBEBRUSH_H
#define TUBEBRUSH_H
#include "Brush.h"
#include "math/CS123Algebra.h"

// This is a brush mimicking the texture and light diffusion of a tube brush stroke

class TubeBrush : public Brush
{
public:
    TubeBrush(BGRA color, int flow, int radius);
    virtual ~TubeBrush();

    virtual void paintOnce(int x, int y, Canvas2D* canvas);
    virtual void clearPaint();

protected:
    void makeMask(); /// Constructs the mask for this brush.
    void makeBrushTip();
    BGRA *m_distri;
};

#endif // TUBEBRUSH_H
