#include <rt/materials/fuzzyconductor.h>
#include <core/scalar.h>
#include <rt/solids/disc.h>

namespace rt {

FuzzyConductorMaterial::FuzzyConductorMaterial(float eta, float kappa, float fuzzyangle)
{
    eta_ = eta;
    kappa_ = kappa;
    eta2_kappa2 = sqr(eta_) + sqr(kappa_);
    fuzzyangle_ = fuzzyangle;
}

RGBColor FuzzyConductorMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
    CG_UNUSED(texPoint);
    CG_UNUSED(normal);
    CG_UNUSED(outDir);
    CG_UNUSED(inDir);
    
    return RGBColor::rep(0.0f);
}

RGBColor FuzzyConductorMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    CG_UNUSED(texPoint);
    CG_UNUSED(normal);
    CG_UNUSED(outDir);
    
    return RGBColor::rep(0.0f);
}

void FuzzyConductorMaterial::getPerturbedReflection(const Point& texPoint, const Vector& IdealReflection, Vector& perturbedReflection) const{

    float discRadius = tan(fuzzyangle_); // radius of perturbationDisc, tan(fuzzyangle_) = discRadius / hit_distance
    Point discCenter = texPoint + 1.0f * IdealReflection.normalize(); // center of perturbationDisc
    Disc perturbationDisc(discCenter, IdealReflection.normalize(), discRadius, nullptr, nullptr); // perturbationDisc
    perturbedReflection = perturbationDisc.sample().point - texPoint; // perturbed reflection direction
}

Material::SampleReflectance FuzzyConductorMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    
    Vector IdealReflection = -outDir + 2 * dot(normal, outDir) * normal; // sampling incoming direction
    
    Vector perturbedReflection = -normal;
    while (dot(perturbedReflection.normalize(), normal) <= 0.0f) { // perturbed ray may go inside material
        getPerturbedReflection(texPoint, IdealReflection, perturbedReflection);
    }

    float cosTheta_i = dot(outDir, normal);
    float r_parallel2 = (eta2_kappa2 * sqr(cosTheta_i) - 2 * eta_ * cosTheta_i + 1) / (eta2_kappa2 * sqr(cosTheta_i) + 2 * eta_ * cosTheta_i + 1);
    float r_perp2 = (eta2_kappa2 - 2 * eta_ * cosTheta_i + sqr(cosTheta_i)) / (eta2_kappa2 + 2 * eta_ * cosTheta_i + sqr(cosTheta_i));

    float reflectance = 0.5 * (r_parallel2 + r_perp2);

    return SampleReflectance(perturbedReflection.normalize(), RGBColor::rep(reflectance));
}

Material::Sampling FuzzyConductorMaterial::useSampling() const {
    return SAMPLING_ALL;
}

}
