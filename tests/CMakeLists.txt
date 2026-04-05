cmake_minimum_required(VERSION 3.16)
project(vectorfontstorm_tests CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)

# Root of the vectorfontstorm repository (parent of this tests/ directory).
# Use CMAKE_CURRENT_SOURCE_DIR so this works whether the file is processed
# as a standalone top-level source or via add_subdirectory from the root.
set(VECTORFONTSTORM_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/..)

# ---------------------------------------------------------------------------
# Catch2 – testing framework
# ---------------------------------------------------------------------------
include(FetchContent)
# Allow direct FetchContent_Populate calls for header-only repos that ship no
# CMakeLists.txt (vectorstorm, cast_if_required).  CMP0169 made this pattern
# deprecated in CMake 3.28; setting it OLD keeps the old behaviour.
cmake_policy(SET CMP0169 OLD)

FetchContent_Declare(Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.5.3
  GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(Catch2)

# ---------------------------------------------------------------------------
# cast_if_required – safe numeric cast helper (no CMakeLists.txt; use Populate)
# Shared across VoxelStorm projects; header-only, lives in the repo root.
# ---------------------------------------------------------------------------
FetchContent_Declare(cast_if_required
  GIT_REPOSITORY https://github.com/VoxelStorm-Ltd/cast_if_required.git
  GIT_TAG        0237e7df3a25b5c34edb864ac20eb7e98d3f24cc
  GIT_SHALLOW    FALSE
)
FetchContent_GetProperties(cast_if_required)
if(NOT cast_if_required_POPULATED)
  FetchContent_Populate(cast_if_required)
endif()

# ---------------------------------------------------------------------------
# vectorstorm – header-only math library (no CMakeLists.txt; use Populate)
# This library is an intended dependency of vectorfontstorm and provides the
# vector2, vector3 etc. types used throughout the geometry pipeline.
# ---------------------------------------------------------------------------
FetchContent_Declare(vectorstorm
  GIT_REPOSITORY https://github.com/VoxelStorm-Ltd/vectorstorm.git
  GIT_TAG        d2c319bb6d5ba47ddb0f5ad671a5ae9c920542d8
)
FetchContent_GetProperties(vectorstorm)
if(NOT vectorstorm_POPULATED)
  FetchContent_Populate(vectorstorm)
  # GCC 13 rejects uninitialized scalar variables in constexpr functions.
  # vectorstorm/sincos.h declares two float temporaries without initializers;
  # patch them in-place so the build succeeds on modern compilers.
  execute_process(
    COMMAND sed -i
      "s/float out_sin_temp;/float out_sin_temp = 0.0f;/g;s/float out_cos_temp;/float out_cos_temp = 0.0f;/g"
      vectorstorm/sincos.h
    WORKING_DIRECTORY ${vectorstorm_SOURCE_DIR}
  )
endif()

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
# ---------------------------------------------------------------------------
add_library(poly2tri STATIC
  ${VECTORFONTSTORM_ROOT}/vectorfontstorm/poly2tri/common/shapes.cc
  ${VECTORFONTSTORM_ROOT}/vectorfontstorm/poly2tri/sweep/advancing_front.cc
  ${VECTORFONTSTORM_ROOT}/vectorfontstorm/poly2tri/sweep/sweep.cc
  ${VECTORFONTSTORM_ROOT}/vectorfontstorm/poly2tri/sweep/sweep_context.cc
)

# ---------------------------------------------------------------------------
# Locate a TrueType font for integration tests.
# Pass -DTEST_FONT_PATH=/path/to/font.ttf to override.
# ---------------------------------------------------------------------------
if(NOT DEFINED TEST_FONT_PATH)
  find_file(_found_font
    NAMES DejaVuSans.ttf dejavu-sans.ttf
    PATHS
      /usr/share/fonts/truetype/dejavu
      /usr/share/fonts/TTF
      /usr/share/fonts/truetype
      /usr/local/share/fonts
  )
  if(_found_font)
    set(TEST_FONT_PATH "${_found_font}")
  else()
    set(TEST_FONT_PATH "")
  endif()
endif()

# ---------------------------------------------------------------------------
# Test executable
# vectorfontstorm is a header-based library intended for direct inclusion into
# projects; its source files are compiled directly into the test executable
# rather than being built as a separate named library target.
# ---------------------------------------------------------------------------
add_executable(vectorfontstorm_tests
  tests.cpp
  ${VECTORFONTSTORM_ROOT}/vectorfontstorm/point.cpp
  ${VECTORFONTSTORM_ROOT}/vectorfontstorm/manager.cpp
)

target_compile_definitions(vectorfontstorm_tests PRIVATE
  TEST_FONT_PATH="${TEST_FONT_PATH}"
)

target_include_directories(vectorfontstorm_tests PRIVATE
  ${VECTORFONTSTORM_ROOT}              # top-level: exposes "vectorfontstorm/*.h"
  ${cast_if_required_SOURCE_DIR}       # cast_if_required.h
  ${vectorstorm_SOURCE_DIR}            # vectorstorm headers (vector2f, vector3f, etc.)
)

target_link_libraries(vectorfontstorm_tests PRIVATE
  poly2tri
  Freetype::Freetype
  Catch2::Catch2WithMain
)

# ---------------------------------------------------------------------------
# Code coverage
# ---------------------------------------------------------------------------
option(ENABLE_COVERAGE "Enable gcov/lcov code-coverage instrumentation" OFF)
if(ENABLE_COVERAGE)
  target_compile_options(poly2tri               PRIVATE --coverage -O0 -g)
  target_link_options(poly2tri                  PRIVATE --coverage)
  target_compile_options(vectorfontstorm_tests  PRIVATE --coverage -O0 -g)
  target_link_options(vectorfontstorm_tests     PRIVATE --coverage)
endif()

# ---------------------------------------------------------------------------
# Register with CTest
# Explicitly add the Catch2 extras directory so that include(Catch) can find
# Catch.cmake regardless of how CMake_MODULE_PATH was configured.
# ---------------------------------------------------------------------------
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(Catch)
catch_discover_tests(vectorfontstorm_tests)
