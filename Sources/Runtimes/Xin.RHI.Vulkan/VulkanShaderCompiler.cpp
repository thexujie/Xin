#include "PCH.h"
#include "VulkanShaderCompiler.h"
#include "VulkanDevice.h"

#include "Xin.ShaderCompiler/Xin.ShaderCompiler.h"

namespace Xin::RHI::Vulkan
{
	class FVulkanShaderCompilerContext : public ShaderCompiler::FShaderCompilerContext
	{
	public:
		FVulkanShaderCompilerContext(ShaderCompiler::FShaderCompilerContext & CompilerContext) : CompilerContext(CompilerContext) { }

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


	FVulkanShaderCompiler::FVulkanShaderCompiler(FVulkanDevice & Device) : Device(Device)
	{
		
	}

	bool FVulkanShaderCompiler::CompilerFromFile(FStringV FilePath, FShaderCompilerContext & CompilerContext)
	{
		return false;
	}

	bool FVulkanShaderCompiler::CompilerFromSource(FStringV ShaderSource, FShaderCompilerContext & CompilerContext)
	{
		FVulkanShaderCompilerContext VulkanCompilerContext(CompilerContext);
		{
			VulkanCompilerContext.FilePath = CompilerContext.FilePath;
			VulkanCompilerContext.ModuleName = CompilerContext.ModuleName;

			VulkanCompilerContext.DeviceRHI = EDeviceRHI::Vulkan;
			VulkanCompilerContext.SourceLanguage = CompilerContext.SourceLanguage;
			VulkanCompilerContext.TargetArchitecture = EShaderArchitecture::SPIRV;

			VulkanCompilerContext.ShaderStage = CompilerContext.ShaderStage;
			VulkanCompilerContext.EntryName = CompilerContext.EntryName;

			VulkanCompilerContext.DebugCompile = CompilerContext.DebugCompile;
			VulkanCompilerContext.OptimizationLevel = CompilerContext.OptimizationLevel;
		}

		ShaderCompiler::FShaderCompiler ShaderCompiler;
		return ShaderCompiler.CompilerFromMemory(ShaderSource, VulkanCompilerContext);
	}
}
