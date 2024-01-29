#include <rt/cameras/orthographic.h>
#include <rt/ray.h>
#include <core/point.h>

namespace rt {

OrthographicCamera::OrthographicCamera(const Point& center, const Vector& forward, const Vector& up, 
                                       float scaleX, float scaleY): center_(center), ofwd(forward), scaleX_(scaleX), scaleY_(scaleY)
{
    // Left Handed Coordinate System
    forward_ = forward.normalize(); // z-axis
    right_ = cross(forward, up).normalize(); // x-axis
    up_ = cross(right_, forward_).normalize(); // y-axis

    // Right Handed Coordinate System
    // forward_ = forward.normalize(); // z-axis
    // right_ = cross(up, forward).normalize(); // x-axis
    // up_ = cross(forward_, right_).normalize(); // y-axis
}

Ray OrthographicCamera::getPrimaryRay(float x, float y) const {
    Point shifted_center = center_ + (x*(scaleX_/2.0)*right_ + y*(scaleY_/2.0)*up_);
    return Ray(shifted_center, ofwd.normalize());
}

}
