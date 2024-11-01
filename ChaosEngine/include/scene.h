#pragma once

#include "common.h"
#include "camera.h"

#include "entity.h"
#include "transform.h"

namespace chaos
{	

	class CHAOS_API Scene
	{
		friend class Application;
		friend class Renderer3D;

	private:
		Color mClearColor = {};
		Camera mCamera = {};
		Keyboard* kbd;
	protected:
		Entity entities[10];

		Scene() {}
		~Scene() = default;

		Scene(const Scene&) = delete;
		Scene& operator =(const Scene&) = delete;

		virtual void Initialize() = 0;
		virtual void Destroy() = 0;
		virtual void Update() = 0;

		void SetCamera(Camera& pCamera)
		{
			mCamera = pCamera;
		}

		void SetClearColor(Color pColor)
		{
			mClearColor = pColor;
		}
	
	};

}