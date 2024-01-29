#ifndef CG1RAYTRACER_SOLIDS_DISC_HEADER
#define CG1RAYTRACER_SOLIDS_DISC_HEADER

#include <rt/solids/solid.h>
#include <core/matrix.h>

namespace rt {

class Disc : public Solid {
public:
    Disc() {}
    Disc(const Point& center, const Vector& normal, float radius, CoordMapper* texMapper, Material* material);

    virtual BBox getBounds() const;
    virtual Intersection intersect(const Ray& ray, float tmin = 0, float tmax = FLT_MAX) const;
    virtual Vector polar2cartesian(float r, float theta) const;
    virtual Matrix getOrthogonalBasis(const Vector normal) const;
    virtual Sample sample() const;
    virtual float getArea() const;
private:
    Point center_, min_ex, max_ex;
    Vector normal_;
    float radius_, dx_, dy_, dz_;
    Matrix M;
};

}

#endif