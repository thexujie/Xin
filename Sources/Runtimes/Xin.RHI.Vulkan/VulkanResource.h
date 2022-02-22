#pragma once

#include "VulkanTypes.h"
#include "VulkanResourceView.h"

namespace Xin::RHI::Vulkan
{
	class FVulkanHeapInstance;
	class FVulkanResourceView;

	struct FPooledBlockLocation
	{
		uint64 Offset = 0;
		uint64 Size = 0;

		explicit operator bool() const { return !!Size; }
		bool operator ==(const FPooledBlockLocation &) const = default;
		auto operator <=>(const FPooledBlockLocation & Another) const = default;

		friend uintx Hash(const FPooledBlockLocation & PooledBlockLocation)
		{
			return HashCombine(PooledBlockLocation.Offset, PooledBlockLocation.Size);
		}
	};


	class RHI_VULKAN_API FVulkanResource : public IRHIResource
	{
	public:
		//FVulkanResource(FVulkanContext & Context, IVulkanResource * VulkanResource, Vulkan_RESOURCE_STATES ResourceState);
		FVulkanResource(FVulkanDevice & VulkanDevice, const FResourceDesc & ResourceDesc);
		FVulkanResource(FVulkanDevice & VulkanDevice, const FResourceDesc & ResourceDesc, const FRHIMemory & MemoryBlock);
		FVulkanResource(FVulkanDevice & VulkanDevice, const FResourceDesc & ResourceDesc, IHeapAllocator & HeapAllocator);
		FVulkanResource(FVulkanDevice & VulkanDevice, const FResourceDesc & ResourceDesc, VkImage Image, EResourceAccess InitialAccess);
		~FVulkanResource();
		void SetDebugName(FStringV Name) override;

	public:
		IRHIResourceView * GetResourceView(uint32 ArrayIndex = NullIndex) const final
		{
			if (ArrayIndex == NullIndex)
				return ResourceView;
			else
				return SubresourceViews[ArrayIndex];
		}
		forceinline void * GetSharedHandle() const final { return nullptr; }

	private:
		void CreateResource(TFunction<FRHIMemory(EResourceUsage, uint64, uint32)> MemoryAllocator);
		void CreateResourceViews();

	public:
		FVulkanDevice & VulkanDevice;
		VkImage vkImage = nullptr;
		VkBuffer vkBuffer = nullptr;

		FRHIMemory Memory;

		bool ExternalImage = false;

		EResourceAccess InitialAccess = EResourceAccess::Common;
		EResourceAccess ResourceAccess = EResourceAccess::Common;

		VkPipelineStageFlags PipelineStages = VK_PIPELINE_STAGE_NONE;
		VkAccessFlags AccessFlags = VK_ACCESS_NONE;
		VkImageLayout ImageLayout = VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED;

	public:
		FVulkanResourceViewRef ResourceView;
		TList<FVulkanResourceViewRef> SubresourceViews;
	};
	using FVulkanResourceRef = TReferPtr<FVulkanResource>;
}
