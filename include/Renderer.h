
#pragma once
#include <memory>
#include "Renderer/Color.h"
#include "Renderer/RenderSubmission.h"
#include "Renderer/Texture.h"
#include "Renderer/Shader.h"

namespace TONGUE {

class Mesh;
class Renderer {
public:
    Renderer();
    ~Renderer();
    bool Init(int width, int height, const char* title);
    void Shutdown();
    void BeginFrame();
    void SetViewport(int x, int y, int width, int height);
    void Clear(const Color& color);
    void Submit(const RenderSubmission& submission);
    void EndFrame();
    void Present();
    bool WindowShouldClose() const;
    std::shared_ptr<Mesh> CreateMesh(/* params */);
    std::shared_ptr<Texture> CreateTexture(/* params */);
    std::shared_ptr<Shader> CreateShader(/* params */);
    void SetHWND(void* hwnd);
private:
    struct Impl;
    std::unique_ptr<Impl> impl;
};

} // namespace TONGUE
