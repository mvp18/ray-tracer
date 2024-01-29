#include <rt/lights/pointlight.h>
#include <core/scalar.h>

namespace rt {

PointLight::PointLight(const Point& position, const RGBColor& intensity)
{
    position_ = position;
    intensity_ = intensity;
}

LightHit PointLight::getLightHit(const Point& p) const {
    
    LightHit lightHit;
    lightHit.direction = position_ - p;
    lightHit.distance = lightHit.direction.length();
    lightHit.direction = lightHit.direction.normalize();
    lightHit.normal = -lightHit.direction;
    
    return lightHit;
}

RGBColor PointLight::getIntensity(const LightHit& irr) const {
    return intensity_ / sqr(irr.distance);
}

}