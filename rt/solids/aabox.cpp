#include <rt/solids/aabox.h>

using namespace std;

namespace rt {

AABox::AABox(const Point& corner1, const Point& corner2, CoordMapper* texMapper, Material* material): Solid(texMapper, material)
{
    corner1_ = corner1;
    corner2_ = corner2;

    min_ex = min(corner1_, corner2_);
    max_ex = max(corner1_, corner2_);

    bounds[0] = corner1_;
    bounds[1] = corner2_;
}

BBox AABox::getBounds() const {
    return BBox(min_ex, max_ex);
}

Solid::Sample AABox::sample() const {
    NOT_IMPLEMENTED;
}

float AABox::getArea() const {
    Vector diagonal = max_ex - min_ex;
    float l, w, h;
    l = fabs(diagonal.x);
    w = fabs(diagonal.y);
    h = fabs(diagonal.z);
    return 2*(l*w + l*h + w*h);
}

Intersection AABox::intersect(const Ray& ray, float tmin, float tmax) const {

    Intersection int_fail = Intersection::failure();

    // https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection
    
    int sign[3];
    sign[0] = (ray.inv_dir.x < 0);
    sign[1] = (ray.inv_dir.y < 0);
    sign[2] = (ray.inv_dir.z < 0);

    float t1, t2, tymin, tymax, tzmin, tzmax;
    Vector normal;

    t1 = (bounds[sign[0]].x - ray.o.x)*ray.inv_dir.x; //txmin
    t2 = (bounds[1-sign[0]].x - ray.o.x)*ray.inv_dir.x; // txmax

    normal = sign[0]==1 ? Vector(1.0f, 0.0f, 0.0f) : Vector(-1.0f, 0.0f, 0.0f);

    tymin = (bounds[sign[1]].y - ray.o.y)*ray.inv_dir.y;
    tymax = (bounds[1-sign[1]].y - ray.o.y)*ray.inv_dir.y;

    if (t1 > tymax || tymin > t2) 
        return int_fail;

    if (tymin > t1){
        t1 = tymin;
        normal = sign[1]==1 ? Vector(0.0f, 1.0f, 0.0f) : Vector(0.0f, -1.0f, 0.0f);
    }
    if (tymax < t2){
        t2 = tymax;
    }

    tzmin = (bounds[sign[2]].z - ray.o.z)*ray.inv_dir.z;
    tzmax = (bounds[1-sign[2]].z - ray.o.z)*ray.inv_dir.z;

    if (t1 > tzmax || tzmin > t2) 
        return int_fail;

    if (tzmin > t1){
        t1 = tzmin;
        normal = sign[2]==1 ? Vector(0.0f, 0.0f, 1.0f) : Vector(0.0f, 0.0f, -1.0f);
    }
    if (tzmax < t2){
        t2 = tzmax;
    }

    if (t1 < rt::epsilon){
        swap(t1, t2);
        normal = -normal;
    }

    if (t1 < tmin || t1 > tmax) return int_fail;
    
    Point local_ = ray.getPoint(t1);
    return Intersection(t1, ray, this, normal.normalize(), local_);
}

}
