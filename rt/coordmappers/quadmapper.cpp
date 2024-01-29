#include <rt/coordmappers/quadmapper.h>
#include <core/homogeneouscoord.h>

namespace rt {

QuadMapper::QuadMapper(const Vector& offset, const Vector& scale){

    offset_ = offset;
    scale_ = scale;
}

Point QuadMapper::getCoords(const Intersection& hit) const {
    
    HomogeneousCoord p = HomogeneousCoord(scale_) * HomogeneousCoord(hit.local()) + HomogeneousCoord(offset_);    
    return Point(p[0], p[1], p[2]);
}

}