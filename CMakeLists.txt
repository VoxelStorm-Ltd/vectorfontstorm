cmake_minimum_required(VERSION 3.16)
project(vectorfontstorm CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------------------------------------------------------------------
# System dependencies
# FreeType2 is required to load and rasterise font outlines.
# GLEW, GLFW, and Boost are expected to be installed system-wide
# (e.g. libglew-dev, libglfw3-dev, libboost-dev on Ubuntu); their headers
# are picked up from standard compiler search paths without explicit -I flags.
# ---------------------------------------------------------------------------
find_package(Freetype REQUIRED)

# ---------------------------------------------------------------------------
# poly2tri – bundled 2-D Delaunay triangulation library
# All poly2tri source files use relative or same-directory includes, so no
# additional include directories are required beyond the defaults.
# ---------------------------------------------------------------------------
add_library(poly2tri STATIC
  vectorfontstorm/poly2tri/common/shapes.cc
  vectorfontstorm/poly2tri/sweep/advancing_front.cc
  vectorfontstorm/poly2tri/sweep/sweep.cc
  vectorfontstorm/poly2tri/sweep/sweep_context.cc
)

# ---------------------------------------------------------------------------
# Minimal vectorstorm stub headers
# The full vectorstorm library (VoxelStorm-Ltd/vectorstorm) has GCC 13 /
# C++17 incompatibilities in headers that are not exercised by the
# vectorfontstorm geometry pipeline (constexpr sincos, C++23 operator[],
# boost::math constants).  A minimal stub is provided that defines only the
# vector2f type required by the core library.
# ---------------------------------------------------------------------------
set(VECTORSTORM_STUBS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests/stubs)

# ---------------------------------------------------------------------------
# vectorfontstorm – the library under test
# ---------------------------------------------------------------------------
add_library(vectorfontstorm_lib STATIC
  vectorfontstorm/point.cpp
  vectorfontstorm/manager.cpp
)
target_include_directories(vectorfontstorm_lib
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}   # exposes "vectorfontstorm/*.h" to dependents
  PRIVATE
    ${VECTORSTORM_STUBS_DIR}      # vectorstorm stubs for compiling .cpp files
)
target_link_libraries(vectorfontstorm_lib PUBLIC
  poly2tri
  Freetype::Freetype
)

# ---------------------------------------------------------------------------
# Tests
# ---------------------------------------------------------------------------
option(ENABLE_COVERAGE "Enable gcov/lcov code-coverage instrumentation" OFF)

include(CTest)
if(BUILD_TESTING)
  add_subdirectory(tests)
endif()
