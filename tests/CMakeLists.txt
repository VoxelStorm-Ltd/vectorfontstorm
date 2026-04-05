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
# ---------------------------------------------------------------------------
add_executable(vectorfontstorm_tests tests.cpp)

target_compile_definitions(vectorfontstorm_tests PRIVATE
  TEST_FONT_PATH="${TEST_FONT_PATH}"
)

target_link_libraries(vectorfontstorm_tests PRIVATE
  vectorfontstorm_lib
  Catch2::Catch2WithMain
)

# The test executable includes vectorfontstorm headers that transitively pull
# in vectorstorm and GL headers; add the stubs directory first so that the
# minimal stubs take precedence over any full vectorstorm installation.
target_include_directories(vectorfontstorm_tests PRIVATE
  ${VECTORSTORM_STUBS_DIR}   # must come before any other vectorstorm path
)

# ---------------------------------------------------------------------------
# Code coverage
# ---------------------------------------------------------------------------
if(ENABLE_COVERAGE)
  target_compile_options(vectorfontstorm_lib    PRIVATE --coverage -O0 -g)
  target_link_options(vectorfontstorm_lib       PRIVATE --coverage)
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
