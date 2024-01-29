#include <rt/solids/environmentsolid.h>

namespace rt {

EnvironmentSolid::EnvironmentSolid(CoordMapper* texMapper, Material* material) : Solid(texMapper, material)
{}

BBox EnvironmentSolid::getBounds() const {
    return BBox::full();
}

Intersection EnvironmentSolid::intersect(const Ray& ray, float tmin, float tmax) const {
    
    Intersection int_fail = Intersection::failure();
    
    float t = FLT_MAX;
    if (t < tmin || t > tmax) return int_fail;
    else return Intersection(t, ray, this, Vector::rep(0.0f), Point::rep(0.0f));
}

Solid::Sample EnvironmentSolid::sample() const {
    NOT_IMPLEMENTED;
}

float EnvironmentSolid::getArea() const {
    return FLT_MAX;
}

}