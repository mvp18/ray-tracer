#ifndef CG1RAYTRACER_TEXTURES_PERLIN_HEADER
#define CG1RAYTRACER_TEXTURES_PERLIN_HEADER

#include <rt/textures/texture.h>
#include <vector>

namespace rt {

class PerlinTexture : public Texture {
public:
    typedef struct Octave {
        float amplitude;
        float frequency;
    } Octave;

    PerlinTexture(const RGBColor& white, const RGBColor& black);
    void addOctave(float amplitude, float frequency);
    float getNoise(const Point& p);
    virtual RGBColor getColor(const Point& coord);
    virtual RGBColor getColorDX(const Point& coord);
    virtual RGBColor getColorDY(const Point& coord);
private:
    RGBColor white_, black_;
    std::vector<Octave> octaves;
};

}

#endif