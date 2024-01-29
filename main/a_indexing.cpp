
#include <core/assert.h>
#include <core/scalar.h>
#include <core/image.h>
#include <rt/world.h>
#include <rt/renderer.h>
#include <rt/loaders/obj.h>
#include <rt/groups/bvh.h>
#include <rt/solids/sphere.h>
#include <rt/cameras/perspective.h>
#include <rt/integrators/casting.h>
#include <iomanip> // for setprecision()

using namespace rt;

void a_indexing() {
    Image img(800, 600);
    clock_t start, end;

    start = clock();

    BVH* scene = new BVH();
    // scene->useSAH_ = true;
    scene->add(new Sphere(Point(2.5f,  .5f,  -1), 0.5  , nullptr, nullptr));
    scene->add(new Sphere(Point(2.5f,  -1.f,  -1), 0.5, nullptr, nullptr));
    scene->add(new Sphere(Point(4.5f,  .5f,  -1), 0.5 , nullptr, nullptr));

    loadOBJ(scene, "models/assignments/", "cow.obj");
    // loadOBJ(scene, "models/assignments/crocodile/", "CROCODIL.OBJ");
  
	scene->rebuildIndex();
    World world;
    world.scene = scene;

    PerspectiveCamera cam1(Point(-8.85f, -7.85f, 7.0f), Vector(1.0f,1.0f,-0.6f), Vector(0, 0, 1), pi/8, pi/6);
    PerspectiveCamera cam2(Point(16.065f, -12.506f, 1.771f), Point(-0.286f, -0.107f, 1.35f)-Point(16.065f, -12.506f, 1.771f), Vector(0, 0, 1), pi/8, pi/6);
    RayCastingIntegrator integrator(&world);

    Renderer engine1(&cam1, &integrator);
    engine1.render(img);
    img.writeEXR("a3-1.exr");
    // img.writeEXR("a3-croc1.exr");

    Renderer engine2(&cam2, &integrator);
    engine2.render(img);
    img.writeEXR("a3-2.exr");
    // img.writeEXR("a3-croc2.exr");

    end = clock();
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
    cout << "Time taken for rendering: " << fixed << time_taken << setprecision(2);
    cout << " seconds " << endl; // currently 3.23 seconds for non-SAH and 4.41 seconds for SAH
}