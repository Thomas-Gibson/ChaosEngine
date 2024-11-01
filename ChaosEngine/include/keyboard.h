#pragma once

#include "common.h"

#include <bitset>

namespace chaos 
{
	class CHAOS_API Keyboard
	{
		friend class Win32Window;
	public:
		Keyboard() = default;
		~Keyboard() = default;

		Keyboard(const Keyboard&) = delete;
		Keyboard& operator =(Keyboard&) = delete;

		bool IsKeyPressed(unsigned char keyCode) const noexcept
		{
			return currentStates.test(keyCode);
		}

		bool IsKeyReleased(unsigned char keyCode) const noexcept
		{
			return !currentStates.test(keyCode);
		}

		bool IsKeyTapped(unsigned char keyCode)
		{
			return currentStates.test(keyCode) && !pastStates.test(keyCode);
		}

	private:
		void OnChar(wchar_t character)
		{
		}

		void OnKeyPress(unsigned char keyCode)
		{
			currentStates.set(keyCode);
		}

		void OnKeyRelease(unsigned char keyCode)
		{
			currentStates.set(keyCode, false);
		}

		void UpdatePast()
		{
			pastStates = currentStates;
		}

		std::bitset<256> currentStates;
		std::bitset<256> pastStates;
	};
}