#include "IGraphics.h"

namespace chaos
{


IGraphics::IShader* IGraphics::IShader::Factory::Create(ShaderType type, wstring path)
{
    return nullptr;
}

}