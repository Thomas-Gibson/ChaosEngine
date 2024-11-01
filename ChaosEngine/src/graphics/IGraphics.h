#pragma once

#include "window/IWindow.h"
#include <span>
#include <stdexcept>

namespace chaos
{
	class IGraphics : Diagnostic<IGraphics>
	{
	private:

	public:
		virtual ~IGraphics() = default;

		virtual void ClearScreen(Color color) const = 0;
		virtual void Present() const = 0;
		virtual void Draw(size_t vertex_count) const = 0;
		virtual void DrawIndex(size_t index_count) const = 0;


		class Factory
		{
		public:
			static IGraphics* Create(IWindow* window);
		};
	};

	class IVertexShader : Diagnostic<IVertexShader>
	{
	public:

		virtual ~IVertexShader() = default;
		virtual void Bind() = 0;
		virtual void UnBind() = 0;

		enum class Format
		{
			FLOAT_1,
			FLOAT_2,
			FLOAT_3,
			FLOAT_4,
			UNSIGNED_INTEGER_1,
			UNSIGNED_INTEGER_2,
			UNSIGNED_INTEGER_3,
			UNSIGNED_INTEGER_4,
			SIGNED_INTEGER_1,
			SIGNED_INTEGER_2,
			SIGNED_INTEGER_3,
			SIGNED_INTEGER_4,
		};

		struct Input
		{
			lstring semantic_name;
			Format data_format;
		};

		class Factory
		{
		public:
			static IVertexShader* Create(wstring path, std::span<Input> input_layout);
		};
	};

	class IPixelShader : Diagnostic<IPixelShader>
	{
	public:

		virtual ~IPixelShader() = default;

		virtual void Bind() = 0;
		virtual void UnBind() = 0;

		class  Factory
		{
		public:
			static IPixelShader* Create(wstring path);
		};
	};

	class CHAOS_API IBuffer : Diagnostic<IBuffer>
	{
	public:
		virtual ~IBuffer() = default;

		enum class Type
		{
			VERTEX,
			INDEX,
			CONSTANT
		};

		enum class ShaderStage
		{
			VERTEX,
			PIXEL,
			GEOMETRY,
			COMPUTE
		};

		virtual void Bind(size_t size = 0) = 0;
		virtual void UnBind() = 0;

		virtual void BindToShader(ShaderStage stage) = 0;
		virtual void Update(void* data, size_t size) = 0;

		class Factory
		{
		public:
			static IBuffer* Create(IBuffer::Type type, bool cpu_write, size_t size_bytes, void* data);
		};
	};

	class ITexture
	{
	public:
		virtual ~ITexture() = default;

		virtual void Bind(u32 slot) = 0;
		virtual void UnBind() = 0;

		class Factory
		{
		public:
			static ITexture* Create(wstring path);
		};
	};
}

