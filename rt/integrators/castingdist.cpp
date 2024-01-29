#include <rt/integrators/castingdist.h>
#include <rt/intersection.h>
#include <core/color.h>
#include <rt/world.h>
#include <cmath>

namespace rt {

RayCastingDistIntegrator::RayCastingDistIntegrator(World* world, const RGBColor& nearColor, float nearDist, const RGBColor& farColor, float farDist)
    : Integrator(world)
{
    nearColor_ = nearColor;
    nearDist_ = nearDist;
    farColor_ = farColor;
    farDist_ = farDist;
}

RGBColor RayCastingDistIntegrator::getRadiance(const Ray& ray) const {
    
    Intersection intersection_ = world->scene->intersect(ray);
    RGBColor rgb;
    float coeff, cos_theta;

    if (intersection_){

        coeff = (intersection_.distance - nearDist_)/(farDist_ - nearDist_);
        if (coeff > 1) coeff = 1;
        if (coeff < 0) coeff = 0;
        cos_theta = dot(-ray.d, intersection_.normal());
        rgb = (nearColor_ + (farColor_ - nearColor_)*coeff)*cos_theta;
        return rgb;
    }
    else return RGBColor::rep(0.0f);
}

}
