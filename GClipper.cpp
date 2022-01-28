/* 
*  2021 Michael Womick
*  COMP 475, Mike Reed
*  University of North Carolina at Chapel Hill
*/

#include "GClipper.h"

//  ===========  LUT ============
//  0110    |   0010    |   0011
//  --------+-----------+--------
//  0100    |   0000    |   0001
//  --------+-----------+--------
//  1100    |   1000    |   1001
//  =============================

const uint8_t INSIDE = 0;
const uint8_t RIGHT = 1;
const uint8_t TOP = 2;
const uint8_t LEFT = 4;
const uint8_t BOTTOM = 8;
const uint8_t CORNER = 16;

GClipper::GClipper(const GIRect& bounds) : fBounds(bounds) {}

uint8_t GClipper::ComputeOutCode(GPoint p) {    
    if(p.x() < fBounds.left())
    {
        if(p.y() > fBounds.bottom())    return(28);
        if(p.y() < fBounds.top())       return(22);
        return(4);
    }
    else if(p.x() > fBounds.right())
    {
        if(p.y() > fBounds.bottom())    return(21);
        if(p.y() < fBounds.top())       return(15);
        return(1);
    }
    else if(p.y() > fBounds.bottom())        return(8);
    else if(p.y() < fBounds.top())           return(2);
    else return(0);
}

void GClipper::ClipPoints(const GPoint* pts, int count, std::vector<GPoint>& dest)
{
    dest.reserve(count + (count >> 1));
    GPoint p0 = pts[count-1];
    GPoint p1 = pts[0];
    bool set = 0;
    uint8_t c0 = ComputeOutCode(p0);
    uint8_t c1 = ComputeOutCode(p1);

    int ii = 0;
    while(ii < count)
    {
        if((c0|c1) == INSIDE)                   // both inside: quick accept
        {
            dest.push_back(p0);
            dest.push_back(p1);
        }
        else if((c0&c1) != 0)                   // both outside: quick reject
        {
            if((c1&CORNER) != 0)
            {
                set = 1;
                switch(c1) {
                    case 28: dest.emplace_back(GPoint::Make(fBounds.left(), fBounds.bottom()));       break;
                    case 22: dest.emplace_back(GPoint::Make(fBounds.left(), fBounds.top()));          break;
                    case 21: dest.emplace_back(GPoint::Make(fBounds.right(), fBounds.bottom()));      break;
                    case 15: dest.emplace_back(GPoint::Make(fBounds.right(), fBounds.top()));         break;
                }
            }
        }
        else
        {
            GPoint dst[2];   
            uint8_t tmp = c0;
            dst[0] = p0;
            dst[1] = p1;
            bool i = 0;
            if(tmp == 0)
            {
                tmp = c1;
                i = 1;
            }
            while(true)
            {
                if((tmp & LEFT) != 0)
                {
                    dst[i].fY = dst[i].fY + (fBounds.left()-dst[i].x())*(dst[1].y()-dst[0].y())/(dst[1].x()-dst[0].x());
                    dst[i].fX = fBounds.left();
                }
                else if((tmp & RIGHT) != 0)
                {
                    dst[i].fY = dst[i].fY + (fBounds.right()-dst[i].x())*(dst[1].y()-dst[0].y())/(dst[1].x()-dst[0].x());
                    dst[i].fX = fBounds.right();
                }

                if((tmp & BOTTOM) != 0)
                {
                    dst[i].fX = dst[i].fX + (fBounds.bottom()-dst[i].y())*(dst[1].x()-dst[0].x())/(dst[1].y()-dst[0].y());
                    dst[i].fY = fBounds.bottom();
                }
                else if((tmp & TOP) != 0)
                {
                    dst[i].fX = dst[i].fX + (fBounds.top()-dst[i].y())*(dst[1].x()-dst[0].x())/(dst[1].y()-dst[0].y());
                    dst[i].fY = fBounds.top();
                }

                if(dst[i].fY < fBounds.top())                dst[i].fY = fBounds.top();
                else if(dst[i].fY > fBounds.bottom())        dst[i].fY = fBounds.bottom();
                if(dst[i].fX < fBounds.left())               dst[i].fX = fBounds.left();
                else if(dst[i].fX > fBounds.right())         dst[i].fX = fBounds.right();

                if(i || !c1) break;
                i = true;
                tmp = c1;
            }
            if(set)
            {
                dest.push_back(dst[0]);
                set = 0;
            }
            dest.push_back(dst[0]);
            dest.push_back(dst[1]);
        }
        ii++;
        p0 = p1;

        // Skip segments less than 1 pixel in length
        while(abs(pts[ii].x()-p0.x()) + abs(pts[ii].y()-p0.y()) < 1) { ii++; }
        
        p1 = pts[ii];
        
        c0 = c1;
        c1 = ComputeOutCode(p1);

    }

}