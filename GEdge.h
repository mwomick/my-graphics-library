/**
 * \file GEdge.h
 * 
 * \author Michael Womick
 * COMP 475 - 2D Computer Graphics 
 * Prof. Mike Reed
 * \date 08/23/2021
 */

#ifndef GEdge_DEFINED
#define GEdge_DEFINED

#include "include/GPoint.h"
#include "include/GRect.h"
/**
 * \struct GEdge
 * \brief Polygon component
 */
struct GEdge {
public:

    float       fX;
    float       fDX; 
    int32_t     fTop;
    int32_t     fBottom;
    bool        fWinding = false;

    /**
     * Initializes a linear edge between two points.
     * 
     * \returns false if line is horizontal or outside the bounds
     */
    bool setLine(const GPoint& p0, const GPoint& p1);  

#ifdef G_DEBUG
    void dump() {
        printf("edge:\n\tfX: %f\n\tfDX: %f\n\tfTop: %d\n\tfBottom: %d\n\tfWinding: %d\n", fX, fDX, fTop, fBottom, fWinding);
    }
#else
    void dump() {}
#endif

};

#endif