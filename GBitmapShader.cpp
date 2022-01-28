#include "GBitmapShader.h"

bool GBitmapShader::isOpaque()
{
    return fBitmap.isOpaque();
}

bool GBitmapShader::setContext(const GMatrix& ctm)
{
    bool res = (ctm * fLocalMatrix).invert(&fInverse);
    
    fDx = fInverse[GMatrix::SX];
    fDy = fInverse[GMatrix::KY];

    /* 
    if(fCache) {
        free(fCache);
    }

    int width = abs((int)GRoundToInt(((float)fBitmap.width()) / fInverse[GMatrix::SX]));
    int height;
    if(fInverse[GMatrix::KY] == 0) {
        height = fBitmap.height();
    }
    else {
        height = abs((int)GRoundToInt(((float)fBitmap.height()) / fInverse[GMatrix::KY]));
    }
    
    fCache = new GShaderCache(width, height);
    */
   
    return res;
}

void GBitmapShader::shadeRow(int x, int y, int count, GPixel row[])
{ 
    (this->*fProc)(x, y, count, row);
}

std::unique_ptr<GShader> GCreateBitmapShader(const GBitmap& bitmap, const GMatrix& localM, GShader::TileMode mode)
{
    if (!bitmap.pixels())
        return nullptr;

    return std::unique_ptr<GShader>(new GBitmapShader(bitmap, localM, mode));
}