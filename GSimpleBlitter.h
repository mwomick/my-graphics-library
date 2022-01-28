/*
 * GBlitter.h
 * 
 * Michael Womick
 * COMP 475 - 2D Computer Graphics 
 * Prof. Mike Reed
 * 08/20/2021
 */

#ifndef GSimpleBlitter_DEFINED
#define GSimpleBlitter_DEFINED

#include "GBlitter.h"

#include "include/GBitmap.h"
#include "include/GPaint.h"
#include "include/GRect.h"

#include "GBlitRow.h"

/*
 * GSimpleBlitter blits solid colors to the device.
 */
class GSimpleBlitter : public GBlitter {
public:

    GSimpleBlitter(const GBitmap& bitmap, const GPaint& paint) : fDevice(bitmap) { 
        switch(paint.getBlendMode()) {
            case GBlendMode::kSrc:
                paint.getColor().pinToUnit();
                fSrc = raw_mul_pack(paint.getColor());
                fSrc = premul_alpha(fSrc, GPixel_GetA(fSrc));
                fProc = GBlitRow::kProcs[GBlitRow::kSrc];
                break;
            case GBlendMode::kClear:
                fProc = GBlitRow::kProcs[GBlitRow::kClear];
                break;
            default:
                break;
        }
    }

    void blitH(int x, int y, int w) override {
        fProc(fDevice.getAddr(x, y), fSrc, w);
    }

    void blitRect(int l, int t, int r, int b) override {
        int step = fDevice.rowBytes() >> 2;
        int w = r - l;
        GPixel* Y = fDevice.getAddr(l, t);
        while(t++ < b) {
            fProc(Y, fSrc, w);
            Y += step;
        }
    }

private:
    const GBitmap&      fDevice;
    GPixel              fSrc = 0;
    GBlitRow::Proc32    fProc = GBlitRow::kProcs[GBlitRow::kDst];
};
#endif