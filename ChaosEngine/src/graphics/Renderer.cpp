#include "Renderer.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "transform.h"
#include "mesh.h"
/*
class Node
{
private:
	u32 id;
	s32 parent;

public:
	Node(u32 entity) : id(entity), parent(-1) {}

	void SetParent(u32 parentID)
	{
		this->parent = parentID;
	}

	void Update()
	{
		//localTransforms[id].position += velocities[id];

		if (parent >= 0)
		{
			//worldTransforms[parent].rotation.z += localTransforms[parent].rotation.z;

			glm::mat4 parentRotationMatrixX = glm::rotate(glm::mat4(1.0f), glm::radians(worldTransforms[parent].rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
			glm::mat4 parentRotationMatrixY = glm::rotate(glm::mat4(1.0f), glm::radians(worldTransforms[parent].rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
			glm::mat4 parentRotationMatrixZ = glm::rotate(glm::mat4(1.0f), glm::radians(worldTransforms[parent].rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

			glm::mat4 parentRotationMatrix = parentRotationMatrixX * parentRotationMatrixY * parentRotationMatrixZ;

			glm::vec4 childRotatedPosition = parentRotationMatrix * glm::vec4(localTransforms[id].position, 1.0f);


			worldTransforms[id].position = worldTransforms[parent].position + glm::vec3(childRotatedPosition);

			worldTransforms[id].rotation = worldTransforms[parent].rotation + localTransforms[id].rotation;

			worldTransforms[id].scale = worldTransforms[parent].scale * localTransforms[id].scale;
		}
		else
		{
			worldTransforms[id] = localTransforms[id];
		}
	}
};
*/
chaos::IVertexShader* vshader;
chaos::IPixelShader* pshader;
chaos::IBuffer* vbuffer;
chaos::IBuffer* indexbuffer;
chaos::IBuffer* cbuffer;

chaos::ITexture* diffuse_texture;
chaos::ITexture* uv_texture;
chaos::ITexture* metallic_texture;
chaos::ITexture* roughness_texture;
chaos::ITexture* ao_texture;

u32 indicies = 0;

struct CB
{
	glm::mat4x4 worldMatrix;
	glm::mat4x4 viewMatrix;
	glm::mat4x4 projectionMatrix;
	glm::vec4 lightDirection;
	glm::vec4 lightColor;
	glm::vec4 ambientLightColor;
	float ambientLightStrength;
};

chaos::Renderer3D::Renderer3D(IWindow* window)
	: gfx(IGraphics::Factory::Create(window))
{
	IVertexShader::Input input[]
	{
		{"POSITION", IVertexShader::Format::FLOAT_3},
		{"TEXCOORD", IVertexShader::Format::FLOAT_2},
		{"NORMAL", IVertexShader::Format::FLOAT_3},
	};

	// Vertex Shader
	vshader = IVertexShader::Factory::Create(L"VertexShader.hlsl", input);
	vshader->Bind();

	// Pixel Shader
	pshader = IPixelShader::Factory::Create(L"PixelShader.hlsl");
	pshader->Bind();

	// Constant Buffer
	cbuffer = IBuffer::Factory::Create(IBuffer::Type::CONSTANT, true, sizeof(CB), nullptr);
	cbuffer->BindToShader(IBuffer::ShaderStage::VERTEX);
	cbuffer->BindToShader(IBuffer::ShaderStage::PIXEL);

	diffuse_texture = ITexture::Factory::Create(L"old-rusty-car/source/rusty.rar/car_d.dds");
	diffuse_texture->Bind(0);

	uv_texture = ITexture::Factory::Create(L"old-rusty-car/source/rusty.rar/car_n.dds");
	uv_texture->Bind(1);

	metallic_texture = ITexture::Factory::Create(L"old-rusty-car/source/rusty.rar/car_m.dds");
	metallic_texture->Bind(2);

	roughness_texture = ITexture::Factory::Create(L"old-rusty-car/source/rusty.rar/car_r.dds");
	roughness_texture->Bind(3);

	ao_texture = ITexture::Factory::Create(L"old-rusty-car/source/rusty.rar/car_ao.dds");
	ao_texture->Bind(4);
}

chaos::Renderer3D::~Renderer3D()
{
	delete gfx;
	delete vshader;
	delete pshader;
	delete vbuffer;
	delete indexbuffer;
	delete cbuffer;

	delete diffuse_texture;
	delete uv_texture;
	delete metallic_texture;
	delete roughness_texture;
	delete ao_texture;
}

void chaos::Renderer3D::SetScene(const Scene* const scene)
{
	if (scene)
	{
		current_scene = scene;

		current_scene->entities[0].mesh->Bind();
		current_scene->entities[0].mesh->Bind();
		indicies = scene->entities[0].mesh->index_count;
	}

}

void chaos::Renderer3D::RenderScene(Scene* pScene)
{
}

void chaos::Renderer3D::BeginFrame()
{

	gfx->ClearScreen(current_scene->mClearColor);

	Transform transform = current_scene->entities[0].worldTransform;

	// World matrix
	glm::mat4 world = glm::mat4(1.0f);
	{
		world = glm::translate(world, transform.position);
		world = glm::rotate(world, glm::radians(transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		world = glm::rotate(world, glm::radians(transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		world = glm::rotate(world, glm::radians(transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		world = glm::scale(world, transform.scale);
	}

	CB cb = {};
	cb.worldMatrix = glm::transpose(world);
	cb.viewMatrix = glm::transpose(current_scene->mCamera.view);
	cb.projectionMatrix = glm::transpose(current_scene->mCamera.projection);
	cb.lightColor = glm::vec4(1, 1.0, 1.0, 0);
	cb.lightDirection = glm::vec4(0, 0, 1, 0);
	cb.ambientLightColor = glm::vec4(1, 0.6, 0.6, 0);
	cb.ambientLightStrength = 0.35f;

	cbuffer->Update(&cb, sizeof(CB));

	gfx->DrawIndex(indicies);
}

void chaos::Renderer3D::EndFrame()
{
	gfx->Present();
}





