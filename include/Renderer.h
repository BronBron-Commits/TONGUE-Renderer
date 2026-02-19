
#pragma once
#include <memory>
#include <vector>


#include "Renderer/Color.h"
#include "Renderer/RenderSubmission.h"

namespace TONGUE {

class Window;
class Mesh;
class Texture;
class Shader;
class Camera;
class Character;
struct Viewport;

// The only legal frontend interface for rendering

class Renderer {
public:
    Renderer();
    ~Renderer();

    // --- Lifecycle ---
    bool Init(int width, int height, const char* title = "TONGUE Renderer");
    void Shutdown();

    // --- Frame Control (single entry point for all drawing) ---
    void BeginFrame();
    void SetViewport(int x, int y, int width, int height);
    void Clear(const Color& color);
    void Submit(const RenderSubmission& submission); // Accepts mesh, transform, material, etc.

    void EndFrame();
    void Present();
    bool WindowShouldClose() const;

    // --- Resource Management (all GPU objects owned here) ---
    std::shared_ptr<Mesh>   CreateMesh(/* params */);
    std::shared_ptr<Texture> CreateTexture(/* params */);
    std::shared_ptr<Shader>  CreateShader(/* params */);

    // --- Scene/Entity Translation (optional, for high-level helpers) ---
    void Submit(const Character& character);

    // --- No frontend may call graphics APIs directly ---
    // All rendering must go through Renderer.

private:
    struct Impl;
    std::unique_ptr<Impl> impl;
};

} // namespace TONGUE
