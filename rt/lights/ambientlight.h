#ifndef CG1RAYTRACER_LIGHTS_AMBIENTLIGHT_HEADER
#define CG1RAYTRACER_LIGHTS_AMBIENTLIGHT_HEADER

#include <rt/lights/light.h>
#include <core/color.h>

namespace rt {

class AmbientLight : public Light {
public:
	AmbientLight() {}
	AmbientLight(const RGBColor& intensity, bool reflectance_mode = false);
	virtual LightHit getLightHit(const Point& p) const;
    virtual RGBColor getIntensity(const LightHit& irr) const;
private:
	RGBColor intensity_;
};

}

#endif