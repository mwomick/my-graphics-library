/*
 * GBitmapDevice.h
 * 
 * Michael Womick
 * COMP 475 - 2D Computer Graphics 
 * Prof. Mike Reed
 * 08/20/2021
 */

#ifndef GBitmapDevice_DEFINED
#define GBitmapDevice_DEFINED

#include "include/GPaint.h"
#include "include/GRect.h"
#include "GDraw.h"
#include "GBlitter.h"
#include "GScan.h"
/* 
 * GBitmapDevice is the interface for the canvas to draw the geometry and write it to memory.
 */
class GBitmapDevice {
public:
    GBitmapDevice(const GBitmap& bitmap) : fBitmap(bitmap), fBounds(GRect::MakeWH(bitmap.width(), bitmap.height()).round()) {}
    
    float width() const { return fBitmap.width(); }
    float height() const { return fBitmap.height(); }

    /**
     * \param rect pre-rounded rect
     * \param paint 
    */
    void drawIRect(const GIRect& rect, const GPaint& paint) {
        GBlitter* blitter = GBlitter::Choose(fBitmap, paint);
        if(blitter) {
            GScan::FillRect(rect, fBounds, blitter);
        }
    }

    void drawPath(const GPath& path, const GPaint& paint) {
        GBlitter* blitter = GBlitter::Choose(fBitmap, paint);
        if(blitter) {
            GScan::FillPath(path, fBounds, blitter);
        }
    }

    void drawPoints(const GPoint* points, int count, const GPaint& paint, bool isConvex = true) {
        GBlitter* blitter = GBlitter::Choose(fBitmap, paint);
        if(blitter) {
            GScan::FillConvexPoly(points, count, fBounds, blitter);
        }
    } 

    void drawQuad(const GPoint verts[4], const GColor colors[4], const GPoint texs[4],
                              int level, const GPaint& paint, const GMatrix& ctm) {
        GDraw draw = GDraw();
        draw.fDst = &fBitmap;
        draw.fBounds = &fBounds;
        draw.drawQuad(verts, colors, texs, level, paint, ctm); 
    }

    void drawMesh(const GPoint verts[], const GColor colors[], const GPoint texs[],
                              int count, const int indices[], const GPaint& paint, const GMatrix& ctm) {
        GDraw draw = GDraw();
        draw.fDst = &fBitmap;
        draw.fBounds = &fBounds;
        draw.drawVertices(verts, colors, texs, count, indices, paint, ctm); 
    }

    void drawPaint(const GPaint& paint) {
        this->drawIRect(fBounds, paint);
    }

private:
    const GBitmap&    fBitmap;
    const GIRect      fBounds;
};

#endif