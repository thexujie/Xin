#pragma once

#include "VulkanTypes.h"

namespace Xin::RHI::Vulkan
{
	class RHI_VULKAN_API FVulkanCommandAllocator : public IRHIObject
	{
	public:
		FVulkanCommandAllocator(FVulkanDevice & VulkanDevice);
		~FVulkanCommandAllocator();

		VkCommandBuffer AcquireCommandBuffer();
		void FreeCommandBuffer(VkCommandBuffer vkCommandBuffer)
		{
			vkCommandBuffers.Add(vkCommandBuffer);
		}

		void ResetAllocator();

	public:
		FVulkanDevice & VulkanDevice;
		VkCommandPool vkCommandPool = nullptr;

		TList<VkCommandBuffer> vkCommandBuffers;
		uintx NumCommandBuffers = 0;
	};
	using FVulkanCommandAllocatorRef = TReferPtr<FVulkanCommandAllocator>;
}
