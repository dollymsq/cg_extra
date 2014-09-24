#include "FInvert.h"

FInvert::FInvert()
    :Filter()
{
    m_numoffilter = 0;
}

void FInvert::doFilter(BGRA* cancol)
{
    int canvasIndex;
    for(int col = m_minY; col < m_maxY +1; col++)
    {
        for(int row = m_minX; row < m_maxX +1; row++)
        {
            canvasIndex = col * oriWidth + row;
            cancol[canvasIndex].b = 255 - cancol[canvasIndex].b;
            cancol[canvasIndex].g = 255 - cancol[canvasIndex].g;
            cancol[canvasIndex].r = 255 - cancol[canvasIndex].r;
        }
    }
}
