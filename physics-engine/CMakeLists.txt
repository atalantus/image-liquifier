cmake_minimum_required(VERSION 3.22)
project(physics_engine)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

set(SOURCES
        src/engine.cpp
        src/collision.cpp)

add_executable(test_physics_engine test.cpp ${SOURCES})
add_executable(benchmark_physics_engine benchmark.cpp ${SOURCES})
