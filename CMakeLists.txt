
cmake_minimum_required(VERSION 3.20)
project(TONGUE-Renderer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Gather all public headers and sources
file(GLOB RENDERER_HEADERS
    "include/Renderer.h"
    "include/Renderer/*.h"
    "include/Renderer/MasterRenderer.h"
)
file(GLOB RENDERER_SOURCES
    "src/Mesh.cpp"
    "src/MeshLoader.cpp"
    "src/Renderer.cpp"
    "src/Shader.cpp"
    "src/Texture.cpp"
    "src/Window.cpp"
    "src/MasterRenderer.cpp"
)

# Build the renderer as a static library
add_library(TongueRenderer STATIC ${RENDERER_HEADERS} ${RENDERER_SOURCES})

# Demo executable
add_executable(TongueRendererDemo WIN32 src/Main.cpp)
target_link_libraries(TongueRendererDemo PRIVATE TongueRenderer glfw d3d12 dxgi d3dcompiler)
target_include_directories(TongueRendererDemo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Export public headers
target_include_directories(TongueRenderer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)


# Find and link dependencies here only
find_package(glfw3 CONFIG REQUIRED)
# find_package(SDL2 REQUIRED) # Uncomment if using SDL2
target_link_libraries(TongueRenderer PRIVATE glfw d3d12 dxgi d3dcompiler)
# target_link_libraries for DirectX 12 will be added here
target_link_libraries(TongueRenderer PRIVATE glfw d3d12 dxgi d3dcompiler)
# target_link_libraries(TongueRenderer PUBLIC SDL2::SDL2) # Uncomment if using SDL2


# Build renderer validation tests
add_subdirectory(tests)
