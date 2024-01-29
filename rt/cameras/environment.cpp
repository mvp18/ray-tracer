#include <rt/cameras/environment.h>
#include <rt/ray.h>
#include <core/vector.h>
#include <cmath>

namespace rt {

EnvironmentCamera::EnvironmentCamera(const Point& center, const Vector& forward, const Vector& up, float elevationAngle, 
                                    float horizontalOpeningAngle): center_(center), elevationAngle_(elevationAngle), horizonalOpeningAngle_(horizontalOpeningAngle)
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

Ray EnvironmentCamera::getPrimaryRay(float x, float y) const {
    
    float theta, phi, scale_x, scale_y, scale_z;
    
    theta = y*(elevationAngle_/2.0f);
    phi = x*(horizonalOpeningAngle_/2.0f);
    
    scale_x = cos(theta)*cos(phi);
    scale_y = cos(theta)*sin(phi);
    scale_z = sin(theta);
    
    Vector dir = scale_x*right_ + scale_y*up_ + scale_z*forward_;
    return Ray(center_, dir.normalize());
}

}
