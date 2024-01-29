#include <rt/solids/sphere.h>
#include <core/scalar.h>

namespace rt {

Sphere::Sphere(const Point& center, float radius, CoordMapper* texMapper, Material* material): Solid(texMapper, material)
{
    center_ = center;
    radius_ = radius;

    min_ex = center_ - Vector(radius_, radius_, radius_);
    max_ex = center_ + Vector(radius_, radius_, radius_);
}

BBox Sphere::getBounds() const {
    return BBox(min_ex, max_ex);
}

Intersection Sphere::intersect(const Ray& ray, float tmin, float tmax) const {

    // https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection
    
    float A, B, C, discriminant, t;
    Intersection int_fail = Intersection::failure();

    A = dot(ray.d, ray.d);
    B = 2 * dot(ray.d, ray.o - center_);
    C = dot(ray.o - center_, ray.o - center_) - sqr(radius_);
    discriminant = sqr(B) - 4*A*C;

    if (discriminant < 0) return int_fail;
    if (discriminant == 0) t = -B/(2*A); 
    else{
        float sign_B, t1, t2, q;
        // t1 = (-B + sqrtf(discriminant))/(2*A);
        // t2 = (-B - sqrtf(discriminant))/(2*A);

        // trying more numerically stable version
        sign_B = (B > 0.0f) - (B < 0.0f);
        q = -0.5*(B + sign_B*sqrtf(discriminant));

        t1 = q/A;
        t2 = C/q;

        bool t1_valid = (t1>tmin) && (t1<tmax);
        bool t2_valid = (t2>tmin) && (t2<tmax);

        if (!t1_valid && !t2_valid) return int_fail;
        if (t1_valid && !t2_valid) t = t1;
        if (!t1_valid && t2_valid) t = t2;
        if (t1_valid && t2_valid) t = min(t1, t2);
    }

    Point local_ = ray.getPoint(t);
    Vector sphere_normal = Vector(local_ - center_);

    return Intersection(t, ray, this, sphere_normal.normalize(), local_);
}

Solid::Sample Sphere::sample() const {
	NOT_IMPLEMENTED;
}

float Sphere::getArea() const {
    return 4 * pi* sqr(radius_);
}

}
