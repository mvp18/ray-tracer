#include <rt/materials/dielectric.h>
#include <core/scalar.h>
#include <core/random.h>
#include <cmath>

namespace rt {

DielectricMaterial::DielectricMaterial(float eta)
{
    eta_ = eta;
}

RGBColor DielectricMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
    CG_UNUSED(texPoint);
    CG_UNUSED(normal);
    CG_UNUSED(outDir);
    CG_UNUSED(inDir);
    
    return RGBColor::rep(0.0f);
}

RGBColor DielectricMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    CG_UNUSED(texPoint);
    CG_UNUSED(normal);
    CG_UNUSED(outDir);
    
    return RGBColor::rep(0.0f);
}

Material::SampleReflectance DielectricMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    
    float cosTheta_i = dot(outDir, normal);

    Vector hit_normal;
    float etaI, etaT, etaR;
    
    // light entering the material
    if (cosTheta_i > 0.0f){
        etaI = 1.0f;
        etaT = eta_;
        hit_normal = normal;
    }
    // light exiting the material
    else{
        etaI = eta_;
        etaT = 1.0f;
        cosTheta_i = -cosTheta_i;
        hit_normal = -normal;
    }
    etaR = etaI / etaT;

    // Snell's law
    float sinTheta_i = sqrtf(1.0f - sqr(cosTheta_i));
    float sinTheta_t = etaR * sinTheta_i;
    float cosTheta_t = sqrtf(1.0f - sqr(sinTheta_t));

    // Fresnel formula
    float r_parallel = (etaT * cosTheta_i - etaI * cosTheta_t) / (etaT * cosTheta_i + etaI * cosTheta_t);
    float r_perp = (etaI * cosTheta_i - etaT * cosTheta_t) / (etaI * cosTheta_i + etaT * cosTheta_t);

    // Fresnel term for unpolarized right
    float reflectance = 0.5 * (sqr(r_parallel) + sqr(r_perp));
    float transmittance = 1.0f - reflectance;

    // reflection direction
    Vector IdealReflection = -outDir + 2 * dot(hit_normal, outDir) * hit_normal;

    // Total Internal Reflection
    if (sinTheta_t > 1.0f){
        return SampleReflectance(IdealReflection.normalize(), RGBColor::rep(1.0f));
    }

    float coin_toss = random(0.0f, 1.0f);

    // reflection sampled
    if (coin_toss < 0.5){
        return SampleReflectance(IdealReflection.normalize(), RGBColor::rep(2*reflectance));
    }
    else{
        // refraction sampled
        Vector IdealRefraction = -etaR * outDir + (etaR * cosTheta_i - cosTheta_t) * hit_normal;
        // test case expects 2*transmittance/(sqr(eta_)), but glass sphere will be much darker in the rendered image
        return SampleReflectance(IdealRefraction.normalize(), RGBColor::rep(2*transmittance/(sqr(etaR)))); // Lec06 slide 32
    }
}

Material::Sampling DielectricMaterial::useSampling() const {
    return SAMPLING_ALL;
}

}