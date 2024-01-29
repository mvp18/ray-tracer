#include <rt/materials/lambertian.h>
#include <rt/textures/texture.h>
#include <core/scalar.h>

namespace rt {

LambertianMaterial::LambertianMaterial(Texture* emission, Texture* diffuse, float Kd)
{
    emission_ = emission;
    diffuse_ = diffuse;
    Kd_ = Kd; // Ks = 0, hence Cs = pi
}

RGBColor LambertianMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
    CG_UNUSED(outDir);
    
    float cosTheta_IN = max(0.0f, dot(inDir, normal));
    return Kd_ * diffuse_->getColor(texPoint) * cosTheta_IN;
}

RGBColor LambertianMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    CG_UNUSED(normal);
    CG_UNUSED(outDir);

    return emission_->getColor(texPoint);
}

Material::SampleReflectance LambertianMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    CG_UNUSED(texPoint);
    CG_UNUSED(normal);
    CG_UNUSED(outDir);
	UNREACHABLE;
}

Material::Sampling LambertianMaterial::useSampling() const {
	return SAMPLING_NOT_NEEDED;
}

}