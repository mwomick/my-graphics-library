#ifndef GGradientShader_DEFINED
#define GGradientShader_DEFINED

#include "../include/GShader.h"
#include "../include/GMatrix.h"
#include "../utils/GMemUtils.h"
#include "../utils/GColorUtils.h"
#include "../utils/GUtils.h"

class GGradientShader : public GShader
{   
public:
    typedef void (GGradientShader::*ShaderProc32)(int x, int y, int count, GPixel row[]);

    GGradientShader(GPoint p0, GPoint p1, const GColor colors[], int count, GShader::TileMode mode) : fStart(p0), fEnd(p1), fCount(count) {
        fColors = (GColor*) malloc(count * sizeof(GColor));
        memcpy(fColors, colors, count * sizeof(GColor));

        float dx = abs(p1.x() - p0.x());
        float dy = abs(p1.y() - p0.y());
        fUnit = GMatrix(dx, -dy, p0.x(), dy, dx, p0.y());

        int i = 0;
        for(; i < count; i++)
        {
            colors[i].pinToUnit();
            if(colors[i].a < 1.F)
            {
                fIsOpaque = false;
                break;
            }
        }
        for(; i < count; i++)
        {
            colors[i].pinToUnit();
        }

        switch(mode) {
            case GShader::TileMode::kMirror:
                fProc = &GGradientShader::shadeRow_mirror;
                break;
            case GShader::TileMode::kRepeat:
                fProc = &GGradientShader::shadeRow_repeat;
                break;
            default: 
                break;
        }

    }

    void shadeRow_clamp(int x, int y, int count, GPixel row[]) {
        GPoint loc = fInverse * GPoint{x + .5, y + .5};
        float dx = fInverse[GMatrix::SX];
        for(int i = 0; i < count; i++) {
            float x = loc.x() * (fCount-1);
            int x_flr = floor(x);
            GColor c = (1 - (x - x_flr)) * fColors[clamp(x, fCount-1)] + (x - x_flr) * fColors[clamp(x+1, fCount-1)]; 
            row[i] = raw_mul_pack(c);
            row[i] = premul_alpha(row[i], GPixel_GetA(row[i]));
            loc.fX += dx;
        }
    }

    void shadeRow_repeat(int x, int y, int count, GPixel row[]) {
        GPoint loc = fInverse * GPoint{x + .5, y + .5};
        float dx = fInverse[GMatrix::SX];
        for(int i = 0; i < count; i++) {
            float x = loc.x() * (fCount-1);
            int x_flr = floor(x);
            GColor c = (1 - (x - x_flr)) * fColors[abs(x_flr % (fCount-1))] + (x - x_flr) * fColors[abs(x_flr+1 % (fCount-1))]; 
            row[i] = raw_mul_pack(c);
            row[i] = premul_alpha(row[i], GPixel_GetA(row[i]));
            loc.fX += dx;
        } 
    }

    void shadeRow_mirror(int x, int y, int count, GPixel row[]) {
       GPoint loc = fInverse * GPoint{x + .5, y + .5};
        float dx = fInverse[GMatrix::SX];
        for(int i = 0; i < count; i++) {
            float x = loc.x() * (fCount-1);
            int x_flr = floor(x);
            GColor c = (1 - (x - x_flr)) * fColors[abs(x_flr % (fCount-1))] + (x - x_flr) * fColors[abs(x_flr+1 % (fCount-1))]; 
            row[i] = raw_mul_pack(c);
            row[i] = premul_alpha(row[i], GPixel_GetA(row[i]));
            loc.fX += dx;
        }  
    }


    bool isOpaque() override;
    bool setContext(const GMatrix& ctm) override;
    void shadeRow(int x, int y, int count, GPixel row[]) override;

private:
    GPoint                  fStart;
    float                   fStep;
    GPoint                  fEnd;
    GColor*                 fColors;
    int                     fCount;
    const GMatrix           fLocalMatrix;
    GMatrix                 fInverse;
    GMatrix                 fUnit;

    bool                    fIsOpaque = true;

    float                   fDx;
    float                   fDy;
    GColor                  fDcDx;
    GColor                  fDcDy;
    GPixel                  fCurr;

    ShaderProc32            fProc = &GGradientShader::shadeRow_clamp;

};


class GColorShader : public GShader {
public:
    GColorShader(const GColor& color) {
        fColor = raw_mul_pack(color);
        fColor = premul_alpha(fColor, GPixel_GetA(fColor));
        if(color.a == 1) { fOpaque = true; }
    }

    void shadeRow(int x, int y, int count, GPixel row[]) override {
        quickset(row, fColor, count);
    }
    
    bool isOpaque() override {
        return fOpaque;
    }

    bool setContext(const GMatrix& ctm) override { return true; }

private:
    GPixel fColor;
    bool fOpaque = false;
 
};

#endif