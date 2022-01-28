#include <stack>

#include "include/GCanvas.h"
#include "include/GShader.h"

#include "GBitmapDevice.h"

class MyCanvas : public GCanvas {
public:
    static bool nothingToDraw(const GPaint& paint) {
        if(paint.getShader()) { return false; }
        switch (paint.getBlendMode()) {
            case GBlendMode::kSrcOver:
            case GBlendMode::kDstOut:
            case GBlendMode::kDstOver:
                return 0 == paint.getAlpha();
                break;
            case GBlendMode::kDst:
                return true;
            default:
                break;
        }
        return false;
    }

    MyCanvas(const GBitmap& device) : fDevice(new GBitmapDevice(device))
                                       {
        fBounds = (GRect::MakeLTRB(0, 0, device.width(), device.height()).round());
        fCTMStack.push(GMatrix());
    }

    void save() override
    {
        fCTMStack.push(fCTMStack.top());
        fSaveCount++;
    }

    void restore() override 
    {
        if(fSaveCount > 0) {
            fCTMStack.pop();
            fSaveCount--;
        }
    }

    void concat(const GMatrix& matrix) override
    {
        fCTMStack.top() = fCTMStack.top() * matrix;
    }

    bool quickReject(const GIRect& rect) {
        GPoint upper_left = GPoint::Make(rect.left(), rect.top());
        GPoint lower_right = GPoint::Make(rect.right(), rect.bottom());

        return(lower_right.x() < fBounds.left() ||
                upper_left.x() > fBounds.right() ||
                upper_left.y() > fBounds.bottom() ||
                lower_right.y() < fBounds.top() );
    }

    void drawRect(const GRect& rect, 
                  const GPaint& paint) override
    {
        if(nothingToDraw(paint)) { return; }
        if(fSaveCount == 0) {
            GIRect r = rect.round();
            if(quickReject(r)) { return; }
            if(paint.getShader()) {
                    if(!paint.getShader()->setContext(fCTMStack.top()))
                    return;            
            }
            fDevice->drawIRect(r, paint);
        }
        else {
            GPoint pts[4] = {   GPoint::Make(rect.left(), rect.top()), 
                                GPoint::Make(rect.right(), rect.top()),
                                GPoint::Make(rect.right(), rect.bottom()),
                                GPoint::Make(rect.left(), rect.bottom())    };

            this->drawConvexPolygon(pts, 4, paint);
        }
    }

    GRect makeBounds(const GPoint points[], int count) {
        float minX = points[0].x();
        float minY = points[0].y();
        float maxX = points[0].x();
        float maxY = points[0].y();

        for(int i = 1; i < count; i++) {
            if(points[i].x() < minX) {
                minX = points[i].x();
            }
            else if(points[i].x() > maxX) {
                maxX = points[i].x();
            }
            if(points[i].y() < minY) {
                minY = points[i].y();
            }
            else if(points[i].y() > maxY) {
                maxY = points[i].y();
            }
        }

        return GRect::MakeLTRB(minX, minY, maxX, maxY);
    }

    void drawConvexPolygon(const GPoint points[], int count, const GPaint& paint) override {
        if(nothingToDraw(paint)) { return; }

        GPoint pts[count];
        fCTMStack.top().mapPoints(pts, points, count);
        GIRect bounds = makeBounds(pts, count).round();
        if(quickReject(bounds)) { 
            return; 
        }

        if(paint.getShader()) {
             if(!paint.getShader()->setContext(fCTMStack.top()))
                return;            
        }
        fDevice->drawPoints(pts, count, paint);
    }

    void drawPaint(const GPaint& paint) override {
        if(nothingToDraw(paint)) { return; }
        if(paint.getShader()) {
             if(!paint.getShader()->setContext(fCTMStack.top()))
                return;            
        }
        fDevice->drawPaint(paint);
    }

    void drawPath(const GPath& path, const GPaint& paint) override {
       if(paint.getShader()) {
             if(!paint.getShader()->setContext(fCTMStack.top()))
                return;            
        }
        GPath p(path);
        p.transform(fCTMStack.top());
        fDevice->drawPath(p, paint); 
    }

    void drawQuad(const GPoint verts[4], const GColor colors[4], const GPoint texs[4],
                              int level, const GPaint& paint) override {
        fDevice->drawQuad(verts, colors, texs, level, paint, fCTMStack.top());

    }

    void drawMesh(const GPoint verts[], const GColor colors[], const GPoint texs[],
                              int count, const int indices[], const GPaint& paint) override{
        fDevice->drawMesh(verts, colors, texs, count, indices, paint, fCTMStack.top());
    }

private:

    GBitmapDevice*          fDevice;
    GIRect                  fBounds;
    std::stack<GMatrix>     fCTMStack;
    int                     fSaveCount = 0; 
};

std::unique_ptr<GCanvas> GCreateCanvas(const GBitmap& device) 
{
    if (!device.pixels())
        return nullptr;

    return std::unique_ptr<GCanvas>(new MyCanvas(device)); 
}

#include "samples/SampleMap.h"

std::string GDrawSomething(GCanvas* canvas, GISize dim)
{
    GPath p;
    p.addCircle({128, 128}, 16);
    GColor c { 1., 0., 0, 1. };
    GPaint paint = GPaint(c);
    canvas->drawPath(p, paint);

    return "nc";
} 