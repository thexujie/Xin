#include "PCH.h"
#include "D3D12ShaderCompiler.h"
#include "D3D12Device.h"

#include "Xin.ShaderCompiler/Xin.ShaderCompiler.h"

namespace Xin::RHI::D3D12
{
	class FD3D12ShaderCompilerContext : public ShaderCompiler::FShaderCompilerContext
	{
	public:
		FD3D12ShaderCompilerContext(ShaderCompiler::FShaderCompilerContext & CompilerContext) : CompilerContext(CompilerContext) { }

	public:
		bool IncludeFile(FStringV FilePath, TList<byte> & FileContent) override
		{
			return CompilerContext.IncludeFile(FilePath, FileContent);
		}

		void OnProcessed(FStringV ProcessedSource) override
		{
			return CompilerContext.OnProcessed(ProcessedSource);
		}

		void OnCompiled(TView<byte> ShaderCode, TView<byte> ShaderPDB, const FReflectParameters & ReflectParameters) override
		{
			return CompilerContext.OnCompiled(ShaderCode, ShaderPDB, ReflectParameters);
		}

	public:
		ShaderCompiler::FShaderCompilerContext & CompilerContext;
	};


	FD3D12ShaderCompiler::FD3D12ShaderCompiler(FD3D12Device & D3D12Devcie) : D3D12Devcie(D3D12Devcie)
	{
		
	}

	bool FD3D12ShaderCompiler::CompilerFromFile(FStringV FilePath, FShaderCompilerContext & CompilerContext)
	{
		return false;
	}

	bool FD3D12ShaderCompiler::CompilerFromSource(FStringV ShaderSource, FShaderCompilerContext & CompilerContext)
	{
		FD3D12ShaderCompilerContext D3D12CompilerContext(CompilerContext);
		{
			D3D12CompilerContext.FilePath = CompilerContext.FilePath;
			D3D12CompilerContext.ModuleName = CompilerContext.ModuleName;

			D3D12CompilerContext.DeviceRHI = EDeviceRHI::D3D12;
			D3D12CompilerContext.SourceLanguage = CompilerContext.SourceLanguage;
			D3D12CompilerContext.TargetArchitecture = EShaderArchitecture::DXBC;

			D3D12CompilerContext.ShaderStage = CompilerContext.ShaderStage;
			D3D12CompilerContext.EntryName = CompilerContext.EntryName;

			D3D12CompilerContext.DebugCompile = CompilerContext.DebugCompile;
			D3D12CompilerContext.OptimizationLevel = CompilerContext.OptimizationLevel;
		}

		ShaderCompiler::FShaderCompiler ShaderCompiler;
		return ShaderCompiler.CompilerFromMemory(ShaderSource, D3D12CompilerContext);
	}
}
