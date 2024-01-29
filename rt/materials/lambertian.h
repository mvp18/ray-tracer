#ifndef CG1RAYTRACER_MATERIALS_LAMBERTIAN_HEADER
#define CG1RAYTRACER_MATERIALS_LAMBERTIAN_HEADER

#include <rt/materials/material.h>
#include <core/scalar.h>

namespace rt {

class Texture;

class LambertianMaterial : public Material {
public:
    LambertianMaterial(Texture* emission, Texture* diffuse, float Kd = 1.f/pi);
    virtual RGBColor getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const;
    virtual RGBColor getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const;
    virtual SampleReflectance getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const;
    virtual Sampling useSampling() const;
private:
    Texture* emission_;
    Texture* diffuse_;
    float Kd_;
};

}

#endif