/**
 * @file   Brush.cpp
 *
 * Implementation of common functionality of bitmap brushes.
 *
 * You should fill this file in while scompleting the Brush assignment.
 */

#include "Brush.h"
#include "Canvas2D.h"

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
    makeMask();
}


void Brush::setRadius(int radius)
{
    m_radius = radius;
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
    int size = canvas->width() * canvas->height();
    for (int i = 0; i < size; ++i) {
        pix[i].r = 255;
        pix[i].g = 0;
        pix[i].b = 0;
        pix[i].a = 255;
    }

}


