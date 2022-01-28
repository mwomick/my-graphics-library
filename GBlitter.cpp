#include "GBlitter.h"

void GBlitter::blitRect(int l, int t, int r, int b) {
    for(int w = r-l; t < b; t++) {
        blitH(l, t, w);
    }
}

#include "GSimpleBlitter.h"
#include "GComplexBlitter.h"
#include "GShaderBlitter.h"

CoeffRec get_rec(const GPaint& src) {
    CoeffRec rec = gCoeffs[static_cast<int>(src.getBlendMode())]; 
    // ideally we would check is shader srca is constant
    if(!src.getShader() && src.getAlpha() > .999f || src.getShader() && src.getShader()->isOpaque()) {
        if( rec.fDst == GBlendModeCoeff::kSa ) { rec.fDst = GBlendModeCoeff::kOne; }
        else if( rec.fDst == GBlendModeCoeff::kISa ) { rec.fDst = GBlendModeCoeff::kZero; }
    }
    else if( !src.getShader() && src.getAlpha() < 0.001f ) {
        if( rec.fDst == GBlendModeCoeff::kSa ) { rec.fDst = GBlendModeCoeff::kZero; }
        else if( rec.fDst == GBlendModeCoeff::kISa ) { rec.fDst = GBlendModeCoeff::kOne; } 
    }
    return rec;
}

GBlitter* GBlitter::Choose(const GBitmap& bitmap, const GPaint& paint) {
    /** TODO: Consider opacity of shader */

    CoeffRec rec = get_rec(paint);

    if(rec.fDst == GBlendModeCoeff::kOne && rec.fSrc == GBlendModeCoeff::kZero) { return nullptr; }

    if(rec.fDst == GBlendModeCoeff::kZero) {
        if(rec.fSrc == GBlendModeCoeff::kOne) {
            if(paint.getShader()) {
                return new GShaderBlitter(bitmap, paint);
            }
            else {
                GPaint p(paint);
                p.setBlendMode(GBlendMode::kSrc);
                return new GSimpleBlitter(bitmap, p);
            }
        }
        if(rec.fSrc == GBlendModeCoeff::kZero) {
            GPaint p(paint);
            p.setBlendMode(GBlendMode::kClear);
            return new GSimpleBlitter(bitmap, p);
        }
    }

    if(paint.getShader()) {
        return new GCompositeShaderBlitter(bitmap, paint, rec);
    }

    return new GComplexBlitter(bitmap, paint, rec);
}