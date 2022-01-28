/*
 * SampleSandArt.h
 * 
 * Michael Womick
 * COMP 475 - 2D Computer Graphics 
 * Prof. Mike Reed
 * 08/20/2021
 */

#ifndef SampleSandArt_DEFINED
#define SampleSandArt_DEFINED

#include <stdio.h>      /* NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include "Sample.h"
#include "../include/GPoint.h"

class SandArt : public Sample {

public:
    void draw(GCanvas* canvas, GBlendMode mode = GBlendMode::kSrcOver) override
    {
        canvas->drawPaint(GPaint(GColor{1.0, 1.0, 1.0, 1.0}));
        srand (time(NULL));

        GColor color1 {(rand()%256)/255.f, (rand()%256)/255.f, (rand()%256)/255.f, 1.0f};
        GColor color2 {(rand()%256)/255.f, (rand()%256)/255.f, (rand()%256)/255.f, 1.0f}; 
        GColor dcolor = (color2 - color1) * GColor{0.08, 0.08, 0.08, 1.f};

        for(int i = -25; i < 276; i+=20) {
            int max_y = 0;
            std::vector<GPoint> points;
            int sign = rand()%2;
            
            if(sign == 0) { sign = -1; }
            if(sign == 1) { sign = 1; }
            
            int y = i;
            y += sign * (rand() % 15);
            int x = 0;
            points.emplace_back(GPoint{0, y});
            if(y > max_y) { max_y = y; } 
            for(int j = 0; j < 5; j++) {
                y += sign * (rand() % 6);
                x += rand() % 16;
                points.emplace_back(GPoint{x, y});
                if(y > max_y) { max_y = y; } 
            }
            y += sign * (rand() % 15-(sign*(i-y)));
            if(y > max_y) { max_y = y; } 
            points.emplace_back(GPoint{256, y});
            points.emplace_back(GPoint{256, max_y + 150});
            points.emplace_back(GPoint{0, max_y + 150});
            
            GPoint* pts = &points[0];
            GPaint p = GPaint();
            p.setColor(color1);
            canvas->drawConvexPolygon(pts, 9, p);
            color1 += dcolor;
        }

    }

};


#endif