/*
 * GMemUtils.h
 * 
 * Michael Womick
 * COMP 475 - 2D Computer Graphics 
 * Prof. Mike Reed
 * 08/20/2021
 */

#ifndef GMemUtils_DEFINED
#define GMemUtils_DEFINED

#include "../include/GPixel.h"

inline static void quickset(GPixel* ptr, uint64_t value, int num) 
{
   /* int i = 1;
    ptr[0] = value;
    int j = num >> 1;
    while(i < j){
        memcpy(ptr+i, ptr, i<<2);
        i<<=1;
    }
    i>>=1;
    while(++i < num) {
        ptr[i] = value;
    }*/

    uint64_t* pointer = (uint64_t*)ptr;
    uint64_t two_pixels = ((value << 32) & 0xFFFFFFFF00000000 )|(0x00000000FFFFFFFF&value);
    for(int i = num>>1; i > 0; i--) {
        *pointer++ = two_pixels;
    }
    if((num&1)==1){ptr[num^1]=value;}
}

#endif