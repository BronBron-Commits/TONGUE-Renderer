cmake_minimum_required(VERSION 3.10)
project(TONGUE-Renderer)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

file(GLOB RENDERER_HEADERS
    "include/Renderer.h"
    "include/Renderer/*.h"
)
file(GLOB RENDERER_SOURCES
    "src/*.cpp"
)

add_library(TONGUE-Renderer ${RENDERER_HEADERS} ${RENDERER_SOURCES})

target_link_libraries(TONGUE-Renderer TONGUE-Core)

# Optionally add tests
# add_subdirectory(tests)
