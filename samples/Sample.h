#ifndef Sample_DEFINED
#define Sample_DEFINED

#include "../include/GPaint.h"

class Sample {
public:
    Sample() : fWidth(0), fHeight(0) {}

    virtual ~Sample() = default;

    int    width() const { return fWidth; }
    int    height() const { return fHeight;}

    virtual void draw(GCanvas* canvas, GBlendMode mode = GBlendMode::kSrcOver) = 0;

private:
    int fWidth;
    int fHeight;

};

#endif