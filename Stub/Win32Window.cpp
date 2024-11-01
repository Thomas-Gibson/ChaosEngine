#include "Win32Window.h"

namespace chaos
{

	Win32Window::Win32Window(u32 width, u32 height, wstring title)
	{
		HINSTANCE hInstance = GetModuleHandleW(nullptr);

		WNDCLASSEXW wc = {};
		wc.cbSize = sizeof(WNDCLASSEXW);
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc = WindowProc;
		wc.cbClsExtra = 0u;
		wc.cbWndExtra = 0u;
		wc.hInstance = hInstance;
		wc.hIcon = LoadIconW(nullptr, MAKEINTRESOURCEW(IDI_APPLICATION));
		wc.hCursor = LoadCursorW(nullptr, MAKEINTRESOURCEW(IDC_ARROW));
		wc.hbrBackground = nullptr;
		wc.lpszMenuName = nullptr;
		wc.lpszClassName = title;
		wc.hIconSm = LoadIconW(nullptr, MAKEINTRESOURCEW(IDI_APPLICATION));

		if (!RegisterClassExW(&wc))
			MessageBoxW(nullptr, L"Failed to register window class!", 0, MB_ICONERROR);

		u32 x = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
		u32 y = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;

		hWnd =
			CreateWindowExW(
				0,
				title,
				title,
				WS_OVERLAPPEDWINDOW,
				x, y, width, height,
				nullptr,
				nullptr,
				hInstance,
				nullptr);

		if (!hWnd)
			MessageBoxW(nullptr, L"Failed to create window!", 0, MB_ICONERROR);
	}

	void Win32Window::Show() const noexcept
	{
		ShowWindow(this->hWnd, SW_SHOW);
	}

	void Win32Window::Hide() const noexcept
	{
		ShowWindow(this->hWnd, SW_HIDE);
	}

	bool Win32Window::ProcessEvents() const noexcept
	{
		MSG msg;


		if (PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}

		return msg.message != WM_QUIT;
	}

	void Win32Window::SetIcon(const wstring path) const noexcept
	{
		HICON hIcon = static_cast<HICON>(LoadImageW(nullptr, path, ICON_BIG, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED));

		if (!hIcon)
			MessageBox(nullptr, L"Failed to set icon!\n\nPotential issues:\nThe file path was not found.\nThe image file extension is not .bmp or .ico.", L"Warning", MB_ICONWARNING);

		SendMessage(this->hWnd, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(hIcon));

		SendMessage(this->hWnd, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(hIcon));
	}

	void Win32Window::SetCaptionColor(const Color& color) const noexcept
	{
		COLORREF caption_color = color.b << 16 | color.g << 8 | color.r;
		DwmSetWindowAttribute(this->hWnd, DWMWA_CAPTION_COLOR, &caption_color, sizeof(COLORREF));
	}

	void Win32Window::SetBoarderColor(const Color& color) const noexcept
	{
		COLORREF boarder_color = color.b << 16 | color.g << 8 | color.r;
		DwmSetWindowAttribute(this->hWnd, DWMWA_BORDER_COLOR, &boarder_color, sizeof(COLORREF));
	}

	void Win32Window::SetBorderEdges(const bool is_rounded) const noexcept
	{
		if (is_rounded) return;

		DWM_WINDOW_CORNER_PREFERENCE preference = DWMWCP_DONOTROUND;
		DwmSetWindowAttribute(this->hWnd, DWMWA_WINDOW_CORNER_PREFERENCE, &preference, sizeof(DWM_WINDOW_CORNER_PREFERENCE));
	}

	void Win32Window::SetCaptionText(wstring text) const noexcept
	{
		SetWindowTextW(this->hWnd, text);
	}

	void Win32Window::SetResizable(const bool is_resizable) const noexcept
	{
		LONG style = GetWindowLong(this->hWnd, GWL_STYLE);

		if (is_resizable)
			style |= (WS_SIZEBOX | WS_MAXIMIZEBOX);
		else
			style &= ~(WS_SIZEBOX | WS_MAXIMIZEBOX);

		SetWindowLong(this->hWnd, GWL_STYLE, style);

		SetWindowPos(this->hWnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
	}

	void Win32Window::SetStyle(Settings::DisplayModes mode) const noexcept
	{
		LONG style = GetWindowLong(this->hWnd, GWL_STYLE);

		switch (mode)
		{
			case Settings::DisplayModes::STANDARD:
				style &= ~WS_POPUPWINDOW;
				style |= (WS_OVERLAPPEDWINDOW & ~(WS_SIZEBOX | WS_MAXIMIZEBOX));
				break;

			case Settings::DisplayModes::BORDERLESS_WINDOW:
				style &= ~WS_OVERLAPPEDWINDOW;
				style |= (WS_POPUPWINDOW & ~(WS_SIZEBOX | WS_MAXIMIZEBOX));
				break;

			case Settings::DisplayModes::FULLSCREEN:
				style &= ~WS_OVERLAPPEDWINDOW;
				style |= (WS_POPUPWINDOW & ~(WS_SIZEBOX | WS_MAXIMIZEBOX));
				SetWindowLong(this->hWnd, GWL_STYLE, style);
				SetWindowPos(
					this->hWnd,
					NULL,
					0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
					SWP_NOZORDER | SWP_FRAMECHANGED | SWP_NOACTIVATE);
				return;
				break;
			default:
				return;
		}

		SetWindowLong(this->hWnd, GWL_STYLE, style);

		SetWindowPos(this->hWnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
	}

	u32 Win32Window::Width() const noexcept
	{
		RECT cr;
		GetClientRect(hWnd, &cr);

		return cr.right - cr.left;
	}

	u32 Win32Window::Height() const noexcept
	{
		RECT cr;
		GetClientRect(hWnd, &cr);

		return cr.bottom - cr.top;
	}

	void* Win32Window::Handle() const noexcept
	{
		return hWnd;
	}

	LRESULT CALLBACK Win32Window::WindowProc(_In_ HWND hWnd, _In_ UINT msg, _In_ WPARAM wParam, _In_ LPARAM lParam)
	{
		LRESULT result = 0;

		switch (msg)
		{
			case WM_CREATE:
			{

			} break;

			case WM_SIZE:
			{

			} break;

			case WM_CLOSE:
			{
				DestroyWindow(hWnd);
			} break;

			case WM_DESTROY:
			{
				PostQuitMessage(0);
			} break;

			default:
				result = DefWindowProcW(hWnd, msg, wParam, lParam);
				break;
		}

		return result;
	}

}