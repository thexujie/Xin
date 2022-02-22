#include "PCH.h"
#include "VulkanHeap.h"
#include "VulkanResource.h"
#include "VulkanDevice.h"
#include "VulkanResourceView.h"

namespace Xin::RHI::Vulkan
{
	FVulkanResource::FVulkanResource(FVulkanDevice & VulkanDevice, const FResourceDesc & ResourceDesc)
		: IRHIResource(ResourceDesc), VulkanDevice(VulkanDevice), InitialAccess(GetInitialAccess(ResourceDesc))
	{
		ResourceAccess = InitialAccess;
		PipelineStages = FromResourceAccess_PipelineStage(InitialAccess);
		AccessFlags = FromResourceAccess_AccessFlags(InitialAccess);

		CreateResource([this](EResourceUsage Usage, uint64 Size, uint32 Alignment)
		{
			FRHIMemory AllocatedMemory;
			AllocatedMemory.Instance = nullptr;
			AllocatedMemory.PlaceHeap = new FVulkanHeap(this->VulkanDevice, FHeapDesc{ EHeapUsage::AllowMask, Alignment, Size });
			AllocatedMemory.Location = { 0, Size, Size };
			return AllocatedMemory;
		});
		CreateResourceViews();
	}

	FVulkanResource::FVulkanResource(FVulkanDevice & VulkanDevice, const FResourceDesc & ResourceDesc, const FRHIMemory & MemoryBlock)
		: IRHIResource(ResourceDesc), VulkanDevice(VulkanDevice), InitialAccess(GetInitialAccess(ResourceDesc))
	{
		ResourceAccess = InitialAccess;
		PipelineStages = FromResourceAccess_PipelineStage(InitialAccess);
		AccessFlags = FromResourceAccess_AccessFlags(InitialAccess);

		CreateResource([&MemoryBlock](EResourceUsage Usage, uint64 Size, uint32 Alignment)
			{
				return MemoryBlock;
			});
		CreateResourceViews();
	}

	FVulkanResource::FVulkanResource(FVulkanDevice & VulkanDevice, const FResourceDesc & ResourceDesc, IHeapAllocator & HeapAllocator)
		: IRHIResource(ResourceDesc), VulkanDevice(VulkanDevice), InitialAccess(GetInitialAccess(ResourceDesc))
	{
		ResourceAccess = InitialAccess;
		PipelineStages = FromResourceAccess_PipelineStage(InitialAccess);
		AccessFlags = FromResourceAccess_AccessFlags(InitialAccess);

		CreateResource([&](EResourceUsage Usage, uint64 Size, uint32 Alignment) { return HeapAllocator.Allocate(Usage, Size, Alignment); });
		CreateResourceViews();
	}

	FVulkanResource::FVulkanResource(FVulkanDevice & VulkanDevice, const FResourceDesc & ResourceDesc, VkImage Image, EResourceAccess InitialAccess)
		: IRHIResource(ResourceDesc), VulkanDevice(VulkanDevice), vkImage(Image), ExternalImage(true), InitialAccess(/*GetInitialAccess(ResourceDesc)*/InitialAccess)
	{
		if (ResourceDesc.Dimension == EResourceDimension::Texture2D)
		{
			ResourceAccess = InitialAccess = EResourceAccess::None;
			PipelineStages = FromResourceAccess_PipelineStage(InitialAccess);
			AccessFlags = FromResourceAccess_AccessFlags(InitialAccess);
			ImageLayout = FromResourceAccess_Layout(ResourceAccess);
		}
		CreateResourceViews();
	}

	FVulkanResource::~FVulkanResource()
	{
		if (vkImage)
		{
			if (!ExternalImage)
			{
				vkDestroyImage(VulkanDevice.vkDevice, vkImage, nullptr);
			}
			vkImage = nullptr;
		}

		if (vkBuffer)
		{
			vkDestroyBuffer(VulkanDevice.vkDevice, vkBuffer, nullptr);
			vkBuffer = nullptr;
		}

		if (Memory.Instance)
		{
			if (!(ResourceDesc.Usage & EResourceUsage::Transisent))
				Memory.Instance->Deallocate(Memory);
			Memory = { };
		}
	}

	void FVulkanResource::SetDebugName(FStringV Name)
	{
		IRHIResource::SetDebugName(Name);

		if (vkBuffer)
		{
			SetVulkanObjectName(VulkanDevice.vkDevice, vkBuffer, DebugName, VkObjectType::VK_OBJECT_TYPE_BUFFER);
		}
		else if (vkImage)
		{
			SetVulkanObjectName(VulkanDevice.vkDevice, vkImage, DebugName, VkObjectType::VK_OBJECT_TYPE_IMAGE);
		}
		else {}
	}

	void FVulkanResource::CreateResource(TFunction<FRHIMemory(EResourceUsage, uint64, uint32)> MemoryAllocator)
	{
		if (ResourceDesc.Dimension == EResourceDimension::Texture2D)
		{
			// initialLayout but must be UNDEFINED or PREINITIALIZED.
			ResourceAccess = EResourceAccess::None;
			ImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			VkImageCreateInfo ImageCI = ImageCreateInfoFromResourceDesc(ResourceDesc, ImageLayout);

			VkResult Result = vkCreateImage(VulkanDevice.vkDevice, &ImageCI, nullptr, &vkImage);

			VkMemoryRequirements MemoryRequirements;
			vkGetImageMemoryRequirements(VulkanDevice.vkDevice, vkImage, &MemoryRequirements);
			Memory = MemoryAllocator(ResourceDesc.Usage, MemoryRequirements.size, uint32(MemoryRequirements.alignment));

			FVulkanHeap & VulkanHeap = StaticCastRef<FVulkanHeap>(Memory.PlaceHeap);
			Result = vkBindImageMemory(VulkanDevice.vkDevice, vkImage, VulkanHeap.vkMemory, Memory.Location.Offset);

			if (ResourceDesc.Usage & EResourceUsage::HostMask)
				BaseAddress.CPUAddress = VulkanHeap.BaseAddress.CPUAddress + Memory.Location.Offset;
			BaseAddress.GPUAddress = FGPUAddress(vkImage);
			Location = { 0, MemoryRequirements.size, Memory.Location.BlockSize };
		}
		else if (ResourceDesc.Dimension == EResourceDimension::Buffer)
		{
			VkBufferCreateInfo BufferCI = BufferCreateInfoFromResourceDesc(ResourceDesc);
			VkResult Result = vkCreateBuffer(VulkanDevice.vkDevice, &BufferCI, nullptr, &vkBuffer);

			VkMemoryRequirements MemoryRequirements;
			vkGetBufferMemoryRequirements(VulkanDevice.vkDevice, vkBuffer, &MemoryRequirements);
			Memory = MemoryAllocator(ResourceDesc.Usage, MemoryRequirements.size, uint32(MemoryRequirements.alignment));

			FVulkanHeap & VulkanHeap = StaticCastRef<FVulkanHeap>(Memory.PlaceHeap);
			Result = vkBindBufferMemory(VulkanDevice.vkDevice, vkBuffer, VulkanHeap.vkMemory, Memory.Location.Offset);

			if (ResourceDesc.Usage & EResourceUsage::HostMask)
				BaseAddress.CPUAddress = VulkanHeap.BaseAddress.CPUAddress + Memory.Location.Offset;
			BaseAddress.GPUAddress = FGPUAddress(vkBuffer);
			Location = { 0, ResourceDesc.Size.X * ResourceDesc.Size.Y, Memory.Location.BlockSize };
		}
	}

	void FVulkanResource::CreateResourceViews()
	{
		if (!vkImage && !vkBuffer)
			return;

		//ImplicitStateTransition = IsImplicitStateTransitionResource(*this);
		ResourceView = new FVulkanResourceView(*this);
		if (ResourceDesc.Layers > 1)
		{
			SubresourceViews.Resize(ResourceDesc.Layers);
			for (uint32 LayerIndex = 0; LayerIndex < ResourceDesc.Layers; ++LayerIndex)
			{
				SubresourceViews[LayerIndex] = new FVulkanResourceView(*this, LayerIndex);
			}
		}
	}
}
