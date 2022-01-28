/*
 * SampleMario.h
 * 
 * Michael Womick
 * COMP 475 - 2D Computer Graphics 
 * Prof. Mike Reed
 * 08/20/2021
 */

#ifndef SampleMario_DEFINED
#define SampleMario_DEFINED

#include "Sample.h"


class Mario : public Sample {

const GColor ALPHA  { 0.F, 0.F, 0.F, 0.F };
const GColor RED { 0.65F, 0.24F, 0.16F, 1.F }; 
const GColor YELLOW { 0.87F, 0.63F, 0.25F, 1.F };
const GColor GREEN { 0.42F, .43F, 0.12F, 1.F };

const GColor MARIO_2BIT_COL_MAP[4] = { ALPHA, RED, YELLOW, GREEN };

const uint32_t MARIO[16] = { 87296,
                                5592384,
                                191472,
                                2812652,
                                2862060,
                                4188860,
                                699008,
                                65008,
                                4161020,
                                16733695,
                                11376250,
                                11097450,
                                10835290,
                                344400,
                                4129020,
                                16711935 };

public:
    void draw(GCanvas* canvas, GBlendMode mode = GBlendMode::kSrcOver) override
    {
        GColor color = GColor();
        for(int i = 0; i < 16; i++)
        {
            for(int j = 0; j < 12; j++)
            {
                color = MARIO_2BIT_COL_MAP[3 & (MARIO[i] >> j) >> j];
                GRect rect = GRect::MakeLTRB(32+j*16, i*16, 32+j*16+16, i*16+16);
                canvas->fillRect(rect, color);
            }
        }
    }

};


#endif