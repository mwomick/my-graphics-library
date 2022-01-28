#include "GBilerpShader.h"

bool GBilerpShader::isOpaque()
{
    return fBitmap.isOpaque();
}

bool GBilerpShader::setContext(const GMatrix& ctm)
{
    bool res = (ctm * fLocalMatrix).invert(&fInverse);
    
    fDx = fInverse[GMatrix::SX];
    fDy = fInverse[GMatrix::KY];

    return res;
}

void GBilerpShader::shadeRow(int x, int y, int count, GPixel row[])
{ 
    (this->*fProc)(x, y, count, row);
}