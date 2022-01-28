#ifndef GBilerpShader_DEFINED
#define GBilerpShader_DEFINED

#include "../include/GShader.h"
#include "../include/GBitmap.h"
#include "../include/GMatrix.h"

#include "../utils/GColorUtils.h"
#include "../utils/GUtils.h"

class GBilerpShader : public GShader
{
public:

    GColor GPixel_ToColor(GPixel p) {
        return GColor::RGBA(GPixel_GetR(p)/255.f, 
                            GPixel_GetG(p)/255.f, 
                            GPixel_GetB(p)/255.f, 
                            GPixel_GetA(p)/255.f);
    }

    typedef void (GBilerpShader::*ShaderProc32)(int x, int y, int count, GPixel row[]);

    GBilerpShader(const GBitmap& bitmap, 
                    const GMatrix matrix, 
                    GShader::TileMode mode = GShader::kClamp) 
                    : fBitmap(bitmap), fLocalMatrix(matrix) {
        fDx = 1;
        fDy = 0;
        setContext(GMatrix{1, 0, 0, 0, 1, 0});
        switch(mode) {
            case GShader::TileMode::kMirror:
                fProc = &GBilerpShader::shadeRow_mirror;
                break;
            case GShader::TileMode::kRepeat:
                fProc = &GBilerpShader::shadeRow_repeat;
                break;
            default: 
                break;
        }
    }

    bool isOpaque() override;
    bool setContext(const GMatrix& ctm) override;
    void shadeRow(int x, int y, int count, GPixel row[]) override;
    
    void shadeRow_clamp(int x, int y, int count, GPixel row[]) {        
        GPoint loc = fInverse * GPoint{x+.5, y+.5};

        int w = fBitmap.width()-1;
        int h = fBitmap.height()-1;

        for(int i = 0; i < count; i++)
        {
            
            GColor ff = GPixel_ToColor(*fBitmap.getAddr(clamp(GRoundToInt(loc.fX)-1, w), 
                                                        clamp(GRoundToInt(loc.fY)-1, h)));
            GColor rf = GPixel_ToColor(*fBitmap.getAddr(clamp(GRoundToInt(loc.fX), w), 
                                                        clamp(GRoundToInt(loc.fY)-1, h)));
            GColor fr = GPixel_ToColor(*fBitmap.getAddr(clamp(GRoundToInt(loc.fX)-1, w), 
                                                        clamp(GRoundToInt(loc.fY), h)));
            GColor rr = GPixel_ToColor(*fBitmap.getAddr(clamp(GRoundToInt(loc.fX), w), 
                                                        clamp(GRoundToInt(loc.fY), h)));

            float ltc = loc.fX+.5 - clamp(floor(loc.fX+.5), w);
            float ttc = loc.fY+.5 - clamp(floor(loc.fY+.5), h);

            GColor x_upper = ff * (1-ltc) + rf * (ltc);
            GColor x_lower = fr * (1-ltc) + rr * (ltc);
            row[i] = raw_mul_pack(x_upper * (1-ttc) + x_lower * (ttc));

            loc.fX += fDx;
            loc.fY += fDy;
        }
    }

    void shadeRow_repeat(int x, int y, int count, GPixel row[]) {
        assert(false);
    }

    void shadeRow_mirror(int x, int y, int count, GPixel row[]) {
        assert(false);
    }

private:
    float                   fDx;
    float                   fDy;
    const GBitmap           fBitmap;
    const GMatrix           fLocalMatrix;
    GMatrix                 fInverse;
    ShaderProc32            fProc = &GBilerpShader::shadeRow_clamp;
};
#endif