#include <core/scalar.h>
#include <core/point.h>
#include <core/color.h>
#include <core/interpolate.h>
#include <rt/textures/perlin.h>

namespace rt {

// returns a value in range -1 to 1
static inline float noise(int x, int y, int z) {
    int n = x + y * 57 + z * 997;
    n = (n << 13) ^ n;
    return (1.0f - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);
}

PerlinTexture::PerlinTexture(const RGBColor& white, const RGBColor& black)
{
    white_ = white;
    black_ = black;
}

float PerlinTexture::getNoise(const Point& p){

    int l_u, l_v, l_w; // lattice coordinates
    float f_u, f_v, f_w; // interpolation coefficients

    l_u = (int)floor(p.x);
    l_v = (int)floor(p.y);
    l_w = (int)floor(p.z);

    f_u = p.x - floor(p.x);
    f_v = p.y - floor(p.y);
    f_w = p.z - floor(p.z);

    float px0y0z0 = noise(l_u, l_v, l_w);
    float px1y0z0 = noise(l_u + 1, l_v, l_w);
    float px0y1z0 = noise(l_u, l_v + 1, l_w);
    float px1y1z0 = noise(l_u + 1, l_v + 1, l_w);
    float px0y0z1 = noise(l_u, l_v, l_w + 1);
    float px1y0z1 = noise(l_u + 1, l_v, l_w + 1);
    float px0y1z1 = noise(l_u, l_v + 1, l_w + 1);
    float px1y1z1 = noise(l_u + 1, l_v + 1, l_w + 1);

    // Use trilinear interpolation as smoothing function
    return lerp3d(px0y0z0, px1y0z0, px0y1z0, px1y1z0, px0y0z1, px1y0z1, px0y1z1, px1y1z1, f_u, f_v, f_w);
}

RGBColor PerlinTexture::getColor(const Point& coord) {

    // 3d value-based Perlin noise
    float turbulence = 0.0f;
    for (auto& it : octaves){
        // turbulence += fabs(it.amplitude * getNoise(it.frequency * coord));
        turbulence += it.amplitude * getNoise(it.frequency * coord);
    }

    // return lerp(black_, white_, turbulence);
    return lerp(black_, white_, (turbulence + 1) / 2.0f); // [-1, 1] -> [0, 1]
}

RGBColor PerlinTexture::getColorDX(const Point& coord) {
    CG_UNUSED(coord);

    /* TODO */ NOT_IMPLEMENTED;
}

RGBColor PerlinTexture::getColorDY(const Point& coord) {
    CG_UNUSED(coord);

    /* TODO */ NOT_IMPLEMENTED;
}

void PerlinTexture::addOctave(float amplitude, float frequency) {
    
    Octave o;
    o.amplitude = amplitude;
    o.frequency = frequency;
    octaves.push_back(o);

}

}