#pragma once
#pragma once

#include "common.h"

namespace chaos
{

	class CHAOS_API Mouse
	{
		friend class Win32Window;
	public:
		Mouse() = default;
		~Mouse() = default;

		Mouse(const Mouse&) = delete;
		Mouse& operator = (const Mouse&) = delete;

		unsigned short GetX() const noexcept
		{
			return x;
		}

		unsigned short GetY() const noexcept
		{
			return y;
		}

		bool LmbDown() const noexcept { return state & 1; }
		bool RmbDown() const noexcept { return state & 2; }

	private:
		void OnMouseMove(int x, int y) noexcept { this->x = x, this->y = y; }
		void OnLeftClick() noexcept { state |= 1; }
		void OnRightClick() noexcept { state |= 2; }

		void OnLeftRelease() noexcept { state &= 2; }
		void OnRightRelease() noexcept { state &= 1; }

		s32 x, y;
		u32 state;
	};

}