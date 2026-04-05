# ---------------------------------------------------------------------------
# Catch2 – testing framework
# ---------------------------------------------------------------------------
include(FetchContent)

FetchContent_Declare(Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.5.3
  GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(Catch2)

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
  ${CMAKE_SOURCE_DIR}/vectorfontstorm/poly2tri/common/shapes.cc
  ${CMAKE_SOURCE_DIR}/vectorfontstorm/poly2tri/sweep/advancing_front.cc
  ${CMAKE_SOURCE_DIR}/vectorfontstorm/poly2tri/sweep/sweep.cc
  ${CMAKE_SOURCE_DIR}/vectorfontstorm/poly2tri/sweep/sweep_context.cc
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
  ${CMAKE_SOURCE_DIR}/vectorfontstorm/point.cpp
  ${CMAKE_SOURCE_DIR}/vectorfontstorm/manager.cpp
)

target_compile_definitions(vectorfontstorm_tests PRIVATE
  TEST_FONT_PATH="${TEST_FONT_PATH}"
)

target_include_directories(vectorfontstorm_tests PRIVATE
  ${CMAKE_SOURCE_DIR}          # top-level: exposes "vectorfontstorm/*.h" and "cast_if_required.h"
  ${vectorstorm_SOURCE_DIR}    # vectorstorm headers (vector2f, vector3f, etc.)
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
