#include "PCH.h"
#include "VulkanResourceView.h"
#include "VulkanDevice.h"
#include "VulkanResource.h"

namespace Xin::RHI::Vulkan
{
	FVulkanResourceView::FVulkanResourceView(FVulkanResource & Resource, uint32 SubresourceIndex)
		: IRHIResourceView(Resource, FResourceViewDesc(SubresourceIndex, Resource.ResourceDesc)), VulkanDevice(Resource.VulkanDevice), VulkanResource(Resource)
	{
		if (Resource.ResourceDesc.Dimension == EResourceDimension::Buffer)
		{
			CreateBufferView();
		}
		else if (Resource.ResourceDesc.Dimension == EResourceDimension::Texture2D)
		{
			vkImageView = CreateImageView(Resource.ResourceDesc.Format);
		}
		else
		{

		}
	}

	FVulkanResourceView::FVulkanResourceView(FVulkanResource & Resource, const FResourceViewDesc & ViewDesc)
		: IRHIResourceView(Resource, ViewDesc), VulkanDevice(Resource.VulkanDevice), VulkanResource(Resource)
	{
		if (Resource.ResourceDesc.Dimension == EResourceDimension::Buffer)
		{
			CreateBufferView();
		}
		else if (Resource.ResourceDesc.Dimension == EResourceDimension::Texture2D)
		{
			vkImageView = CreateImageView(Resource.ResourceDesc.Format);
		}
		else
		{

		}
	}

	FVulkanResourceView::~FVulkanResourceView()
	{
		if (vkBufferView)
		{
			vkDestroyBufferView(VulkanDevice.vkDevice, vkBufferView, nullptr);
			vkBufferView = nullptr;
		}

		if (vkImageView)
		{
			vkDestroyImageView(VulkanDevice.vkDevice, vkImageView, nullptr);
			vkImageView = nullptr;
		}
	}

	IRHIResource * FVulkanResourceView::GetResource() const
	{
		return &VulkanResource;
	}

	void FVulkanResourceView::CreateBufferView()
	{
		if (!AnyFlags(Resource.ResourceDesc.Usage, EResourceUsage::StorageBuffer | EResourceUsage::UniformTexelBuffer | EResourceUsage::StorageTexelBuffer))
			return;

		AssertExpr(VulkanResource.vkBuffer);

		VkBufferViewCreateInfo BufferViewCreateInfo { VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO };
		BufferViewCreateInfo.flags = VkBufferViewCreateFlags(0);
		BufferViewCreateInfo.buffer = VulkanResource.vkBuffer;
		BufferViewCreateInfo.format = FromFormatSRV(Resource.ResourceDesc.Format);
		BufferViewCreateInfo.offset = VulkanResource.Location.Offset;
		BufferViewCreateInfo.range = VulkanResource.Location.Size;
		VkResult Result = vkCreateBufferView(VulkanResource.VulkanDevice.vkDevice, &BufferViewCreateInfo, nullptr, &vkBufferView);
		VkResultAssert(!Result, u8"vkCreateBufferView has failed."V);
	}

	VkImageView FVulkanResourceView::CreateImageView(EFormat Format)
	{
		VkImageViewCreateInfo ImageViewCreateInfo { VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO };
		ImageViewCreateInfo.components = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };

		ImageViewCreateInfo.format = FromFormat(Format);
		if (VulkanResource.ResourceDesc.Usage & EResourceUsage::DepthStencil)
		{
			ImageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
			if (IsStencilFormat(Format))
				ImageViewCreateInfo.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
		}
		else
			ImageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

		ImageViewCreateInfo.subresourceRange.baseMipLevel = 0;
		ImageViewCreateInfo.subresourceRange.levelCount = VK_REMAINING_MIP_LEVELS;

		if (ViewDesc.SubresourceIndex == NullIndex)
		{
			if (VulkanResource.ResourceDesc.Layers > 1)
			{
				if (VulkanResource.ResourceDesc.Dimension == EResourceDimension::Texture1D)
					ImageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_1D_ARRAY;
				else if (VulkanResource.ResourceDesc.Dimension == EResourceDimension::Texture2D)
					ImageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D_ARRAY;
				else
					LogWarning(u8"Unsupported ResourceViewDimension {}"V, VulkanResource.ResourceDesc.Dimension );

				ImageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
				ImageViewCreateInfo.subresourceRange.layerCount = VK_REMAINING_ARRAY_LAYERS;
			}
			else
			{
				if (VulkanResource.ResourceDesc.Dimension == EResourceDimension::Texture1D)
					ImageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_1D;
				else if (VulkanResource.ResourceDesc.Dimension == EResourceDimension::Texture2D)
					ImageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
				else if (VulkanResource.ResourceDesc.Dimension == EResourceDimension::Texture3D)
					ImageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_3D;
				else
					LogWarning(u8"Unsupported ResourceViewDimension {}"V, VulkanResource.ResourceDesc.Dimension );

				ImageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
				ImageViewCreateInfo.subresourceRange.layerCount = 1;
			}
		}
		else
		{
			if (VulkanResource.ResourceDesc.Dimension == EResourceDimension::Texture1D)
				ImageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_1D;
			else if (VulkanResource.ResourceDesc.Dimension == EResourceDimension::Texture2D)
				ImageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			else if (VulkanResource.ResourceDesc.Dimension == EResourceDimension::Texture3D)
				ImageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_3D;
			else
				LogWarning(u8"Unsupported ResourceViewDimension {}"V, VulkanResource.ResourceDesc.Dimension );

			ImageViewCreateInfo.subresourceRange.baseArrayLayer = ViewDesc.SubresourceIndex;
			ImageViewCreateInfo.subresourceRange.layerCount = 1;
		}
		ImageViewCreateInfo.flags = 0;
		ImageViewCreateInfo.image = VulkanResource.vkImage;

		VkImageView vkImageView = nullptr;
		VkResult Result = vkCreateImageView(VulkanResource.VulkanDevice.vkDevice, &ImageViewCreateInfo, nullptr, &vkImageView);
		return vkImageView;
	}

	VkImageView FVulkanResourceView::GetImageViewMutable(EFormat Format)
	{
		if (Format == Resource.ResourceDesc.Format)
			return vkImageView;

		if (auto Iter = vkImageViewsMutable.Find(Format); Iter != vkImageViewsMutable.End())
			return Iter->Second;

		VkImageView vkImageView = CreateImageView(Format);
		vkImageViewsMutable[Format] = vkImageView;
		return vkImageView;
	}
}
