#pragma once

// Main interface for the TONGUE Renderer

namespace TONGUE {

class Renderer {
public:
    bool Init(int width, int height);
    void BeginFrame();
    void EndFrame();
    void Draw(); // Placeholder for drawing logic
};

} // namespace TONGUE
