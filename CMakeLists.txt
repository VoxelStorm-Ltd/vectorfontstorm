cmake_minimum_required(VERSION 3.16)
project(vectorfontstorm CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# vectorfontstorm is a header-based library intended for direct inclusion into
# projects, not to be built as a standalone library.  This CMakeLists exists
# solely to drive the test suite.
include(CTest)
if(BUILD_TESTING)
  add_subdirectory(tests)
endif()
