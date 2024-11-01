#include "D3D11_Graphics.h"
#include <WICTextureLoader.h>

chaos::D3D11::Texture::Texture(wstring file)
{
	DirectX::CreateWICTextureFromFile(device.Get(), file, nullptr, shader_resource_view.GetAddressOf());
}

void chaos::D3D11::Texture::Bind(u32 slot)
{
	this->slot = slot;
	context->PSSetShaderResources(slot, 1, shader_resource_view.GetAddressOf());
}

void chaos::D3D11::Texture::UnBind()
{
	context->PSSetShaderResources(0, this->slot, nullptr);
}
