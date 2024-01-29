#include <core/scalar.h>
#include <core/random.h>
#include <core/image.h>
#include <rt/renderer.h>
#include <rt/integrators/integrator.h>
#include <rt/cameras/camera.h>
#include <rt/ray.h>

#include <omp.h>

using namespace std;

rt::RGBColor a1computeColor(rt::uint x, rt::uint y, rt::uint width, rt::uint height);
rt::RGBColor a2computeColor(const rt::Ray& r);

namespace rt {

    Renderer::Renderer(Camera* cam, Integrator* integrator): cam(cam), integrator(integrator), samples(1)
    {

    }

    void Renderer::render(Image& img) {

        float imageAspectRatio = img.width()/img.height();

#pragma omp parallel for collapse(2) schedule(dynamic)
        for(uint i=0; i<img.width(); i++){
            for(uint j=0; j<img.height(); j++){

                RGBColor radiance = RGBColor::rep(0.0f);
                float cam_X, cam_Y;
                
                for (uint k = 0; k < samples; ++k){

                    if (samples==1){
                        cam_X = (2*float(i + 0.5)/float(img.width()) - 1.0)*imageAspectRatio;
                        cam_Y = 1.0 - 2*float(j + 0.5)/float(img.height());
                    }
                    else {
                        cam_X = (2*float(i + random(0.0f, 1.0f))/float(img.width()) - 1.0)*imageAspectRatio;
                        cam_Y = 1.0 - 2*float(j + random(0.0f, 1.0f))/float(img.height());
                    }
                    
                    const Ray r = cam->getPrimaryRay(cam_X, cam_Y);
                    radiance = radiance + integrator->getRadiance(r);
                }
                img(i, j) = radiance/samples;
            }
        }
        
    }

    void Renderer::test_render1(Image& img) {
        
        for(uint i=0; i<img.width(); i++){
            for(uint j=0; j<img.height(); j++){
                img(i, j) = a1computeColor(i, j, img.width(), img.height());
            }
        }
    }

    void Renderer::test_render2(Image& img) {

        // https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-generating-camera-rays/generating-camera-rays
        
        float imageAspectRatio = img.width()/img.height();

        for(uint i=0; i<img.width(); i++){
            for(uint j=0; j<img.height(); j++){
                float cam_X = (2*float(i + 0.5)/float(img.width()) - 1.0)*imageAspectRatio;
                float cam_Y = 1.0 - 2*float(j + 0.5)/float(img.height());

                const Ray r = cam->getPrimaryRay(cam_X, cam_Y);
                img(i, j) = a2computeColor(r);
            }
        }
    }

}
