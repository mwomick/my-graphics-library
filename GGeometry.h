#include "include/GPoint.h"
#include "include/GMath.h"

struct GQuadCoeff {
    GQuadCoeff(const GPoint src[3]) {
        fC = src[0] - GPoint{0, 0};
        fB = 2*((src[1] - GPoint{0,0}) - fC);
        fA = src[2] - 2*src[1] + fC;
    }

    GPoint eval(float t) {
        return (fA * t + fB) * t + fC;
    }

    GVector fA;
    GVector fB; 
    GVector fC;
};


static GPoint GEvalQuadAt(const GPoint src[3], float t) {
    return GQuadCoeff(src).eval(t);
}

struct GCubicCoeff {
     GCubicCoeff(const GPoint src[4]) {
        fD = src[0] - GPoint{0, 0};
        fA = src[3] + 3*(src[1] - src[2]) - src[0];
        fB = 3 * (src[2] - 2* src[1] + fD);
        fC = 3 * (src[1] - src[0]);
    }

    GPoint eval(float t) {
        return ((fA * t + fB) * t + fC) * t + fD;
    }

    GVector fA;
    GVector fB;
    GVector fC;
    GVector fD;
};

static GPoint GEvalCubicAt(const GPoint src[4], float t) {
    return GCubicCoeff(src).eval(t);
}

#define G_TOLERANCE 0.25

static float dist(GVector p0, GVector p1) {
    return sqrt(p0.x() * p1.x() + p0.y() * p1.y());
}

static int GFlattenQuad(const GPoint src[4]) {
    GVector vec = G_TOLERANCE * (src[0] - 2 * src[1] + (src[2] - GPoint{0, 0}));
    return GCeilToInt(sqrt(dist(vec, vec)/G_TOLERANCE));
}

static int GFlattenCubic(const GPoint src[4]) {
    GVector vec1 = src[0] - 2 * src[1] + (src[2] - GPoint{0, 0});
    GVector vec2 = src[1] - 2 * src[2] + (src[3] - GPoint{0, 0});

    float m1 = dist(vec1, vec1);
    float m2 = dist(vec2, vec2);

    return GCeilToInt(sqrt(.75 * std::max(m1, m2)/G_TOLERANCE));
}