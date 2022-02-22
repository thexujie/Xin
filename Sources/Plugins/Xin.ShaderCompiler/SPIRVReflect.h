#pragma once

#include "Xin.ShaderCompiler.Types.h"

namespace Xin::ShaderCompiler
{
	class XIN_SHADERCOMILER_API FSPIRVReflect
	{
	public:
		FSPIRVReflect() = default;

		bool Reflect(TView<byte> ShaderCode, FReflectParameters & ReflectParameters);
	};
}