#pragma once

#include "IGraphics.h"

namespace chaos
{
	using entity = u32;

	class CHAOS_API Renderer3D
	{
	private:
		IGraphics* gfx;
	public:
		Renderer3D() = default;
		Renderer3D(IWindow* window);
		~Renderer3D();

		Renderer3D(const Renderer3D&) = delete;
		Renderer3D& operator =(const Renderer3D&) = delete;


		void Update();

	};
}
  