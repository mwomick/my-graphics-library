#include "include/GPath.h"
#include "include/GMatrix.h"
#include "GPathMakers.h"

GPath& GPath::addRect(const GRect& r, Direction dir) {
    GPath_RectPointIterator iter(r, dir);
    
    this->moveTo(iter.current());
    this->lineTo(iter.next());
    this->lineTo(iter.next());
    this->lineTo(iter.next());

    return *this;
}

GPath& GPath::addPolygon(const GPoint pts[], int count) {
    this->moveTo(pts[0]);
    for(int i = 1; i < count; i++) {
        this->lineTo(pts[i]);
    }
    return *this;
}

static GRect makeBounds(const GPoint points[], int count) {
    if(count < 1) return GRect();
    float minX = points[0].x();
    float minY = points[0].y();
    float maxX = points[0].x();
    float maxY = points[0].y();

    for(int i = 1; i < count; i++) {
        if(points[i].x() < minX) {
            minX = points[i].x();
        }
        else if(points[i].x() > maxX) {
            maxX = points[i].x();
        }
        if(points[i].y() < minY) {
            minY = points[i].y();
        }
        else if(points[i].y() > maxY) {
            maxY = points[i].y();
        }
    }

    return GRect::MakeLTRB(minX, minY, maxX, maxY);
}


GRect GPath::bounds() const {
    return makeBounds(&fPts[0], fPts.size());
}

void GPath::transform(const GMatrix& m) {
    m.mapPoints(&fPts[0], fPts.size());
}

GPath& GPath::addCircle(GPoint center, float radius, Direction dir) {
    GPath_CirclePointIterator iter = GPath_CirclePointIterator(center, radius, dir);
    this->moveTo(iter.current());
    for(int i = 0; i < 8; i++) {
        this->quadTo(iter.next(), iter.next());
    }
    return *this;
}

static inline GPoint interp(GPoint a, GPoint b, float t) {
    return a + (b-a) * t;
}

void GPath::ChopQuadAt(const GPoint src[3], GPoint dst[5], float t) {
    GPoint p0 = src[0];
    GPoint p1 = src[1];
    GPoint p2 = src[2];

    GPoint p01 = interp(p0, p1, t);
    GPoint p12 = interp(p1, p2, t);

    dst[0] = p0;
    dst[1] = p01;
    dst[2] = interp(p01, p12, t);
    dst[3] = p12;
    dst[4] = p2;
}

void GPath::ChopCubicAt(const GPoint src[4], GPoint dst[7], float t) {

    GPoint p0 = src[0];
    GPoint p1 = src[1];
    GPoint p2 = src[2];
    GPoint p3 = src[3];

    GPoint p01 = interp(p0, p1, t);
    GPoint p12 = interp(p1, p2, t);
    GPoint p23 = interp(p2, p3, t);

    dst[0] = p0;
    dst[1] = p01;
    dst[2] = interp(p01, p12, t);
    dst[4] = interp(p12, p23, t);
    dst[3] = interp(dst[2], dst[4], t);
    dst[5] = p23;
    dst[6] = p3;
}
