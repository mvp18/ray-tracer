#include <rt/cameras/fisheye.h>
#include <rt/ray.h>
#include <core/vector.h>
#include <cmath>

namespace rt {

FisheyeCamera::FisheyeCamera(const Point& center, const Vector& forward, const Vector& up, 
                             float inclinationAngle): center_(center), inclinationAngle_(inclinationAngle)
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

Ray FisheyeCamera::getPrimaryRay(float x, float y) const {
    
    float r, phi, theta, scale_x, scale_y, scale_z;
    
    r = sqrt(x*x + y*y);
    phi = atan2(y, x);
    theta = r*(inclinationAngle_/2.0f);
    
    scale_x = sin(theta)*cos(phi);
    scale_y = sin(theta)*sin(phi);
    scale_z = cos(theta);
    
    Vector dir = scale_x*right_ + scale_y*up_ + scale_z*forward_;
    return Ray(center_, dir.normalize());
}

}
