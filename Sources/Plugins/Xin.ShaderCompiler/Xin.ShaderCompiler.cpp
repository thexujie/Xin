#include "PCH.h"
#include "Xin.ShaderCompiler.h"

namespace Xin::ShaderCompiler
{
	bool FShaderCompiler::CompilerFromFile(FStringV FilePath, FShaderCompilerContext & CompilerInput)
	{
		FString FileContent = FFile::ReadString(FilePath);
		return CompilerFromMemory(FileContent, CompilerInput);
	}

	bool FShaderCompiler::CompilerFromMemory(FStringV ShaderSource, FShaderCompilerContext & CompilerContext)
	{
		if (CompilerContext.SourceLanguage == EShaderLanguage::HLSL)
		{
			switch (CompilerContext.TargetArchitecture)
			{
			default:
			case EShaderArchitecture::DXIL:
			case EShaderArchitecture::SPIRV:
			{
				FDXCShaderCompiler DXCShaderCompiler;
				return DXCShaderCompiler.CompilerFromMemory(ShaderSource, CompilerContext);
			}
			case EShaderArchitecture::DXBC:
			{
				FFXCShaderCompiler FXCShaderCompiler;
				return FXCShaderCompiler.CompilerFromMemory(ShaderSource, CompilerContext);
			}
			}
		}
		else
		{
			return false;
		}
	}
}
