#include "IGraphics.h"
#include "D3D11_Graphics.h"


namespace chaos
{

	IGraphics* IGraphics::Factory::Create(IWindow* window)
	{
		static D3D11 d3d11 = D3D11(window);

		return &d3d11;
	}

	IVertexShader* chaos::IVertexShader::Factory::Create(wstring path)
	{
		return new D3D11::VertexShader(path);
	}

	IPixelShader* chaos::IPixelShader::Factory::Create(wstring path)
	{
		return new D3D11::PixelShader(path);
	}

}