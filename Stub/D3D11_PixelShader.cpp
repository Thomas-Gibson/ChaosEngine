#include "D3D11_Graphics.h"

chaos::D3D11::PixelShader::PixelShader(wstring path)
{
	ComPtr<ID3DBlob> pixelBlob, errorBlob;
	D3DCompileFromFile(path, nullptr, nullptr, "main", "ps_5_0", 0u, 0u, pixelBlob.GetAddressOf(), errorBlob.GetAddressOf());

	device->CreatePixelShader(pixelBlob->GetBufferPointer(), pixelBlob->GetBufferSize(), nullptr, pixel_shader.GetAddressOf());
}

void chaos::D3D11::PixelShader::Bind()
{
	context->PSSetShader(pixel_shader.Get(), nullptr, 0u);
}

void chaos::D3D11::PixelShader::UnBind()
{
	context->PSSetShader(nullptr, nullptr, 0u);
}
