#pragma once

#include "common.h"
#include "Settings.h"
#include "IWindow.h"
#include "Renderer.h"

namespace chaos
{

	class CHAOS_API Application
	{
	
	public:
		~Application();

	protected:

		//void ConfigureSettings(Settings& settings);

		void ConfigureSettings(Settings& settings);

	private:
		static void Update();
		static Settings settings;
		static Renderer3D renderer;
	};

}