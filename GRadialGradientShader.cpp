#include "GRadialGradientShader.h"

bool GRadialGradientShader::isOpaque() {
    return fIsOpaque;
}

bool GRadialGradientShader::setContext(const GMatrix& ctm) {
    return (ctm * fUnit * fLocalMatrix).invert(&fInverse);
}

void GRadialGradientShader::shadeRow(int x, int y, int count, GPixel row[]) {
    (this->*fProc)(x, y, count, row);
}