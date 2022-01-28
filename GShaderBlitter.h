/*
 * GBlitter.h
 * 
 * Michael Womick
 * COMP 475 - 2D Computer Graphics 
 * Prof. Mike Reed
 * 09/15/2021
 */

#ifndef GShaderBlitter_DEFINED
#define GShaderBlitter_DEFINED

#include "GBlitter.h"
#include "GComplexBlitter.h"

#include "include/GBitmap.h"
#include "include/GPaint.h"
#include "include/GRect.h"
#include "include/GShader.h"


class GShaderBlitter : public GBlitter {
public:

    GShaderBlitter(const GBitmap& bitmap, const GPaint& paint) : fDevice(bitmap) { 
        fShader = paint.getShader();
    }

    void blitH(int x, int y, int w) override {
        uint32_t* dst = fDevice.getAddr(x, y);
        fShader->shadeRow(x, y, w, dst);
    }

private:
    const GBitmap&      fDevice;
    GShader*            fShader;
};

#define FLIP8 0xFF
#define COPY8 0x00

class GCompositeShaderBlitter : public GBlitter {
public:
    GCompositeShaderBlitter(const GBitmap& bitmap, const GPaint& paint, CoeffRec rec) : fDevice(bitmap) {
        fShader = paint.getShader();
        
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
                fUsesSa = true;
                break;
            case GBlendModeCoeff::kISa:
                fUsesSa = true;
                fFlipSa = FLIP8;
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

    void blitH(int x, int y, int w) override {
        GPixel* dst = fDevice.getAddr(x, y);
        GPixel storage [w];
        GPixel* src = storage;
        fShader->shadeRow(x, y, w, storage);

        if(fUsesDa && fUsesSa) {
            while(w-->0) {
                *dst = quad_mul_div255(*dst, fFlipSa ^ (*src >> 24)) + 
                            quad_mul_div255(*src, fFlipDa ^ (*dst >> 24));
                dst++;
                src++;
            }
        }
        else if(fUsesDa) {
            while(w-->0) {
                *dst = quad_mul_div255(*dst, fDstCoeff) + 
                            quad_mul_div255(*src, fFlipDa ^ (*dst >> 24));
                dst++;
                src++;
            }
        }
        else if(fUsesSa) {
            while(w-->0) {
                *dst = quad_mul_div255(*dst, fFlipSa ^ (*src >> 24)) + 
                            quad_mul_div255(*src, fSrcCoeff);
                dst++;
                src++;
            }
        }



    }

private:
    const GBitmap&      fDevice;
    GShader*            fShader;

    bool                fUsesSa = false;
    uint8_t             fFlipSa = COPY8;
    bool                fUsesDa = false;
    uint8_t             fFlipDa = COPY8;
    uint8_t             fSrcCoeff = 0;
    uint8_t             fDstCoeff = 0;

};

#endif