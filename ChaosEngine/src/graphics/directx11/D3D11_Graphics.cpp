#include "D3D11_Graphics.h"

#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"

struct ConstantBufferrr
{
	DirectX::XMMATRIX wvp;
};

ComPtr<ID3D11Device> chaos::D3D11::device = nullptr;
ComPtr<ID3D11DeviceContext> chaos::D3D11::context = nullptr;

chaos::D3D11::D3D11(IWindow* window)
	:window(window)
{
	CreateDevice();
	CreateSwapChain();
	CreateRenderTargetView();
	CreateDepthStencilView();
	CreateSamplerState();
	SetPipeline();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init(window->Handle());
	ImGui_ImplDX11_Init(device.Get(), context.Get());
	ImGui::StyleColorsDark();

}

chaos::D3D11::~D3D11()
{
	if (device) device.Reset();
	if (context) context.Reset();

	ImGui_ImplWin32_Shutdown();
	ImGui_ImplDX11_Shutdown();
	ImGui::DestroyContext();

	CoUninitialize();
}

void chaos::D3D11::Draw(size_t vertex_count) const
{
	context->Draw(vertex_count, 0u);
}

void chaos::D3D11::DrawIndex(size_t index_count) const
{
	context->DrawIndexed(index_count, 0u, 0);
}

HRESULT chaos::D3D11::CreateDevice()
{
	u32 createDeviceFlag = 0;

	#ifdef _DEBUG
	createDeviceFlag |= D3D11_CREATE_DEVICE_DEBUG;
	#endif

	D3D_FEATURE_LEVEL fl[] = { D3D_FEATURE_LEVEL_11_0 };
	D3D_FEATURE_LEVEL selectedFL;

	return
		D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			createDeviceFlag,
			fl,
			ARRAYSIZE(fl),
			D3D11_SDK_VERSION,
			device.GetAddressOf(),
			&selectedFL,
			context.GetAddressOf());
}

HRESULT chaos::D3D11::CreateSwapChain()
{
	ComPtr<IDXGIFactory>factory;
	CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(factory.GetAddressOf()));

	DXGI_SWAP_CHAIN_DESC desc = {};
	desc.BufferDesc.Width = 0;
	desc.BufferDesc.Height = 0;
	desc.BufferDesc.RefreshRate = { 60, 1 };
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.BufferCount = 2;
	desc.OutputWindow = static_cast<HWND>(window->Handle());
	desc.Windowed = true;
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	desc.Flags = NULL;

	return factory->CreateSwapChain(device.Get(), &desc, swap_chain.GetAddressOf());
}

HRESULT chaos::D3D11::CreateRenderTargetView()
{
	ComPtr<ID3D11Texture2D> render_target;
	swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(render_target.GetAddressOf()));

	return device->CreateRenderTargetView(render_target.Get(), nullptr, render_target_view.GetAddressOf());
}

inline HRESULT chaos::D3D11::CreateDepthStencilView()
{
	ComPtr<ID3D11Texture2D> depth_stencil_texture;

	D3D11_TEXTURE2D_DESC texture_desc = {};
	texture_desc.Width = window->Width();
	texture_desc.Height = window->Height();
	texture_desc.MipLevels = 1;
	texture_desc.ArraySize = 1;
	texture_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	texture_desc.SampleDesc.Count = 1;
	texture_desc.SampleDesc.Quality = 0;
	texture_desc.Usage = D3D11_USAGE_DEFAULT;
	texture_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	texture_desc.CPUAccessFlags = 0u;
	texture_desc.MiscFlags = 0u;

	device->CreateTexture2D(&texture_desc, nullptr, depth_stencil_texture.GetAddressOf());

	D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_desc = {};

	depth_stencil_desc.Format = texture_desc.Format;
	depth_stencil_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depth_stencil_desc.Flags = 0u;
	depth_stencil_desc.Texture2D.MipSlice = 0u;

	return
		device->CreateDepthStencilView(depth_stencil_texture.Get(), &depth_stencil_desc, depth_stencil_view.GetAddressOf());
}
inline HRESULT chaos::D3D11::CreateRasterizerState()
{
	return E_NOTIMPL;
}
inline HRESULT chaos::D3D11::CreateDepthStencilState()
{
	return E_NOTIMPL;
}
inline HRESULT chaos::D3D11::CreateSamplerState()
{
	D3D11_SAMPLER_DESC sampler_desc = {};
	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.MipLODBias = 0.0f;
	sampler_desc.MaxAnisotropy = 0u;
	sampler_desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampler_desc.BorderColor[0] = 0.0f;
	sampler_desc.BorderColor[1] = 0.0f;
	sampler_desc.BorderColor[2] = 0.0f;
	sampler_desc.BorderColor[3] = 0.0f;
	sampler_desc.MinLOD = 0.0f;
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;

	return device->CreateSamplerState(&sampler_desc, sampler_state.GetAddressOf());

}

inline HRESULT chaos::D3D11::SetPipeline()
{
	// Input Assembler Stage
	{
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	// Vertex Shader Stage
	{
		context->VSSetConstantBuffers(0u, 1u, constant_buffer.GetAddressOf());
	}

	// Rasterizer Stage
	{
		D3D11_VIEWPORT viewport = {};
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
		context->PSSetConstantBuffers(0u, 1u, constant_buffer.GetAddressOf());
		context->PSSetSamplers(0, 1u, sampler_state.GetAddressOf());
	}

	// Output-Merger Stage
	{
		context->OMSetRenderTargets(1u, render_target_view.GetAddressOf(), depth_stencil_view.Get());
	}

	return 0;
}

void chaos::D3D11::ClearScreen(Color color) const
{
	FLOAT clear_color[4] = { (r32)color.r / 255, (r32)color.g / 255, (r32)color.b / 255, 0 };
	context->ClearRenderTargetView(render_target_view.Get(), clear_color);
	context->ClearDepthStencilView(depth_stencil_view.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void chaos::D3D11::Present() const
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	swap_chain->Present(1u, 0u);
}


