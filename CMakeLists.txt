cmake_minimum_required(VERSION 3.14)
project(formal)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS "--coverage")

add_executable(formal main.cpp)

add_subdirectory(googletest-master)
include_directories(googletest-master/googletest/include)
include_directories(googletest-master/googlemock/include)
target_link_libraries(formal gtest gtest_main)