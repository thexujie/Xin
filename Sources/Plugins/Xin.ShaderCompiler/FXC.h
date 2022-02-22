#pragma once

#include "Xin.ShaderCompiler.Types.h"

namespace Xin::ShaderCompiler
{
	class XIN_SHADERCOMILER_API FFXCShaderCompiler
	{
	public:
		FFXCShaderCompiler();

		bool CompilerFromMemory(FStringV ShaderSource, FShaderCompilerContext & CompilerContext);
	};
}