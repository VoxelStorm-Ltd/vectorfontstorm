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
# Minimal vectorstorm stub headers
# The full vectorstorm library (VoxelStorm-Ltd/vectorstorm) has GCC 13 /
# C++17 incompatibilities in headers that are not exercised by the
# vectorfontstorm geometry pipeline (constexpr sincos, C++23 operator[],
# boost::math constants).  A minimal stub is provided that defines only the
# vector2f type required by the core library.
# ---------------------------------------------------------------------------
set(VECTORSTORM_STUBS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/stubs)

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
  ${VECTORSTORM_STUBS_DIR}     # minimal vectorstorm stubs (must precede any full vectorstorm install)
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
# ---------------------------------------------------------------------------
include(Catch)
catch_discover_tests(vectorfontstorm_tests)
