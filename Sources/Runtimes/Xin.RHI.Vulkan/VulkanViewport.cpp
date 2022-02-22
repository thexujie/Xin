#include "PCH.h"
#include "VulkanViewport.h"

#include "VulkanCommandQueue.h"
#include "VulkanDevice.h"
#include "VulkanResource.h"


namespace Xin::RHI::Vulkan
{
	FVulkanViewport::FVulkanViewport(FVulkanCommandQueue & CommandQueue, voidp WindowHandle, SizeU Size, uint32 NumBackBuffers,
		EFormat Format, EPresentMode PresentMode, FMSAA MSAA)
		: CommandQueue(CommandQueue), WindowHandle(WindowHandle), Size(Size == SizeU::Zero ? Windows::GetClientSize(HWND(WindowHandle)) : Size), NumBackBuffers(NumBackBuffers)
		, Format(Format), PresentMode(PresentMode), MSAA(MSAA)
	{
		AllowTearing = true;
		//EnableFrameLatency = true;
		Initialize();
	}

	void FVulkanViewport::SetDebugName(FStringV ResourceLabel)
	{
		IRHIViewport::SetDebugName(ResourceLabel);
		//for (uint32 BackBufferIndex = 0; BackBufferIndex < BackBuffers.Size; ++BackBufferIndex)
		//	SetVulkanObjectName(BackBuffers[BackBufferIndex]->InnerResource, Xin::Format(u8"{}.BackBuffer[{}]"V, ResourceLabel, BackBufferIndex));
	}

	FVulkanViewport::~FVulkanViewport()
	{
		FinalizeSwapchain();

		if (vkSurface)
		{
			vkDestroySurfaceKHR(CommandQueue.VulkanDevice.vkInstance, vkSurface, nullptr);
			vkSurface = nullptr;
		}

		for (VkSemaphore Semaphore : FrameSemephores)
			vkDestroySemaphore(CommandQueue.VulkanDevice.vkDevice, Semaphore, nullptr);
		FrameSemephores.Reset();
	}

	void FVulkanViewport::Initialize()
	{
		VkWin32SurfaceCreateInfoKHR SurfaceCI = { VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR };
		SurfaceCI.pNext = NULL;
		SurfaceCI.flags = VkWin32SurfaceCreateFlagsKHR(0);
		SurfaceCI.hinstance = Windows::Instance();
		SurfaceCI.hwnd = HWND(WindowHandle);
		VkResult Result = vkCreateWin32SurfaceKHR(CommandQueue.VulkanDevice.vkInstance, &SurfaceCI, NULL, &vkSurface);

		InitializeSwapchain();
	}

	uint32 FVulkanViewport::BeginFrame()
	{
		uintx FrameBufferIndex = FrameIndex % NumBackBuffers;
		VkSemaphoreWaitInfo SemaphoreWaitInfo { VK_STRUCTURE_TYPE_SEMAPHORE_WAIT_INFO };
		SemaphoreWaitInfo.semaphoreCount = 1;
		SemaphoreWaitInfo.pSemaphores = &CommandQueue.QueueFence.vkSemaphore;
		SemaphoreWaitInfo.pValues = &FrameFenceValues[FrameBufferIndex];

		vkWaitSemaphores(CommandQueue.VulkanDevice.vkDevice, &SemaphoreWaitInfo, UINT64_MAX);

		uint32 AcquiredImageIndex;
		VkResult Result = vkAcquireNextImageKHR(CommandQueue.VulkanDevice.vkDevice, vkSwapchain, 0, FrameSemephores[FrameBufferIndex], nullptr, &AcquiredImageIndex);
		if (Result == VK_ERROR_OUT_OF_DATE_KHR)
			return NullIndex;

		AssertExpr(Result == VK_SUCCESS);
		ImageIndex = AcquiredImageIndex;

		return ImageIndex;
	}

	void FVulkanViewport::EndFrame(uint32 SyncInterval)
	{
		uint32 BackBufferIndex = FrameIndex % NumBackBuffers;

		VkPresentInfoKHR PresentInfo { VK_STRUCTURE_TYPE_PRESENT_INFO_KHR };

		PresentInfo.waitSemaphoreCount = 1;
		PresentInfo.pWaitSemaphores = &FrameSemephores[BackBufferIndex];

		PresentInfo.swapchainCount = 1;
		PresentInfo.pSwapchains = &vkSwapchain;
		PresentInfo.pImageIndices = &ImageIndex;

		VkResult Result = vkQueuePresentKHR(CommandQueue.PrimaryQueue, &PresentInfo);

		VkSemaphoreSubmitInfo SemaphoreSubmitInfo { VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO };
		{
			FrameFenceValues[BackBufferIndex] = ++CommandQueue.QueueFence.SignalValue;
			{
				SemaphoreSubmitInfo.pNext = NULL;
				SemaphoreSubmitInfo.semaphore = CommandQueue.QueueFence.vkSemaphore;
				SemaphoreSubmitInfo.value = FrameFenceValues[BackBufferIndex];
				SemaphoreSubmitInfo.stageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT ;
			}
		}

		VkSubmitInfo2 SubmitInfo2 { VK_STRUCTURE_TYPE_SUBMIT_INFO_2 };
		{
			SubmitInfo2.signalSemaphoreInfoCount = 1;
			SubmitInfo2.pSignalSemaphoreInfos = &SemaphoreSubmitInfo;
		}
		Result = vkQueueSubmit2(CommandQueue.PrimaryQueue, 1, &SubmitInfo2, VK_NULL_HANDLE);

		++FrameIndex;
	}

	void FVulkanViewport::Resize(SizeU NewSize)
	{
		FinalizeSwapchain();

		Size = NewSize;
		InitializeSwapchain();
	}

	void FVulkanViewport::InitializeSwapchain()
	{
		VkSurfaceCapabilitiesKHR SurfaceCapabilities {};
		VkResult Result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(CommandQueue.VulkanDevice.vkPhysicalDevice, vkSurface, &SurfaceCapabilities);

		uint32 NumSurfaceFormats;
		Result = vkGetPhysicalDeviceSurfaceFormatsKHR(CommandQueue.VulkanDevice.vkPhysicalDevice, vkSurface, &NumSurfaceFormats, nullptr);
		TList<VkSurfaceFormatKHR> SurfaceFormats(NumSurfaceFormats);
		Result = vkGetPhysicalDeviceSurfaceFormatsKHR(CommandQueue.VulkanDevice.vkPhysicalDevice, vkSurface, &NumSurfaceFormats, SurfaceFormats.Data);

		uint32 NumPresentModes;
		Result = vkGetPhysicalDeviceSurfacePresentModesKHR(CommandQueue.VulkanDevice.vkPhysicalDevice, vkSurface, &NumPresentModes, nullptr);
		TList<VkPresentModeKHR> PresentModes(NumPresentModes);
		Result = vkGetPhysicalDeviceSurfacePresentModesKHR(CommandQueue.VulkanDevice.vkPhysicalDevice, vkSurface, &NumPresentModes, PresentModes.Data);

		VkSwapchainCreateInfoKHR SwapchainCI { VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR };
		{
			SwapchainCI.surface = vkSurface;
			SwapchainCI.minImageCount = NumBackBuffers;
			SwapchainCI.imageFormat = FromFormat(Format);
			SwapchainCI.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
			SwapchainCI.imageExtent = { Size.Width, Size.Height };
			SwapchainCI.imageArrayLayers = 1;
			SwapchainCI.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
			SwapchainCI.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			SwapchainCI.queueFamilyIndexCount = 0;
			SwapchainCI.pQueueFamilyIndices = nullptr;
			SwapchainCI.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
			SwapchainCI.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
			SwapchainCI.presentMode = FromPresentMode(PresentMode);
			if (!PresentModes.Contains(SwapchainCI.presentMode))
				SwapchainCI.presentMode = PresentModes[0];

			SwapchainCI.clipped = false;
			SwapchainCI.oldSwapchain = nullptr;
		}

		bool FormatSupported = false;
		for (const VkSurfaceFormatKHR & SurfaceFormat  : SurfaceFormats)
		{
			if (SurfaceFormat.format == SwapchainCI.imageFormat && SurfaceFormat.colorSpace == SwapchainCI.imageColorSpace)
			{
				FormatSupported = true;
				break;;
			}
		}

		if (!FormatSupported)
		{
			LogError(u8"Format {} color space {} is not supprted."V, Format, SwapchainCI.imageColorSpace);
			return;
		}

		Result = vkCreateSwapchainKHR(CommandQueue.VulkanDevice.vkDevice, &SwapchainCI, nullptr, &vkSwapchain);

		TList<VkImage> SwapchainImages(NumBackBuffers);
		uint32 NumSwapchainImages = NumBackBuffers;
		Result = vkGetSwapchainImagesKHR(CommandQueue.VulkanDevice.vkDevice, vkSwapchain, &NumSwapchainImages, SwapchainImages.Data);
		AssertExpr(NumSwapchainImages == NumBackBuffers);

		FrameBuffers.ResizeTo(NumBackBuffers);
		FrameSemephores.ResizeTo(NumBackBuffers);
		FrameFenceValues.ResizeTo(NumBackBuffers, 0);

		for (uint32 BackBufferIndex = 0; BackBufferIndex < NumBackBuffers; BackBufferIndex++)
		{
			FResourceDesc ResourceDesc;
			ResourceDesc.Dimension = EResourceDimension::Texture2D;
			ResourceDesc.Usage = EResourceUsage::RenderTarget;
			ResourceDesc.Format = Format;
			ResourceDesc.Size = Size;
			ResourceDesc.Layers = 1;
			ResourceDesc.Mipmaps = 1;
			FrameBuffers[BackBufferIndex] = new FVulkanResource(CommandQueue.VulkanDevice, ResourceDesc, SwapchainImages[BackBufferIndex], EResourceAccess::Present);

			VkSemaphoreCreateInfo SemaphoreCreateInfo = { VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };
			vkCreateSemaphore(CommandQueue.VulkanDevice.vkDevice, &SemaphoreCreateInfo, nullptr, &FrameSemephores[BackBufferIndex]);
		}
	}

	void FVulkanViewport::FinalizeSwapchain()
	{
		//vkDeviceWaitIdle(CommandQueue.VulkanDevice.vkDevice);

		FrameBuffers.Reset();

		if (vkSwapchain)
		{
			vkDestroySwapchainKHR(CommandQueue.VulkanDevice.vkDevice, vkSwapchain, nullptr);
			vkSwapchain = nullptr;
		}
	}
}

