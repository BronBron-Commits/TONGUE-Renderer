// TriangleStressTest.cpp
// Renderer bring-up and stress test for TONGUE-Renderer
// This is a stub. Actual rendering logic must be implemented with your graphics API of choice.

#include "Renderer.h"
#include "Renderer/Mesh.h"
#include "Renderer/Shader.h"
#include "Renderer/Window.h"
#include <chrono>
#include <thread>
#include <vector>
#include <iostream>

// Dummy Vector2/Vector3/Matrix4 for illustration (replace with Core types)
struct Vector2 { float x, y; };
struct Vector3 { float x, y, z; };
struct Matrix4 { float m[16]; };

// Hardcoded triangle mesh
static std::vector<Vertex> GetTriangleVerts() {
    return {
        { { -0.6f, -0.4f, 0.0f }, { 0, 0, 1 }, { 1, 0, 0 }, { 0, 0 } },
        { {  0.6f, -0.4f, 0.0f }, { 0, 0, 1 }, { 0, 1, 0 }, { 1, 0 } },
        { {  0.0f,  0.6f, 0.0f }, { 0, 0, 1 }, { 0, 0, 1 }, { 0.5f, 1 } }
    };
}
static std::vector<uint32_t> GetTriangleIndices() { return { 0, 1, 2 }; }

int main() {
    Renderer renderer;
    if (!renderer.Init(1280, 720)) {
        std::cerr << "Failed to initialize renderer!\n";
        return 1;
    }

    Mesh triangle;
    triangle.Upload(GetTriangleVerts(), GetTriangleIndices());

    Shader shader;
    shader.Load("../assets/triangle.vert", "../assets/triangle.frag");

    auto start = std::chrono::high_resolution_clock::now();
    int frameCount = 0;
    float time = 0.0f;
    while (true) {
        renderer.BeginFrame();
        shader.Bind();
        shader.SetMatrix("uProjection", Matrix4{}); // Replace with real projection
        shader.SetMatrix("uView", Matrix4{});       // Replace with real view
        shader.SetMatrix("uModel", Matrix4{});      // Replace with real model
        shader.SetVector("uResolution", Vector3{1280, 720, 0});
        shader.SetVector("uTime", Vector3{time, 0, 0});
        triangle.Draw();
        shader.Unbind();
        renderer.EndFrame();
        renderer.Present();
        frameCount++;
        time += 1.0f / 60.0f;
        if (frameCount % 60 == 0) {
            auto now = std::chrono::high_resolution_clock::now();
            float elapsed = std::chrono::duration<float>(now - start).count();
            std::cout << "FPS: " << frameCount / elapsed << "\n";
            frameCount = 0;
            start = now;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
    renderer.Shutdown();
    return 0;
}
