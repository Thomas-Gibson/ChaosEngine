#include "IGraphics.h"
#include "directx11/D3D11_Graphics.h"
#include <array>

namespace chaos
{

	IGraphics* IGraphics::Factory::Create(IWindow* window)
	{
		return new D3D11(window);
	}

	IVertexShader* chaos::IVertexShader::Factory::Create(wstring path, std::span<Input> input_layout)
	{
		if (input_layout.size() > 10)
			throw std::exception("The input layout has too many inputs");

		std::array<D3D11_INPUT_ELEMENT_DESC, 10> desc = {};

		u32 i = 0;
		for (Input I : input_layout)
		{
			DXGI_FORMAT dxFormat = {};

			switch (I.data_format)
			{
				case chaos::IVertexShader::Format::FLOAT_1:
					dxFormat = DXGI_FORMAT_R32_FLOAT;
					break;
				case chaos::IVertexShader::Format::FLOAT_2:
					dxFormat = DXGI_FORMAT_R32G32_FLOAT;
					break;
				case chaos::IVertexShader::Format::FLOAT_3:
					dxFormat = DXGI_FORMAT_R32G32B32_FLOAT;
					break;
				case chaos::IVertexShader::Format::FLOAT_4:
					dxFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
					break;
				case chaos::IVertexShader::Format::UNSIGNED_INTEGER_1:
					dxFormat = DXGI_FORMAT_R32_UINT;
					break;
				case chaos::IVertexShader::Format::UNSIGNED_INTEGER_2:
					dxFormat = DXGI_FORMAT_R32G32_UINT;
					break;
				case chaos::IVertexShader::Format::UNSIGNED_INTEGER_3:
					dxFormat = DXGI_FORMAT_R32G32B32_UINT;
					break;
				case chaos::IVertexShader::Format::UNSIGNED_INTEGER_4:
					dxFormat = DXGI_FORMAT_R32G32B32A32_UINT;
					break;
				case chaos::IVertexShader::Format::SIGNED_INTEGER_1:
					dxFormat = DXGI_FORMAT_R32_SINT;
					break;
				case chaos::IVertexShader::Format::SIGNED_INTEGER_2:
					dxFormat = DXGI_FORMAT_R32G32_SINT;
					break;
				case chaos::IVertexShader::Format::SIGNED_INTEGER_3:
					dxFormat = DXGI_FORMAT_R32G32B32_SINT;
					break;
				case chaos::IVertexShader::Format::SIGNED_INTEGER_4:
					dxFormat = DXGI_FORMAT_R32G32B32A32_SINT;
					break;
				default:
					throw std::runtime_error("Unsupported format");
					break;
			}

			D3D11_INPUT_ELEMENT_DESC d3d11_input_element_desc =
			{
				I.semantic_name, 0u, dxFormat, 0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0u
			};

			desc.at(i) = d3d11_input_element_desc;


			i++;
		}



		return new D3D11::VertexShader(path, desc.data(), input_layout.size());
	}

	IPixelShader* chaos::IPixelShader::Factory::Create(wstring path)
	{
		return new D3D11::PixelShader(path);
	}

	IBuffer* chaos::IBuffer::Factory::Create(IBuffer::Type type, bool cpu_write, size_t size_bytes, void* data)
	{
		#ifdef _WIN32

		D3D11_BUFFER_DESC desc = {};
		desc.MiscFlags = 0u;
		desc.StructureByteStride = 0u;
		desc.Usage = (cpu_write) ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
		desc.CPUAccessFlags = (cpu_write) ? D3D11_CPU_ACCESS_WRITE : 0;
		desc.ByteWidth = size_bytes;

		switch (type)
		{
			case chaos::IBuffer::Type::VERTEX:
				desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
				break;
			case chaos::IBuffer::Type::INDEX:
				desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
				break;
			case chaos::IBuffer::Type::CONSTANT:
				desc.ByteWidth = size_bytes + ((16 - size_bytes) % 16);
				desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
				break;
			default:
				throw std::runtime_error("Unsupported Type!");
				break;
		}

		D3D11_SUBRESOURCE_DATA subresource = {};
		subresource.pSysMem = data;

		return new D3D11::Buffer(&desc, (cpu_write) ? nullptr : &subresource, type);

		#endif // _WIN32
	}

	ITexture* chaos::ITexture::Factory::Create(wstring path)
	{
		return new D3D11::Texture(path);
	}
}