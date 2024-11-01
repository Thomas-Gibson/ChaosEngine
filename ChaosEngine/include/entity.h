#pragma once

#include "common.h"

#include "transform.h"
#include "mesh.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace chaos
{
	struct Entity
	{
		Transform worldTransform;
		Transform localTransform;
		Mesh* mesh;
	};

}