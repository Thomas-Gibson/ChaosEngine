#pragma once

#include "chaos_math.h"
#include "IWindow.h"
#include <span>

namespace chaos
{
	struct Vertex
	{
		math::Vector3 position;
		math::Vector3 normal;
		math::Vector3 texture_coordinate;
	};

	struct Transform
	{
		chaos::math::Vector3 position;
		chaos::math::Vector3 rotation;
		chaos::math::Vector3 scale = { 1.0f,1.0f,1.0f };
	};

	struct Camera
	{
		Transform transform;
	};

	class CHAOS_API IGraphics
	{
	private:

	public:
		virtual ~IGraphics()
		{
			int x = 0;
		}

		virtual void ClearScreen(Color color) const = 0;
		virtual void Present() const = 0;


		class CHAOS_API Factory
		{
		public:
			static IGraphics* Create(IWindow* window);
		};
	};

	class Bindable
	{
		virtual void Bind() = 0;
		virtual void UnBind() = 0;
	};


	class CHAOS_API IVertexShader : public Bindable
	{
	public:

		virtual ~IVertexShader() = default;

		class CHAOS_API Factory
		{
		public:
			static IVertexShader* Create(wstring path);
		};
	};

	class CHAOS_API IPixelShader : public Bindable
	{
	public:

		virtual ~IPixelShader() = default;

		class CHAOS_API Factory
		{
		public:
			static IPixelShader* Create(wstring path);
		};
	};

	template <int num_input>
	class CHAOS_API IShaderInput
	{
	private:

	public:

		class 
	};

}

