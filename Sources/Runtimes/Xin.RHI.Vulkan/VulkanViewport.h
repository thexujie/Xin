#pragma once

#include "VulkanCommandQueue.h"
#include "VulkanFence.h"
#include "VulkanTypes.h"
#include "VulkanResource.h"

namespace Xin::RHI::Vulkan
{
	class FVulkanDevice;
	class FVulkanCommandQueue;

	class RHI_VULKAN_API FVulkanViewport : public IRHIViewport
	{
	public:
		FVulkanViewport(FVulkanCommandQueue & CommandQueue, voidp WindowHandle, SizeU Size, uint32 NumBackBuffers,
			EFormat Format = EFormat::BGRAUF, EPresentMode PresentMode = EPresentMode::FlipDiscard, FMSAA MSAA = {});
		~FVulkanViewport();
		void SetDebugName(FStringV ResourceLabel) override;

		void Initialize();

	public:
		uint32 BeginFrame() override;
		void EndFrame(uint32 SyncInterval) override;

		uint32 GetNumBackBuffers() const override { return NumBackBuffers; }
		uint32 GetCurrentBufferIndex() const override { return ImageIndex; }
		IRHIResource * GetBackBuffer(uint32 BufferIndex) const override { return FrameBuffers[BufferIndex].Get(); }
		SizeU GetSize() const override { return Size; }
		void Resize(SizeU NewSize) override;

	private:
		void InitializeSwapchain();
		void FinalizeSwapchain();

	public:
		FVulkanCommandQueue & CommandQueue;
		uint32 FrameIndex = 0;
		uint32 ImageIndex = 0;

		voidp WindowHandle = nullptr;
		SizeU Size;
		uint32 NumBackBuffers;
		EFormat Format;
		EPresentMode PresentMode;
		FMSAA MSAA;
		uint32 SwapChainFlag = 0;

		VkSurfaceKHR vkSurface { nullptr };
		VkSwapchainKHR vkSwapchain { nullptr };

		TList<uint64> FrameFenceValues;
		TList<VkSemaphore> FrameSemephores;
		TList<FVulkanResourceRef> FrameBuffers;

		bool AllowTearing = false;
		bool EnableFrameLatency = false;
	};
}
