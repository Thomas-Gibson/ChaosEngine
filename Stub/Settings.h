#pragma once

#include "common.h"
#include "Scene.h"

namespace chaos
{

	struct CHAOS_API Settings
	{
		struct
		{
			wstring name;
			wstring icon;
			Scene* starting_scene;
		} Project;

		enum class DisplayModes
		{
			STANDARD,
			BORDERLESS_WINDOW,
			FULLSCREEN
		};

		struct
		{
			u32 width;
			u32 height;
			Color caption_color;
			Color border_color;
			bool resizable;
			bool rounded_edges;
			DisplayModes display_mode;

		} Display;
	};

}