#ifndef SpecialBrush_Header
#define SpecialBrush_Header

#include "QuadraticBrush.h"

/**
 * @class SmudgeBrush
 *
 * Smudge brush, which smudges color values across the canvas as the mouse is dragged.
 */
class SmudgeBrush : public QuadraticBrush
{

public:
    SmudgeBrush(BGRA color, int flow, int radius);
    virtual ~SmudgeBrush();


protected:
    // Constructs the mask for this brush.
    void makeMask();

    //! Copy paint in the bounding box of the brush from canvas to m_paint
    void pickUpPaint(int x, int y, Canvas2D* canvas);

    //! Apply the brush one time to the given canvas centered at (x,y)
    virtual void paintOnce(int x, int y, Canvas2D* canvas);

};

#endif
