#pragma once
#include <string>
#ifdef _WIN32
#include <windows.h>
#endif

namespace TONGUE {
class Window {
public:
    bool Create(int width, int height, const std::string& title);
    void Destroy();
    void SwapBuffers();
    bool ShouldClose() const;
    int GetWidth() const;
    int GetHeight() const;

#ifdef _WIN32
    void* GetHWND() const { return hwnd; }
#endif

private:
#ifdef _WIN32
    HWND hwnd = nullptr;
    HDC hdc = nullptr;
    // DirectX 12 device/context handles will go here
    // Example: ComPtr<ID3D12Device> device;
    int width = 0;
    int height = 0;
    bool shouldClose = false;
#endif
};
}
