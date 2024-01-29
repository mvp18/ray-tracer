#include <rt/solids/disc.h>
#include <core/scalar.h>
#include <core/random.h>

namespace rt {

Disc::Disc(const Point& center, const Vector& normal, float radius, CoordMapper* texMapper, Material* material): Solid(texMapper, material)
{
    center_ = center;
    normal_ = normal;
    radius_ = radius;

    dx_ = sqrt(normal_.y*normal_.y + normal_.z*normal_.z);
    dy_ = sqrt(normal_.x*normal_.x + normal_.z*normal_.z);
    dz_ = sqrt(normal_.x*normal_.x + normal_.y*normal_.y);

    min_ex = center_ - radius_*Vector(dx_, dy_, dz_);
    max_ex = center_ + radius_*Vector(dx_, dy_, dz_);

    M = getOrthogonalBasis(normal_.normalize());
}

BBox Disc::getBounds() const {
    return BBox(min_ex, max_ex);
}

Matrix Disc::getOrthogonalBasis(Vector n) const {

    Vector s, s_, t;

    // like in rt/primmod/instance.cpp
    if (fabs(n.x)<fabs(n.y) && fabs(n.x)<fabs(n.z)) s_ = Vector(0, -n.z, n.y);
    if (fabs(n.y)<fabs(n.x) && fabs(n.y)<fabs(n.z)) s_ = Vector(-n.z, 0, n.x);
    if (fabs(n.z)<fabs(n.x) && fabs(n.z)<fabs(n.y)) s_ = Vector(-n.y, n.x, 0);
    s = s_.normalize();
    t = cross(n, s).normalize();

    Matrix M = Matrix::system(s, t, n);

    return M;
}

Intersection Disc::intersect(const Ray& ray, float tmin, float tmax) const {
    Intersection int_fail = Intersection::failure();
    float den = dot(ray.d, normal_.normalize());
    float radsqr = sqr(radius_);
    
    if (den==0) return int_fail;

    float t = dot((center_ - ray.o), normal_.normalize())/den;
    Point local_ = ray.getPoint(t);

    if (t < tmin || t > tmax || (local_ - center_).lensqr() > radsqr) return int_fail;
    return Intersection(t, ray, this, normal_.normalize(), local_);
}

Vector Disc::polar2cartesian(float r, float theta) const{
    return M * Vector(r*cos(theta), r*sin(theta), 0);
}

Solid::Sample Disc::sample() const {

    Solid::Sample s;
    
    s.normal = normal_.normalize();
    
    float u = random(0.0f, 1.0f);
    float v = random(0.0f, 1.0f);

    Vector center_offset = polar2cartesian(sqrtf(v) * radius_, 2 * pi * u);
    s.point = center_ + center_offset;

    return s;
}

float Disc::getArea() const {
    return pi * sqr(radius_);
}

}
