## Course Webpage

https://graphics.cg.uni-saarland.de/courses/cg1-2022/index.html

## Setup

### Installing build tools and libraries

#### Ubuntu
```
sudo apt update # update package index
sudo apt install -y build - essential git cm ake # install build tools
sudo apt install -y xorg - dev libglu1 -mesa - dev zlib1g - dev zenity # install dependencies for tev
sudo apt install -y libpng - dev # install the PNG library required by our framework
```

#### macOS
```
brew install cm ake # install CMake
brew install libpng # install the PNG library required by our framework
```

### Installing VSCode

#### Ubuntu
```
sudo snap install --classic code
```
Alternatively, you can download a .deb from https://code.visualstudio.com/#alt-downloads and run:
```
sudo dpkg -i ~/ Downloads / code_ *. deb # path to downloaded .deb file
```

#### macOS
```
brew install visual-studio-code
```

### Installing tev (image viewer, optional)

#### Ubuntu

Tev needs to be installed from source, which is luckily a straightforward process
```
git clone -- recursive https :// github .com / Tom94 / tev .git
mkdir tev / build && cd tev/ build
cmake ..
sudo make -j i nstall
```

#### macOS

Go to https://github.com/Tom94/tev/releases and download the latest tev.dmg. Open the dmg file and move tev to your Applications folder. For security reasons, the first time you run tev you must right click on the application and click “Open”.

## Build

You would need to install 2 extensions in VSCode:
- The “CMake Tools” extension to be able to build your project
- The “C/C++” extension enables intelligent code completion and debugging.

CMakeLists.txt allows you to use any build system. The respective assignment-xx.cmake lists all ﬁles that were added with a particular assignment. On Linux libpng-dev is required for the build and should be automatically detected.

By invoking:

```
mkdir build debug && cd build debug # create and enter a build directory for CMake
cmake -DCMAKE BUILD TYPE=Debug .. # conﬁgure the project in debug mode
cmake --build . # compile the project, producing executable cgray
```

For release mode use cmake -DCMAKE BUILD TYPE=Release .. in a diﬀerent build release directory respectively.

## Features

### Cameras

- Perspective Camera: rt/camera/perspective.h
- Depth of Field Perspective Camera: rt/camera/dofperspective.h
- Orthographic Camera: rt/camera/orthographic.h
- Fish Eye Camera: rt/camera/fisheye.h
- Environment Camera: rt/camera/environment.h

### Solids

- Sphere: rt/solids/sphere.h
- Axis-Aligned Bounding Box: rt/solids/aabox.h
- Triangle: rt/solids/triangle.h
- Smooth Triangle: rt/solids/striangle.h
- Disc: rt/solids/disc.h
- Plane: rt/solids/infiniteplane.h
- Quad: rt/solids/quad.h
- Environment Solid: rt/solids/environmentsolid.h

### Acceleration Structures

- Bounding Volume Hierarchy: rt/groups/bvh.h

### Materials

- Conductors: rt/materials/conductor.h
- Fuzzy Conductor: rt/materials/fuzzyconductor.h
- Dielectrics: rt/materials/dielectric.h
- Flat Material: rt/materials/flatmaterial.h
- Lambertian: rt/materials/lambertian.h
- Phong: rt/materials/phong.h
- Combined Material: rt/materials/combine.h

### Lights

- Point Light: rt/lights/pointlight.h
- Directional Light: rt/lights/directional.h
- Spot Light: rt/lights/spotlight.h
- Area Light: rt/lights/arealight.h
- Ambient Light: rt/lights/ambientlight.h

### Textures

- Constant Texture: rt/textures/constant.h
- Image Texture: rt/textures/imagetex.h
- Checkerboard Texture: rt/textures/checkerboard.h
- Perlin Noise: rt/textures/perlin.h

### Coordinate Mappers

- World Mapper: rt/coordmappers/world.h
- Plane Mapper: rt/coordmappers/plane.h
- Cylinder Mapper: rt/coordmappers/cylindrical.h
- Spherical Mapper: rt/coordmappers/spherical.h
- Triangle Mapper: rt/coordmappers/tmapper.h
- Environment Map: rt/coordmappers/environment.h
- Quad Mapper: rt/coordmappers/quadmapper.h

## Sample Images

The rendered images for each assignment and also the rendering competition are stored in the `images` folder. Models required for `main/a_RC.cpp` can be found [here](https://drive.google.com/file/d/1oOZi4IxYJirrIpk2-_R3tiVDGSWiBW-4/view?usp=sharing). You can find further details about our submission for the rendering competition [here](https://mvp18.github.io/RC/home.html).

## References

- [Scratch a Pixel](https://www.scratchapixel.com/)
- [Ray Tracing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html)
- [PBR Book](https://pbrt.org)