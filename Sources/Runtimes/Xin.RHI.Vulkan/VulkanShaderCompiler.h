#pragma once

#include "VulkanTypes.h"

namespace Xin::RHI::Vulkan
{
	class FVulkanDevice;

	class FVulkanShaderCompiler : public IRHIShaderCompiler
	{
	public:
		FVulkanShaderCompiler(FVulkanDevice & Device);

		bool CompilerFromFile(FStringV FilePath, FShaderCompilerContext & CompilerContext) override;
		bool CompilerFromSource(FStringV ShaderSource, FShaderCompilerContext & CompilerContext) override;

	public:
		FVulkanDevice & Device;
	};
}
