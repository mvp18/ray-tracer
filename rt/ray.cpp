#include <core/assert.h>
#include <core/macros.h>
#include <rt/ray.h>

namespace rt {

Ray::Ray(const Point& o, const Vector& d)
{
    rt_assert(d != Vector::rep(0.0f));
    this->o = o;
    this->d = d;

    this->inv_dir.x = 1/this->d.x;
    this->inv_dir.y = 1/this->d.y;
    this->inv_dir.z = 1/this->d.z;
}

Point Ray::getPoint(float distance) const {
    return o + distance * d;
}

}
