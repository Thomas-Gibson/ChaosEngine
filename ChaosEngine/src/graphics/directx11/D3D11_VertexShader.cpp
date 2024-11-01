#include "D3D11_Graphics.h"

chaos::D3D11::VertexShader::VertexShader(wstring path, D3D11_INPUT_ELEMENT_DESC* desc, u32 num_elements)
{
	ComPtr<ID3DBlob> vertexBlob, errorBlob;
	D3DCompileFromFile(path, nullptr, nullptr, "main", "vs_5_0", 0u, 0u, vertexBlob.GetAddressOf(), errorBlob.GetAddressOf());

	device->CreateVertexShader(vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize(), nullptr, vertex_shader.GetAddressOf());

	device->CreateInputLayout(desc, num_elements, vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize(), input_layout.GetAddressOf());
}

void chaos::D3D11::VertexShader::Bind()
{
	context->VSSetShader(vertex_shader.Get(), nullptr, 0u);
	context->IASetInputLayout(input_layout.Get());
}

void chaos::D3D11::VertexShader::UnBind()
{
	context->VSSetShader(nullptr, nullptr, 0u);

}
