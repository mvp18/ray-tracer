#include <rt/coordmappers/tmapper.h>
#include <core/homogeneouscoord.h>
#include <rt/intersection.h>
#include <core/interpolate.h>

namespace rt {

TriangleMapper::TriangleMapper(Point ntv[3])
{
    ntv_[0] = ntv[0];
    ntv_[1] = ntv[1];
    ntv_[2] = ntv[2];
}

TriangleMapper::TriangleMapper(const Point& tv0, const Point& tv1, const Point& tv2)
{
    ntv_[0] = tv0;
    ntv_[1] = tv1;
    ntv_[2] = tv2;
}

Point TriangleMapper::getCoords(const Intersection& hit) const {
    
    Point local = hit.local(); // triangle barycentric coordinates - local.x = u, local.y = v, local.z = 1-u-v
    Point texPoint = lerpbar(ntv_[0], ntv_[1], ntv_[2], local.x, local.y); // interpolate texture coordinates
    return texPoint;
}

}