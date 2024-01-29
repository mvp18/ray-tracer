#ifndef CG1RAYTRACER_CAMERAS_ENVIRONMENT_HEADER
#define CG1RAYTRACER_CAMERAS_ENVIRONMENT_HEADER

#include <rt/cameras/camera.h>
#include <core/vector.h>
#include <core/point.h>

namespace rt {

class EnvironmentCamera : public Camera {
public:
    EnvironmentCamera(
        const Point& center,
        const Vector& forward,
        const Vector& up,
        float elevationAngle,
        float horizonalOpeningAngle
        );

    virtual Ray getPrimaryRay(float x, float y) const;
private:
    const Point center_;
    float elevationAngle_, horizonalOpeningAngle_;
    Vector forward_, right_, up_;
};

}


#endif