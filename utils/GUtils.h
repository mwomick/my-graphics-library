/*
 * GUtils.h
 * 
 * Michael Womick
 * COMP 475 - 2D Computer Graphics 
 * Prof. Mike Reed
 * 08/20/2021
 */
#ifndef GUtils_DEFINED
#define GUtils_DEFINED

#include <stdlib.h>

const float G_FL_EPSILON = 0.000005;

static inline int sign(int x) {
    int m = x >> 31;
    return (x^m) -m;
}

static inline bool G_FL_EQUAL(float a, float b) { return abs(a-b) < G_FL_EPSILON; }
/* 
static inline float clamp(float x, float max) {
    if(x > max) return max;
    if(x < 0) return 0;
    return x;
} */

static inline int clamp(int x, int max) {
    if(x > max) return max;
    if(x < 0) return 0;
    return x;
}

#endif