#pragma once

#include "Xin.ShaderCompiler.Types.h"

#include <d3d12shader.h>

namespace Xin::ShaderCompiler
{
	void D3D12GenerateReflectParameters(EShaderStage ShaderStage, FReflectParameters & ReflectParameters, ID3D12ShaderReflection * D3D12ShaderReflection);
}

