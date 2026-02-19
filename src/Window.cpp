#include "Renderer/Window.h"
#ifdef _WIN32
#include <GL/gl.h>
#include <GL/glu.h>
#include <string>
#include <tchar.h>

namespace TONGUE {

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg == WM_CLOSE) {
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

bool Window::Create(int w, int h, const std::string& title) {
	width = w;
	height = h;
	shouldClose = false;

	HINSTANCE hInstance = GetModuleHandle(nullptr);
	WNDCLASS wc = {};
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = _T("TONGUE_WindowClass");
	wc.style = CS_OWNDC;

	RegisterClass(&wc);

#ifdef UNICODE
	std::wstring wtitle(title.begin(), title.end());
	LPCTSTR windowTitle = wtitle.c_str();
#else
	LPCTSTR windowTitle = title.c_str();
#endif

	hwnd = CreateWindowEx(
		0,
		_T("TONGUE_WindowClass"),
		windowTitle,
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, width, height,
		nullptr, nullptr, hInstance, nullptr
	);
	if (!hwnd) return false;

	hdc = GetDC(hwnd);
	PIXELFORMATDESCRIPTOR pfd = {};
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int pf = ChoosePixelFormat(hdc, &pfd);
	if (!pf) return false;
	if (!SetPixelFormat(hdc, pf, &pfd)) return false;

	hglrc = wglCreateContext(hdc);
	if (!hglrc) return false;
	if (!wglMakeCurrent(hdc, hglrc)) return false;

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);
	return true;
}

void Window::Destroy() {
	if (hglrc) {
		wglMakeCurrent(nullptr, nullptr);
		wglDeleteContext(hglrc);
		hglrc = nullptr;
	}
	if (hwnd && hdc) {
		ReleaseDC(hwnd, hdc);
		hdc = nullptr;
	}
	if (hwnd) {
		DestroyWindow(hwnd);
		hwnd = nullptr;
	}
}

void Window::SwapBuffers() {
	if (hdc) ::SwapBuffers(hdc);
	// Poll messages
	MSG msg;
	while (PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE)) {
		if (msg.message == WM_QUIT) {
			shouldClose = true;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

bool Window::ShouldClose() const {
	return shouldClose;
}

int Window::GetWidth() const {
	return width;
}

int Window::GetHeight() const {
	return height;
}

} // namespace TONGUE
#endif
