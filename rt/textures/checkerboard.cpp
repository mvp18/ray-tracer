#include <core/scalar.h>
#include <core/point.h>
#include <rt/textures/checkerboard.h>

namespace rt {

CheckerboardTexture::CheckerboardTexture(const RGBColor& white, const RGBColor& black)
{
    white_ = white;
    black_ = black;
}

RGBColor CheckerboardTexture::getColor(const Point& coord) {
    
    // // 3d lattice coordinates
    int l_u = floor(coord.x / 0.5); // as each cube edge length is 0.5
    int l_v = floor(coord.y / 0.5);
    int l_w = floor(coord.z / 0.5);
    
    if ((l_u + l_v + l_w) % 2 == 0) return white_; // color0
    else return black_; // color1
}

RGBColor CheckerboardTexture::getColorDX(const Point& coord) {
    CG_UNUSED(coord);
    /* TODO */ NOT_IMPLEMENTED;
}

RGBColor CheckerboardTexture::getColorDY(const Point& coord) {
    CG_UNUSED(coord);
    /* TODO */ NOT_IMPLEMENTED;
}

}