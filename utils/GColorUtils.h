/*
 * GColorUtils.h
 * 
 * Michael Womick
 * COMP 475 - 2D Computer Graphics 
 * Prof. Mike Reed
 * 08/20/2021
 */

#ifndef GColorUtils_DEFINED
#define GColorUtils_DEFINED

#include "../include/GColor.h"
#include "../include/GPixel.h"

const uint64_t QUAD_HALF_64 = 0x0080008000800080;
const uint64_t QUAD_FULL_64 = 0x00FF00FF00FF00FF;

static inline uint64_t splay(uint64_t x) { return ((x & 0xFF00FF00) << 24) | (x & 0x00FF00FF); }

static inline uint32_t unsplay(uint64_t x) { return ( ((x >> 24) & 0xFF00FF00) | (x & 0xFF00FF) ); }

static inline int div255(int x) { return (x + 128) * 257 >> 16; }

static inline uint32_t quad_mul_div255(uint32_t x, uint8_t a)
{
    uint64_t res = splay(x) * a + QUAD_HALF_64;
    res += (res >> 8) & QUAD_FULL_64;
    return unsplay(res >> 8);
}

static inline uint32_t raw_mul_pack(const GColor& color) {
    return  GRoundToInt(color.a*255.F) << GPIXEL_SHIFT_A |
            GRoundToInt(color.r*255.F) << GPIXEL_SHIFT_R |
            GRoundToInt(color.g*255.F) << GPIXEL_SHIFT_G |
            GRoundToInt(color.b*255.F) << GPIXEL_SHIFT_B;
}

static inline uint32_t premul_alpha(uint32_t x, uint8_t a)
{
    /* TODO: This is not the most efficient way, but it's "readable" */
    uint64_t res = splay(x) * a + QUAD_HALF_64;
    res += (res >> 8) & QUAD_FULL_64;
    return a << 24 | ((unsplay(res >> 8) << 8) >> 8); 
}

static inline uint32_t mult_pixels(uint32_t a, uint32_t b) {
    uint16_t ar = GPixel_GetR(a);
    uint16_t ag = GPixel_GetG(a);
    uint16_t ab = GPixel_GetB(a);
    uint16_t aa = GPixel_GetA(a);

    uint16_t br = GPixel_GetR(b);
    uint16_t bg = GPixel_GetG(b);
    uint16_t bb = GPixel_GetB(b);
    uint16_t ba = GPixel_GetA(b);

    ar = (ar * br + 0x08) >> 8;
    ag = (ag * bg + 0x08) >> 8;
    ab = (ab * bb + 0x08) >> 8;
    aa = (aa * ba + 0x08) >> 8;

    return  aa << GPIXEL_SHIFT_A |
            ar << GPIXEL_SHIFT_R |
            ag << GPIXEL_SHIFT_G |
            ab << GPIXEL_SHIFT_B;
}

static inline void srcover(uint32_t src, uint32_t* dst) {
    *dst = src + quad_mul_div255(*dst, ~GPixel_GetA(src));
}

#endif