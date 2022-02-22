#pragma once

#include "Xin.ShaderCompiler.Types.h"

#include "DXC.h"
#include "FXC.h"
#include "SPIRVReflect.h"

namespace Xin::ShaderCompiler
{
	class XIN_SHADERCOMILER_API FShaderCompiler
	{
	public:
		FShaderCompiler() = default;

		bool CompilerFromFile(FStringV FilePath, FShaderCompilerContext & CompilerInput);
		bool CompilerFromMemory(FStringV ShaderSource, FShaderCompilerContext & CompilerContext);
	};
}