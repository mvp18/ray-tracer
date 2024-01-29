cmake_minimum_required(VERSION 3.9.4 FATAL_ERROR)

# Prevent in source builds
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "CMake generation within the source directory is prohibited! Please run 'cmake ..' from a build/ directory " )
endif()

# Omit superfluous "Up-to-date" messages.
if(NOT DEFINED CMAKE_INSTALL_MESSAGE)
    set(CMAKE_INSTALL_MESSAGE "LAZY")
endif()

project(CG1-raytracer)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "limited config" FORCE)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(ASSIGNMENT_LIBRARIES)

find_package(PNG QUIET)
if (PNG_FOUND)
    include_directories(${PNG_INCLUDE_DIRS})
    add_definitions(${PNG_DEFINITIONS})
    set(ASSIGNMENT_LIBRARIES ${ASSIGNMENT_LIBRARIES} ${PNG_LIBRARIES})
elseif(WIN32)
    add_definitions(-DGDIPLUS_SUPPORT)
    set(ASSIGNMENT_LIBRARIES ${ASSIGNMENT_LIBRARIES} gdiplus)
else()
    find_package(PNG)
    message(FATAL_ERROR "libpng-dev is required to build the CG1-raytracer.\nPlease install libpng-dev.")
endif ()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
set(ALL_ASSIGNMENT_SOURCES)

file(GLOB ASSIGNMENT_FILES "${CMAKE_CURRENT_SOURCE_DIR}/assignment-*.cmake")
foreach(f ${ASSIGNMENT_FILES})
    get_filename_component(_fname ${f} NAME_WE)
    message(STATUS "Found files of ${_fname}")
    include(${f})
    foreach(_s ${ASSIGNMENT_SOURCES})
        message(STATUS " - ${_s}")
    endforeach()
    list(APPEND ALL_ASSIGNMENT_SOURCES ${ASSIGNMENT_SOURCES})
endforeach()

add_executable(cgray ${ALL_ASSIGNMENT_SOURCES} main/main.cpp)

# Make sure all the warnings are used and traited as errors!
# Only exception is unused-parameter (as students work iteratively), which is only traited as a warning
if(MSVC)
  target_compile_options(cgray PRIVATE /W4 /WX
    /wd4100 /wd4316 /wd4324 /wd4458 /wd4706 /wd4127
  )
else()
  target_compile_options(cgray PRIVATE
    -Wall -Wextra -Wpedantic -Werror
    -Wno-newline-eof -Wno-unused-private-field
    -Wno-unused-parameter -Wno-unused-function
  )
endif()

# Necessary for tinyexr.h
include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/core/exr)

# Compile with IPO in Release mode
include(CheckIPOSupported)
check_ipo_supported(RESULT IPO_SUPPORTED OUTPUT IPO_ERROR_MESSAGE)
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    if (IPO_SUPPORTED)
        message(STATUS "Compiling with IPO")
        set_property(TARGET cgray PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
    else()
        message(STATUS "IPO unavailable on this platform: ${IPO_ERROR_MESSAGE}")
    endif()
endif()
target_link_libraries(cgray PUBLIC ${ASSIGNMENT_LIBRARIES})

# Set this to true if you want to use OpenMP (and know what it is)
option(CG_USE_OPENMP "Search and make use of the OpenMP framework if available" TRUE)
if (CG_USE_OPENMP)
    find_package(OpenMP)
    if (OpenMP_CXX_FOUND)
        target_link_libraries(cgray PUBLIC OpenMP::OpenMP_CXX)
        message(STATUS "Using OpenMP")
    endif()
endif()

include(cgray-tests.cmake OPTIONAL)
include(cgray-bench.cmake OPTIONAL)

source_group(core/exr core/exr/)
source_group(core/random core/random/)
source_group(main main/)
source_group(core core/)
source_group(rt/cameras rt/cameras/)
source_group(rt/coordmappers rt/coordmappers/)
source_group(rt/groups rt/groups/)
source_group(rt/integrators rt/integrators/)
source_group(rt/lights rt/lights/)
source_group(rt/loaders rt/loaders/)
source_group(rt/materials rt/materials/)
source_group(rt/primmod rt/primmod/)
source_group(rt/solids rt/solids/)
source_group(rt/textures rt/textures/)
source_group(rt/ rt/)
