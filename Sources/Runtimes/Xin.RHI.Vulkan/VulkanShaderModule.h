#pragma once

#include "VulkanTypes.h"

namespace Xin::RHI::Vulkan
{
	class FVulkanDevice;

	class FVulkanShaderModule : public IRHIShaderModule
	{
	public:
		FVulkanShaderModule(FVulkanDevice & Device, IShaderInstance & ShaderInstance);
		~FVulkanShaderModule();

	public:
		FVulkanDevice & Device;
		VkShaderModule vkShaderModule = nullptr;
	};
}
