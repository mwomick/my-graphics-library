#include "include/GFinal.h"
#include "GRadialGradientShader.h"
#include "GBilerpShader.h"

class MyFinal : public GFinal {
    std::unique_ptr<GShader> createRadialGradient(GPoint center, float radius,
                                                          const GColor colors[], int count,
                                                          GShader::TileMode mode) override {
        if(count < 1) {
            return nullptr;
        }
        else {
            return std::unique_ptr<GShader>(new GRadialGradientShader(center, radius, colors, count, mode));
        }
    }


    std::unique_ptr<GShader> createBilerpShader(const GBitmap& bitmap,
                                                        const GMatrix& localMatrix) override {
            return std::unique_ptr<GShader>(new GBilerpShader(bitmap, localMatrix));
    }

    void addLine(GPath* path, GPoint p0, GPoint p1, float width, CapType cap) override {
        float rad = width / 2;
        float dydx = (p1.fY - p0.fY) / (p1.fX - p0.fX);
        float norm = 1 / -dydx;
        float angle = atan(norm);
        float dx = cos(angle) * rad;
        float dy = sin(angle) * rad;

        switch(cap) {
            case GFinal::kRound:
                path->addCircle(p0, rad);
                 break;
            case GFinal::kSquare:
                GRect rect =  GRect::MakeWH(width, rad);
                GMatrix m = GMatrix::Translate(-2*dx, -2*dy) * GMatrix::Rotate(angle) * GMatrix::Translate(p0.x()+rad, p0.y()+rad);
                path->transform(m);
                path->addRect(rect);
                m.invert(&m);
                path->transform(m); 
                break;
        }

        path->moveTo(GPoint::Make(p0.fX + dx, p0.fY + dy)).
                lineTo(GPoint::Make(p0.fX - dx, p0.fY - dy)).
                lineTo(GPoint::Make(p1.fX - dx, p1.fY - dy)).
                lineTo(GPoint::Make(p1.fX + dx, p1.fY + dy));

        switch(cap) {
            case GFinal::kRound:
                path->addCircle(p1, rad);
                break;
            case GFinal::kSquare:
                GRect rect =  GRect::MakeWH(width, rad);
                GMatrix m = GMatrix::Translate(-dx, -dy) * GMatrix::Rotate(angle) * GMatrix::Translate(p1.x()+rad, p1.y()+rad);
                path->transform(m);
                path->addRect(rect);
                m.invert(&m);
                path->transform(m); 
                break;
        }
    }
};

std::unique_ptr<GFinal> GCreateFinal() {
    return std::unique_ptr<GFinal>(new MyFinal());
}