#pragma once

#include "VulkanTypes.h"
#include "VulkanHeap.h"

namespace Xin::RHI::Vulkan
{
	class FVulkanDevice;
	class FVulkanResource;
	
	class RHI_VULKAN_API FVulkanResourceView : public IRHIResourceView
	{
	public:
		FVulkanResourceView(FVulkanResource & Resource, uint32 SubresourceIndex = NullIndex);
		FVulkanResourceView(FVulkanResource & Resource, const FResourceViewDesc & ViewDesc);
		~FVulkanResourceView();
		IRHIResource * GetResource() const override;

		void CreateBufferView();
		VkImageView CreateImageView(EFormat Format);

		VkImageView GetImageViewMutable(EFormat Format);

	public:
		FVulkanDevice & VulkanDevice;
		FVulkanResource & VulkanResource;

		VkBufferView vkBufferView { nullptr };
		VkImageView vkImageView { nullptr };
		TMap<EFormat, VkImageView> vkImageViewsMutable;
	};
	using FVulkanResourceViewRef = TReferPtr<FVulkanResourceView>;
}
