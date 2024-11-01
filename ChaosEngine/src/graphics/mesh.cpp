#include "mesh.h"
#include "IGraphics.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

chaos::Mesh::Mesh(std::string pFile)
{
	std::vector<Vertex> vertices;
	std::vector<u32> indices;

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(pFile.c_str(),
		aiProcess_Triangulate |
		aiProcess_FlipUVs |
		aiProcess_JoinIdenticalVertices |
		aiProcess_OptimizeMeshes |
		aiProcess_PreTransformVertices |
		aiProcess_ConvertToLeftHanded);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		std::cerr << "ASSIMP Error: " << importer.GetErrorString() << std::endl;


	for (int i = 0; i < scene->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[i];

		for (unsigned int j = 0; j < mesh->mNumVertices; j++)
		{
			Vertex vertex = {};
			vertex.position.x = mesh->mVertices[j].x;
			vertex.position.y = mesh->mVertices[j].y;
			vertex.position.z = mesh->mVertices[j].z;

			if (mesh->mTextureCoords[0])
			{
				vertex.text_coord.x = mesh->mTextureCoords[0][j].x;
				vertex.text_coord.y = mesh->mTextureCoords[0][j].y;
			}
			else
			{
				vertex.text_coord = { 0.0f, 0.0f };
			}

			if (mesh->HasNormals())
			{
				vertex.normal.x = mesh->mNormals[j].x;
				vertex.normal.y = mesh->mNormals[j].y;
				vertex.normal.z = mesh->mNormals[j].z;
			}

			vertices.push_back(vertex);
		}

		for (unsigned int j = 0; j < mesh->mNumFaces; j++)
		{
			aiFace face = mesh->mFaces[j];
			for (unsigned int k = 0; k < face.mNumIndices; k++)
			{
				indices.push_back(face.mIndices[k]);
			}
		}
	}

	index_count = indices.size();

	vertexBuffer = IBuffer::Factory::Create(IBuffer::Type::VERTEX, false, vertices.size() * sizeof(Vertex), vertices.data());
	indexBuffer = IBuffer::Factory::Create(IBuffer::Type::INDEX, false, indices.size() * sizeof(u32), indices.data());
}

chaos::Mesh::~Mesh()
{
	delete static_cast<IBuffer*>(vertexBuffer);
	delete static_cast<IBuffer*>(indexBuffer);
}

void chaos::Mesh::Draw(void* const gfx) const
{
	static_cast<IGraphics*>(gfx)->DrawIndex(index_count);
}

void chaos::Mesh::Bind() const
{
	static_cast<IBuffer*>(vertexBuffer)->Bind(sizeof(Vertex));
	static_cast<IBuffer*>(indexBuffer)->Bind(sizeof(u32));
}

void chaos::Mesh::UnBind() const
{
	static_cast<IBuffer*>(vertexBuffer)->UnBind();
	static_cast<IBuffer*>(indexBuffer)->UnBind();
}
