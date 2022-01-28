#ifndef GDraw_DEFINED
#define GDraw_DEFINED

#include "include/GPoint.h"
#include "include/GPaint.h"
#include "include/GBitmap.h"
#include "include/GRect.h"
#include "include/GMatrix.h"

class GDraw {
public:
    GDraw() {}

    void drawVertices(const GPoint verts[], const GColor colors[], const GPoint texs[],
                              int count, const int indices[], const GPaint& paint, const GMatrix& ctm);

    void drawQuad(const GPoint verts[4], const GColor colors[4], const GPoint texs[4],
                              int level, const GPaint& paint, const GMatrix& ctm);

    const GBitmap*       fDst{nullptr};
    const GIRect*        fBounds{nullptr};
};

#endif