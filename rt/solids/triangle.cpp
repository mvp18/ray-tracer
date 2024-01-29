#include <rt/solids/triangle.h>
#include <core/random.h>
#include <core/interpolate.h>

namespace rt {

Triangle::Triangle(Point vertices[3], CoordMapper* texMapper, Material* material): Solid(texMapper, material)
{
    a = vertices[0];
    b = vertices[1];
    c = vertices[2];

    normal = cross(b - a, c - a); // triangle normal at vertex a

    min_ex = min(a, min(b, c));
    max_ex = max(a, max(b, c));
}

Triangle::Triangle(const Point& v1, const Point& v2, const Point& v3, CoordMapper* texMapper, Material* material): Solid(texMapper, material)
{
    a = v1;
    b = v2;
    c = v3;

    normal = cross(b - a, c - a); // triangle normal at vertex a

    min_ex = min(a, min(b, c));
    max_ex = max(a, max(b, c));
}

BBox Triangle::getBounds() const {
    return BBox(min_ex, max_ex);
}

Intersection Triangle::intersect(const Ray& ray, float tmin, float tmax) const {

    Intersection int_fail = Intersection::failure();

    /*Möller-Trumbore algorithm:
    https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/moller-trumbore-ray-triangle-intersection*/

    float t, u, v;

    Vector pvec = cross(ray.d, c - a);
    float det = dot(b - a, pvec);

    /*if the determinant is negative the triangle is 'back facing'
    if the determinant is close to 0, the ray misses the triangle*/
    // if (det < rt::epsilon) return int_fail; // When culling is active, rays intersecting triangles from behind will be discarded.
    
    // ray and triangle are parallel if det is close to 0
    if (fabs(det) < rt::epsilon) return int_fail;

    float invDet = 1/det;
    Vector tvec = ray.o - a;
    Vector qvec = cross(tvec, b - a);

    u = dot(tvec, pvec)*invDet;
    if (u < 0 || u > 1) return int_fail;

    v = dot(ray.d, qvec)*invDet;
    if (v < 0 || u + v > 1) return int_fail;

    t = dot(c - a, qvec)*invDet;

    if (t < tmin || t > tmax) return int_fail;

    return Intersection(t, ray, this, normal.normalize(), Point(1-u-v, u, v));



    /*Edge-based Method from lecture - doesn't work :(*/
    
    // float den = dot(ray.d, normal);
    // if (den==0) return int_fail;

    // Vector n_ab, n_bc, n_ca;
    // float obap, ocbp, oacp;

    // n_ab = cross(b - ray.o, a - ray.o); // area of triangle oabx2
    // n_bc = cross(c - ray.o, b - ray.o); // area of triangle obcx2
    // n_ca = cross(a - ray.o, c - ray.o); // area of triangle ocax2

    // // volumesx6
    // obap = dot(n_ab, ray.d);
    // ocbp = dot(n_bc, ray.d);
    // oacp = dot(n_ca, ray.d);

    // float l1, l2, l3;

    // den = obap + ocbp + oacp;
    
    // l1 = ocbp/den;
    // l2 = oacp/den;
    // l3 = obap/den;

    // if (l1 > 0 && l2 > 0 && l3 > 0){
        
    //     Point hit = l1*a + l2*b + l3*c;
    //     Vector dir = hit - ray.o;
    //     float t = dir.length();

    //     if (t < tmin || t > tmax) return int_fail;
    //     return Intersection(t, ray, this, normal.normalize(), Point(l1, l2, l3));
    // }

    // return int_fail;

    
    
    /*Plane-Based Method - https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/barycentric-coordinates*/

    // float den = dot(ray.d, normal);
    // if (den==0) return int_fail;

    // // compute d of triangle plane defined by ax + by + cz + d = 0

    // float d = -dot(normal, a - Point::rep(0.0f));
    // float t = -(dot(normal, ray.o - Point::rep(0.0f)) + d)/den;

    // if (t < tmin || t > tmax) return int_fail;

    // Point hit = ray.getPoint(t);

    // // inside-outside tests
    // if (dot(normal, cross(b - a, hit - a)) < 0) return int_fail; // edge ab
    // if (dot(normal, cross(c - b, hit - b)) < 0) return int_fail; // edge bc
    // if (dot(normal, cross(a - c, hit - c)) < 0) return int_fail; // edge ca

    // return Intersection(t, ray, this, normal.normalize(), hit);
}

Solid::Sample Triangle::sample() const {
    
    Solid::Sample s;
    
    s.normal = normal.normalize();

    float u = random(0.0f, 1.0f);
    float v = random(0.0f, 1.0f);
    if (u + v > 1) {
        u = 1 - u;
        v = 1 - v;
    }
    s.point = lerpbar(a, b, c, u, v);

    return s;
}

float Triangle::getArea() const {
    return normal.length()/2;

}

}