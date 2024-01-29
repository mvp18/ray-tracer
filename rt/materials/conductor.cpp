#include <rt/materials/conductor.h>
#include <rt/materials/material.h>
#include <core/scalar.h>

namespace rt {

ConductorMaterial::ConductorMaterial(float eta, float kappa)
{
    eta_ = eta;
    kappa_ = kappa;
    eta2_kappa2 = sqr(eta_) + sqr(kappa_);
}

RGBColor ConductorMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
    CG_UNUSED(texPoint);
    CG_UNUSED(normal);
    CG_UNUSED(outDir);
    CG_UNUSED(inDir);
    
    return RGBColor::rep(0.0f);
}

RGBColor ConductorMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    CG_UNUSED(texPoint);
    CG_UNUSED(normal);
    CG_UNUSED(outDir);
    
    return RGBColor::rep(0.0f);
}

Material::SampleReflectance ConductorMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    CG_UNUSED(texPoint);
    
    Vector IdealReflection = -outDir + 2 * dot(normal, outDir) * normal; // sampling incoming direction
    
    float cosTheta_i = dot(outDir, normal);
    float r_parallel2 = (eta2_kappa2 * sqr(cosTheta_i) - 2 * eta_ * cosTheta_i + 1) / (eta2_kappa2 * sqr(cosTheta_i) + 2 * eta_ * cosTheta_i + 1);
    float r_perp2 = (eta2_kappa2 - 2 * eta_ * cosTheta_i + sqr(cosTheta_i)) / (eta2_kappa2 + 2 * eta_ * cosTheta_i + sqr(cosTheta_i));

    float reflectance = 0.5 * (r_parallel2 + r_perp2);

    return SampleReflectance(IdealReflection.normalize(), RGBColor::rep(reflectance));
}

Material::Sampling ConductorMaterial::useSampling() const {
    return SAMPLING_ALL;
}

}