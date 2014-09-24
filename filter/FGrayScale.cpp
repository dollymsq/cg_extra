#include "FGrayScale.h"

FGrayScale::FGrayScale()
    :Filter()
{
    m_numoffilter = 0;
}

void FGrayScale::doFilter(BGRA* cancol)
{
    int canvasIndex;
    for(int col = m_minY; col < m_maxY +1; col++)
    {
        for(int row = m_minX; row < m_maxX +1; row++)
        {
            canvasIndex = col * oriWidth + row;
            cancol[canvasIndex].r = 0.299 * cancol[canvasIndex].r
                    + 0.587 * cancol[canvasIndex].g
                    + 0.114 * cancol[canvasIndex].b;
            cancol[canvasIndex].g = cancol[canvasIndex].r;
            cancol[canvasIndex].b = cancol[canvasIndex].r;
        }
    }
}
