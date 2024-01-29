#include <rt/coordmappers/world.h>
#include <core/vector.h>
#include <rt/intersection.h>

namespace rt {

Point WorldMapper::getCoords(const Intersection& hit) const {
    
    Point hitPoint = hit.hitPoint(); // world hit point
    Point texPoint = scale_ * hitPoint;

    return texPoint;
}

WorldMapper::WorldMapper()
{
    scale_ = HomogeneousCoord::rep(1.0f);
}

WorldMapper::WorldMapper(const Vector& scale)
{
    scale_ = HomogeneousCoord(scale);
    scale_[3] = 1.0f;
}

}