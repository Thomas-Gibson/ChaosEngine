#pragma once

#include "common.h"

namespace chaos
{
	struct CHAOS_API Transform
	{
		glm::vec3 position = {};
		glm::vec3 rotation = {};
		glm::vec3 scale = { 1.0f,1.0f,1.0f };
	};
}
