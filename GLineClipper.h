bool GEdgeClipper::clipLine(GPoint p0, GPoint p1, const GRect& clip) {

}

static float sect_with_horizontal(const GPoint pts[2], GScalar Y) {
    return pts[0].fX + (Y - pts[0].fY) * (pts[1].fX - pts[0].fX) / (pts[1].fY - pts[0].fY);
}

static float sect_with_vertical(const GPoint pts[2], GScalar Y) {
    return pts[0].fY + (X - pts[0].fX) * (pts[1].fY - pts[0].fY) / (pts[1].fX - pts[0].fX);
}

int ClipLine(const GPoint pts[], const GRect& clip, GPoint lines[]) {
    int minY, maxY;

    if(pts[0].fY < pts[1].fY) {
        minY = 0;
        maxY = 1;
    }
    else {
        minY = 1;
        maxY = 0;
    }

    if(pts[maxY].fY <= clip.fTop) {
        return 0;
    }
    if(pts[minY].fY >= clip.fBottom) {
        return 0;
    }

    GPoint tmp[2];
    memcpy(tmp, pts, sizeof(tmp));

    if(pts[minY] < clip.fTop) {
        tmp[0].set(sect_with_horizontal(pts, clip.fTop), clip.fTop);
    }
    if(pts[maxY] > clip.fBottom) {
        tmp[0].set(sect_with_horizontal(pts, clip.fBottom), clip.fBottom);
    }

    

}