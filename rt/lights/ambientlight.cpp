#include <rt/lights/ambientlight.h>
#include <core/scalar.h>

namespace rt {

AmbientLight::AmbientLight(const RGBColor& intensity, bool reflectance_mode)
{
    intensity_ = intensity;
    this->reflectance_mode = reflectance_mode;
    this->isAmbient = true;
}

LightHit AmbientLight::getLightHit(const Point& p) const {
    
    LightHit lightHit;
    lightHit.direction = Vector::rep(0.0f);
    lightHit.distance = 0.0f; // would light up all objects in the scene
    lightHit.normal = Vector::rep(0.0f);
    
    return lightHit;
}

RGBColor AmbientLight::getIntensity(const LightHit& irr) const {
    return intensity_;
}

}