// Minimal DirectX 12 triangle demo using TONGUE Renderer/Window classes
#include "Renderer.h"
#include "Renderer/Window.h"
#include <windows.h>

int main() {
    TONGUE::Window window;
    if (!window.Create(800, 600, "DirectX 12 Triangle"))
        return -1;

    TONGUE::Renderer renderer;
    renderer.SetHWND(window.GetHWND());
    if (!renderer.Init(800, 600, "DirectX 12 Triangle"))
        return -1;

    // Main loop
    while (!window.ShouldClose()) {
        renderer.BeginFrame();
        renderer.Clear({0.1f, 0.1f, 0.2f, 1.0f});
        // TODO: Draw triangle here
        renderer.EndFrame();
        renderer.Present();
        window.SwapBuffers();
    }

    renderer.Shutdown();
    window.Destroy();
    return 0;
}
