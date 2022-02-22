#pragma once

#include "VulkanTypes.h"
#include "VulkanFence.h"
#include "VulkanHeap.h"
#include "VulkanCommandAllocator.h"

namespace Xin::RHI::Vulkan
{
	class RHI_VULKAN_API FVulkanCommandQueue: public IRHICommandQueue
	{
	public:
		FVulkanCommandQueue(FVulkanDevice & VulkanDevice, const FCommandQueueDesc & ExecutorDesc);
		~FVulkanCommandQueue();

	public:
		void SetMarker(FStringV Event, const FColor & Color) override;
		void BeginEvent(FStringV Event, const FColor & Color) override;
		void EndEvent() override;

	public:
		IRHIFence & GetFence() const override { return ConstCast(QueueFence); }
		void UpdateContext() override;
		void WaitFinish(uint32 TimeoutMS = 0) override;

	public:
		void FenceResource(IRHIResource * RHIResource) override {}
		void TileMapping(IRHIResource * Resource, const FRHIMemory & Memory) override;

		void FenceBuffer(VkBuffer Buffer);
		void FenceMemory(VkDeviceMemory Memory);

		TList<TPair<uint64, VkBuffer>> FencedBuffers;
		TList<TPair<uint64, VkDeviceMemory>> FencedMemorys;

	public:
		void FenceCommandBuffer(VkCommandBuffer vkCommandBuffer, FVulkanCommandAllocatorRef CommandAllocator);
		TList<TPair<uint64, TPair<VkCommandBuffer, FVulkanCommandAllocatorRef>>> FencedCommandBuffers;

		void FenceCommandAllocator(FVulkanCommandAllocator * CommandAllocator);
		TList<TPair<uint64, FVulkanCommandAllocatorRef>> FencedCommandAllocators;

	public:
		void FenceOnlineDescriptorHeap(FVulkanDescriptorHeapRef DescriptorHeap);
		//TList<FD3D12DescriptorHeapRef> FreeOnlineDescriptorHeaps;
		mutable TList<TPair<uint64, FVulkanDescriptorHeapRef>> FencedOnlineDescriptorHeaps;

	public:
		FVulkanDevice & VulkanDevice;
		FVulkanFence QueueFence;

	public:
		TList<TList<VkQueue>> CommandQueues;
		VkQueue PrimaryQueue { nullptr };
	};
	using FVulkanCommandExecutorRef = TReferPtr<FVulkanCommandQueue>;
}
