#include "PCH.h"
#include "VulkanCommandAllocator.h"

#include "VulkanDevice.h"
#include "VulkanResource.h"
#include "VulkanCommandQueue.h"
#include "VulkanHeap.h"
#include "VulkanResourceView.h"

namespace Xin::RHI::Vulkan
{
	FVulkanCommandAllocator::FVulkanCommandAllocator(FVulkanDevice & VulkanDevice) : VulkanDevice(VulkanDevice)
	{
		VkCommandPoolCreateInfo CommandPoolCreateInfo { VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO };
		CommandPoolCreateInfo.queueFamilyIndex = 0;
		CommandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

		vkCreateCommandPool(VulkanDevice.vkDevice, &CommandPoolCreateInfo, nullptr, &vkCommandPool);
	}

	FVulkanCommandAllocator::~FVulkanCommandAllocator()
	{
		[[assume(vkCommandPool)]]

		if (vkCommandBuffers.Size)
		{
			vkFreeCommandBuffers(VulkanDevice.vkDevice, vkCommandPool, uint32(vkCommandBuffers.Size), vkCommandBuffers.Data);
			vkCommandBuffers.Clear();
		}

		if (vkCommandPool)
		{
			vkDestroyCommandPool(VulkanDevice.vkDevice, vkCommandPool, nullptr);
			vkCommandPool = nullptr;
		}
	}

	VkCommandBuffer FVulkanCommandAllocator::AcquireCommandBuffer()
	{
		if (vkCommandBuffers.Size)
		{
			VkCommandBuffer CommandBuffer = vkCommandBuffers.PopBack();
			VkCommandBufferBeginInfo CommandBufferBeginInfo { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
			vkBeginCommandBuffer(CommandBuffer, &CommandBufferBeginInfo);
			return CommandBuffer;
		}

		VkCommandBufferAllocateInfo CommandBufferAllocateInfo { VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
		CommandBufferAllocateInfo.commandPool = vkCommandPool;
		CommandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		CommandBufferAllocateInfo.commandBufferCount = 1;

		VkCommandBuffer CommandBuffer;
		vkAllocateCommandBuffers(VulkanDevice.vkDevice, &CommandBufferAllocateInfo, &CommandBuffer);

		VkCommandBufferBeginInfo CommandBufferBeginInfo { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
		vkBeginCommandBuffer(CommandBuffer, &CommandBufferBeginInfo);

		++NumCommandBuffers;
		return CommandBuffer;
	}

	void FVulkanCommandAllocator::ResetAllocator()
	{
		AssertForaml(NumCommandBuffers == vkCommandBuffers.Size, u8"{} of {} VkCommandBuffers of this VkCommandPool is online."V, NumCommandBuffers - vkCommandBuffers.Size, vkCommandBuffers.Size);
		vkResetCommandPool(VulkanDevice.vkDevice, vkCommandPool, VkCommandPoolResetFlags(0));
	}
}
