#ifndef CG1RAYTRACER_GROUPS_SIMPLEGROUP_HEADER
#define CG1RAYTRACER_GROUPS_SIMPLEGROUP_HEADER

#include <rt/groups/group.h>

using namespace std;

namespace rt {

class SimpleGroup : public Group {
public:
    virtual BBox getBounds() const;
    virtual Intersection intersect(const Ray& ray, float tmin = 0, float tmax = FLT_MAX) const;
    virtual void rebuildIndex();
    virtual void add(Primitive* p);
    virtual void setMaterial(Material* m);
    virtual void setCoordMapper(CoordMapper* cm);
private:
    vector<Primitive *> primitives_;
    BBox b = BBox::empty();
};

}

#endif