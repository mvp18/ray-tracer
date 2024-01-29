#include <core/assert.h>
#include <core/scalar.h>
#include <core/image.h>

#include <rt/world.h>
#include <rt/integrators/recraytrace.h>
#include <rt/renderer.h>
#include <rt/loaders/obj.h>

#include <rt/groups/bvh.h>
#include <rt/groups/simplegroup.h>

#include <rt/cameras/perspective.h>
#include <rt/cameras/dofperspective.h>

#include <rt/solids/disc.h>
#include <rt/solids/quad.h>
#include <rt/solids/environmentsolid.h>
#include <rt/solids/sphere.h>

#include <rt/lights/pointlight.h>
#include <rt/lights/spotlight.h>
#include <rt/lights/ambientlight.h>
#include <rt/lights/arealight.h>
#include <rt/lights/directional.h>

#include <rt/materials/flatmaterial.h>
#include <rt/materials/lambertian.h>
#include <rt/materials/dielectric.h>
#include <rt/materials/conductor.h>

#include <rt/textures/constant.h>
#include <rt/textures/imagetex.h>
#include <rt/textures/perlin.h>

#include <rt/coordmappers/coordmapper.h>
#include <rt/coordmappers/spherical.h>
#include <rt/coordmappers/quadmapper.h>
#include <rt/coordmappers/environment.h>

#include <cstdio>
#include <iomanip> // for setprecision()

using namespace rt;

MatLib* getmanskin() {

    MatLib* man = new MatLib;

    ImageTexture* t = new ImageTexture("models/finale.png");
    Texture* black = new ConstantTexture(RGBColor::rep(0.0f));
    Material* lab_mat = new LambertianMaterial(black, t);    
    man->insert(std::pair<std::string, Material*>("rp_manuel_animated_001_mat", lab_mat));
    
    return man;
}

MatLib* getbookshelf() {

    MatLib* shelf = new MatLib;

    ImageTexture* t = new ImageTexture("models/SHELF_TEXTURE.png");
    Texture* black = new ConstantTexture(RGBColor::rep(0.0f));
    Material* lab_mat = new LambertianMaterial(black, t);    
    shelf->insert(std::pair<std::string, Material*>("SHELF1", lab_mat));
    
    return shelf;
}

MatLib* getmonitorscreen() {

    MatLib* screen = new MatLib;

    ImageTexture* t = new ImageTexture("models/cgpain.png");
    Texture* black = new ConstantTexture(RGBColor::rep(0.0f));
    Material* lab_mat = new LambertianMaterial(black, t);    
    screen->insert(std::pair<std::string, Material*>("Material.023", lab_mat));
    
    return screen;
}

MatLib* getfloormat() {

    MatLib* floor = new MatLib;

    ImageTexture* t = new ImageTexture("models/owlshadow.png");
    Texture* black = new ConstantTexture(RGBColor::rep(0.0f));
    Material* lab_mat = new LambertianMaterial(black, t);    
    floor->insert(std::pair<std::string, Material*>("Material.008", lab_mat));
    
    return floor;
}

MatLib* getmoonmat() {

    MatLib* floor = new MatLib;

    ImageTexture* t = new ImageTexture("models/moontex.png");
    // Texture* black = new ConstantTexture(RGBColor::rep(0.0f));
    FlatMaterial* lab_mat = new FlatMaterial(t);    
    floor->insert(std::pair<std::string, Material*>("Material.024", lab_mat));
    
    return floor;
}

MatLib* getchairmat() {

    MatLib* chair = new MatLib;

    Texture* black = new ConstantTexture(RGBColor::rep(0.0f));
    Texture* white = new ConstantTexture(RGBColor(0.5f,0.2f,0.2f));
    Material* lab_mat = new LambertianMaterial(black, white);    
    chair->insert(std::pair<std::string, Material*>("None", lab_mat));
    
    return chair;
}

MatLib* gettablemat() {

    MatLib* table = new MatLib;
    
    ImageTexture* t = new ImageTexture("models/tabletex.png");
    Texture* black = new ConstantTexture(RGBColor::rep(0.0f));
    //Texture* white = new ConstantTexture(RGBColor::rep(0.5f));
    Material* lab_mat = new LambertianMaterial(black, t);    
    table->insert(std::pair<std::string, Material*>("Material.021", lab_mat));

    return table;
}


MatLib* getlampshademat() {

    MatLib* shade = new MatLib;
    
    DielectricMaterial* dim = new DielectricMaterial(1.5f);    
    shade->insert(std::pair<std::string, Material*>("Material.0", dim));

    return shade;
}

MatLib* getceiling() {

    MatLib* ceiling = new MatLib;

    Texture* black = new ConstantTexture(RGBColor::rep(0.0f));
    Texture* white = new ConstantTexture(RGBColor::rep(1.0f));
    Material* mat = new LambertianMaterial(black, white);    
    ceiling->insert(std::pair<std::string, Material*>("Material.010", mat));

    return ceiling;
}

MatLib* getmirrormat() {

    MatLib* mir = new MatLib;
    
    Material* floorMaterial3 = new ConductorMaterial(2.485f, 3.433f);
    mir->insert(std::pair<std::string, Material*>("Material.014", floorMaterial3));

    return mir;
}

MatLib* getboxmat() {

    MatLib* box = new MatLib;
    
    Texture* blacktex = new ConstantTexture(RGBColor::rep(0.0f));
    FlatMaterial* mat = new FlatMaterial(blacktex);
    box->insert(std::pair<std::string, Material*>("Material", mat));

    return box;
}

MatLib* getwallmat() {

    MatLib* wallib = new MatLib;
    
    Texture* blacktex = new ConstantTexture(RGBColor::rep(0.0f));    
    PerlinTexture* walltex = new PerlinTexture(RGBColor(0.5f, .5f, 0.5f), RGBColor(0.0f, 0.0f, 0.0f));
    walltex->addOctave(0.5f, 5.0f);
    walltex->addOctave(0.25f, 10.0f);
    walltex->addOctave(0.125f, 20.0f);
    walltex->addOctave(0.125f, 40.0f);
    
    Material* wallmat = new LambertianMaterial(blacktex, walltex);
    wallib->insert(std::pair<std::string, Material*>("Material", wallmat));

    return wallib;
}

void a_RC(){
    
    int samples = 400;
    int recursionDepth = 10;
    Image img(1920, 1240);
    
    clock_t start, end;
    start = clock();

    Texture* blacktex = new ConstantTexture(RGBColor::rep(0.0f));

    MatLib* manskin = getmanskin();
    MatLib* bookshelf = getbookshelf();
    MatLib* chair = getchairmat();
    MatLib* table = gettablemat();
    MatLib* mirrormat=getmirrormat();
    MatLib* shademat = getlampshademat();
    MatLib* box = getboxmat();
    MatLib* wallib = getwallmat();
    // MatLib* ceiling = getceiling();
    MatLib* moontex = getmoonmat();

    ImageTexture* screen = new ImageTexture("models/cgpain.png");
    FlatMaterial* bilinear = new FlatMaterial(screen);

    ImageTexture* painting = new ImageTexture("models/calypso_beats.png");
    LambertianMaterial* painted = new LambertianMaterial(blacktex, painting);

    ImageTexture* Book = new ImageTexture("models/funcg.png");
    LambertianMaterial* bookcover = new LambertianMaterial(blacktex, Book);

    DielectricMaterial* windowmat = new DielectricMaterial(1.5f);
    ImageTexture* floor = new ImageTexture("models/owlshadowcarpet.png");
    LambertianMaterial* floorflat = new LambertianMaterial(blacktex, floor, 0.05f);
    CoordMapper* mapper = new QuadMapper(Vector::rep(0.0f), Vector::rep(1.0f));

    World world;
    BVH* scene = new BVH();
    scene->useSAH_ = false;
    SimpleGroup* simple_scene = new SimpleGroup();
    world.scene = simple_scene;

    //Floor
    scene->add(new Quad(Point(-6.298645973205566, -5.256464004516602, 12.384418487548828),(Point(0.30394184589385986, -6.817958831787109, 12.384418487548828)-Point(-6.298645973205566, -5.256464004516602, 12.384418487548828)),(Point(-8.740751266479492, -15.582609176635742, 12.384418487548828)-Point(-6.298645973205566, -5.256464004516602, 12.384418487548828)), mapper, floorflat));
    //Screen
    scene->add(new Quad(Point(-3.395834445953369, -10.655473709106445, 13.226070404052734),(Point(-3.395834445953369, -11.119258880615234, 13.226070404052734)-Point(-3.395834445953369, -10.655473709106445, 13.226070404052734)),(Point(-3.395834445953369, -10.655473709106445, 12.963741302490234)-Point(-3.395834445953369, -10.655473709106445, 13.226070404052734)), mapper, bilinear));

    ImageTexture* moontext = new ImageTexture("models/moontex.png");
    FlatMaterial* moonmat = new FlatMaterial(moontext);
    CoordMapper* smapper = new SphericalCoordMapper(Point(-8.299f, -29.8f, 16.04f), Vector(0.0f,0.0f,1.0f),Vector(1.0f,0.0f,0.0f));
    scene->add(new Sphere(Point(-8.299f, -29.8f, 16.04f), 0.470f, smapper, moonmat));

    //Window
    scene->add(new Quad(Point(-2.8564109802246094, -16.301759719848633, 14.380800247192383),(Point(-4.665025234222412, -16.301759719848633, 14.380800247192383)-Point(-2.8564109802246094, -16.301759719848633, 14.380800247192383)),(Point(-2.8564109802246094, -16.301759719848633, 13.094829559326172)-Point(-2.8564109802246094, -16.301759719848633, 14.380800247192383)), mapper, windowmat));
    //Painting
    scene->add(new Quad(Point(-2.4332621097564697, -11.3984956741333, 14.257647514343262),(Point(-2.4332618713378906, -12.228333473205566, 14.257647514343262)-Point(-2.4332621097564697, -11.3984956741333, 14.257647514343262)),(Point(-2.433262348175049, -11.3984956741333, 13.503863334655762)-Point(-2.4332621097564697, -11.3984956741333, 14.257647514343262)), mapper, painted));
    //Book
    scene->add(new Quad(Point(-3.5022971630096436, -11.14393138885498, 12.923319816589355),(Point(-3.5022971630096436, -11.009709358215332, 12.923319816589355)-Point(-3.5022971630096436, -11.14393138885498, 12.923319816589355)),(Point(-3.3311588764190674, -11.14393138885498, 12.923319816589355)-Point(-3.5022971630096436, -11.14393138885498, 12.923319816589355)), mapper, bookcover));

    // load all OBJ files
    loadOBJ(scene, "models/", "manrot.obj", manskin);
    // loadOBJ(scene, "models/", "glass.obj",glassmat);
    loadOBJ(scene, "models/", "chairmoved.obj", chair);
    loadOBJ(scene, "models/", "shelf.obj", bookshelf);
    loadOBJ(scene, "models/", "wallsfixed.obj", wallib);
    loadOBJ(scene, "models/", "monitorbox.obj", box);
    loadOBJ(scene, "models/", "paintingframe.obj", box);

    loadOBJ(scene, "models/", "Tabletopfixedrender.obj", table);
    loadOBJ(scene, "models/", "windowpane.obj", box);
    loadOBJ(scene, "models/", "keyboardmouse.obj", box);
    loadOBJ(scene, "models/", "mirrorbase.obj", box);
    loadOBJ(scene, "models/", "mirrorglass.obj", mirrormat);
    loadOBJ(scene, "models/", "owlbody.obj", box);
    loadOBJ(scene, "models/", "owleyes.obj", box);
    loadOBJ(scene, "models/", "lamplegs.obj", box);
    loadOBJ(scene, "models/", "lampshade.obj", shademat);
    loadOBJ(scene, "models/", "Bookpages.obj", box);
    loadOBJ(scene, "models/", "Bookcover.obj", box);
    loadOBJ(scene, "models/", "moon.obj", moontex);
    loadOBJ(scene, "models/", "roof.obj", box);
    loadOBJ(scene, "models/", "mylamp1.obj", box);
    loadOBJ(scene, "models/", "bicycle.obj", box);
    
    //Lighting
    ConstantTexture* lightsrctex = new ConstantTexture(RGBColor::rep(7.0f));
    Material* lightsource = new LambertianMaterial(lightsrctex, blacktex);
    
    Quad* glare1 = new Quad(Point(-3.4465, -12.95, 14.733),(Point(-3.0163, -12.95, 14.763)-Point(-3.4465, -12.95, 14.733)),(Point(-3.4465, -13.38, 14.733)-Point(-3.4465, -12.95, 14.733)), nullptr, lightsource);
    Quad* glare2 = new Quad(Point(-6.2713, -12.95, 14.733),(Point(-5.8412, -12.95, 14.763)-Point(-6.2713, -12.95, 14.733)),(Point(-6.2713, -13.38, 14.733)-Point(-6.2713, -12.95, 14.733)), nullptr, lightsource);
    scene->add(glare1);
    scene->add(glare2);
    
    float scale=0.01f;

    //Lamp and surrounding lights
    world.light.push_back(new PointLight(Point(-3.4753, -11.26, 13.142), RGBColor::rep(700.0f*scale*scale)));
    world.light.push_back(new PointLight(Point(-3.6045, -11.26, 13.142), RGBColor::rep(150.0f*scale*scale)));
    world.light.push_back(new PointLight(Point(-3.3546, -11.26, 13.142), RGBColor::rep(150.0f*scale*scale)));
    world.light.push_back(new PointLight(Point(-3.4753, -11.391, 13.142), RGBColor::rep(150.0f*scale*scale)));
    world.light.push_back(new PointLight(Point(-3.4753, -11.151, 13.142), RGBColor::rep(150.0f*scale*scale)));

    world.light.push_back(new SpotLight(Point(-3.4958, -10.887, 13.095), Vector(-1.0f, 0.f, 0.f), pi*2/3,8.0f, RGBColor::rep(900.0f*scale*scale)));
    world.light.push_back(new SpotLight(Point(-2.5604, -11.813, 14.346), Vector(0.4572, 0.f, -0.8893), pi*33.7/180,8.0f, RGBColor::rep(90000.0f*scale*scale)));

    // moon light
    world.light.push_back(new DirectionalLight(Vector(0.202583, 0.94539, -0.25534).normalize(), RGBColor::rep(1.0f)));

    // Ambient light with a small intensity
    world.light.push_back(new AmbientLight(RGBColor(0.005f, 0.005f, 0.005f), false));

    // area lights in the ceiling
    AreaLight als1(glare1);
    AreaLight als2(glare2);
    world.light.push_back(&als1);
    world.light.push_back(&als2);

    // Texture* envtex = new ImageTexture("models/wp2610857-stars-background.png", ImageTexture::MIRROR, ImageTexture::BILINEAR);
    Texture* envtex = new ImageTexture("models/night_sky_high_res.png");
    Material* envmat = new FlatMaterial(envtex);

    scene->rebuildIndex();
    simple_scene->add(scene);
    simple_scene->add(new EnvironmentSolid(new EnvironCoordMapper(), envmat));

    // PerspectiveCamera cam(Point(-4.18f, -10.32f, 13.36f), Vector(0.2252, -0.9586, -0.1742f), Vector(0.0399f, -0.1697f, 0.9847f), pi*90/180, pi*112/180);
    DOFPerspectiveCamera dofcam(Point(-4.18f, -10.32f, 13.36f), Vector(0.2252, -0.9586, -0.1742f), Vector(0.0399f, -0.1697f, 0.9847f), pi*90/180, pi*112/180, 0.54f, 0.0001125f);

    cout << "Dimension: " << img.width() << "x" << img.height() << " Samples: " << samples << ", Depth: " << recursionDepth << endl;

    RecursiveRayTracingIntegrator integrator(&world);
    integrator.setMaxDepth(recursionDepth);

    Renderer engine(&dofcam, &integrator);
    // Renderer engine(&cam, &integrator);
    engine.setSamples(samples);
    engine.render(img);
    
    std::stringstream img_name;
    // img_name << "rc_" << img.width() << "x" << img.height() << "_samples" << samples << "_env" << "_persp.exr";
    img_name << "rc_" << img.width() << "x" << img.height() << "_samples" << samples<< "_env" << "_dof.exr";
    std::string imgsave_name = img_name.str();
    img.writeEXR(imgsave_name);

    end = clock();
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
    cout << "Time taken for rendering: " << fixed << time_taken << setprecision(2);
    cout << " seconds " << endl;
}