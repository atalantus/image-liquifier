cmake_minimum_required(VERSION 3.22)
project(physics_engine)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

set(SOURCES
        src/engine.cpp
        src/collision.cpp)

find_package(SFML 2 REQUIRED COMPONENTS network audio graphics window system)

add_executable(test_physics_engine test.cpp ${SOURCES})

add_executable(benchmark_physics_engine benchmark.cpp ${SOURCES})
target_link_libraries(benchmark_physics_engine sfml-system sfml-window sfml-graphics)
if (UNIX)
    target_link_libraries(benchmark_physics_engine pthread)
endif (UNIX)

add_library(image_liquifier STATIC imageLiquifier.cpp ${SOURCES})