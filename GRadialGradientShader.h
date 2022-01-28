#ifndef GRadialGradientShader_DEFINED
#define GRadialGradientShader_DEFINED

#include "../include/GShader.h"
#include "../include/GMatrix.h"
#include "../utils/GMemUtils.h"
#include "../utils/GColorUtils.h"
#include "../utils/GUtils.h"

class GRadialGradientShader : public GShader
{   
float dist(GPoint a, GPoint b) {
    return sqrt(pow(a.x() - b.x(), 2) + pow(a.y() - b.y(), 2));
}
float magnitude(float x, float y) {
    return (sqrt(x*x + y*y));
}

public:
    typedef void (GRadialGradientShader::*ShaderProc32)(int x, int y, int count, GPixel row[]);

    GRadialGradientShader(GPoint center, float radius, const GColor colors[], int count,
                                    GShader::TileMode mode) : fCount(count), fRadius(radius), fCenter(center) {
        fColors = (GColor*) malloc(count * sizeof(GColor));
        memcpy(fColors, colors, count * sizeof(GColor));

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
                fProc = &GRadialGradientShader::shadeRow_mirror;
                break;
            case GShader::TileMode::kRepeat:
                fProc = &GRadialGradientShader::shadeRow_repeat;
                break;
            default: 
                break;
        }

    }

    void shadeRow_clamp(int x, int y, int count, GPixel row[]) {
        GPoint loc = fInverse * GPoint{x + .5, y + .5};
        GPoint cntr = fInverse * fCenter;
        float dx = fInverse[GMatrix::SX];
        float distance = dist(cntr, loc)/fRadius;
        for(int i = 0; i < count; i++) {
            float x = distance / (fRadius/ fCount);
            int x_flr = floor(x);
            GColor c = (1 - (x - x_flr)) * fColors[clamp(x, fCount-1)] + (x - x_flr) * fColors[clamp(x+1, fCount-1)]; 
            row[i] = raw_mul_pack(c);
            row[i] = premul_alpha(row[i], GPixel_GetA(row[i]));
            loc.fX += dx;
            distance = dist(fCenter, loc);
        }
    }

    void shadeRow_repeat(int x, int y, int count, GPixel row[]) {
        GPoint loc = fInverse * GPoint{x + .5, y + .5};
        GPoint cntr = fInverse * fCenter;
        float dx = fInverse[GMatrix::SX];
        float distance = dist(cntr, loc);
        for(int i = 0; i < count; i++) {
            float x = distance / (fRadius/ fCount);
            int x_flr = floor(x);
            GColor c = (1 - (x - x_flr)) * fColors[x_flr%(fCount)] + (x - x_flr) * fColors[(x_flr+1) % (fCount)]; 
            row[i] = raw_mul_pack(c);
            row[i] = premul_alpha(row[i], GPixel_GetA(row[i]));
            loc.fX += dx;
            distance = dist(fCenter, loc);
        } 
    }

    void shadeRow_mirror(int x, int y, int count, GPixel row[]) {
    }

    bool isOpaque() override;
    bool setContext(const GMatrix& ctm) override;
    void shadeRow(int x, int y, int count, GPixel row[]) override;

private:
    GPoint                  fCenter;
    float                   fStep;
    GColor*                 fColors;
    int                     fCount;
    const GMatrix           fLocalMatrix;
    GMatrix                 fInverse;
    GMatrix                 fUnit;

    bool                    fIsOpaque = true;

    float                   fRadius;
    float                   fRadPerCol;
    GColor                  fDcDx;
    GColor                  fDcDy;
    GPixel                  fCurr;

    ShaderProc32            fProc = &GRadialGradientShader::shadeRow_clamp;

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