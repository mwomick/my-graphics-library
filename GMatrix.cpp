#include "include/GMatrix.h"

GMatrix GMatrix::Translate(float tx, float ty)
{
    return GMatrix(1, 0, tx,
                    0, 1, ty);
}


GMatrix GMatrix::Scale(float sx, float sy)
{
    return GMatrix(sx, 0, 0,
                    0, sy, 0);
}


GMatrix GMatrix::Rotate(float radians)
{
    float a(cos(radians));
    float b(sin(radians));
    return GMatrix(a, -b, 0,
                    b, a, 0);
}


 GMatrix GMatrix::Concat(const GMatrix& a, const GMatrix& b)
{
    float ma (a[0] * b[0] + a[1] * b[3]);
    float mb (a[0] * b[1] + a[1] * b[4]);
    float mc (a[0] * b[2] + a[1] * b[5] + a[2]);
    float md (a[3] * b[0] + a[4] * b[3]);
    float me (a[3] * b[1] + a[4] * b[4]);
    float mf (a[3] * b[2] + a[4] * b[5] + a[5]);
    return GMatrix(ma, mb, mc,
                    md, me, mf);
}

GMatrix::GMatrix() : fMat{1, 0, 0, 0, 1, 0} {}

bool GMatrix::invert(GMatrix* inverse) const
{
    float det(fMat[0] * fMat[4] - fMat[3] * fMat[1]);
    if(det == 0) return false;
    else
    {
        float inv(1.0F / det);

        float tx (fMat[1]*fMat[5] - fMat[2]*fMat[4]);
        float ty (fMat[2]*fMat[3] - fMat[0]*fMat[5]);
        
        float a(fMat[4] * inv);
        float c(tx * inv);
        float e(fMat[0] * inv);
        float f(ty * inv);
        inv *= -1;
        float b(fMat[1] * inv); 
        float d(fMat[3] * inv);

        inverse->fMat[0] = a;
        inverse->fMat[1] = b; 
        inverse->fMat[2] = c;
        inverse->fMat[3] = d;
        inverse->fMat[4] = e;
        inverse->fMat[5] = f;
        
        return true;
    }
}


void GMatrix::mapPoints(GPoint dst[], const GPoint src[], int count) const
{
    for(int i = 0; i < count; i++)
    {
        float xi = src[i].x();
        float yi = src[i].y();

        float xt = fMat[0] * xi + fMat[1] * yi + fMat[2];
        float yt = fMat[4] * yi + fMat[3] * xi + fMat[5];

        dst[i] = GPoint::Make(xt, yt);
    }
}