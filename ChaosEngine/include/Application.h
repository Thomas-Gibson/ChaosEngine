#pragma once
/*****************************************************************//**
 * \file   Application.h
 * \brief  To run the application
 * 
 * \author Thomas Gibson
 * \date   October 2024
 *********************************************************************/

#include "common.h"
#include "Settings.h"

#include <iostream>

#include "diagnostic.h"
#include "keyboard.h"
#include "mouse.h"


namespace chaos
{
	Keyboard* keyboard = nullptr;
	Mouse* mouse = nullptr;

	class CHAOS_API Application
	{
	
	public:
		Application();
		~Application();

	protected:

		void ConfigureSettings(Settings& settings);

	private:
		Settings settings;
	};
}