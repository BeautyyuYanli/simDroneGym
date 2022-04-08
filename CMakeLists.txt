cmake_minimum_required(VERSION 3.15)

project(simDroneGym LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
)
FetchContent_MakeAvailable(googletest)

add_library(simDroneGym STATIC simDroneGym/src/simdronegym.cpp)

enable_testing()
add_executable(alltest test/test.cpp)
target_include_directories(alltest PUBLIC simDroneGym/include)
target_link_libraries(alltest gtest_main simDroneGym)
include(GoogleTest)
gtest_discover_tests(alltest)

add_executable(demo1 demo/demo1.cpp)
target_link_libraries(demo1 simDroneGym)
add_executable(demo2 demo/demo2.cpp)
target_link_libraries(demo2 simDroneGym)