#include <rt/solids/infiniteplane.h>

namespace rt {

InfinitePlane::InfinitePlane(const Point& origin, const Vector& normal, CoordMapper* texMapper, Material* material): Solid(texMapper, material)
{
    origin_ = origin;
    normal_ = normal;
}

BBox InfinitePlane::getBounds() const {
    return BBox::full();
}

Intersection InfinitePlane::intersect(const Ray& ray, float tmin, float tmax) const {

    Intersection int_fail = Intersection::failure();
    float den = dot(ray.d, normal_.normalize());
    
    if (den==0) return int_fail; // not returning infinite solutions if ray is in the plane?

    float t = dot((origin_ - ray.o), normal_.normalize())/den;

    if (t < tmin || t > tmax) return int_fail;
    return Intersection(t, ray, this, normal_.normalize(), ray.getPoint(t));
}

Solid::Sample InfinitePlane::sample() const {
    /* TODO */ NOT_IMPLEMENTED;
}

float InfinitePlane::getArea() const {
    return FLT_MAX;
}

}
