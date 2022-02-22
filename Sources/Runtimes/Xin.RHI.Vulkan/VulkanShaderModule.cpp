#include "PCH.h"
#include "VulkanShaderModule.h"

#include "VulkanDevice.h"

namespace Xin::RHI::Vulkan
{
	FVulkanShaderModule::FVulkanShaderModule(FVulkanDevice & Device, IShaderInstance & ShaderInstance)
		: Device(Device), IRHIShaderModule(ShaderInstance)
	{
		TView<byte> ShaderCode = ShaderInstance.GetShaderCode();

		VkShaderModuleCreateInfo ShaderModuleCreateInfo { VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO };
		ShaderModuleCreateInfo.codeSize = ShaderCode.Size;
		ShaderModuleCreateInfo.pCode = (const uint32 *)ShaderCode.Data;
		VkResult Result = vkCreateShaderModule(Device.vkDevice, &ShaderModuleCreateInfo, nullptr, &vkShaderModule);
	}

	FVulkanShaderModule::~FVulkanShaderModule()
	{
		if (vkShaderModule)
		{
			vkDestroyShaderModule(Device.vkDevice, vkShaderModule, nullptr);
			vkShaderModule = nullptr;
		}
	}
}
