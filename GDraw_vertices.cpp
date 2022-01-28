#include "utils/GColorUtils.h"
#include "include/GShader.h"
#include "include/GMatrix.h"
#include "GShaderBlitter.h"
#include "GScan.h"

#include "GDraw.h"

static GMatrix compute_basis(const GPoint* verts) {
        GPoint Pa = verts[1] - verts[0];
        GPoint Pb = verts[2] - verts[0];
        return GMatrix (  Pa.x(),  Pb.x(),  verts[0].x(),
                          Pa.y(),  Pb.y(),  verts[0].y() );
}   

class GTriTexShader : public GShader {
public:
    GTriTexShader(GShader* shader, GPoint verts[3], GPoint texs[3], GMatrix ctm)
        : fRealShader(shader) {
            GMatrix P(compute_basis(verts));
            GMatrix T_inv;
            compute_basis(texs).invert(&T_inv);
            fExtraTransform = P * T_inv;
            setContext(ctm);
        }

    bool isOpaque() override { return fRealShader->isOpaque(); }

    bool setContext(const GMatrix& ctm) override {
        return fRealShader->setContext(ctm * fExtraTransform);
    }
    
    void shadeRow(int x, int y, int count, GPixel row[]) override {
        fRealShader->shadeRow(x, y, count, row);
    }
private:
    GShader* fRealShader;
    GMatrix  fExtraTransform;
};

class GTriColorShader : public GShader {
public:
    GTriColorShader(GPoint verts[3], GColor colors[3], GMatrix ctm) 
    : fColors(colors), fBasis(compute_basis(verts)) {
        fDC1 = fColors[1] - fColors[0];
        fDC2 = fColors[2] - fColors[0]; 
        setContext(ctm);
    }

    void shadeRow(int x, int y, int count, GPixel row[]) override {
        GPoint p0 = fInverse * GPoint{x + 0.5 , y + 0.5 };
        GColor c0 = p0.x() * fDC1 + p0.y() * fDC2 + fColors[0];
        for(int i = 0; i < count; i++) { 
            GPixel p = raw_mul_pack(c0);
            row[i] = premul_alpha(p, GPixel_GetA(p));
            c0 += fDC;
        }
    }

    bool isOpaque() override { return false; }
    bool setContext(const GMatrix& ctm) override { 
        bool res =  (ctm * fBasis).invert(&fInverse);
        fDC = fInverse[GMatrix::SX]*fDC1 + fInverse[GMatrix::KY]*fDC2; 
        return res;
    }

private:
    const GColor*   fColors;
    GMatrix         fBasis;    
    GMatrix         fInverse;
    GColor          fDC1, fDC2, fDC;
};

#include "utils/GColorUtils.h"

class GMultiplyShader : public GShader {
public:
    GMultiplyShader(GShader* shader1, GShader* shader2)
        : fShader1(shader1), fShader2(shader2) { 
        }

    bool isOpaque() override { return fShader1->isOpaque() && fShader2->isOpaque(); }

    bool setContext(const GMatrix& ctm) override {
        return fShader1->setContext(ctm) && fShader2->setContext(ctm);
    }
    
    void shadeRow(int x, int y, int count, GPixel row[]) override {
        GPixel tmp1[count];
        fShader1->shadeRow(x, y, count, tmp1);
        GPixel tmp2[count];
        fShader2->shadeRow(x, y, count, tmp2);
        for(int i = 0; i < count; ++i) {
            row[i] = mult_pixels(tmp1[i], tmp2[i]);
        }
    }

private:
    GShader*    fShader1;
    GShader*    fShader2;
};

std::unique_ptr<GShader> GCreateTriColorShader(GPoint verts[3], GColor colors[3], GMatrix ctm) 
{
    return std::unique_ptr<GShader>(new GTriColorShader(verts, colors, ctm));
}

std::unique_ptr<GShader> GCreateTriTexShader(GShader* shader, GPoint verts[3], GPoint texs[3], GMatrix ctm) 
{
    return std::unique_ptr<GShader>(new GTriTexShader(shader, verts, texs, ctm));
}

std::unique_ptr<GShader> GCreateMultiplyShader(GShader* shader1, GShader* shader2) 
{
    return std::unique_ptr<GShader>(new GMultiplyShader(shader1, shader2));
}

void GDraw::drawVertices(const GPoint verts[], const GColor colors[], const GPoint texs[],
                            int count, const int indices[], const GPaint& paint, const GMatrix& ctm) {
    if(texs && colors) {
        int n = 0;
        for(int i = 0; i < count; ++i) {
            GPoint pts[3] = { verts[indices[n]], verts[indices[n+1]], verts[indices[n+2]] };
            GColor cols[3] = { colors[indices[n]], colors[indices[n+1]], colors[indices[n+2]] };
            GPoint tex[3] = { texs[indices[n]], texs[indices[n+1]], texs[indices[n+2]] };
            auto shader1 = GCreateTriColorShader(pts, cols, ctm);
            auto shader2 = GCreateTriTexShader(paint.getShader(), pts, tex, ctm);
            auto shader = GCreateMultiplyShader(shader1.get(), shader2.get());
            const GPaint p(shader.get());
            auto blitter = GBlitter::Choose(*fDst, p);
            ctm.mapPoints(pts, pts, 3);
            GScan::FillTriangle(pts, *fBounds, blitter);
            n += 3;
        }
    }
    else if(colors) {
        int n = 0;
        for(int i = 0; i < count; ++i) {
            GPoint pts[3] = { verts[indices[n]], verts[indices[n+1]], verts[indices[n+2]] };
            GColor cols[3] = { colors[indices[n]], colors[indices[n+1]], colors[indices[n+2]] };
            auto shader = GCreateTriColorShader(pts, cols, ctm);
            const GPaint p(shader.get());
            auto blitter = GBlitter::Choose(*fDst, p);
            ctm.mapPoints(pts, pts, 3);
            GScan::FillTriangle(pts, *fBounds, blitter);
            n += 3;
        }
    }
    else if(texs) {
        int n = 0;
        for(int i = 0; i < count; ++i) {
            GPoint pts[3] = { verts[indices[n]], verts[indices[n+1]], verts[indices[n+2]] };
            GPoint tex[3] = { texs[indices[n]], texs[indices[n+1]], texs[indices[n+2]] };
            auto shader = GCreateTriTexShader(paint.getShader(), pts, tex, ctm);
            const GPaint p(shader.get());
            auto blitter = GBlitter::Choose(*fDst, p);
            ctm.mapPoints(pts, pts, 3);
            GScan::FillTriangle(pts, *fBounds, blitter);
            n += 3;
        }
    }
    else {
        int n = 0;
        for(int i = 0; i < count; ++i) {
            GPoint pts[3] = { verts[indices[n]], verts[indices[n+1]], verts[indices[n+2]] };
            auto blitter = GBlitter::Choose(*fDst, paint);
            ctm.mapPoints(pts, pts, 3);
            GScan::FillTriangle(pts, *fBounds, blitter);
            n += 3;
        } 
    }

}