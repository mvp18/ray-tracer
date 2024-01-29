#ifndef CG1RAYTRACER_SOLIDS_ENVIRONMENTSOLID_HEADER
#define CG1RAYTRACER_SOLIDS_ENVIRONMENTSOLID_HEADER

#include <rt/solids/solid.h>

namespace rt {

class EnvironmentSolid : public Solid {
public:
    EnvironmentSolid() {}
    EnvironmentSolid(CoordMapper* texMapper, Material* material);

    virtual BBox getBounds() const;
    virtual Intersection intersect(const Ray& ray, float tmin = 0, float tmax = FLT_MAX) const;
    virtual Sample sample() const;
    virtual float getArea() const;
};

}

#endif