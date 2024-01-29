#include <rt/solids/striangle.h>
#include <rt/intersection.h>
#include <core/interpolate.h>

namespace rt {

SmoothTriangle::SmoothTriangle(Point vertices[3], Vector normals[3], CoordMapper* texMapper, Material* material)
:Triangle(vertices, texMapper, material)
{
    tri = Triangle(vertices, texMapper, material);

    na = normals[0];
    nb = normals[1];
    nc = normals[2];
}

SmoothTriangle::SmoothTriangle(const Point& v1, const Point& v2, const Point& v3, const Vector& n1, const Vector& n2, const Vector& n3, CoordMapper* texMapper, Material* material)
:Triangle(v1, v2, v3, texMapper, material)
{
    tri = Triangle(v1, v2, v3, texMapper, material);

    na = n1;
    nb = n2;
    nc = n3;
}

Intersection SmoothTriangle::intersect(const Ray& ray, float tmin, float tmax) const {
    
    Intersection hit = tri.intersect(ray, tmin, tmax);
    
    if (hit){
        Point barycentric_coord = hit.local();
        Vector normal_interpolated = lerpbar(na, nb, nc, barycentric_coord.x, barycentric_coord.y);
        hit.normal_ = normal_interpolated.normalize();
    }

    return hit;

}

}