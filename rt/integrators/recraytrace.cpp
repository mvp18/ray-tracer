#include <rt/integrators/recraytrace.h>
#include <rt/world.h>
#include <rt/solids/solid.h>
#include <rt/lights/light.h>
#include <rt/materials/material.h>
#include <rt/coordmappers/coordmapper.h>
#include <rt/coordmappers/world.h>

namespace rt {

RGBColor RecursiveRayTracingIntegrator::RecursiveRadiance(int depth, const Ray& ray) const {
    
    Intersection hit = world->scene->intersect(ray);

    if (hit){
        Vector surface_normal = hit.normal();
        Vector view_direction = -ray.d;
        
        CoordMapper* mapper = hit.solid->texMapper;
        if (mapper == nullptr) mapper = new WorldMapper(Vector::rep(1.0f));
        Point local_hit = mapper->getCoords(hit); // local texture coordinates

        // for environmentsolid
        if (hit.distance == FLT_MAX && hit.solid != nullptr){
            return hit.solid->material->getEmission(local_hit, surface_normal, view_direction);
        }

        RGBColor color = hit.solid->material->getEmission(local_hit, surface_normal, view_direction);
        
        Material::Sampling mat_sampling = hit.solid->material->useSampling();
        if (mat_sampling != Material::Sampling::SAMPLING_ALL){
            for (auto& it : world->light){
                LightHit light_hit = it->getLightHit(hit.hitPoint());

                // If ambient light, shadow ray has no meaning
                if (it->isAmbient){
                    RGBColor ambientLight = it->getIntensity(light_hit);
                    if (it->reflectance_mode){
                        ambientLight = ambientLight * hit.solid->material->getReflectance(local_hit, surface_normal, view_direction, light_hit.direction);
                    }
                    color = color + ambientLight;
                    continue;
                }
                
                // set origin to hitpoint + bias*light_direction to prevent surface-acne/self-intersection
                Ray ShadowRay = Ray(hit.hitPoint() + rt::epsilon*surface_normal, light_hit.direction);
                
                // ensure shadow and primary (view direction) rays leave surface on the same side
                float shadow_ray_normal_dot = dot(ShadowRay.d, surface_normal);
                float view_ray_normal_dot = dot(view_direction, surface_normal);

                if (shadow_ray_normal_dot*view_ray_normal_dot > rt::epsilon){
                    Intersection shadow_ray_obstacle = world->scene->intersect(ShadowRay, rt::epsilon, light_hit.distance);
                    if (!shadow_ray_obstacle || shadow_ray_obstacle.distance == FLT_MAX){
                        RGBColor light_intensity = it->getIntensity(light_hit);
                        RGBColor material_color = hit.solid->material->getReflectance(local_hit, surface_normal, view_direction, ShadowRay.d);
                        color = color + material_color * light_intensity;
                    }
                }
            }
        }

        if (depth == 0) return color;

        if (mat_sampling != Material::Sampling::SAMPLING_NOT_NEEDED){
            Material::SampleReflectance SR = hit.solid->material->getSampleReflectance(local_hit, surface_normal, view_direction);
            Ray SecondaryRay = Ray(hit.hitPoint() + rt::epsilon*SR.direction, SR.direction);
            color = color + SR.reflectance * RecursiveRadiance(--depth, SecondaryRay);
        }
        return color;
    }
    return RGBColor::rep(0.0f);
}

RGBColor RecursiveRayTracingIntegrator::getRadiance(const Ray& ray) const {
    return RecursiveRadiance(maxDepth, ray);
}

}