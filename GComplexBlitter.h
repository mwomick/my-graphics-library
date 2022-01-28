#ifndef GComplexBlitter_DEFINED
#define GComplexBlitter_DEFINED

#include "GBlitter.h"
#include "utils/GMemUtils.h"

#define FLIP8 0xFF
#define COPY8 0x00
// #define MAX_MISSES -8

class GComplexBlitter : public GBlitter {
public:
    GComplexBlitter(const GBitmap& bitmap, 
                    const GPaint& paint, 
                    CoeffRec rec) : fDevice(bitmap) { 
        paint.getColor().pinToUnit();
        fSrc = raw_mul_pack(paint.getColor());
        fSrc = premul_alpha(fSrc, GPixel_GetA(fSrc));
        
        switch(rec.fSrc) {
            case GBlendModeCoeff::kDa:
                fUsesDa = true;
                break;
            case GBlendModeCoeff::kIDa:
                fUsesDa = true;
                fFlipDa = FLIP8;
                break;
            case GBlendModeCoeff::kOne:
                fSrcCoeff = 0xFF;
                break;
            case GBlendModeCoeff::kZero:
                break;
            default: 
                printf("error - invalid GBlendModeCoeff\n");
                assert(false);
                break;
        }
        
        switch(rec.fDst) {
            case GBlendModeCoeff::kZero:
                break;
            case GBlendModeCoeff::kSa:
                fDstCoeff = GPixel_GetA(fSrc);
                break;
            case GBlendModeCoeff::kISa:
                fDstCoeff = ~GPixel_GetA(fSrc);
                break;
            case GBlendModeCoeff::kOne:
                fDstCoeff = 0xFF;
                break;
            default:
                printf("error - invalid GBlendModeCoeff\n");
                assert(false); 
                break;
        }
    }

    static int count_static_run(GPixel* dst, int count) {
        GPixel a = *dst;
        int i = 0;
        while(*dst++ == a && ++i<count) {}
        return i; 
    }

    static int count_static_run2(GPixel* dst, int count) {
        /** TODO: two pixels at a time */
        return 1;
    }


    void blitH(int x, int y, int w) override {
        GPixel* dst = fDevice.getAddr(x, y);
        GPixel p = quad_mul_div255(fSrc, fSrcCoeff);
        
        if(fUsesDa) {
            while(w > 0) {
                int run = count_static_run(dst, w);
                if (run < 2) { goto da_one_at_a_time; }
                w -= run;
                quickset(dst, 
                            quad_mul_div255(*dst, fDstCoeff) + 
                            quad_mul_div255(fSrc, fFlipDa ^ (*dst >> 24)), 
                            run);
                dst += run;
            }
            return;
            da_one_at_a_time:
            while(w-->0) {
                *dst = quad_mul_div255(*dst, fDstCoeff) + 
                            quad_mul_div255(fSrc, fFlipDa ^ (*dst >> 24));
                dst++;
            }
        }
        else {
            while(w > 0) {
                int run = count_static_run(dst, w);
                if(run < 2) { goto one_at_a_time; }
                w -= run;
                quickset(dst, quad_mul_div255(*dst, fDstCoeff) + p, run);
                dst += run; 
            }
            return;
            one_at_a_time:
            while(w-->0) {
                *dst = quad_mul_div255(*dst, fDstCoeff) + p;
                dst++;
            }
        }
    }

private: 
    GPixel              fSrc;
    bool                fUsesDa = false;
    uint8_t             fFlipDa = COPY8;
    uint8_t             fSrcCoeff = 0;
    uint8_t             fDstCoeff = 0;
    const GBitmap&      fDevice;
};

#endif