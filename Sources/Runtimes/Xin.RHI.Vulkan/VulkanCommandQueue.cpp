#include "PCH.h"
#include "VulkanCommandQueue.h"
#include "VulkanDevice.h"
#include "VulkanFence.h"

namespace Xin::RHI::Vulkan
{
	FVulkanCommandQueue::FVulkanCommandQueue(FVulkanDevice & VulkanDevice, const FCommandQueueDesc & ExecutorDesc)
		: IRHICommandQueue(VulkanDevice, ExecutorDesc), VulkanDevice(VulkanDevice), QueueFence(VulkanDevice)
	{
		uintx NumQueueFamilyProperties = VulkanDevice.PhysicalDevice.QueueFamilyProperties.Size;

		CommandQueues.Resize(NumQueueFamilyProperties);
		for (uint32_t QueueFamilyIndex = 0U; QueueFamilyIndex < NumQueueFamilyProperties; ++QueueFamilyIndex)
		{
			const VkQueueFamilyProperties & QueueFamilyProperty = VulkanDevice.PhysicalDevice.QueueFamilyProperties[QueueFamilyIndex];

			if (!QueueFamilyProperty.queueCount)
				continue;

			for (uint32_t QueueIndex = 0U; QueueIndex < QueueFamilyProperty.queueCount; ++QueueIndex)
			{
				VkQueue QueueHandle;
				vkGetDeviceQueue(VulkanDevice.vkDevice, QueueFamilyIndex, QueueIndex, &QueueHandle);
				CommandQueues[QueueFamilyIndex].Add(QueueHandle);
			}

			if (!PrimaryQueue && AllBits(QueueFamilyProperty.queueFlags, VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT))
			{
				PrimaryQueue = CommandQueues[QueueFamilyIndex][0];
			}
		}
	}

	FVulkanCommandQueue::~FVulkanCommandQueue()
	{
		if (QueueFence.GetSignalValue() < QueueFence.GetCompletedValue())
			FVulkanCommandQueue::WaitFinish();
	}

	void FVulkanCommandQueue::SetMarker(FStringV Event, const FColor & Color)
	{
#if XIN_Vulkan_PIX
		::PIXSetMarker(InnerCommandQueue.Get(), Color.ToARGB(), (const char *)Event.Data);
#endif
	}

	void FVulkanCommandQueue::BeginEvent(FStringV Event, const FColor & Color)
	{
#if XIN_Vulkan_PIX
		::PIXBeginEvent(InnerCommandQueue.Get(), Color.ToARGB(), (const char *)Event.Data);
#endif
	}

	void FVulkanCommandQueue::EndEvent()
	{
#if XIN_Vulkan_PIX
		::PIXEndEvent(InnerCommandQueue.Get());
#endif
	}

	void FVulkanCommandQueue::UpdateContext()
	{
		uint64 FenceCompletedValue = QueueFence.GetCompletedValue();

		{
			auto Iter = FencedBuffers.Begin();
			while (Iter != FencedBuffers.End())
			{
				if (Iter->First <= FenceCompletedValue)
				{
					vkDestroyBuffer(VulkanDevice.vkDevice, Iter->Second, nullptr);
					Iter = FencedBuffers.Erase(Iter);
				}
				else
					++Iter;
			}
		}

		{
			auto Iter = FencedMemorys.Begin();
			while (Iter != FencedMemorys.End())
			{
				if (Iter->First <= FenceCompletedValue)
				{
					vkFreeMemory(VulkanDevice.vkDevice, Iter->Second, nullptr);
					Iter = FencedMemorys.Erase(Iter);
				}
				else
					++Iter;
			}
		}

		{
			auto Iter = FencedCommandBuffers.Begin();
			while (Iter != FencedCommandBuffers.End())
			{
				if (Iter->First <= FenceCompletedValue)
				{
					Iter->Second.Second->FreeCommandBuffer(Iter->Second.First);
					Iter = FencedCommandBuffers.Erase(Iter);
				}
				else
					++Iter;
			}
		}

		{
			auto Iter = FencedCommandAllocators.Begin();
			while (Iter != FencedCommandAllocators.End())
			{
				if (Iter->First <= FenceCompletedValue)
				{
					VulkanDevice.FreeCommandAllocator(Iter->Second);
					Iter = FencedCommandAllocators.Erase(Iter);
				}
				else
					++Iter;
			}
		}

		{
			auto Iter = FencedOnlineDescriptorHeaps.Begin();
			while (Iter != FencedOnlineDescriptorHeaps.End())
			{
				if (Iter->First <= FenceCompletedValue)
				{
					VulkanDevice.ForfietDescriptorHeap(Iter->Second);
					Iter = FencedOnlineDescriptorHeaps.Erase(Iter);
				}
				else
					++Iter;
			}
		}
	}

	void FVulkanCommandQueue::WaitFinish(uint32 TimeoutMS)
	{
		VkSemaphoreWaitInfo SemaphoreWaitInfo { VK_STRUCTURE_TYPE_SEMAPHORE_WAIT_INFO };
		SemaphoreWaitInfo.semaphoreCount = 1;
		SemaphoreWaitInfo.pSemaphores = &QueueFence.vkSemaphore;
		SemaphoreWaitInfo.pValues = &QueueFence.SignalValue;

		VkResult Result = vkWaitSemaphores(VulkanDevice.vkDevice, &SemaphoreWaitInfo, TimeoutMS ? TimeoutMS : UINT64_MAX);
		UpdateContext();
	}

	void FVulkanCommandQueue::TileMapping(IRHIResource * Resource, const FRHIMemory & Memory)
	{
		
	}

	void FVulkanCommandQueue::FenceBuffer(VkBuffer Buffer)
	{
		FencedBuffers.Add({ QueueFence.SignalValue, Buffer });
	}

	void FVulkanCommandQueue::FenceMemory(VkDeviceMemory Memory)
	{
		FencedMemorys.Add({ QueueFence.SignalValue, Memory });
	}

	void FVulkanCommandQueue::FenceCommandBuffer(VkCommandBuffer vkCommandBuffer, FVulkanCommandAllocatorRef CommandAllocator)
	{
		FencedCommandBuffers.Add({ QueueFence.SignalValue, { vkCommandBuffer, CommandAllocator } });
	}

	void FVulkanCommandQueue::FenceCommandAllocator(FVulkanCommandAllocator * CommandAllocator)
	{
		FencedCommandAllocators.Add({ QueueFence.SignalValue, CommandAllocator });
	}

	void FVulkanCommandQueue::FenceOnlineDescriptorHeap(FVulkanDescriptorHeapRef DescriptorHeap)
	{
		FencedOnlineDescriptorHeaps.Add({ QueueFence.SignalValue, DescriptorHeap });
	}
}
