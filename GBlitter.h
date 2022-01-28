#ifndef GBlitter_DEFINED
#define GBlitter_DEFINED

#include "include/GBitmap.h"
#include "GPaint.h"

enum class GBlendModeCoeff {
    kZero,
    kOne,
    kDa,
    kSa,
    kIDa,
    kISa,
};

struct CoeffRec {
    GBlendModeCoeff fSrc;
    GBlendModeCoeff fDst;
};

const CoeffRec gCoeffs[] = {
    { GBlendModeCoeff::kZero,    GBlendModeCoeff::kZero }, // clear
    { GBlendModeCoeff::kOne,     GBlendModeCoeff::kZero }, // src
    { GBlendModeCoeff::kZero,    GBlendModeCoeff::kOne  }, // dst
    { GBlendModeCoeff::kOne,     GBlendModeCoeff::kISa  }, // src-over
    { GBlendModeCoeff::kIDa,     GBlendModeCoeff::kOne  }, // dst-over
    { GBlendModeCoeff::kDa,      GBlendModeCoeff::kZero }, // src-in
    { GBlendModeCoeff::kZero,    GBlendModeCoeff::kSa   }, // dst-in
    { GBlendModeCoeff::kIDa,     GBlendModeCoeff::kZero }, // src-out
    { GBlendModeCoeff::kZero,    GBlendModeCoeff::kISa  }, // dst-out
    { GBlendModeCoeff::kDa,      GBlendModeCoeff::kISa  }, // src-atop
    { GBlendModeCoeff::kIDa,     GBlendModeCoeff::kSa   }, // dst-atop
    { GBlendModeCoeff::kIDa,     GBlendModeCoeff::kISa  }, // xor
};

/*
 * GBlitter objects are responsible for actually writing to the bitmap with the most efficient operation.
 */
class GBlitter {
public:
    virtual ~GBlitter() {};    

    virtual void blitH(int x, int y, int w) = 0;
    virtual void blitRect(int l, int t, int r, int b);

    static GBlitter* Choose(const GBitmap& bitmap, const GPaint& paint);

};

#endif