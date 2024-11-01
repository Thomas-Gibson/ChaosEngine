#pragma once

#include "common.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace chaos
{

	class CHAOS_API Camera
	{
		friend class Renderer3D;
	private:
		glm::mat4 view;
		glm::mat4 projection;
	public:

		Camera() = default;

		Camera(glm::vec3 pPosition, glm::vec3 pTarget, r32 pFOV, r32 pAspectRatio, r32 pNear, r32 pFar)
		{
			glm::vec3 upDirection(0.0f, 1.0f, 0.0f);
			view = glm::lookAtLH(pPosition, pTarget, upDirection);

			pFOV = glm::radians(pFOV);
			projection = glm::perspectiveLH(pFOV, pAspectRatio, pNear, pFar);
		}

		Camera(r32 pWidth, r32 pHeight, glm::vec3 pPosition, glm::vec3 pTarget, r32 pNear, r32 pFar)
		{
			glm::vec3 upDirection(0.0f, 1.0f, 0.0f);
			view = glm::lookAtLH(pPosition, pTarget, upDirection);

			r32 w = pWidth / 2.0f;
			r32 h = pHeight / 2.0f;
			projection = glm::orthoLH(-w, w, -h, h, pNear, pFar);
		}

		Camera(const Camera&) = delete;

	};

}