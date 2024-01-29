#include <rt/materials/dummy.h>
#include <core/scalar.h>
#include <core/assert.h>

namespace rt {

DummyMaterial::DummyMaterial() {
    fr_ = 1.0f;
}

RGBColor DummyMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
    CG_UNUSED(texPoint);
    CG_UNUSED(outDir);

    float cosTheta_IN = max(0.0f, dot(inDir, normal));
    return RGBColor::rep(fr_*cosTheta_IN);
}

RGBColor DummyMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    CG_UNUSED(texPoint);
    CG_UNUSED(normal);
    CG_UNUSED(outDir);
    
    return RGBColor::rep(0.0f);
}

Material::SampleReflectance DummyMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    CG_UNUSED(texPoint);
    CG_UNUSED(normal);
    CG_UNUSED(outDir);
    
    return SampleReflectance();
}

}