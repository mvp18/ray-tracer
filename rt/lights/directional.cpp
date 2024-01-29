#include <rt/lights/directional.h>
#include <core/scalar.h>
#include <core/vector.h>

namespace rt {

DirectionalLight::DirectionalLight(const Vector& direction, const RGBColor& color)
{
    direction_ = direction.normalize();
    color_ = color;
}

LightHit DirectionalLight::getLightHit(const Point& p) const {
    
    LightHit lightHit;
    lightHit.distance = FLT_MAX;
    lightHit.direction = -direction_;
    lightHit.normal = direction_;

    return lightHit;
}

RGBColor DirectionalLight::getIntensity(const LightHit& irr) const {
    return color_;
}

}
