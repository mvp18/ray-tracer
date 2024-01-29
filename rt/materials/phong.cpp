#include <rt/materials/phong.h>
#include <rt/textures/texture.h>
#include <core/scalar.h>

namespace rt {

PhongMaterial::PhongMaterial(Texture* specular, float exponent)
{
    specular_ = specular;
    exponent_ = exponent;
    // http://www.farbrausch.de/~fg/stuff/phong.pdf, http://www.thetenthplanet.de/archives/255
    Ks = (exponent_ + 2)/(2 * pi); // Kd = 0, hence Cs = (n+2)/(2*pi)
}

RGBColor PhongMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
    
    Vector IdealReflection = -inDir + 2 * dot(normal, inDir) * normal;
    float cosTheta_RV = max(0.0f, dot(IdealReflection.normalize(), outDir));
    float cosTheta_IN = max(0.0f, dot(inDir, normal));
    return Ks * specular_->getColor(texPoint) * pow(cosTheta_RV, exponent_) * cosTheta_IN;
}

RGBColor PhongMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    CG_UNUSED(texPoint);
    CG_UNUSED(normal);
    CG_UNUSED(outDir);
    
    return RGBColor::rep(0.0f);
}

Material::SampleReflectance PhongMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    CG_UNUSED(texPoint);
    CG_UNUSED(normal);
    CG_UNUSED(outDir);

	UNREACHABLE;
}

Material::Sampling PhongMaterial::useSampling() const {
	return SAMPLING_NOT_NEEDED;
}

}