/**
 * @file   QuadraticBrush.cpp
 *
 * Implementation of a brush with a quadratic mask distribution.
 *
 * You should fill this file in while completing the Brush assignment.
 */

#include "QuadraticBrush.h"

QuadraticBrush::QuadraticBrush(BGRA color, int flow, int radius)
    : Brush(color, flow, radius)
{
    // @TODO: [BRUSH] You'll probably want to set up the mask right away.
    makeMask();
}

QuadraticBrush::~QuadraticBrush()
{
    // @TODO: [BRUSH] Delete any resources owned by this brush, so you don't leak memory.

}

void QuadraticBrush::makeMask()
{
    // @TODO: [BRUSH] Set up the mask for your Quadratic brush here...

}


