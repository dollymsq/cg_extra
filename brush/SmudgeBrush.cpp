/*!
   @file   Brush.cpp
   @author Ben Herila (ben@herila.net)
   @author Evan Wallace (evan.exe@gmail.com)
   @date   Fall 2010

   @brief
     Implementation of a brush that smudges the image on the canvas as
   the mouse is dragged. You should fill this file in for the Brush
   assignment.

*/

#include <iostream>
#include <CS123Common.h>

#include "SmudgeBrush.h"
#include "Canvas2D.h"

using namespace std;


SmudgeBrush::SmudgeBrush(BGRA color, int flow, int radius) : QuadraticBrush(color, flow, radius)
{
    // @TODO: [BRUSH] Initialize any memory you are going to use here. Hint - you are going to
    //       need to store temporary image data in memory. This might be a good place to
    //       allocate that memory.

    m_paintUnderMask = NULL;
    makeMask();
}


SmudgeBrush::~SmudgeBrush()
{
    // @TODO: [BRUSH] Be sure not to leak memory!
    if(m_paintUnderMask!= NULL)
        delete[] m_paintUnderMask;
}


//! create a mask with a distribution of your choice (probably want to use quadratic for best results)
void SmudgeBrush::makeMask()
{
    // @TODO: [BRUSH] Set up the mask for your brush here. For this brush you will probably want
    //        to use a quadratic distribution for the best results. Linear or Gaussian would
    //        work too, however. Feel free to paste your code from the Linear or Quadratic brushes
    //        or modify the class inheritance to be able to take advantage of one of those class's
    //        existing implementations. The choice is yours!
    //
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
                    m_mask[rowcounter * len + colcounter] = float(m_radius - dist) / float(m_radius)
                            * float(m_radius - dist) / float(m_radius);
        }
    }

}

//! Picks up paint from the canvas before drawing begins.
void SmudgeBrush::pickUpPaint(int x, int y, Canvas2D* canvas)
{
    // @TODO: [BRUSH] Perform the "pick up paint" step described in the assignment handout here.
    //        In other words, you should store the "paint" under the brush mask in a temporary
    //        buffer (which you'll also have to figure out where and how to allocate). Then,
    //        in the paintOnce() method, you'll paste down the paint that you picked up here.
    //
    int width = canvas->width();
    int height = canvas->height();

    BGRA* pix = canvas->data();

    int rowstart = max(0, y - m_radius);
    int rowend = min(height, y + m_radius + 1);
    int colstart = max(0, x - m_radius);
    int colend = min(width, x + m_radius + 1);

    int rowcounter;
    int colcounter;

    int convas_idx;
    int paint_idx;
    int i;

    BGRA current_convascolor;

    if(m_paintUnderMask == NULL)
        m_paintUnderMask = new BGRA[(2 * m_radius + 1) * (2 * m_radius + 1)];

    //fill in the empty part with color black(0, 0, 0, 255)
    for(i = 0; i< (2 * m_radius + 1) * ( 2 * m_radius + 1); i++)
    {
        m_paintUnderMask[i].b = 0;
        m_paintUnderMask[i].g = 0;
        m_paintUnderMask[i].r = 0;
        m_paintUnderMask[i].a = 255;
    }

    for(rowcounter = rowstart; rowcounter < rowend; rowcounter ++)
    {
        for(colcounter = colstart; colcounter < colend; colcounter ++)
        {
            convas_idx = rowcounter * width + colcounter;
            paint_idx = (rowcounter - y + m_radius) * ( 2 * m_radius + 1 )
                    + m_radius + colcounter - x;
            m_paintUnderMask[paint_idx] = pix[convas_idx];
        }
    }
}

void SmudgeBrush::paintOnce(int mouse_x, int mouse_y, Canvas2D* canvas)
{
    // @TODO: [BRUSH] Here, you'll need to paste down the paint you picked up in
    //        the previous method. Be sure to take the mask into account! You can
    //        ignore the alpha parameter, but you can also use it (smartly) if you
    //        would like to.

    if(m_paintUnderMask != NULL)
    {
        int width = canvas->width();
        int height = canvas->height();

        BGRA* pix = canvas->data();

        int rowstart = max(0, mouse_y - m_radius);
        int rowend = min(height, mouse_y + m_radius + 1);
        int colstart = max(0, mouse_x - m_radius);
        int colend = min(width, mouse_x + m_radius + 1);

        int rowcounter;
        int colcounter;

        int canvas_idx;
        int paint_idx; //the index for the paint picked up and also mask

        float alpha = float(m_flow) / (255.0);

        BGRA current_canvascolor;

        for(rowcounter = rowstart; rowcounter < rowend; rowcounter ++)
        {
            for(colcounter = colstart; colcounter < colend; colcounter ++)
            {
                canvas_idx = rowcounter * width + colcounter;
                paint_idx = (rowcounter - mouse_y + m_radius) * ( 2 * m_radius + 1 )
                        + m_radius + colcounter - mouse_x;
                current_canvascolor = pix[canvas_idx];
                pix[canvas_idx].b = round((1 - alpha * m_mask[paint_idx]) * current_canvascolor.b
                                         + alpha * m_mask[paint_idx] * m_paintUnderMask[paint_idx].b) ;
                pix[canvas_idx].g = round((1 - alpha * m_mask[paint_idx]) * current_canvascolor.g
                        + alpha * m_mask[paint_idx] * m_paintUnderMask[paint_idx].g);
                pix[canvas_idx].r = round((1 - alpha * m_mask[paint_idx]) * current_canvascolor.r
                        + alpha * m_mask[paint_idx] * m_paintUnderMask[paint_idx].r);
            }
        }

    }

    // now pick up paint again...
    pickUpPaint(mouse_x, mouse_y, canvas);

}

void SmudgeBrush::clearPaint()
{
    delete[] m_paintUnderMask;
    m_paintUnderMask = NULL;
}



