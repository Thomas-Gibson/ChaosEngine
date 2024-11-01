#include "D3D11_Graphics.h"

struct ConstantBuffer
{
	DirectX::XMMATRIX wvp;
};


ComPtr<ID3D11Device> chaos::D3D11::device = nullptr;
ComPtr<ID3D11DeviceContext> chaos::D3D11::context = nullptr;

chaos::D3D11::D3D11(IWindow* window)
	:window(window)
{
	CoInitialize(nullptr);
	CreateDevice();
	CreateSwapChain();
	CreateRenderTargetView();
	CreateShadersAndInputLayout();
	CreateBuffers();
	SetPipeline();
}

chaos::D3D11::~D3D11()
{
	if (device) device.Reset();
	if (context) context.Reset();

	CoUninitialize();
}

void chaos::D3D11::Draw(std::span<Vertex> vertices, std::span<u32> indices, Transform transform)
{
	
	using namespace DirectX;
	D3D11_MAPPED_SUBRESOURCE resource;

	context->Map(vertex_buffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &resource);
	memcpy(resource.pData, vertices.data(), vertices.size_bytes());
	context->Unmap(vertex_buffer.Get(), 0u);

	context->Map(index_buffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &resource);
	memcpy(resource.pData, indices.data(), indices.size_bytes());
	context->Unmap(index_buffer.Get(), 0u);

	// World Matrix
	XMMATRIX world;
	{
		world = XMMatrixTransformation(
			XMVectorSet(0, 0, 0, 0),
			XMVectorSet(0, 0, 0, 0),
			XMVectorSet(transform.scale.x, transform.scale.y, transform.scale.z, 0.0f),
			XMVectorSet(0, 0, 0, 0),
			XMQuaternionRotationRollPitchYaw(
				XMConvertToRadians(transform.rotation.x),
				XMConvertToRadians(transform.rotation.y),
				XMConvertToRadians(transform.rotation.z)),
			XMVectorSet(transform.position.x, transform.position.y, transform.position.z, 0));
	}

	// View Matrix
	XMMATRIX view;
	{
		view = XMMatrixLookAtLH(
			XMVectorSet(0.0f, 0.0f, -100.0f, 0.0f),
			XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
			XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	}
	// Projection Matrix
	XMMATRIX projection;
	{
		projection = XMMatrixOrthographicLH(window->Width(), window->Height(), 0.1f, 1000.0f);
	}

	ConstantBuffer cb;
	cb.wvp = world * view * projection;
	cb.wvp = XMMatrixTranspose(cb.wvp);

	context->Map(constant_buffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &resource);
	memcpy(resource.pData, &cb, sizeof(ConstantBuffer));
	context->Unmap(constant_buffer.Get(), 0u);

	context->DrawIndexed(indices.size(), 0u, 0);

}

HRESULT chaos::D3D11::CreateDevice()
{
	u32 flags = 0;

	#ifdef _DEBUG
	flags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif // _DEBUG

	D3D_FEATURE_LEVEL fl[] = { D3D_FEATURE_LEVEL_11_0 };

	return
		D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			flags,
			fl,
			ARRAYSIZE(fl),
			D3D11_SDK_VERSION,
			device.GetAddressOf(),
			nullptr,
			context.GetAddressOf());
}

HRESULT chaos::D3D11::CreateSwapChain()
{
	DXGI_SWAP_CHAIN_DESC swap_chain_desc = {};

	swap_chain_desc.BufferDesc.Width = window->Width();
	swap_chain_desc.BufferDesc.Height = window->Height();
	swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60;
	swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
	swap_chain_desc.BufferDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	swap_chain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swap_chain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING::DXGI_MODE_SCALING_UNSPECIFIED;
	swap_chain_desc.SampleDesc.Count = 1;
	swap_chain_desc.SampleDesc.Quality = 0;
	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_desc.BufferCount = 2;
	swap_chain_desc.OutputWindow = static_cast<HWND>(window->Handle());
	swap_chain_desc.Windowed = true;
	swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD;
	swap_chain_desc.Flags = 0;

	// Create IDXGI factory
	ComPtr<IDXGIFactory> factory;
	CreateDXGIFactory(_uuidof(IDXGIFactory), reinterpret_cast<void**>(factory.GetAddressOf()));

	return factory->CreateSwapChain(device.Get(), &swap_chain_desc, swap_chain.GetAddressOf());
}

HRESULT chaos::D3D11::CreateRenderTargetView()
{
	ComPtr<ID3D11Texture2D> render_target;
	swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(render_target.GetAddressOf()));

	return device->CreateRenderTargetView(render_target.Get(), nullptr, render_target_view.GetAddressOf());
}

inline HRESULT chaos::D3D11::CreateShadersAndInputLayout()
{
	ComPtr<ID3DBlob> error_blob = nullptr;

	// Create Shaders and Input Layout
	{
		ComPtr<ID3DBlob> errorBlob;

		// Vertex Shader
		{
			ComPtr<ID3DBlob> vertexBlob;
			D3DCompileFromFile(TEXT("VertexShader.hlsl"), nullptr, nullptr, "main", "vs_5_0", 0u, 0u, vertexBlob.GetAddressOf(), errorBlob.GetAddressOf());

			device->CreateVertexShader(vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize(), nullptr, vertex_shader.GetAddressOf());

			// Input Layout
			{

				/*
					LPCSTR SemanticName;
					UINT SemanticIndex;
					DXGI_FORMAT Format;
					UINT InputSlot;
					UINT AlignedByteOffset;
					D3D11_INPUT_CLASSIFICATION InputSlotClass;
					UINT InstanceDataStepRate;
				*/
				D3D11_INPUT_ELEMENT_DESC desc[] =
				{
					{"POSITION", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u},
				};

				device->CreateInputLayout(desc, ARRAYSIZE(desc), vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize(), input_layout.GetAddressOf());
			}

		}

		// Pixel Shader
		{
			ComPtr<ID3DBlob> pixelBlob;
			D3DCompileFromFile(TEXT("PixelShader.hlsl"), nullptr, nullptr, "main", "ps_5_0", 0u, 0u, pixelBlob.GetAddressOf(), errorBlob.GetAddressOf());

			device->CreatePixelShader(pixelBlob->GetBufferPointer(), pixelBlob->GetBufferSize(), nullptr, pixel_shader.GetAddressOf());
		}
	}

	return 0;
}

inline HRESULT chaos::D3D11::CreateBuffers()
{
	D3D11_BUFFER_DESC desc;

	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0u;
	desc.StructureByteStride = 0u;

	// Vertex Buffer
	{
		desc.ByteWidth = sizeof(Vertex) * 10'000;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		device->CreateBuffer(&desc, nullptr, vertex_buffer.GetAddressOf());
	}

	// Index Buffer
	{
		desc.ByteWidth = sizeof(u32) * 10'000;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		device->CreateBuffer(&desc, nullptr, index_buffer.GetAddressOf());
	}

	// Constant Buffer
	{
		desc.ByteWidth = sizeof(ConstantBuffer) * 10'000;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		device->CreateBuffer(&desc, nullptr, constant_buffer.GetAddressOf());
	}

	return 0;
}

inline HRESULT chaos::D3D11::SetPipeline()
{
	// Input Assembler Stage
	{
		u32 stride = sizeof(Vertex);
		u32 offset = 0u;
		context->IASetVertexBuffers(0u, 1u, vertex_buffer.GetAddressOf(), &stride, &offset);
		context->IASetIndexBuffer(index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0u);
		context->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		context->IASetInputLayout(input_layout.Get());
	}

	// Vertex Shader Stage
	{
		context->VSSetShader(vertex_shader.Get(), nullptr, 0u);
		context->VSSetConstantBuffers(0u, 1u, constant_buffer.GetAddressOf());
	}

	// Rasterizer Stage
	{

		D3D11_VIEWPORT viewport;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		viewport.Width = static_cast<r32>(window->Width());
		viewport.Height = static_cast<r32>(window->Height());
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		context->RSSetViewports(1u, &viewport);
	}

	// Pixel Shader Stage
	{
		context->PSSetShader(pixel_shader.Get(), nullptr, 0u);
		context->PSSetConstantBuffers(0u, 1u, constant_buffer.GetAddressOf());
	}

	// Output-Merger Stage
	{
		context->OMSetRenderTargets(1u, render_target_view.GetAddressOf(), nullptr);
	}

	return 0;
}

void chaos::D3D11::ClearScreen(Color color) const
{
	FLOAT clear_color[4] = { (r32)color.r / 255, (r32)color.g / 255, (r32)color.b / 255, 0 };
	context->ClearRenderTargetView(render_target_view.Get(), clear_color);
}

void chaos::D3D11::Present() const
{
	swap_chain->Present(1u, 0u);
}


