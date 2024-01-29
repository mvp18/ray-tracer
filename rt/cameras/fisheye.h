#ifndef CG1RAYTRACER_CAMERAS_FISHEYE_HEADER
#define CG1RAYTRACER_CAMERAS_FISHEYE_HEADER

#include <rt/cameras/camera.h>
#include <core/vector.h>
#include <core/point.h>

namespace rt {

class FisheyeCamera : public Camera {
public:
    FisheyeCamera(
        const Point& center,
        const Vector& forward,
        const Vector& up,
        float inclinationAngle
        );

    virtual Ray getPrimaryRay(float x, float y) const;
private:
    const Point center_;
    float inclinationAngle_;
    Vector forward_, right_, up_;
};

}


#endif