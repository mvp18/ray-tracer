#include <rt/coordmappers/plane.h>
#include <core/point.h>
#include <core/vector.h>
#include <core/matrix.h>
#include <rt/intersection.h>

namespace rt {

PlaneCoordMapper::PlaneCoordMapper(const Vector& e1, const Vector& e2) {
    normal = cross(e1, e2);
    transform_inv = Matrix::system(e1, e2, normal).invert();
}

Point PlaneCoordMapper::getCoords(const Intersection& hit) const {

   return transform_inv * hit.local();
}

}