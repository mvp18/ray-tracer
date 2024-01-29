#ifndef CG1RAYTRACER_COORDMAPPERS_QUADMAPPER_HEADER
#define CG1RAYTRACER_COORDMAPPERS_QUADMAPPER_HEADER

#include <rt/coordmappers/coordmapper.h>
#include <core/vector.h>
#include <rt/intersection.h>


namespace rt {

class QuadMapper : public CoordMapper {
    public:
        QuadMapper(const Vector& offset, const Vector& scale = Vector::rep(1.f));
        virtual Point getCoords(const Intersection& hit) const;
    private:
        Vector offset_, scale_;
    };
}

#endif