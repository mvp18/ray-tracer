#ifndef CG1RAYTRACER_INTEGRATORS_RECURSIVERAYTRACING_HEADER
#define CG1RAYTRACER_INTEGRATORS_RECURSIVERAYTRACING_HEADER

#include <rt/integrators/integrator.h>

namespace rt {

class RecursiveRayTracingIntegrator : public Integrator {
public:
    int maxDepth;
    RecursiveRayTracingIntegrator(World* world) : Integrator(world), maxDepth(6) {}
    virtual RGBColor getRadiance(const Ray& ray) const;
    virtual RGBColor RecursiveRadiance(int depth, const Ray& ray) const;
    void setMaxDepth(int depth) { maxDepth = depth; }
};

}

#endif