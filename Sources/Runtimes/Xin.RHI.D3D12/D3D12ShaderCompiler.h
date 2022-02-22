#pragma once

#include "D3D12Types.h"

namespace Xin::RHI::D3D12
{
	class FD3D12Device;

	class FD3D12ShaderCompiler : public IRHIShaderCompiler
	{
	public:
		FD3D12ShaderCompiler(FD3D12Device & D3D12Devcie);

		bool CompilerFromFile(FStringV FilePath, FShaderCompilerContext & CompilerContext) override;
		bool CompilerFromSource(FStringV ShaderSource, FShaderCompilerContext & CompilerContext) override;

	public:
		FD3D12Device & D3D12Devcie;
	};
}
