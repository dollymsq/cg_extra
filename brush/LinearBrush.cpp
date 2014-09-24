/*!
   @file   LinearBrush.cpp
   @author Ben Herila (ben@herila.net)
   @author Evan Wallace (evan.exe@gmail.com)
   @date   Fall 2010

   @brief  Implementation of a brush with a linear mask distribution.
   You should fill this file in while completing the Brush assignment.

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
    int rowcounter;
    int colcounter;

    int len = 2 * m_radius + 1;
    float dist;

    m_mask = new float[len* len];
    for(rowcounter = 0; rowcounter < len; rowcounter ++)
    {
        for(colcounter = 0; colcounter < len; colcounter ++)
        {
            dist = float(DistanceToCenter(rowcounter, colcounter));
            if(dist > m_radius)
                    m_mask[rowcounter * len + colcounter] = 0.0;
            else
                    m_mask[rowcounter * len + colcounter] = float(m_radius - dist) / float(m_radius);
        }
    }

}


