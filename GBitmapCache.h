#include <iostream>
#include <fstream>
class GShaderCache {
public:

    GShaderCache() {}

    GShaderCache(int width, int height) : fWidth(width), fHeight(height) {
        fBitmap = (GPixel*)calloc(width*height, sizeof(GPixel));
    }

    void insert(int x, int y, GPixel p) {
        assert(x < fWidth && x >= 0);
        assert(y < fHeight && y >= 0);
        *(fBitmap + y * fWidth + x) = p;
    }

    int get(int x, int y, GPixel* dst, int max) {
        assert(x < fWidth && x >= 0);
        assert(y < fHeight && y >= 0);
        int a = y * fWidth + x;
        int m = fWidth - x;
        if(m > max) m = max;
        if(y >= fHeight) { return 0; }
        GPixel* ptr = fBitmap + a;
        int i = 0;
        while(i < m && *ptr != 0) { ptr++; i++; }
        if(i > 1) {
            memcpy(dst, ptr, (i-1)*sizeof(GPixel));
            return i-1;
        }
        return 0;
    }

    bool dump() {
        std::ofstream file("dump.ppm",  std::ofstream::trunc);
        if(file.is_open()) {
            file << "P3\n" << fWidth << " " << fHeight << "\n255\n";
            for(int y = 0; y < fHeight; y++) {
                for(int x = 0; x <  fWidth; x++) {
                    if(x % 23 == 22) { file << "\n"; }
                    GPixel p = *(fBitmap + y * fWidth + x);
                    file << GPixel_GetR(p) << 
                            " " << GPixel_GetG(p) << 
                            " " << GPixel_GetB(p) << " ";
                }
                file << "\n";
            }
            file.close();
            return true;
        }
        else {
            std::cout << "Unable to open file";
            return false;
        }
    }

private:
    int                     fWidth;
    int                     fHeight;
    GPixel*                 fBitmap;
};
