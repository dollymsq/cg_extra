/**
 * @file   ConstantBrush.cpp
 *
 * Implementation of a brush with a constant mask distribution.
 *
 * You should fill this file in while completing the Brush assignment.
 */

#include "ConstantBrush.h"

ConstantBrush::ConstantBrush(BGRA color, int flow, int radius)
    : Brush(color, flow, radius)
{
    // @TODO: [BRUSH] You'll probably want to set up the mask right away.
    makeMask();
}


ConstantBrush::~ConstantBrush()
{
    // @TODO: [BRUSH] Delete any resources owned by this brush, so you don't leak memory.

}

void ConstantBrush::makeMask()
{
    // @TODO: [BRUSH] Set up the mask for your Constant brush here...

}


