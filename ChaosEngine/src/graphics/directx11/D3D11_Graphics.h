#pragma once

#include "graphics/IGraphics.h"

#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi1_3.h>
#include <dxgidebug.h>
#include <DirectXMath.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxgi.lib")

#include <wrl/client.h>
using namespace Microsoft::WRL;

#include <span>

namespace chaos
{
	class D3D11 : public IGraphics
	{
	private:
		static ComPtr<ID3D11Device> device;
		static ComPtr<ID3D11DeviceContext> context;
		ComPtr<IDXGISwapChain> swap_chain;
		ComPtr<ID3D11RenderTargetView> render_target_view;
		ComPtr<ID3D11DepthStencilView> depth_stencil_view;

		ComPtr<ID3D11RasterizerState> rasterizer_state;
		ComPtr<ID3D11DepthStencilState> depth_stencil_state;
		ComPtr<ID3D11SamplerState> sampler_state;

		ComPtr<ID3D11Buffer> vertex_buffer, index_buffer, constant_buffer;

		IWindow* window;
	public:
		D3D11() = default;
		D3D11(IWindow* window);
		~D3D11();

		void Draw(size_t vertex_count) const override;
		void DrawIndex(size_t index_count) const override;


		class VertexShader;
		class PixelShader;
		class Buffer;
		class Texture;

	private:
		inline HRESULT CreateDevice();
		inline HRESULT CreateSwapChain();
		inline HRESULT CreateRenderTargetView();
		inline HRESULT CreateDepthStencilView();
		inline HRESULT CreateRasterizerState();
		inline HRESULT CreateDepthStencilState();
		inline HRESULT CreateSamplerState();
		inline HRESULT SetPipeline();

	public:
		// Inherited via IGraphics
		void ClearScreen(Color color) const;
		void Present() const;
	};


	class D3D11::VertexShader : public IVertexShader
	{
	private:
		ComPtr<ID3D11VertexShader> vertex_shader;
		ComPtr<ID3D11InputLayout> input_layout;
	public:
		VertexShader(wstring path, D3D11_INPUT_ELEMENT_DESC* desc ,u32 num_elements);

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

	class D3D11::Buffer : public IBuffer
	{
	private:
		ComPtr<ID3D11Buffer> buffer;
		Type type;
	public:
		Buffer(D3D11_BUFFER_DESC* desc, D3D11_SUBRESOURCE_DATA* data, Type type);

		// Inherited via IBuffer
		void Bind(size_t size) override;
		void UnBind() override;
		void BindToShader(ShaderStage stage) override;
		virtual void Update(void* data, size_t size) override;
	};

	class D3D11::Texture : public ITexture
	{
	private:
		ComPtr<ID3D11ShaderResourceView> shader_resource_view;
		u32 slot;
	public:
		Texture(wstring file);

		// Inherited via ITexture
		void Bind(u32 slot) override;

		void UnBind() override;

	};
}