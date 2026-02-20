#include "Renderer/Window.h"
#ifdef _WIN32
#include <windows.h>
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
	hwnd = nullptr;
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
	// DirectX 12 swap chain and device creation will go here
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);
	return true;
}

void Window::Destroy() {
	// DirectX 12 context cleanup will go here
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
