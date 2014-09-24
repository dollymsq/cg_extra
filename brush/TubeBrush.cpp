#include "TubeBrush.h"
#include "Canvas2D.h"

using namespace std;

TubeBrush::TubeBrush(BGRA color, int flow, int radius)
: Brush(color, flow, radius)
{
    makeMask();
    m_distri = NULL;
}

TubeBrush::~TubeBrush()
{
    if(m_distri != NULL)
        delete[] m_distri;
}

void TubeBrush::makeMask()
{
    // @TODO: [BRUSH] Set up the mask for your Constant brush here...
    int rowcounter;
    int colcounter;

    int len = 2 * m_radius + 1;

    m_mask = new float[len* len];
    for(rowcounter = 0; rowcounter < len; rowcounter ++)
    {
        for(colcounter = 0; colcounter < len; colcounter ++)
        {
            if(DistanceToCenter(rowcounter,colcounter) > m_radius - 4)
                    m_mask[rowcounter * len + colcounter] = 0.0;
            else
                    m_mask[rowcounter * len + colcounter] = 1.0;
        }
    }
}

void TubeBrush::makeBrushTip()
{
    int rowcounter;
    int colcounter;

    int len = 2 * m_radius + 1;

    Vector3 currentpos(0, 0, 0);//for each mask point imaginery position
    Vector3 light(-1.0 * m_radius, 3 * m_radius, -1.0 * m_radius); // to make it proportional
    Vector3 lightray(0,0,0);
    Vector3 n(0,0,0);

    float d = 0, h = 0;
    float ka = 0.9, kd = 0.7;

    float coeff, dotproduct;

    m_distri = new BGRA[len*len];
    for(rowcounter = 0; rowcounter < len; rowcounter ++)
    {
        for(colcounter = 0; colcounter < len; colcounter ++)
        {
            d = DistanceToCenter(rowcounter,colcounter);
            if(d > m_radius)
                    m_mask[rowcounter * len + colcounter] = 0.0;
            else
            {
                h = sqrt(m_radius * m_radius - d*d);
                currentpos = Vector3(colcounter - m_radius, h, rowcounter - m_radius);
                lightray = light - currentpos;
                lightray.normalize();
                n = currentpos.getNormalized();
                dotproduct = max(lightray.dot(n), 0.0);

                coeff = max(lightray.dot(n), 0.0)*kd + ka/8.0;
                m_distri[rowcounter * len + colcounter].b = coeff *m_color.b;
                m_distri[rowcounter * len + colcounter].g = coeff *m_color.g;
                m_distri[rowcounter * len + colcounter].r = coeff *m_color.r;
                m_distri[rowcounter * len + colcounter].a = 255;
            }
        }
    }
}

void TubeBrush::paintOnce(int mouse_x, int mouse_y, Canvas2D* canvas)
{
    if(m_distri == NULL)
        makeBrushTip();

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
                    + alpha * m_mask[mask_idx] * m_distri[mask_idx].b;
            pix[convas_idx].g = (1 - alpha * m_mask[mask_idx]) * current_canvascolor.g
                    + alpha * m_mask[mask_idx] * m_distri[mask_idx].g;
            pix[convas_idx].r = (1 - alpha * m_mask[mask_idx]) * current_canvascolor.r
                    + alpha * m_mask[mask_idx] * m_distri[mask_idx].r;

            pix[convas_idx].a = 255;
        }
    }

}

void TubeBrush::clearPaint()
{
    delete[] m_distri;
    m_distri = NULL;
}
