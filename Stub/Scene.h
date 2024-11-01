#pragma once
#include <vector>

namespace chaos
{

	class Scene
	{
	public:
		virtual void Update() = 0;
		virtual void Render() = 0;
	};

}
