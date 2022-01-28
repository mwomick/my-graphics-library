#ifndef GPathMakers_DEFINED
#define GPathMakers_DEFINED

#include "include/GPath.h"

/*
 *  Modeled after Skia's GPathMakers.h
 */

template <unsigned N> class GPath_PointIterator {
public:
    GPath_PointIterator(GPath::Direction dir) 
    : fCurrent(0)
    , fAdvance(dir == GPath::kCW_Direction ? 1 : N-1) { }

    const GPoint& current() const {
        return fPts[fCurrent];
    }

    const GPoint& next() {
        fCurrent = (fCurrent + fAdvance) % N;
        return this->current();
    }

protected:
    GPoint fPts[N];

private:
    unsigned fCurrent;
    unsigned fAdvance;
};

class GPath_RectPointIterator : public GPath_PointIterator<4> {
public:
    GPath_RectPointIterator(const GRect& rect, GPath::Direction dir) 
    : GPath_PointIterator(dir)  {
        fPts[0] = GPoint::Make(rect.fLeft, rect.fTop);
        fPts[1] = GPoint::Make(rect.fRight, rect.fTop);
        fPts[2] = GPoint::Make(rect.fRight, rect.fBottom);
        fPts[3] = GPoint::Make(rect.fLeft, rect.fBottom);
    }
};

class GPath_CirclePointIterator : public GPath_PointIterator<16> {
private:
    const GPoint gUnitCircle[16] = {
        {1,0},
        {1,-0.41421355452505154},
        {0.7071067811865476,-0.7071067811865476},
        {0.41421355452505154,-1},
        {0,-1},
        {-0.41421355452505154,-1},
        {-0.7071067811865476,-0.7071067811865476},
        {-1,-0.41421355452505154},
        {-1,0},
        {-1,0.41421355452505154},
        {-0.7071067811865476,0.7071067811865476},
        {-0.41421355452505154,1},
        {0,1},
        {0.41421355452505154,1},
        {0.7071067811865476,0.7071067811865476},
        {1,0.41421355452505154}
    };
public:
    GPath_CirclePointIterator(GPoint center, float radius, GPath::Direction dir)
    : GPath_PointIterator(dir)  {
        memcpy(fPts, gUnitCircle, sizeof(fPts));
        GMatrix m = GMatrix::Translate(center.x(), center.y()) * GMatrix::Scale(radius, radius);
        m.mapPoints(fPts, 16);
    }
};

#endif