#pragma once

#include "common.h"
#include "Settings.h"
#include "keyboard.h"
#include "mouse.h"

#include <diagnostic.h>

namespace chaos
{
	class IWindow : public Diagnostic<IWindow>
	{
	public:
		virtual ~IWindow() = default;

		virtual void Show() const noexcept = 0;
		virtual void Hide() const noexcept = 0;
		virtual bool ProcessEvents() const noexcept = 0;
		virtual void SetIcon(const wstring path) const noexcept = 0;
		virtual void SetCaptionColor(Color color) const noexcept = 0;
		virtual void SetBoarderColor(Color color) const noexcept = 0;
		virtual void SetBorderEdges(const bool is_rounded) const noexcept = 0;
		virtual void SetCaptionText(wstring text) const noexcept = 0;
		virtual void SetResizable(const bool is_resizable) const noexcept = 0;
		virtual void SetStyle(Settings::DisplayModes mode) const noexcept = 0;
		virtual u32 Width() const noexcept = 0;
		virtual u32 Height() const noexcept = 0;
		virtual void* Handle() const noexcept = 0;
		virtual Keyboard* GetKeyboard() const noexcept = 0;
		virtual Mouse* GetMouse() const noexcept = 0;

		class Factory
		{
		private:
			IWindow* window;

		public:
			Factory() = delete;
			Factory(const Factory&) = delete;
			Factory& operator = (const Factory&) = delete;
			void* operator new(const size_t) = delete;

			static IWindow* Create(u32 width, u32 height, wstring title);
		};

	};

}