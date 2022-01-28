#include "Sample.h"

class Penrose : public Sample {
public:
    void draw(GCanvas* canvas, GBlendMode mode = GBlendMode::kSrcOver) override {
        GColor OOO = GColor{0., 0., 0., 1.};
        GColor OOF = GColor{0.0, 0.0, 1.0, 1.};
        GColor OFO = GColor{0.0, 1.0, 0.0, 1.};
        GColor OFF = GColor{0.0, 1.0, 1.0, 1.};
        GColor FOF = GColor{1.0, 0.0, 1.0, 1.};
        GColor FOO = GColor{1.0, 0.0, 0.0, 1.};
        GColor FFO = GColor{1.0, 1.0, 0.0, 1.};
        GColor FFF = GColor{1.0, 1.0, 1.0, 1.0};
        
        canvas->concat(GMatrix::Translate(80+50, 80+48)); 
        canvas->concat(GMatrix::Scale(1.5, 1.5));

        GPaint paint = GPaint(OOO); 
        canvas->drawPaint(paint);
        
        //////////////////////////////////////

        paint = GPaint(FFF);

        GPoint pts12[7] = {GPoint{33.54,-46.17}, GPoint{24.27,-74.70 }, GPoint{-0.00,-57.06 }};
        canvas->drawConvexPolygon(pts12, 6, paint);
        GPoint pts99[7] = {GPoint{-24.27,-74.70 }, GPoint{-15.00,-46.17 }, GPoint{9.27,-28.53 }, GPoint{-0.00,-57.06}};
        canvas->drawConvexPolygon(pts99, 4, paint);
        GPoint pts13[7] = {GPoint{-33.54,-46.17}, GPoint{-63.54,-46.17 }, GPoint{-54.27,-17.63 }, GPoint{-78.54,0.00 }, GPoint{-48.54,0.00 }, GPoint{-24.27,-17.63 }, GPoint{-54.27,-17.63}};
        canvas->drawConvexPolygon(pts13, 6, paint);
        GPoint pts14[7] = {GPoint{0.00,57.06}, GPoint{24.27,74.70 }, GPoint{33.54,46.17 }, GPoint{63.54,46.17 }, GPoint{39.27,28.53 }, GPoint{9.27,28.53 }, GPoint{33.54,46.17}};
        canvas->drawConvexPolygon(pts14, 6, paint);
        GPoint pts15[7] = {GPoint{-54.27,17.63}, GPoint{-63.54,46.17 }, GPoint{-33.54,46.17 }, GPoint{-24.27,74.70 }, GPoint{-15.00,46.17 }, GPoint{-24.27,17.63 }, GPoint{-33.54,46.17}};
        canvas->drawConvexPolygon(pts15, 6, paint);
        GPoint pts16[7] = {GPoint{54.27,17.63}, GPoint{78.54,-0.00 }, GPoint{54.27,-17.63 }, GPoint{63.54,-46.17 }, GPoint{39.27,-28.53 }, GPoint{30.00,-0.00 }, GPoint{54.27,-17.63}};
        canvas->drawConvexPolygon(pts16, 6, paint);

        //////////////////////////

        paint = GPaint(OFF);

        GPoint pts1[5] = {GPoint{0.00,0.00}, GPoint{9.27,-28.53 }, GPoint{39.27,-28.53 }, GPoint{30.00,-0.00 }};
        canvas->drawConvexPolygon(pts1, 4, paint);

        paint = GPaint(FOF);    
        GPoint pts2[5] = {GPoint{-15.00,-46.17}, GPoint{-24.27,-17.63 }, GPoint{-0.00,-0.00 }, GPoint{9.27,-28.53 }};
        canvas->drawConvexPolygon(pts2, 4, paint);

        paint = GPaint(OOF);
        GPoint pts3[5] = {GPoint{-15.00,-46.17}, GPoint{-24.27,-17.63 }, GPoint{-33.54,-46.17 }, GPoint{-24.27,-74.70 }};
        canvas->drawConvexPolygon(pts3, 4, paint);

        paint = GPaint(FFO);
        GPoint pts4[5] = {GPoint{-15.00,46.17}, GPoint{9.27,28.53 }, GPoint{0.00,0.00 }, GPoint{-24.27,17.63 }};
        canvas->drawConvexPolygon(pts4, 4, paint);
        
        paint = GPaint(OFF);
        GPoint pts5[5] = {GPoint{-15.00,46.17}, GPoint{9.27,28.53 }, GPoint{0.00,57.06 }, GPoint{-24.27,74.70 }};
        canvas->drawConvexPolygon(pts5, 4, paint);

        paint = GPaint(FOO);
        GPoint pts6[5] = {GPoint{39.27,28.53}, GPoint{30.00,-0.00 }, GPoint{0.00,-0.00 }, GPoint{9.27,28.53 }};
        canvas->drawConvexPolygon(pts6, 4, paint);

        paint = GPaint(OOF);
        GPoint pts7[5] = {GPoint{39.27,28.53}, GPoint{30.00,-0.00 }, GPoint{54.27,17.63 }, GPoint{63.54,46.17 }};
        canvas->drawConvexPolygon(pts7, 4, paint);

        paint = GPaint(OFO);
        GPoint pts8[5] = {GPoint{39.27,-28.53}, GPoint{9.27,-28.53 }, GPoint{0.00,-0.00 }, GPoint{30.00,-0.00 }};
        canvas->drawConvexPolygon(pts8, 4, paint);

        paint = GPaint(FOO);
        GPoint pts9[5] = {GPoint{39.27,-28.53}, GPoint{9.27,-28.53 }, GPoint{33.54,-46.17 }, GPoint{63.54,-46.17 }};
        canvas->drawConvexPolygon(pts9, 4, paint);
        
        paint = GPaint(OFO);
        GPoint pts10[5] = {GPoint{-48.54,0.00}, GPoint{-24.27,17.63 }, GPoint{0.00,0.00 }, GPoint{-24.27,-17.63 }};
        canvas->drawConvexPolygon(pts10, 4, paint);
        
        paint = GPaint(FOF);
        GPoint pts11[5] = {GPoint{-48.54,0.00}, GPoint{-24.27,17.63 }, GPoint{-54.27,17.63 }, GPoint{-78.54,0.00 }};
        canvas->drawConvexPolygon(pts11, 4, paint);
    }

};