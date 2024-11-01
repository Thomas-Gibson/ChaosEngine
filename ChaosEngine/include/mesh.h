#pragma once

#include "common.h"

namespace chaos
{

	class CHAOS_API Mesh
	{
		friend class Renderer3D;
	private:
		struct Vertex
		{
			glm::vec3 position;
			glm::vec2 text_coord;
			glm::vec3 normal;
		};

		void* vertexBuffer = nullptr;
		void* indexBuffer = nullptr;
		size_t index_count = 0;

	public:
		Mesh() = default;
		Mesh(std::string pFile);
		~Mesh();
		Mesh(const Mesh&) = delete;
		Mesh& operator = (const Mesh&) = delete;

		void Draw(void* const gfx) const;
		void Bind() const;
		void UnBind() const;

	};

};