#include <rt/lights/arealight.h>
#include <rt/solids/solid.h>
#include <rt/materials/material.h>
#include <core/scalar.h>
#include <core/color.h>

namespace rt {

LightHit AreaLight::getLightHit(const Point& p) const {
    
    LightHit lightHit;
    
    Solid::Sample s = source_->sample();
    lightHit.direction = s.point - p;
    lightHit.distance = lightHit.direction.length() - rt::epsilon;
    lightHit.direction = lightHit.direction.normalize();
    lightHit.normal = s.normal;

    return lightHit;
}

RGBColor AreaLight::getIntensity(const LightHit& irr) const {
    
    RGBColor intensity = source_->material->getEmission(Point(), irr.normal, -irr.direction);
    float cosTheta_OUT = max(0.0f, dot(-irr.direction, irr.normal));
    return intensity * source_->getArea() * cosTheta_OUT / sqr(irr.distance);
}

AreaLight::AreaLight(Solid* source)
{
    source_ = source;
}

}