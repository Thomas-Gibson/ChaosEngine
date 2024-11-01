#pragma once

#include "IGraphics.h"

#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <dxgidebug.h>
#include <DirectXMath.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxgi.lib")

#include <wrl/client.h>
using namespace Microsoft::WRL;

#include "IWindow.h"

#include <span>
#include "chaos_math.h"

namespace chaos
{
	class D3D11 : public IGraphics
	{
	private:
		static ComPtr<ID3D11Device> device;
		static ComPtr<ID3D11DeviceContext> context;
		ComPtr<IDXGISwapChain> swap_chain;
		ComPtr<ID3D11RenderTargetView> render_target_view;
		ComPtr<ID3D11VertexShader> vertex_shader;
		ComPtr<ID3D11PixelShader> pixel_shader;
		ComPtr<ID3D11InputLayout> input_layout;

		ComPtr<ID3D11Buffer> vertex_buffer, index_buffer, constant_buffer;

		IWindow* window;

		UINT num_quality_levels = 0;
		UINT msaa_level = 8;
	public:
		D3D11() = default;
		D3D11(IWindow* window);
		~D3D11();

		void Draw(std::span<Vertex> vertices, std::span<u32> indicies, Transform tranform);


		class VertexShader;
		class PixelShader;

	private:
		inline HRESULT CreateDevice();
		inline HRESULT CreateSwapChain();
		inline HRESULT CreateRenderTargetView();
		inline HRESULT CreateShadersAndInputLayout();
		inline HRESULT CreateBuffers();
		inline HRESULT SetPipeline();

	public:
		// Inherited via IGraphics
		void ClearScreen(Color color) const override;
		void Present() const override;
	};


	class D3D11::VertexShader : public IVertexShader
	{
	private:
		ComPtr<ID3D11VertexShader> vertex_shader;
	public:
		VertexShader(wstring path);

		void Bind() override;
		void UnBind() override;
	};


	class D3D11::PixelShader : public IPixelShader
	{
	private:
		ComPtr<ID3D11PixelShader> pixel_shader;
	public:
		PixelShader(wstring path);

		void Bind() override;
		void UnBind() override;
	};

}