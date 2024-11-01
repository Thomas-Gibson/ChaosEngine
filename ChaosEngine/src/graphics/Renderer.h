#pragma once

#include "IGraphics.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace chaos
{
	using entity = u32;

	class Renderer3D : Diagnostic<Renderer3D>
	{
	private:
		IGraphics* gfx;
		const Scene* current_scene;
	public:
		Renderer3D() = default;
		Renderer3D(IWindow* window);
		~Renderer3D();

		Renderer3D(const Renderer3D&) = delete;
		Renderer3D& operator =(const Renderer3D&) = delete;


		void Update();

		void SetScene(const Scene* const scene);
		void RenderScene(Scene* pScene);
		void BeginFrame();
		void EndFrame();

	};
}
