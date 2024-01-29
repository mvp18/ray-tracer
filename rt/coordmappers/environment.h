#ifndef CG1RAYTRACER_COORDMAPPERS_ENVIRONMENT_HEADER
#define CG1RAYTRACER_COORDMAPPERS_ENVIRONMENT_HEADER

#include <rt/coordmappers/coordmapper.h>

namespace rt {

class EnvironCoordMapper : public CoordMapper{
public:
    EnvironCoordMapper();
    virtual Point getCoords(const Intersection& hit) const;
};

}

#endif