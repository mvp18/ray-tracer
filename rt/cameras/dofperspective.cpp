#include <rt/cameras/dofperspective.h>
#include <rt/solids/disc.h>
#include <rt/ray.h>
#include <core/scalar.h>
#include <core/random.h>

namespace rt {

DOFPerspectiveCamera::DOFPerspectiveCamera(const Point& center, const Vector& forward, const Vector& up, float verticalOpeningAngle, float horizontalOpeningAngle, float focalDistance, float apertureRadius)
    : center_(center), focalDistance_(focalDistance), apertureRadius_(apertureRadius)
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

Ray DOFPerspectiveCamera::getPrimaryRay(float x, float y) const {
    
    Vector dir = x*scale_x*right_ + y*scale_y*up_ + forward_;
    Point focal_point = center_ + focalDistance_ * dir; // dir not normalized here because focal point lies on focal plane and not focal sphere

    float ar = sqrtf(random(0.0f, 1.0f)) * apertureRadius_; // disc sampling
    float phi = 2 * pi * random(0.0f, 1.0f);
    float x_ = ar * cos(phi);
    float y_ = ar * sin(phi);
    Point newCenter = center_ + x_*right_ + y_*up_;

    // Disc lensAperture = Disc(center_, forward_, apertureRadius_, nullptr, nullptr);
    // Point newCenter = lensAperture.sample().point;
    
    Vector newDir = focal_point - newCenter;

    return Ray(newCenter, newDir.normalize());
}

}