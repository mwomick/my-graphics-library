/*
 * GBlitRow.h
 * 
 * Michael Womick
 * COMP 475 - 2D Computer Graphics 
 * Prof. Mike Reed
 * 08/20/2021
 */

#ifndef GBlitRow_DEFINED
#define GBlitRow_DEFINED

#include "utils/GColorUtils.h"
#include "utils/GMemUtils.h"
/* 
 * GBlitRow is a collection of functions from which the blitter may choose to write to memory.
 */
class GBlitRow {
public:
    enum Flags32 {
        kDst,
        kSrc,
        kClear,
    };

    typedef void (*Proc32)(uint32_t dst[], const uint32_t src, int count);

    static void blit_row_src(uint32_t* dst, uint32_t src, int count) {
        quickset(dst, src, count);
    }

    static void blit_row_clear(uint32_t* dst, uint32_t src, int count) {
        memset(dst, 0, count<<2);
    }

    static constexpr Proc32 kProcs[] = {
        nullptr,
        blit_row_src,
        blit_row_clear
    };

};

#endif