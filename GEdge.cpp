/**
 * \file GScan.cpp
 * 
 * \author Michael Womick
 * COMP 475 - 2D Computer Graphics 
 * Prof. Mike Reed
 * \date 08/23/2021
 */

#include "GEdge.h"

bool GEdge::setLine(const GPoint& p0, const GPoint& p1) {
    fTop = GRoundToInt(p0.y());
    fBottom = GRoundToInt(p1.y());

    if(fTop == fBottom) { return false; }

    fX = p0.x();
    float y0 = p0.y();

    // set top and bottom
    if(fTop > fBottom) {
        using std::swap;
        swap(fTop, fBottom);
        y0 = p1.y();
        fX = p1.x();
        fWinding = true;
    }

    fDX = (p1.fX - p0.fX)/(p1.fY - p0.fY);

    fBottom -= 1;
    // update x to correspond to y position
    fX = fX + fDX * ((float)fTop+.5 - y0);
    
    return true;
}