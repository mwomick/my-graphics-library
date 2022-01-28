#include "GGradientShader.h"

bool GGradientShader::isOpaque() {
    return fIsOpaque;
}

bool GGradientShader::setContext(const GMatrix& ctm) {
    return (ctm * fUnit * fLocalMatrix).invert(&fInverse);
}

void GGradientShader::shadeRow(int x, int y, int count, GPixel row[]) {
    (this->*fProc)(x, y, count, row);
}

std::unique_ptr<GShader> GCreateLinearGradient(GPoint p0, GPoint p1, const GColor colors[], int count, GShader::TileMode mode) {
    if(count < 1) {
        return nullptr;
    }
    else if(count == 1) {
        return std::unique_ptr<GShader>(new GColorShader(colors[0]));
    }
    else {
        return std::unique_ptr<GShader>(new GGradientShader(p0, p1, colors, count, mode));

    }
}