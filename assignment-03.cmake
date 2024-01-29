cmake_minimum_required(VERSION 3.0)

set(MY_ADDITIONAL_SOURCES
    # Please add your source files here
    rt/groups/bvh.cpp
)

set(ASSIGNMENT_SOURCES
    rt/groups/bvh.h
    rt/loaders/obj.cpp
    rt/loaders/obj.h
    rt/loaders/objnoshadow.cpp
    rt/loaders/objnoshadow.h
    ${MY_ADDITIONAL_SOURCES}
    main/a_indexing.cpp
)

file(COPY
    models/assignments/cow.obj
    models/assignments/cow.mtl
    DESTINATION ${CMAKE_BINARY_DIR}/models
)
