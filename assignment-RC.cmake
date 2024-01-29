cmake_minimum_required(VERSION 3.0)

set(MY_ADDITIONAL_SOURCES
    # Please add your source files here
)

set(ASSIGNMENT_SOURCES
    core/assert.h
    core/scalar.cpp
    core/scalar.h
    core/image.cpp
    core/image.h

    rt/loaders/obj.cpp
    rt/loaders/obj.h
    rt/loaders/objnoshadow.cpp
    rt/loaders/objnoshadow.h

    rt/world.h
    rt/integrators/recraytrace.cpp
    rt/integrators/recraytrace.h
    rt/renderer.cpp
    rt/renderer.h

    rt/groups/bvh.cpp
    rt/groups/bvh.h
    rt/groups/simplegroup.cpp
    rt/groups/simplegroup.h

    rt/cameras/perspective.cpp
    rt/cameras/perspective.h
    rt/cameras/dofperspective.cpp
    rt/cameras/dofperspective.h

    rt/solids/disc.cpp
    rt/solids/disc.h
    rt/solids/quad.cpp
    rt/solids/quad.h
    rt/solids/environmentsolid.cpp
    rt/solids/environmentsolid.h
    rt/solids/sphere.cpp
    rt/solids/sphere.h

    rt/lights/ambientlight.cpp
    rt/lights/ambientlight.h
    rt/lights/pointlight.cpp
    rt/lights/pointlight.h
    rt/lights/spotlight.cpp
    rt/lights/spotlight.h
    rt/lights/directional.cpp
    rt/lights/directional.h
    rt/lights/arealight.cpp
    rt/lights/arealight.h

    rt/materials/flatmaterial.cpp
    rt/materials/flatmaterial.h
    rt/materials/lambertian.cpp
    rt/materials/lambertian.h
    rt/materials/dielectric.cpp
    rt/materials/dielectric.h
    rt/materials/conductor.cpp
    rt/materials/conductor.h

    rt/coordmappers/coordmapper.h
    rt/coordmappers/spherical.cpp
    rt/coordmappers/spherical.h
    rt/coordmappers/quadmapper.cpp
    rt/coordmappers/quadmapper.h
    rt/coordmappers/environment.cpp
    rt/coordmappers/environment.h
    ${MY_ADDITIONAL_SOURCES}
    main/a_RC.cpp
)

file(COPY
    models/manrot.obj
    models/manrot.mtl
    models/chairmoved.obj
    models/chairmoved.mtl
    models/shelf.obj
    models/shelf.mtl
    models/wallsfixed.obj
    models/wallsfixed.mtl
    models/monitorbox.obj
    models/monitorbox.mtl
    models/paintingframe.obj
    models/paintingframe.mtl
    models/Tabletopfixedrender.obj
    models/Tabletopfixedrender.mtl
    models/windowpane.obj
    models/windowpane.mtl
    models/keyboardmouse.obj
    models/keyboardmouse.mtl
    models/mirrorbase.obj
    models/mirrorbase.mtl
    models/mirrorglass.obj
    models/mirrorglass.mtl
    models/owlbody.obj
    models/owlbody.mtl
    models/owleyes.obj
    models/owleyes.mtl
    models/lamplegs.obj
    models/lamplegs.mtl
    models/lampshade.obj
    models/lampshade.mtl
    models/Bookpages.obj
    models/Bookpages.mtl
    models/Bookcover.obj
    models/Bookcover.mtl
    models/moon.obj
    models/moon.mtl
    models/roof.obj
    models/roof.mtl
    models/mylamp1.obj
    models/mylamp1.mtl
    models/bicycle.obj
    models/bicycle.mtl
    DESTINATION ${CMAKE_BINARY_DIR}/models
)