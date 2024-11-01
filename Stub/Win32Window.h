#pragma once

#include "IWindow.h"

#include <Windows.h>
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")

#include <exception>

namespace chaos
{

	class Win32Window final : public IWindow
	{
	public:
		Win32Window(u32 width, u32 height, wstring title);
		~Win32Window() { };

		Win32Window(const Win32Window&) = delete;
		Win32Window& operator = (const Win32Window&) = delete;

		void Show() const noexcept override;
		void Hide() const noexcept override;
		bool ProcessEvents() const noexcept override;

		void SetIcon(const wstring path) const noexcept override;
		void SetCaptionColor(const Color& color) const noexcept override;
		void SetBoarderColor(const Color& color) const noexcept override;
		void SetBorderEdges(const bool is_rounded) const noexcept override;
		void SetCaptionText(wstring text) const noexcept override;
		void SetResizable(const bool is_resizable) const noexcept override;
		void SetStyle(Settings::DisplayModes mode) const noexcept override;
		u32 Width() const noexcept override;
		u32 Height() const noexcept override;
		void* Handle() const noexcept override;

	private:
		HWND hWnd;
		static LRESULT CALLBACK WindowProc(_In_ HWND hWnd, _In_ UINT msg, _In_ WPARAM wParam, _In_ LPARAM lParam);
	};

}