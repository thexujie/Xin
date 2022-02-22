#pragma once

#include "VulkanTypes.h"

namespace Xin::RHI::Vulkan
{
	class FVulkanDevice;

	class FVulkanFence : public IRHIFence
	{
	public:
		FVulkanFence(FVulkanDevice & Device);
		~FVulkanFence();

		void SetDebugName(FStringV ResourceLabel) override;

	public:
		void WaitFence(uint64 Value) override;
		uint64 GetCompletedValue() const override;

	public:
		FVulkanDevice & Device;

		VkSemaphore vkSemaphore { nullptr };
		//IVulkanFenceRef InnerFence;
		HANDLE hEvent = NULL;
	};
}
