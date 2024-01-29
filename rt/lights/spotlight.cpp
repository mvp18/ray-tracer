#include <rt/lights/spotlight.h>

namespace rt {

SpotLight::SpotLight(const Point& position, const Vector& direction, float angle, float power, const RGBColor& intensity)
{
    position_ = position;
    direction_ = direction.normalize();
    angle_ = angle; // Is this the half or full angle of the cone?
    exp_ = power;
    intensity_ = intensity;
}

LightHit SpotLight::getLightHit(const Point& p) const {
    
    LightHit lightHit;
    lightHit.direction = position_ - p;
    lightHit.distance = lightHit.direction.length();
    lightHit.direction = lightHit.direction.normalize();
    lightHit.normal = -lightHit.direction;
    
    return lightHit;
}

RGBColor SpotLight::getIntensity(const LightHit& irr) const {
    
    float cosTheta = dot(-irr.direction, direction_);
    if (acos(cosTheta) < angle_) return intensity_ * pow(cosTheta, exp_) / sqr(irr.distance);
    else return RGBColor::rep(0.0f);
}

}
