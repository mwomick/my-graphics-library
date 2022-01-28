/*
 * GScan.h
 * 
 * Michael Womick
 * COMP 475 - 2D Computer Graphics 
 * Prof. Mike Reed
 * 08/20/2021
 */

#ifndef GScan_DEFINED
#define GScan_DEFINED

#include <vector>

#include "include/GPath.h"

#include "GEdge.h"
#include "GClipper.h"

/*
 * GScan is responsible for clipping the path and traversing the clipped path, 
 * line by line, and sending it to the blitter.
 */
class GScan {
public:
    GScan();

    static void FillRect(const GIRect& rect, const GIRect& bounds, GBlitter* blitter) {
        int l = rect.left() > bounds.left() ? 
                rect.left() : bounds.left();
        int t = rect.top() > bounds.top() ? 
                rect.top() : bounds.top();
        int r = rect.right() < bounds.right() ? 
                rect.right() : bounds.right();
        int b = rect.bottom() < bounds.bottom() ? 
                rect.bottom() : bounds.bottom();
        if(r>l && b>t) {
            blitter->blitRect(l, t, r, b);
        }
    }

    static inline bool edge_lt(GEdge& e0, GEdge& e1) {
        if(e0.fTop == e1.fTop){ 
            if(e0.fX == e1.fX){
                return e0.fDX < e1.fDX;
            }
            return e0.fX < e1.fX;
        }
        return e0.fTop < e1.fTop;
    }

    static void walk_simple_edges(GEdge* edgeList, int count, GBlitter* blitter) {
        if(count < 2) return;
    
        GEdge e1 (edgeList[0]);
        GEdge e2 (edgeList[1]);
        
        int y(std::min(edgeList[1].fTop, edgeList[0].fTop));
        int y1(edgeList[0].fBottom + 1);
        int y2(edgeList[1].fBottom + 1);

        int j = 1;
        
        while(j < count || y < y1 || y < y2) {
            if(e1.fDX == 0 && e2.fDX == 0) {
                int bottom = y1 < y2 ? y1 : y2;
                if(e1.fX < e2.fX && y <= bottom){
                    blitter->blitRect(GRoundToInt(e1.fX), y, GRoundToInt(e2.fX), bottom);
                }
                y = bottom;
            } 
            else {
                while(y < y1 && y < y2) {
                    blitter->blitH(GRoundToInt(e1.fX), y, 
                                    GRoundToInt(e2.fX)-GRoundToInt(e1.fX));
                    e1.fX += e1.fDX;
                    e2.fX += e2.fDX;
                    y++;
                }
            }

            if(y >= y1) { 
                if(++j < count) {
                    e1 = edgeList[j];
                    y1 = edgeList[j].fBottom + 1;
                }
                else { break; }
            }

            if(y >= y2) {
                if(++j < count) {
                    e2 = edgeList[j];
                    y2 = edgeList[j].fBottom + 1;
                }
                else { break; }
            }

        }
    }

    static void walk_edges(GEdge* edgeList, int count, GBlitter* blitter) {
        if(count < 2) return;

        std::sort(edgeList, edgeList+count,
                        [](GEdge& e0, GEdge& e1) -> bool { return edge_lt(e0, e1); });

        // start at the top of the highest edge
        int y(edgeList[0].fTop);
        
        while(count > 0) {
            
            int curr = 0;
            int left = 0;
            int w = 0;

            while(curr < count && edgeList[curr].fTop <= y) {  
                // if we have a winding value of zero, set left
                if(!w) { left = GRoundToInt(edgeList[curr].fX); }

                // update w
                if(edgeList[curr].fWinding) w++;
                else w--;

                // if we went from 0 to non-zero, blit left to edgeList[curr].fX  
                if(!w){
                    int width = GRoundToInt(edgeList[curr].fX)-left;
                    if(width > 0) {
                        blitter->blitH(left, y, width);
                    }
                }
           
                // if we reached the bottom of this edge, erase it
                if(y >= edgeList[curr].fBottom) {
                    memcpy(&edgeList[curr], &edgeList[curr+1], sizeof(GEdge)*(count-curr));
                    count--;
                }
                else {
                    // otherwise, increment fX
                    edgeList[curr].fX += edgeList[curr].fDX; 
                    curr++;
                }
            }

            // proceed to next y
            y++;

            // get new edges, if any
            while(curr < count && y == edgeList[curr].fTop) { curr++; }
    
            // resort in case of new or crossed edges
            std::sort(edgeList, edgeList+curr,
                [](GEdge& e0, GEdge& e1) -> bool { return e0.fX < e1.fX; });
        }

    }

    static void FillConvexPoly(const GPoint points[], int count, 
                                const GIRect& bounds, GBlitter* blitter) {
    
        std::vector<GPoint> pointList;

        GClipper ec(bounds);
        ec.ClipPoints(points, count, pointList);   
        pointList.push_back(*pointList.begin());

        if(pointList.size() < 3) return;

        GEdge edgeList[pointList.size()];
        int edgeListSize = 0;
        for(int i = 0; i < pointList.size()-1; i++) {
            GEdge e;
            if(e.setLine(pointList[i], pointList[i+1])) { 
                int j = 0;
                while(j <= edgeListSize && edge_lt(edgeList[j++], e)){}       
                memcpy(edgeList+j, edgeList+j-1, 
                        (edgeListSize-j+1)*sizeof(e)); 
                edgeList[j-1] = e;
                edgeListSize++;
            }
        }

        walk_simple_edges(edgeList, edgeListSize, blitter);
    }

    #include "GGeometry.h"

    static void FillPath(const GPath& path, const GIRect& bounds, GBlitter* blitter) {
        GPath::Iter iter(path);
        GPoint pts[GPath::kMaxNextPoints];
        GPath::Verb currVb;

        std::vector<GEdge> edges;

        while(currVb != GPath::Verb::kDone) {       
            std::vector<GPoint> pointList;

            if(currVb == GPath::Verb::kMove) {
                pointList.push_back(pts[0]);
            }
            
            do {
                currVb = iter.next(pts);
                switch(currVb) {
                    case GPath::Verb::kMove:
                        goto CLOSE;
                    case GPath::Verb::kLine:
                        pointList.push_back(pts[1]);
                        break;
                    case GPath::Verb::kQuad: {
                        float step = 1.f / GFlattenQuad(pts);
                        for(float t = 0; t < 1.f + step; t += step) {
                            pointList.push_back(GEvalQuadAt(pts, t));
                        }
                        break;
                    }
                    case GPath::Verb::kCubic: {
                        float step = 1.f / GFlattenCubic(pts);
                        for(float t = 0; t < 1.f + step; t += step) {
                            pointList.push_back(GEvalCubicAt(pts, t));
                        } 
                        break;
                    }
                    case GPath::Verb::kDone:
                        break;
                }
            } while(currVb != GPath::Verb::kDone);
            
            CLOSE:
            if(pointList.size() > 0) {
                pointList.push_back(pointList[0]);
            }

//////////////////////////////////////////////////////
            std::vector<GPoint> points;
            if(pointList.size() > 2){
                GClipper ec(bounds);
                ec.ClipPoints(pointList.data(), pointList.size(), points);   
            }
/////////////////////////////////////////////////////

            if(points.size() < 3) { continue; }

            GEdge edgeList[points.size()];
            int edgeListSize = 0;
            for(int i = 0; i < points.size()-1; i++) {
                GEdge e;
                if(e.setLine(points[i], points[i+1])) { 
                    int j = 0;
                    while(j <= edgeListSize && edge_lt(edgeList[j++], e)){}       
                    memcpy(edgeList+j, edgeList+j-1, 
                            (edgeListSize-j+1)*sizeof(e)); 
                    edgeList[j-1] = e;
                    edgeListSize++;
                }
            } 
            if(edgeListSize > 1) {
                for(int i = 0; i < edgeListSize; i++){
                    edges.push_back(edgeList[i]);
                }
            }
        }

        walk_edges(edges.data(), edges.size(), blitter);

    }
    
    static void FillTriangle(const GPoint points[], const GIRect& bounds, GBlitter* blitter){
        FillConvexPoly(points, 3, bounds, blitter);
    }

};

#endif