#include <rt/cameras/perspective.h>
#include <rt/ray.h>
#include <core/vector.h>
#include <cmath>

namespace rt {

PerspectiveCamera::PerspectiveCamera(const Point& center, const Vector& forward, const Vector& up, 
                                    float verticalOpeningAngle, float horizontalOpeningAngle): center_(center)
{
    // Left Handed Coordinate System
    forward_ = forward.normalize(); // z-axis
    right_ = cross(forward, up).normalize(); // x-axis
    up_ = cross(right_, forward_).normalize(); // y-axis

    // Right Handed Coordinate System
    // forward_ = forward.normalize(); // z-axis
    // right_ = cross(up, forward).normalize(); // x-axis
    // up_ = cross(forward_, right_).normalize(); // y-axis

    scale_x = tan(horizontalOpeningAngle/2.0f);
    scale_y = tan(verticalOpeningAngle/2.0f);
}

Ray PerspectiveCamera::getPrimaryRay(float x, float y) const {
    Vector dir = x*scale_x*right_ + y*scale_y*up_ + forward_;
    return Ray(center_, dir.normalize());
}

}
