#include <rt/coordmappers/cylindrical.h>

namespace rt {

CylindricalCoordMapper::CylindricalCoordMapper(const Point& origin, const Vector& longitudinalAxis, const Vector& polarAxis)
{
    origin_ = origin;

    // similar to perspective camera but right-handed coordinate system
    longitudinalAxis_ = longitudinalAxis.normalize(); // z-axis
    perpendicularAxis_ = cross(longitudinalAxis, polarAxis).normalize(); // y-axis
    polarAxis_ = cross(perpendicularAxis_, longitudinalAxis_).normalize(); // x-axis

    scale_x = polarAxis.length();
    scale_y = longitudinalAxis.length();
}

Point CylindricalCoordMapper::getCoords(const Intersection& hit) const {
    
    Vector globalOriginToHit = hit.local() - origin_;
    // project hit point vector onto cylindrical coordinate system
    Vector localOriginToHit = Vector(dot(globalOriginToHit, polarAxis_), dot(globalOriginToHit, perpendicularAxis_), dot(globalOriginToHit, longitudinalAxis_));

    // https://en.wikipedia.org/wiki/Cylindrical_coordinate_system
    float radial_rho = sqrtf(localOriginToHit.x*localOriginToHit.x + localOriginToHit.y*localOriginToHit.y); // [0, inf]
    float azimuth_phi = -atan2(localOriginToHit.y, localOriginToHit.x); // [-pi, pi]
    float u = azimuth_phi / (2 * pi * scale_x); // [0, 1]
    float v = localOriginToHit.z / scale_y; // [0, 1]

    return Point(u, v, radial_rho);

}

}