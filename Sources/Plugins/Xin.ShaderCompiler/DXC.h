#pragma once

#include "Xin.ShaderCompiler.Types.h"

namespace Xin::ShaderCompiler
{
	class XIN_SHADERCOMILER_API FDXCShaderCompiler
	{
	public:
		FDXCShaderCompiler();

		bool CompilerFromMemory(FStringV ShaderSource, FShaderCompilerContext & CompilerContext);
	};
}