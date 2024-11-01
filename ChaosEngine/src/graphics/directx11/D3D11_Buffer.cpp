#include "D3D11_Graphics.h"

chaos::D3D11::Buffer::Buffer(D3D11_BUFFER_DESC* desc, D3D11_SUBRESOURCE_DATA* data, Type type)
	:type(type)
{
	device->CreateBuffer(desc, data, buffer.GetAddressOf());
}

void chaos::D3D11::Buffer::Bind(size_t size)
{
	switch (type)
	{
		case Type::VERTEX:
		{
			UINT stride = size;
			UINT offset = 0;
			context->IASetVertexBuffers(0, 1, buffer.GetAddressOf(), &stride, &offset);
		} break;
		case Type::INDEX:
		{
			context->IASetIndexBuffer(buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		} break;
		case Type::CONSTANT:
		{
			context->VSSetConstantBuffers(0u, 1u, buffer.GetAddressOf());
			context->PSSetConstantBuffers(0u, 1u, buffer.GetAddressOf());
		} break;
	}
}

void chaos::D3D11::Buffer::UnBind()
{
	switch (type)
	{
		case Type::VERTEX:
		{
			context->IASetVertexBuffers(0, 1, nullptr, nullptr, nullptr);
		} break;
		case Type::INDEX:
		{
			context->IASetIndexBuffer(nullptr, DXGI_FORMAT_R32_UINT, 0);
		} break;
		case Type::CONSTANT:
		{
			context->VSSetConstantBuffers(0u, 1u, nullptr);
			context->PSSetConstantBuffers(0u, 1u, nullptr);
		} break;
	}
}

void chaos::D3D11::Buffer::BindToShader(ShaderStage stage)
{
	if (type != chaos::IBuffer::Type::CONSTANT)
		return;

	switch (stage)
	{
		case ShaderStage::VERTEX:
			context->VSSetConstantBuffers(0u, 1u, buffer.GetAddressOf());
			break;
		case ShaderStage::PIXEL:
			context->PSSetConstantBuffers(0u, 1u, buffer.GetAddressOf());
			break;
		case ShaderStage::GEOMETRY:
			context->GSSetConstantBuffers(0u, 1u, buffer.GetAddressOf());
			break;
		case ShaderStage::COMPUTE:
			context->CSSetConstantBuffers(0u, 1u, buffer.GetAddressOf());
			break;
		default:
			throw std::runtime_error("Unsupported ShaderStage!");
	}
}

void chaos::D3D11::Buffer::Update(void* data, size_t size)
{
	D3D11_MAPPED_SUBRESOURCE sub_resource = {};

	context->Map(buffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &sub_resource);
	memcpy(sub_resource.pData, data, size);
	context->Unmap(buffer.Get(), 0u);


}
