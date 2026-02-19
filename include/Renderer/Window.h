#pragma once

// Window and context management for TONGUE Renderer

namespace TONGUE {

class Window {
public:
    bool Create(int width, int height, const char* title);
    void Destroy();
    void* GetNativeHandle();
};

} // namespace TONGUE
