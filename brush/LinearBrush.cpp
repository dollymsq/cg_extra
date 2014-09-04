/**
 * @file   LinearBrush.cpp
 *
 * Implementation of a brush with a linear mask distribution.
 *
 * You should fill this file in while completing the Brush assignment.
 */

#include "LinearBrush.h"

LinearBrush::LinearBrush(BGRA color, int flow, int radius)
    : Brush(color, flow, radius)
{
    // @TODO: [BRUSH] You'll probably want to set up the mask right away.
    makeMask();
}


LinearBrush::~LinearBrush()
{
    // @TODO: [BRUSH] Delete any resources owned by this brush, so you don't leak memory.

}

void LinearBrush::makeMask()
{
    // @TODO: [BRUSH] Set up the mask for your Linear brush here...

}


