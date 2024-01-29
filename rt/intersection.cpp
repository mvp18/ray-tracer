#include <rt/intersection.h>

namespace rt {

Intersection::Intersection(float distance, const Ray& ray, const Solid* solid, const Vector& normal, const Point& local)
{
    this->distance = distance;
    this->ray = ray;
    this->solid = solid;
    this->intersected = true;
    this->normal_ = normal;
    this->local_ = local;
}

Intersection::operator bool() const {
    return this->intersected;
}

Intersection Intersection::failure() {
    Intersection int_fail;
    int_fail.intersected = false;
    return int_fail;
}

Point Intersection::hitPoint() const {
    return this->ray.getPoint(this->distance);
}

Vector Intersection::normal() const {
    return this->normal_;
}

Point Intersection::local() const {
    return this->local_;
}

}
