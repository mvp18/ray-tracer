#include <rt/coordmappers/spherical.h>

namespace rt {

SphericalCoordMapper::SphericalCoordMapper(const Point& origin, const Vector& zenith, const Vector& azimuthRef)
{
    origin_ = origin;

    // similar to perspective camera but right-handed coordinate system
    zenith_ = zenith.normalize(); // z-axis
    perpendicularAxis_ = cross(zenith, azimuthRef).normalize(); // y-axis
    azimuthRef_ = cross(perpendicularAxis_, zenith_).normalize(); // x-axis

    scale_x = azimuthRef.length();
    scale_y = zenith.length();
}

Point SphericalCoordMapper::getCoords(const Intersection& hit) const {
    
    Vector globalOriginToHit = hit.local() - origin_;
    // project hit point vector onto spherical coordinate system
    Vector localOriginToHit = Vector(dot(globalOriginToHit, azimuthRef_), dot(globalOriginToHit, perpendicularAxis_), dot(globalOriginToHit, zenith_));

    // https://en.wikipedia.org/wiki/Spherical_coordinate_system
    float radial_r = localOriginToHit.length(); // [0, inf]
    float polar_theta = acos(localOriginToHit.z / radial_r); // [0, pi]
    float azimuth_phi = -atan2(localOriginToHit.y, localOriginToHit.x); // [-pi, pi]

    float u = azimuth_phi / (2 * pi * scale_x); // [0, 1]
    float v = polar_theta / (pi * scale_y); // [0, 1]

    return Point(u, v, radial_r); // no idea why I should return the radial_r here, but test case expects it
}

}