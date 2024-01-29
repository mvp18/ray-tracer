#include <rt/integrators/casting.h>
#include <rt/intersection.h>
#include <core/color.h>
#include <rt/world.h>
#include <cmath>

namespace rt {

RGBColor RayCastingIntegrator::getRadiance(const Ray& ray) const {
    
    Intersection intersection_ = world->scene->intersect(ray);
    float ray_normal_dot;

    if (intersection_){
        ray_normal_dot = dot(-ray.d, intersection_.normal());
        return RGBColor::rep(ray_normal_dot);
    }
    else return RGBColor::rep(0.0f);
}

}
