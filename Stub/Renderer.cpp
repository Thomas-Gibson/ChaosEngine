#include "Renderer.h"
/*
chaos::math::Vector3 velocities[255]{};
chaos::Transform localTransforms[256]{};
chaos::Transform worldTransforms[256]{};

chaos::Camera camera;

int width = 16;
int height = 16;

chaos::Vertex v[] =
{
	{ width, -height, 0.0f},
	{-width, -height, 0.0f},
	{-width,  height, 0.0f},
	{ width,  height, 0.0f},
};

u32 i[] =
{
	0,1,2,
	2,3,0
};

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
		using namespace chaos::math;

		localTransforms[id].position += velocities[id];

		if (parent >= 0)
		{
			worldTransforms[parent].rotation.z += localTransforms[parent].rotation.z;

			Matrix4x4 parentRotationMatrixX = Matrix4x4::XRotation((3.14159 / 180.0f) * worldTransforms[parent].rotation.x);
			Matrix4x4 parentRotationMatrixZ = Matrix4x4::YRotation((3.14159 / 180.0f) * worldTransforms[parent].rotation.y);
			Matrix4x4 parentRotationMatrixY = Matrix4x4::ZRotation((3.14159 / 180.0f) * worldTransforms[parent].rotation.z);

			Matrix4x4 parentRotationMatrix = parentRotationMatrixX * parentRotationMatrixY * parentRotationMatrixZ;

			Vector3 childLocalPosition = localTransforms[id].position;

			Vector3 childRotatedPosition = parentRotationMatrix * childLocalPosition;

			worldTransforms[id].position = worldTransforms[parent].position + childRotatedPosition;

			worldTransforms[id].rotation = worldTransforms[parent].rotation + localTransforms[id].rotation;
		}
		else
		{
			worldTransforms[id] = localTransforms[id];
		}
	}
};

Node parentNode = Node(0);
Node childNode = Node(1);
Node childNode2 = Node(2);
*/

chaos::IShader* vshader;
chaos::IShader* pshader;

chaos::Renderer3D::Renderer3D(IWindow* window)
	: gfx(IGraphics::Factory::Create(window))
{
	vshader = IShader::Factory::Create(IShader::ShaderType::Vertex, L"VertexShader.hlsl");
	pshader = IShader::Factory::Create(IShader::ShaderType::Pixel, L"PixelShader.hlsl");
}

chaos::Renderer3D::~Renderer3D()
{
	gfx->~IGraphics();

	delete vshader;
	delete pshader;
}

void chaos::Renderer3D::Update()
{
	gfx->ClearScreen({ 51, 102, 204 });

	//parentNode.Update();
	//childNode.Update();
	//childNode2.Update();
	//localTransforms[0].rotation.z -= 0.5f;
	//localTransforms[1].rotation.z -= 2.0f;
	//localTransforms[0].position.x -= 0.5f;

	gfx->Present();
}
