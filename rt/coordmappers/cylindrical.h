#ifndef CG1RAYTRACER_COORDMAPPERS_CYLINDRICAL_HEADER
#define CG1RAYTRACER_COORDMAPPERS_CYLINDRICAL_HEADER

#include <rt/coordmappers/coordmapper.h>
#include <core/point.h>
#include <core/vector.h>
#include <rt/intersection.h>

namespace rt {

class CylindricalCoordMapper : public CoordMapper {
public:    
    CylindricalCoordMapper(const Point& origin, const Vector& longitudinalAxis, const Vector& polarAxis);
    virtual Point getCoords(const Intersection& hit) const;
private:
    Vector longitudinalAxis_, polarAxis_, perpendicularAxis_;
    Point origin_;
    float scale_x, scale_y;
    
};

}

#endif