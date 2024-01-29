#include <rt/coordmappers/environment.h>
#include <rt/intersection.h>

namespace rt {

EnvironCoordMapper::EnvironCoordMapper() 
{}

Point EnvironCoordMapper::getCoords(const Intersection& hit) const {
   return Point(hit.ray.d.x, hit.ray.d.y, hit.ray.d.z);
}

}