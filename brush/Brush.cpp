/*!
   @file   Brush.cpp
   @author Ben Herila (ben@herila.net)
   @author Evan Wallace (evan.exe@gmail.com)
   @date   Fall 2010

   @brief  Implementation of common functionality of bitmap brushes.
   You should fill this file in while completing the Brush assignment.

*/

#include "Brush.h"
#include "Canvas2D.h"

#include <iostream>

using namespace std;

unsigned char lerp(unsigned char a, unsigned char b, float percent)
{
    float fa = (float)a / 255.0f;
    float fb = (float)b / 255.0f;
    return (unsigned char)((fa + (fb - fa) * percent) * 255.0f + 0.5f);
}



Brush::Brush(BGRA color, int flow, int radius) {

    // @TODO: [BRUSH] Initialize any memory you are going to use here. Hint - you are going to
    //        need to store the mask in memory. This might (or might not) be a good place to
    //        allocate that memory.

    // Example code: (feel free to use)
    m_color = color;
    m_flow = flow;
    m_radius = radius;
    m_mask = NULL;

}


Brush::~Brush()
{
    // @TODO: [BRUSH] Don't forget to delete any memory you allocate. Use delete[] to delete
    //        a whole array. Otherwise you'll just delete the first element!
    //
    //        i.e. delete[] m_mask;
    //
    delete[] m_mask;
}


void Brush::setGreen(int green)
{
    m_color.g = green;
}


void Brush::setRed(int red)
{
    m_color.r = red;
}


void Brush::setBlue(int blue)
{
    m_color.b = blue;
}


void Brush::setFlow(int flow)
{
    m_flow = flow;
}


void Brush::setRadius(int radius)
{
    m_radius = radius;
    delete[] m_mask;
    makeMask();
}


void Brush::paintOnce(int mouse_x, int mouse_y, Canvas2D* canvas)
{
    // @TODO: [BRUSH] You can do any painting on the canvas here. Or, you can
    //        override this method in a subclass and do the painting there.
    //
    // Example: You'll want to delete or comment out this code, which
    // sets all the pixels on the canvas to red.
    //

    BGRA* pix = canvas->data();

    int rowstart = max(0, mouse_y - m_radius);
    int rowend = min(canvas->height(), mouse_y + m_radius + 1);
    int colstart = max(0, mouse_x - m_radius);
    int colend = min(canvas->width(), mouse_x + m_radius + 1);

    int rowcounter;
    int colcounter;

    int mask_idx;
    int convas_idx;
    float alpha = float(m_flow) / (255.0);

    BGRA current_canvascolor;

    for(rowcounter = rowstart; rowcounter < rowend; rowcounter ++)
    {
        for(colcounter = colstart; colcounter < colend; colcounter ++)
        {
            mask_idx = (m_radius + (rowcounter - mouse_y)) * ( 2 * m_radius + 1 )
                    + m_radius + colcounter - mouse_x;
            convas_idx = rowcounter * canvas->width() + colcounter;
            current_canvascolor = pix[convas_idx];

            pix[convas_idx].b = (1 - alpha * m_mask[mask_idx]) * current_canvascolor.b
                    + alpha * m_mask[mask_idx] * m_color.b;
            pix[convas_idx].g = (1 - alpha * m_mask[mask_idx]) * current_canvascolor.g
                    + alpha * m_mask[mask_idx] * m_color.g;
            pix[convas_idx].r = (1 - alpha * m_mask[mask_idx]) * current_canvascolor.r
                    + alpha * m_mask[mask_idx] * m_color.r;

            pix[convas_idx].a = 255;
        }
    }

}

void Brush::paintOnCanvasTmp(int mouse_x, int mouse_y, Canvas2D* canvas, float* flag)
{
    int rowstart = max(0, mouse_y - m_radius);
    int rowend = min(canvas->height(), mouse_y + m_radius + 1);
    int colstart = max(0, mouse_x - m_radius);
    int colend = min(canvas->width(), mouse_x + m_radius + 1);

    int rowcounter;
    int colcounter;

    int mask_idx, canvas_idx;

    for(rowcounter = rowstart; rowcounter < rowend; rowcounter ++)
    {
        for(colcounter = colstart; colcounter < colend; colcounter ++)
        {
            mask_idx = (m_radius + (rowcounter - mouse_y)) * ( 2 * m_radius + 1 )
                    + m_radius + colcounter - mouse_x;
            if(m_mask[mask_idx] != 0)
            {
                canvas_idx = rowcounter * canvas->width() + colcounter;
                flag[canvas_idx] = min(1.0f, flag[canvas_idx] + m_mask[mask_idx]);
            }
        }
    }
}

void Brush::fixAlphaBlendingF(Canvas2D* canvas, BGRA* canvas_ori, float *flag)
{
    BGRA* pix = canvas->data();
    BGRA current_canvascolor;
    float alpha = float(m_flow) / (255.0);

    int rowcounter;
    int colcounter;

    int canvas_idx;

    for(rowcounter = 0; rowcounter < canvas->height(); rowcounter ++)
    {
        for(colcounter = 0; colcounter < canvas->width(); colcounter ++)
        {
            canvas_idx = rowcounter * canvas->width() + colcounter;
            if(flag[canvas_idx] != 0)
            {
                current_canvascolor= canvas_ori[canvas_idx];
                pix[canvas_idx].b = (1 - alpha * flag[canvas_idx]) * current_canvascolor.b
                        + alpha * flag[canvas_idx] * m_color.b;
                pix[canvas_idx].g = (1 - alpha * flag[canvas_idx]) * current_canvascolor.g
                        + alpha * flag[canvas_idx] * m_color.g;
                pix[canvas_idx].r = (1 - alpha * flag[canvas_idx]) * current_canvascolor.r
                        + alpha * flag[canvas_idx] * m_color.r;

                pix[canvas_idx].a = 255;
            }
        }
    }
}
//mainly used by mask
double Brush::DistanceToCenter(int rowcounter, int colcounter)
{
    return sqrt(double((rowcounter - m_radius) * (rowcounter - m_radius))
                + double((colcounter - m_radius) * (colcounter - m_radius)));
}


