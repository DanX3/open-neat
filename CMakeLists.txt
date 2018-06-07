cmake_minimum_required(VERSION 2.6)
include_directories(include)
file(GLOB SOURCES "src/*.cpp")
add_executable(neat ${SOURCES} main.cpp)
set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -g -DTEST=1 -pg")

enable_testing()
add_subdirectory(test)
