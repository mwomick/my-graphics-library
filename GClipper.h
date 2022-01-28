#ifndef GClipper_DEFINED
#define GClipper_DEFINED

#include "include/GRect.h"
#include "include/GPoint.h"

#include <vector>

class GClipper
{
    public:
        GClipper(const GIRect&);
        uint8_t ComputeOutCode(GPoint);
        void ClipPoints(const GPoint* pts, int count, std::vector<GPoint>& dest);

    private:
        const GIRect&   fBounds;

};

#endif