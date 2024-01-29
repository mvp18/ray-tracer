#include <core/image.h>
#include <core/scalar.h>
#include <core/point.h>
#include <core/interpolate.h>
#include <rt/textures/imagetex.h>

namespace rt {

ImageTexture::ImageTexture()
{
    /* TODO */
}

ImageTexture::ImageTexture(const std::string& filename, BorderHandlingType bh, InterpolationType i)
{
    image_.readPNG(filename);
    bh_ = bh;
    i_ = i;

    resU = (float)image_.width();
    resV = (float)image_.height();
}

ImageTexture::ImageTexture(const Image& image, BorderHandlingType bh, InterpolationType i)
{
    image_ = image;
    bh_ = bh;
    i_ = i;

    resU = (float)image_.width();
    resV = (float)image_.height();
}

RGBColor ImageTexture::getColor(const Point& coord) {
    
    // coord may not be normalized like texture coordinates
    float t_u, t_v; // fractional coordinates
    int l_u, l_v; // lattice coordinates

    // local coordinates
    float u = coord.x;
    float v = coord.y;

    if (bh_ == BorderHandlingType::REPEAT){
        t_u = u - floor(u);
        t_v = v - floor(v);
    }
    else if (bh_ == BorderHandlingType::MIRROR){
        t_u = u - floor(u);
        t_v = v - floor(v);

        l_u = (int)floor(u);
        l_v = (int)floor(v);
        
        if (l_u % 2 == 1) t_u = 1 - t_u;
        if (l_v % 2 == 1) t_v = 1 - t_v;
    }
    else if (bh_ == BorderHandlingType::CLAMP){
        t_u = u;
        t_v = v;
        
        if (u < 0) t_u = 0.0;
        if (u > 1) t_u = 1.0;
        if (v < 0) t_v = 0.0;
        if (v > 1) t_v = 1.0;
    }
    
    if (i_ == InterpolationType::NEAREST){
        u = t_u * resU;
        v = t_v * resV;

        l_u = min((int)floor(u), (int)resU - 1);
        l_v = min((int)floor(v), (int)resV - 1);

        return image_(l_u, l_v);
    }

    else if (i_ == InterpolationType::BILINEAR){
        float f_u, f_v; // fractional coordinates

        u = t_u * (resU - 1);
        v = t_v * (resV - 1);

        f_u = u - floor(u);
        f_v = v - floor(v);

        l_u = (int)floor(u);
        l_v = (int)floor(v);

        return lerp2d(image_(l_u, l_v), image_(l_u + 1, l_v), image_(l_u, l_v + 1), image_(l_u + 1, l_v + 1), f_u, f_v);
    }

    return RGBColor::rep(0.0f);
}

RGBColor ImageTexture::getColorDX(const Point& coord) {
    /* TODO */ NOT_IMPLEMENTED;
}

RGBColor ImageTexture::getColorDY(const Point& coord) {
    /* TODO */ NOT_IMPLEMENTED;
}

}