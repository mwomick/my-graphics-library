#ifndef GBitmapShader_DEFINED
#define GBitmapShader_DEFINED

#include "../include/GShader.h"
#include "../include/GBitmap.h"
#include "../include/GMatrix.h"

#include "../utils/GColorUtils.h"
#include "../utils/GUtils.h"

#include "GBitmapCache.h"

class GBitmapShader : public GShader
{
public:

    typedef void (GBitmapShader::*ShaderProc32)(int x, int y, int count, GPixel row[]);

    GBitmapShader(const GBitmap& bitmap, const GMatrix matrix, GShader::TileMode mode) : fBitmap(bitmap), fLocalMatrix(matrix) {
        fDx = 1;
        fDy = 0;
        setContext(GMatrix{1, 0, 0, 0, 1, 0});
        switch(mode) {
            case GShader::TileMode::kMirror:
                fProc = &GBitmapShader::shadeRow_mirror;
                break;
            case GShader::TileMode::kRepeat:
                fProc = &GBitmapShader::shadeRow_repeat;
                break;
            default: 
                break;
        }
    }

    ~GBitmapShader() {
        #if false
        fCache->dump();
        #endif
    }

    bool isOpaque() override;
    bool setContext(const GMatrix& ctm) override;
    void shadeRow(int x, int y, int count, GPixel row[]) override;
    
    void shadeRow_clamp(int x, int y, int count, GPixel row[]) {        
        GPoint loc = fInverse * GPoint{x + .5f, y + .5f};
        
        for(int i = 0; i < count; i++)
        {
            GPixel* center = fBitmap.getAddr(clamp(loc.fX, fBitmap.width()-1), 
                                                clamp(loc.fY, fBitmap.height()-1));
            row[i] = *center;
            loc.fX += fDx;
            loc.fY += fDy;
        }
    }

    void shadeRow_repeat(int x, int y, int count, GPixel row[]) {
        GPoint loc = fInverse * GPoint{x + .5f, y + .5f};
        
        int xt; 
        int yt;

        for(int i = 0; i < count; i++)
        {
            xt= (int)loc.fX % (fBitmap.width());
            yt = (int)loc.fY % (fBitmap.height());

            if(xt < 0) {
                xt = fBitmap.width() + xt;
            }

            if(yt < 0) {
               yt = fBitmap.height() + yt; 
            }

           GPixel* center = fBitmap.getAddr(xt, yt);
                row[i] = *center;
                loc.fX += fDx;
                loc.fY += fDy;
        }
    }

    void shadeRow_mirror(int x, int y, int count, GPixel row[]) {
        GPoint loc = fInverse * GPoint{x + .5f, y + .5f};
        
        int xt, yt;
        int w = fBitmap.width() << 1;
        int h = fBitmap.height() << 1;
        for(int i = 0; i < count; i++)
        {
            xt = abs((int)loc.fX % w);
            yt = abs((int)loc.fY % h);

            if(xt >= fBitmap.width()) {
                xt = fBitmap.width() + ~(xt - fBitmap.width());
            }

            if(yt >= fBitmap.height()) {
                yt = fBitmap.height() + ~(yt - fBitmap.height());
            }

            GPixel* center = fBitmap.getAddr(xt, yt);
            row[i] = *center;
            loc.fX += fDx;
            loc.fY += fDy;
        }
    }

private:
    float                   fDx;
    float                   fDy;
    const GBitmap           fBitmap;
    const GMatrix           fLocalMatrix;
    GMatrix                 fInverse;
    ShaderProc32            fProc = &GBitmapShader::shadeRow_clamp;
    GShaderCache*           fCache = nullptr;

};
#endif