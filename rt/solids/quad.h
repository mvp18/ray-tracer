#ifndef CG1RAYTRACER_SOLIDS_QUAD_HEADER
#define CG1RAYTRACER_SOLIDS_QUAD_HEADER

#include <rt/solids/solid.h>

namespace rt {

class Quad : public Solid {
public:
    Quad() {}
    Quad(const Point& v1, const Vector& span1, const Vector& span2, CoordMapper* texMapper, Material* material);

    virtual BBox getBounds() const;
    virtual Intersection intersect(const Ray& ray, float tmin = 0, float tmax = FLT_MAX) const;
    virtual Sample sample() const;
    virtual float getArea() const;
private:
    Point v1, v2, v3, v4, center, min_ex, max_ex;
    Vector span1_, span2_, normal;
};

}

#endif