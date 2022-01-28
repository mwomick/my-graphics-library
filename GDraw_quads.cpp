#include "GDraw.h"
#include "GColor.h"

/** TODO: Reduce number of memcpys */

struct GTuple {
    float a, b, c, d;
};

GPoint compute_point(GPoint verts[4], float u, float v){
    return (1-u)*(1-v)*verts[0] + (1-v)*u*verts[1]+ (1-u)*v*verts[3] + u*v*verts[2];
}

void compute_points(GPoint verts[4], GTuple m) {
    GPoint res[4] = { compute_point(verts, m.a, m.c) , compute_point(verts, m.b, m.c), 
                        compute_point(verts, m.b, m.d), compute_point(verts, m.a, m.d) };
    memcpy(verts, res, sizeof(GPoint)<<2);
}

GColor compute_color(GColor colors[4], float u, float v){
    return (1-u)*(1-v)*colors[0] + (1-v)*u*colors[1] + (1-u)*v*colors[3] + u*v*colors[2];
}

void compute_colors(GColor colors[4], GTuple m) {
    GColor res[4] = { compute_color(colors, m.a, m.c) , compute_color(colors, m.b, m.c), 
                        compute_color(colors, m.b, m.d), compute_color(colors, m.a, m.d) };
    memcpy(colors, res, sizeof(GColor)<<2);
}

GTuple compute_tuple(float u, float v, float level) {
   return GTuple { (float)u/(float)(level+1), (float)(u+1)/(float)(level+1), 
                        (float)v/(float)(level+1), (float)(v+1)/(float)(level+1) }; 
}

void GDraw::drawQuad(const GPoint verts[4], const GColor colors[4], const GPoint texs[4],
                            int level, const GPaint& paint, const GMatrix& ctm) {
    int indices[6] = {0, 1, 3, 1, 2, 3};

    if(colors) {
        GPoint pts[4];
        GColor cols[4];
        for (int u = 0; u <= level; u++){
            for (int v = 0; v <= level; v++){
                GTuple m = compute_tuple(u, v, level);
                memcpy(pts, verts, sizeof(GPoint)<<2);
                memcpy(cols, colors, sizeof(GColor)<<2);
                compute_points(pts, m);
                compute_colors(cols, m);
                drawVertices(pts, cols, nullptr, 2, indices, paint, ctm);
            }
        }
    }
    else if(texs){
        GPoint pts[4];
        GPoint txs[4];
        for (int u = 0; u <= level; u++){
            for (int v = 0; v <= level; v++){
                GTuple m = compute_tuple(u, v, level);
                memcpy(pts, verts, sizeof(GPoint)<<2);
                memcpy(txs, texs, sizeof(GPoint)<<2);
                compute_points(pts, m);
                compute_points(txs, m);
                drawVertices(pts, nullptr, txs, 2, indices, paint, ctm);
            }
        }
    }
}