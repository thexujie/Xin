#pragma once

#include "Xin.Core/Xin.Core.h"
#include "Xin.RHI/Xin.RHI.h"
#include "Xin.Core/Platform/Windows/WindowsPrivate.h"

#include "Xin.RHI.Vulkan.Config.h"

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>

//#define XIN_VULKAN_DEBUG_UNTILS XIN_DEBUG
#define XIN_VULKAN_DEBUG_UNTILS (!XIN_SHIPPING)

namespace Xin::RHI::Vulkan
{
	class FVulkanDevice;

	using namespace Xin::RHI;

	constexpr uint32 MaxDescriptorSets = 32;
	constexpr uint32 MaxVulkanShaderParameters = 64;
	constexpr uint32 MaxVulkanShaderDescriptors = 64;
	constexpr uint32 MaxVulkanShaderBindingIndex = 32;

	enum class EVulkanRootParameterType
	{
		None = -1,
		UInt32Constant = 0,
		ConstBuffer,
		ShaderResource,
		UnorderedAccess,
		DescriptorTable,
		Sampler,
	};

	enum EVulkanRootParameterTypeIndex
	{
		RPTI_UInt32Constant = 0,
		RPTI_ConstBuffer,
		RPTI_ShaderResource,
		RPTI_UnorderedAccess,
		RPTI_DescriptorTable,
		RPTI_Sampler,
		RPTI_Count,
	};

	constexpr uint32 VulkanRootParameterTypeCount = RPTI_Count;

	struct FVulkanRootRangeParameter
	{
		EDescriptorType DescriptorType = EDescriptorType::None;
		uint32 RegisterIndex = 0;
		uint32 RegisterSpace = 0;
		uint32 RegisterCount = 0;

		friend uintx Hash(const FVulkanRootRangeParameter & This) { return HashCombine(This.DescriptorType, This.RegisterIndex, This.RegisterSpace, This.RegisterCount); }
	};

	struct FVulkanRootParameter
	{
		EVulkanRootParameterType RootParameterType = EVulkanRootParameterType::None;
		EShaderStage ShaderStage = EShaderStage::None;

		uint32 ParameterIndex = 0;
		uint32 ParameterCount = 0;
		uint32 ParameterRangeIndex = 0;
		uint32 ParameterRangeCount = 0;
		uint32 DescriptorIndex = 0;
		uint32 DescriptorCount = 0;
	};

	enum class EVulkanRootSignatureFlags
	{
		None = 0,
		ReverseZ,
	};

	struct FVulkanRootSignature
	{
		TList<FVulkanRootRangeParameter> RangeParameters;
		TList<FVulkanRootParameter> RootParameters;
		EVulkanRootSignatureFlags Flags = EVulkanRootSignatureFlags::None;

		friend uintx Hash(const FVulkanRootSignature & This) { return HashCombine(This.RangeParameters, This.RootParameters, This.Flags); }
	};

	struct FHeapBlock
	{
		uint64 Offset = 0;
		uint64 Size = 0;

		explicit operator bool() const { return !!Size; }
		bool operator ==(const FHeapBlock &) const = default;
		auto operator <=>(const FHeapBlock &) const = default;
		friend uintx Hash(const FHeapBlock & HeapPosition) { return HashCombine(HeapPosition.Offset, HeapPosition.Size); }
	};

	struct FVulkanAddress
	{
		byte * CPUAddress = nullptr;
		uint64 GPUAddress = NullAddress;

		FVulkanAddress() = default;
		FVulkanAddress(NullptrT) {}
		FVulkanAddress(byte * CPUAddress, uint64 GPUAddress) : CPUAddress(CPUAddress), GPUAddress(GPUAddress) {}

		FVulkanAddress & operator =(NullptrT)
		{
			CPUAddress = nullptr;
			GPUAddress = NullAddress;
			return *this;
		}

		bool Offline() const noexcept { return CPUAddress != nullptr; }
		bool Online() const noexcept { return GPUAddress != NullAddress; }
		bool Valid() const noexcept { return CPUAddress != nullptr || GPUAddress != NullAddress; }
		explicit operator bool() const noexcept { return Valid(); }

		bool operator ==(const FVulkanAddress &) const noexcept = default;
		FVulkanAddress operator +(uint32 Offset) const noexcept { return FVulkanAddress { CPUAddress ? CPUAddress + Offset : nullptr, GPUAddress == NullAddress ? NullAddress : GPUAddress + Offset }; }
		FVulkanAddress operator -(uint32 Offset) const noexcept { return FVulkanAddress { CPUAddress ? CPUAddress - Offset : nullptr, GPUAddress == NullAddress ? NullAddress : GPUAddress - Offset }; }
	};

	using FVulkanDescriptorAddress = FVulkanAddress;

	struct FVulkanAddressHandle
	{
		FVulkanAddress RHIAddress;
		uint32 HandleIncrement = 1;

		FVulkanAddressHandle operator +(uint32 Offset) const
		{
			return
			{
				RHIAddress + Offset * HandleIncrement,
				HandleIncrement
			};
		}

		FVulkanAddressHandle & operator +=(uint32 Offset)
		{
			RHIAddress = RHIAddress + Offset * HandleIncrement;
			return *this;
		}
	};

	struct FVulkanCopyableFootprint
	{
		uint64 Offset = 0;
		EFormat Format = EFormat::None;
		Size2U Size;
		uint32 Layer = 1;
		uint32 NumBytes = 0;
	};

	RHI_VULKAN_API VkFormat FromFormat(EFormat Format);
	RHI_VULKAN_API EFormat ToFormat(VkFormat Format);

	RHI_VULKAN_API VkFormat FromFormatSRV(EFormat Format);

	RHI_VULKAN_API VkPresentModeKHR FromPresentMode(EPresentMode PresentMode);

	forceinline VkImageViewType FromResourceViewDimension_RTV(EResourceViewDimension ResourceViewDimension)
	{
		switch (ResourceViewDimension)
		{
		case EResourceViewDimension::None:
			return VK_IMAGE_VIEW_TYPE_MAX_ENUM;
		case EResourceViewDimension::Buffer:
			return VK_IMAGE_VIEW_TYPE_2D;
		case EResourceViewDimension::Texture1D:
			return VK_IMAGE_VIEW_TYPE_1D;;
		case EResourceViewDimension::Texture1DArray:
			return VK_IMAGE_VIEW_TYPE_1D_ARRAY;
		case EResourceViewDimension::Texture2D:
			return VK_IMAGE_VIEW_TYPE_2D;
		case EResourceViewDimension::Texture2DArray:
			return VK_IMAGE_VIEW_TYPE_2D_ARRAY;
		case EResourceViewDimension::Texture3D:
			return VK_IMAGE_VIEW_TYPE_3D;
		default:
			return VK_IMAGE_VIEW_TYPE_MAX_ENUM;
		}
	}

	forceinline VkAttachmentLoadOp FromLoadAction(EResourceLoad LoadAction)
	{
		switch (LoadAction)
		{
		default:
		case EResourceLoad::None: return VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		case EResourceLoad::Load: return VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_LOAD;
		case EResourceLoad::Clear: return VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_CLEAR;
		case EResourceLoad::Discard: return VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		}
	}

	forceinline VkAttachmentStoreOp FromLoadAction(EResourceStoreAction StoreAction)
	{
		switch (StoreAction)
		{
		default:
		case EResourceStoreAction::None: return VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_STORE;
		case EResourceStoreAction::Discard: return VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_DONT_CARE;
		}
	}

	forceinline VkClearValue FromClearValue(const FClearValue & ClearValue)
	{
		VkClearValue Result {};
		switch (ClearValue.ClearMode)
		{
		case EClearMode::Color:
			ArrayCopy(Result.color.float32, ClearValue.Color.Values);
			break;
		case EClearMode::DepthStencil:
			Result.depthStencil.depth = ClearValue.Depth;
			Result.depthStencil.stencil = ClearValue.Stencil;
			break;
		default:
			break;
		}
		return Result;
	}

	forceinline VkPipelineStageFlags FromResourceAccess_PipelineStage(EResourceAccess ResourceAccess)
	{
		TEnumWrapper<VkPipelineStageFlags> Result {};
		Result.Set(VkPipelineStageFlagBits::VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, AnyFlags(ResourceAccess, EResourceAccess::Present));
		Result.Set(VkPipelineStageFlagBits::VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, AnyFlags(ResourceAccess, EResourceAccess::RenderTarget));
		//Result.Set(VkPipelineStageFlagBits::VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT, AnyFlags(ResourceAccess, EResourceAccess::DepthStencil));
		Result.Set(VkPipelineStageFlagBits::VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT, AnyFlags(ResourceAccess, EResourceAccess::DepthStencil));
		Result.Set(VkPipelineStageFlagBits::VK_PIPELINE_STAGE_VERTEX_SHADER_BIT, AnyFlags(ResourceAccess, EResourceAccess::VertexStage));
		Result.Set(VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT, AnyFlags(ResourceAccess, EResourceAccess::HullStage));
		Result.Set(VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT, AnyFlags(ResourceAccess, EResourceAccess::DomainStage));
		Result.Set(VkPipelineStageFlagBits::VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT, AnyFlags(ResourceAccess, EResourceAccess::GeometryStage));
		Result.Set(VkPipelineStageFlagBits::VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, AnyFlags(ResourceAccess, EResourceAccess::PixelStage));
		Result.Set(VkPipelineStageFlagBits::VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, AnyFlags(ResourceAccess, EResourceAccess::ComputeStage));
		return Result;
	}

	forceinline VkImageLayout FromResourceAccess_Layout(EResourceAccess ResourceAccess)
	{
		if (ResourceAccess & EResourceAccess::Common)
			return VK_IMAGE_LAYOUT_GENERAL;
		else if (ResourceAccess & EResourceAccess::Present)
			return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		else if (ResourceAccess & EResourceAccess::RenderTarget)
			return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		else if (ResourceAccess & EResourceAccess::DepthStencil)
			return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		else if (ResourceAccess & EResourceAccess::CopyRead)
			return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		else if (ResourceAccess & EResourceAccess::CopyWrite)
			return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		else if (ResourceAccess & EResourceAccess::StorageTexture)
			return VK_IMAGE_LAYOUT_GENERAL;

		else if (ResourceAccess & EResourceAccess::SampledTexture)
			return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		else if (ResourceAccess & EResourceAccess::UniformBuffer)
			return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		else if (ResourceAccess & EResourceAccess::StorageBuffer)
			return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		else if (ResourceAccess & EResourceAccess::UniformTexelBuffer)
			return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		else if (ResourceAccess & EResourceAccess::StorageTexelBuffer)
			return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

		else
			return VK_IMAGE_LAYOUT_UNDEFINED;
	}

	forceinline VkAccessFlags FromResourceAccess_AccessFlags(EResourceAccess ResourceAccess)
	{
		TEnumWrapper<VkAccessFlags> Result {};
		if (ResourceAccess & EResourceAccess::StagesMask)
		{
			Result.Set(VkAccessFlagBits::VK_ACCESS_UNIFORM_READ_BIT, ResourceAccess & (EResourceAccess::UniformBuffer | EResourceAccess::UniformTexelBuffer));
			//Result.Set(VkAccessFlagBits::VK_ACCESS_INPUT_ATTACHMENT_READ_BIT, ResourceAccess & (EResourceAccess::VertexBuffer | EResourceAccess::IndexBuffer));
			Result.Set(VkAccessFlagBits::VK_ACCESS_SHADER_READ_BIT, ResourceAccess & (EResourceAccess::UniformTexelBuffer | EResourceAccess::SampledTexture));
			Result.Set(VkAccessFlagBits::VK_ACCESS_SHADER_WRITE_BIT, ResourceAccess & (EResourceAccess::StorageBuffer | EResourceAccess::StorageTexelBuffer | EResourceAccess::StorageTexture));
		}
		else if (ResourceAccess & (EResourceAccess::RenderTarget | EResourceAccess::DepthRead | EResourceAccess::DepthWrite))
		{
			Result.Set(VkAccessFlagBits::VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, ResourceAccess & EResourceAccess::RenderTarget);
			Result.Set(VkAccessFlagBits::VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT, ResourceAccess & EResourceAccess::DepthRead);
			Result.Set(VkAccessFlagBits::VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT, ResourceAccess & EResourceAccess::DepthWrite);
		}
		else {}
		return Result;
	}

	forceinline VkShaderStageFlagBits FromShaderStage_FlagBits(EShaderStage ShaderStage)
	{
		switch (ShaderStage)
		{
		default:
		case EShaderStage::None: return VkShaderStageFlagBits(0);
		case EShaderStage::Vertex: return VK_SHADER_STAGE_VERTEX_BIT;
		case EShaderStage::Hull: return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
		case EShaderStage::Domain: return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
		case EShaderStage::Geometry: return VK_SHADER_STAGE_GEOMETRY_BIT;
		case EShaderStage::Pixel: return VK_SHADER_STAGE_FRAGMENT_BIT;
		case EShaderStage::Compute: return VK_SHADER_STAGE_COMPUTE_BIT;
		}
	}


	forceinline VkShaderStageFlagBits FromShaderRelevance(EShaderRelevance ShaderRelevance)
	{
		TEnumWrapper<VkShaderStageFlagBits> Result {};
		Result.Set(VK_SHADER_STAGE_VERTEX_BIT, ShaderRelevance & EShaderRelevance::Vertex);
		Result.Set(VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT, ShaderRelevance & EShaderRelevance::Hull);
		Result.Set(VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT, ShaderRelevance & EShaderRelevance::Domain);
		Result.Set(VK_SHADER_STAGE_GEOMETRY_BIT, ShaderRelevance & EShaderRelevance::Geometry);
		Result.Set(VK_SHADER_STAGE_FRAGMENT_BIT, ShaderRelevance & EShaderRelevance::Pixel);
		Result.Set(VK_SHADER_STAGE_COMPUTE_BIT, ShaderRelevance & EShaderRelevance::Compute);
		return Result;
	}

	//forceinline DXGI_SWAP_EFFECT FromSwapEffect(ESwapEffect SwapEffect)
	//{
	//	switch (SwapEffect)
	//	{
	//	case ESwapEffect::Discard:
	//		return DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD;
	//	case ESwapEffect::Sequential:
	//		return DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_SEQUENTIAL;
	//	case ESwapEffect::FlipSequential:
	//		return DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	//	case ESwapEffect::FlipDiscard:
	//		return DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;
	//	default:
	//		return DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD;
	//	}
	//}

	//forceinline DXGI_FORMAT FromFormatSRV(EFormat Format)
	//{
	//	switch (Format)
	//	{
	//	case EFormat::D16: return FromFormat(EFormat::R16UF);
	//	case EFormat::D24S8: return FromFormat(EFormat::R24G8);
	//	case EFormat::D32: return FromFormat(EFormat::R32F);
	//	case EFormat::R32: return FromFormat(EFormat::R32F);
	//	default: return FromFormat(Format);
	//	}
	//}

	//forceinline DXGI_FORMAT FromFormatUAV(EFormat Format)
	//{
	//	switch (Format)
	//	{
	//	case EFormat::D16: return FromFormat(EFormat::R16UF);
	//	case EFormat::D24S8: return FromFormat(EFormat::R24G8);
	//	case EFormat::D32: return FromFormat(EFormat::R32F);
	//	default: return FromFormat(Format);
	//	}
	//}

	forceinline VkFormat FromFormatDSV(EFormat Format)
	{
		switch (Format)
		{
		case EFormat::D16UF: return VK_FORMAT_D16_UNORM;
		case EFormat::D24UF_S8UI: return VK_FORMAT_D24_UNORM_S8_UINT;
		case EFormat::D32F: return VK_FORMAT_D32_SFLOAT;
		case EFormat::R32: return VK_FORMAT_D32_SFLOAT;
		default: return FromFormat(Format);
		}
	}

	forceinline VkSampleCountFlagBits FromMSAA(const FMSAA & MSAA)
	{
		switch (MSAA.Level)
		{
		default:
		case 1: return VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT;
		case 2: return VkSampleCountFlagBits::VK_SAMPLE_COUNT_2_BIT;
		case 4: return VkSampleCountFlagBits::VK_SAMPLE_COUNT_4_BIT;
		case 8: return VkSampleCountFlagBits::VK_SAMPLE_COUNT_8_BIT;
		case 16: return VkSampleCountFlagBits::VK_SAMPLE_COUNT_16_BIT;
		case 32: return VkSampleCountFlagBits::VK_SAMPLE_COUNT_32_BIT;
		case 64: return VkSampleCountFlagBits::VK_SAMPLE_COUNT_64_BIT;
		}
	}

	forceinline VkIndexType FromIndexFormat(EIndexFormat IndexFormat)
	{
		switch (IndexFormat)
		{
		default:
		case EIndexFormat::Index16: return VK_INDEX_TYPE_UINT16;
		case EIndexFormat::Index32: return VK_INDEX_TYPE_UINT32;
		}
	}

	//forceinline Vulkan_HEAP_TYPE FromHeapType(EHeapUsage HeapType)
	//{
	//	switch (HeapType)
	//	{
	//	case EHeapUsage::Default:
	//		return Vulkan_HEAP_TYPE::Vulkan_HEAP_TYPE_DEFAULT;
	//	case EHeapUsage::Upload:
	//		return Vulkan_HEAP_TYPE::Vulkan_HEAP_TYPE_UPLOAD;
	//	case EHeapUsage::Readback:
	//		return Vulkan_HEAP_TYPE::Vulkan_HEAP_TYPE_READBACK;
	//	default:
	//		return Vulkan_HEAP_TYPE::Vulkan_HEAP_TYPE_DEFAULT;
	//	}
	//}

	//forceinline EHeapUsage ToHeapType(Vulkan_HEAP_TYPE HeapType)
	//{
	//	switch (HeapType)
	//	{
	//	case Vulkan_HEAP_TYPE::Vulkan_HEAP_TYPE_DEFAULT:
	//		return EHeapUsage::Default;
	//	case Vulkan_HEAP_TYPE::Vulkan_HEAP_TYPE_UPLOAD:
	//		return EHeapUsage::Upload;
	//	case Vulkan_HEAP_TYPE::Vulkan_HEAP_TYPE_READBACK:
	//		return EHeapUsage::Readback;
	//	default:
	//		return EHeapUsage::Default;
	//	}
	//}

	//forceinline Vulkan_COMMAND_LIST_TYPE FromCommandType(ECommandType CommandType)
	//{
	//	switch (CommandType)
	//	{
	//	case ECommandType::Direct:
	//		return Vulkan_COMMAND_LIST_TYPE::Vulkan_COMMAND_LIST_TYPE_DIRECT;
	//	case ECommandType::Bundle:
	//		return Vulkan_COMMAND_LIST_TYPE::Vulkan_COMMAND_LIST_TYPE_BUNDLE;
	//	case ECommandType::Compute:
	//		return Vulkan_COMMAND_LIST_TYPE::Vulkan_COMMAND_LIST_TYPE_COMPUTE;
	//	case ECommandType::Copy:
	//		return Vulkan_COMMAND_LIST_TYPE::Vulkan_COMMAND_LIST_TYPE_COPY;
	//	default:
	//		return Vulkan_COMMAND_LIST_TYPE::Vulkan_COMMAND_LIST_TYPE_DIRECT;
	//	}
	//}

	//forceinline Vulkan_RESOURCE_DIMENSION FromResourceDimension(EResourceDimension ResourceDimension)
	//{
	//	switch (ResourceDimension)
	//	{
	//	case EResourceDimension::None:
	//		return Vulkan_RESOURCE_DIMENSION::Vulkan_RESOURCE_DIMENSION_UNKNOWN;
	//	case EResourceDimension::Buffer:
	//		return Vulkan_RESOURCE_DIMENSION::Vulkan_RESOURCE_DIMENSION_BUFFER;
	//	case EResourceDimension::Texture1D:
	//		return Vulkan_RESOURCE_DIMENSION::Vulkan_RESOURCE_DIMENSION_TEXTURE1D;
	//	case EResourceDimension::Texture2D:
	//		return Vulkan_RESOURCE_DIMENSION::Vulkan_RESOURCE_DIMENSION_TEXTURE2D;
	//	case EResourceDimension::Texture3D:
	//		return Vulkan_RESOURCE_DIMENSION::Vulkan_RESOURCE_DIMENSION_TEXTURE3D;
	//	default:
	//		return Vulkan_RESOURCE_DIMENSION::Vulkan_RESOURCE_DIMENSION_UNKNOWN;
	//	}
	//}

	//forceinline EResourceDimension ToResourceDimension(Vulkan_RESOURCE_DIMENSION ResourceDimension)
	//{
	//	switch (ResourceDimension)
	//	{
	//	case Vulkan_RESOURCE_DIMENSION::Vulkan_RESOURCE_DIMENSION_UNKNOWN:
	//		return EResourceDimension::None;
	//	case Vulkan_RESOURCE_DIMENSION::Vulkan_RESOURCE_DIMENSION_BUFFER:
	//		return EResourceDimension::Buffer;
	//	case Vulkan_RESOURCE_DIMENSION::Vulkan_RESOURCE_DIMENSION_TEXTURE1D:
	//		return EResourceDimension::Texture1D;
	//	case Vulkan_RESOURCE_DIMENSION::Vulkan_RESOURCE_DIMENSION_TEXTURE2D:
	//		return EResourceDimension::Texture2D;
	//	case Vulkan_RESOURCE_DIMENSION::Vulkan_RESOURCE_DIMENSION_TEXTURE3D:
	//		return EResourceDimension::Texture3D;
	//	default:
	//		return EResourceDimension::None;
	//	}
	//}

	forceinline VkImageAspectFlagBits FromResourceUsage_AspectFlag(EResourceDimension Dimension, EResourceUsage Usage, EFormat Format)
	{
		TEnumWrapper<VkImageAspectFlagBits> Result;

		switch (Dimension)
		{
		default:
		case EResourceDimension::Buffer:
			break;
		case EResourceDimension::Texture1D:
		case EResourceDimension::Texture2D:
		case EResourceDimension::Texture3D:
			Result.Set(VK_IMAGE_ASPECT_COLOR_BIT, Usage & (EResourceUsage::RenderTarget | EResourceUsage::SampledTexture));
			Result.Set(VK_IMAGE_ASPECT_DEPTH_BIT, Usage & EResourceUsage::DepthStencil);
			Result.Set(VK_IMAGE_ASPECT_STENCIL_BIT, (Usage & EResourceUsage::DepthStencil) && IsStencilFormat(Format));
			break;
		}
		return Result;
	}

	forceinline VkImageUsageFlags FromResourceUsage(EResourceDimension Dimension, EResourceUsage Usage)
	{
		TEnumWrapper<VkImageUsageFlags> Result;

		switch (Dimension)
		{
		default:
		case EResourceDimension::Buffer:
			Result.Set(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, Usage & EResourceUsage::UniformBuffer);
			Result.Set(VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, Usage & EResourceUsage::StorageBuffer);
			Result.Set(VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT, Usage & EResourceUsage::UniformTexelBuffer);
			Result.Set(VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT, Usage & EResourceUsage::StorageTexelBuffer);

			Result.Set(VK_BUFFER_USAGE_INDEX_BUFFER_BIT, Usage & EResourceUsage::IndexBuffer);
			Result.Set(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, Usage & EResourceUsage::VertexBuffer);
			Result.Set(VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT, Usage & EResourceUsage::IndirectBuffer);
			Result.Set(VK_BUFFER_USAGE_TRANSFER_SRC_BIT, Usage & EResourceUsage::CopyRead);
			Result.Set(VK_BUFFER_USAGE_TRANSFER_DST_BIT, Usage & EResourceUsage::CopyWrite);
			break;
		case EResourceDimension::Texture1D:
		case EResourceDimension::Texture2D:
		case EResourceDimension::Texture3D:
			Result.Set(VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, Usage & EResourceUsage::RenderTarget);
			Result.Set(VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, Usage & EResourceUsage::DepthStencil);
			Result.Set(VK_IMAGE_USAGE_SAMPLED_BIT, Usage & EResourceUsage::SampledTexture);
			Result.Set(VK_IMAGE_USAGE_STORAGE_BIT, Usage & EResourceUsage::StorageTexture);
			Result.Set(VK_IMAGE_USAGE_TRANSFER_SRC_BIT, Usage & EResourceUsage::CopyRead);
			Result.Set(VK_IMAGE_USAGE_TRANSFER_DST_BIT, Usage & EResourceUsage::CopyWrite);
			break;
		}
		return Result;
	}

	forceinline VkImageCreateFlags FromResourceFlags(EResourceDimension Dimension, EResourceFlags Flags)
	{
		TEnumWrapper<VkImageCreateFlags> Result;

		switch (Dimension)
		{
		default:
		case EResourceDimension::Buffer:
			break;
		case EResourceDimension::Texture1D:
		case EResourceDimension::Texture2D:
		case EResourceDimension::Texture3D:
			//Result.Set(VK_IMAGE_CREATE_BLOCK_TEXEL_VIEW_COMPATIBLE_BIT, Flags & EResourceFlags::MutableFormat);
			Result.Set(VK_IMAGE_CREATE_MUTABLE_FORMAT_BIT, Flags & EResourceFlags::MutableFormat);
			break;
		}
		return Result;
	}

	inline VkBufferCreateInfo BufferCreateInfoFromResourceDesc(const FResourceDesc & ResourceDesc)
	{
		// Create optimal tiled target image on the device
		VkBufferCreateInfo BufferCI { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
		BufferCI.size = ResourceDesc.Size.X * ResourceDesc.Size.Y;
		BufferCI.usage = FromResourceUsage(EResourceDimension::Buffer, ResourceDesc.Usage);
		BufferCI.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		BufferCI.queueFamilyIndexCount = 0;
		return BufferCI;
	}

	inline VkImageCreateInfo ImageCreateInfoFromResourceDesc(const FResourceDesc & ResourceDesc, VkImageLayout InitialLayout)
	{
		// Create optimal tiled target image on the device
		VkImageCreateInfo ImageCI { VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO };
		ImageCI.imageType = VK_IMAGE_TYPE_2D;
		ImageCI.format = FromFormat(ResourceDesc.Format);
		ImageCI.mipLevels = ResourceDesc.Mipmaps;
		ImageCI.arrayLayers = ResourceDesc.Layers;
		ImageCI.samples = FromMSAA(ResourceDesc.MSAA);
		ImageCI.tiling = VK_IMAGE_TILING_OPTIMAL;
		ImageCI.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		ImageCI.initialLayout = InitialLayout;
		ImageCI.extent = { ResourceDesc.Size.Width, ResourceDesc.Size.Height, 1 };
		ImageCI.usage = FromResourceUsage(ResourceDesc.Dimension, ResourceDesc.Usage);
		ImageCI.flags = FromResourceFlags(ResourceDesc.Dimension, ResourceDesc.Flags);
		return ImageCI;
	}

	//forceinline EResourceUsage ToResourceUsage(Vulkan_RESOURCE_FLAGS VulkanResourceFlags)
	//{
	//	TEnumWrapper<EResourceUsage> Result;
	//	Result.Set(EResourceUsage::RenderTarget, VulkanResourceFlags & Vulkan_RESOURCE_FLAGS::Vulkan_RESOURCE_FLAG_ALLOW_RENDER_TARGET);
	//	Result.Set(EResourceUsage::DepthStencil, VulkanResourceFlags & Vulkan_RESOURCE_FLAGS::Vulkan_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);
	//	Result.Set(EResourceUsage::ShaderResource, !(VulkanResourceFlags & Vulkan_RESOURCE_FLAGS::Vulkan_RESOURCE_FLAG_DENY_SHADER_RESOURCE));
	//	Result.Set(EResourceUsage::UnorderedAccess, VulkanResourceFlags & Vulkan_RESOURCE_FLAGS::Vulkan_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
	//	Result.Set(EResourceUsage::CrossAdapter, VulkanResourceFlags & Vulkan_RESOURCE_FLAGS::Vulkan_RESOURCE_FLAG_ALLOW_CROSS_ADAPTER);
	//	return Result;
	//}

	//forceinline Vulkan_HEAP_FLAGS FromHeapFlags(EHeapFlag HeapFlag)
	//{
	//	Vulkan_HEAP_FLAGS Result {};
	//	SetFlag(Result, Vulkan_HEAP_FLAG_SHARED, HeapFlag & EHeapFlag::Interoperable);
	//	SetFlag(Result, Vulkan_HEAP_FLAG_ALLOW_WRITE_WATCH, HeapFlag & EHeapFlag::WriteWatch);
	//	SetFlag(Result, Vulkan_HEAP_FLAG_SHARED_CROSS_ADAPTER, HeapFlag & EHeapFlag::CrossAdapter);
	//	SetFlag(Result, Vulkan_HEAP_FLAG_CREATE_NOT_ZEROED, !(HeapFlag & EHeapFlag::ZeroInitialize));

	//	SetFlag(Result, Vulkan_HEAP_FLAG_DENY_BUFFERS, !(HeapFlag & EHeapFlag::Allow_Buffers));
	//	SetFlag(Result, Vulkan_HEAP_FLAG_DENY_NON_RT_DS_TEXTURES, (HeapFlag & EHeapFlag::Allow_Mask) == EHeapFlag::Allow_RTsDSs);
	//	SetFlag(Result, Vulkan_HEAP_FLAG_DENY_RT_DS_TEXTURES, !(HeapFlag & EHeapFlag::Allow_RTsDSs));
	//	return Result;
	//}

	//forceinline Vulkan_HEAP_FLAGS FromCommittedHeapFlags(EHeapFlag HeapFlag)
	//{
	//	Vulkan_HEAP_FLAGS Result {};
	//	SetFlag(Result, Vulkan_HEAP_FLAG_SHARED, HeapFlag & EHeapFlag::Interoperable);
	//	SetFlag(Result, Vulkan_HEAP_FLAG_ALLOW_WRITE_WATCH, HeapFlag & EHeapFlag::WriteWatch);
	//	SetFlag(Result, Vulkan_HEAP_FLAG_SHARED_CROSS_ADAPTER, HeapFlag & EHeapFlag::CrossAdapter);
	//	return Result;
	//}

	//forceinline Vulkan_CPU_PAGE_PROPERTY FromCPUPageProperty(EHeapFlag HeapFlags)
	//{
	//	switch (HeapFlags & EHeapFlag::CPU_Mask)
	//	{
	//	case EHeapFlag::CPU_NotAvailabe:
	//		return Vulkan_CPU_PAGE_PROPERTY::Vulkan_CPU_PAGE_PROPERTY_NOT_AVAILABLE;
	//	case EHeapFlag::CPU_WriteCombine:
	//		return Vulkan_CPU_PAGE_PROPERTY::Vulkan_CPU_PAGE_PROPERTY_WRITE_COMBINE;
	//	case EHeapFlag::CPU_WriteBack:
	//		return Vulkan_CPU_PAGE_PROPERTY::Vulkan_CPU_PAGE_PROPERTY_WRITE_BACK;
	//	default:
	//		return Vulkan_CPU_PAGE_PROPERTY::Vulkan_CPU_PAGE_PROPERTY_UNKNOWN;
	//	}
	//}

	//forceinline Vulkan_MEMORY_POOL FromMemoryPool(EHeapFlag HeapFlags)
	//{
	//	switch (HeapFlags & EHeapFlag::MemoryPool_Mask)
	//	{
	//	case EHeapFlag::MemoryPool_L0:
	//		return Vulkan_MEMORY_POOL::Vulkan_MEMORY_POOL_L0;
	//	case EHeapFlag::MemoryPool_L1:
	//		return Vulkan_MEMORY_POOL::Vulkan_MEMORY_POOL_L1;
	//	default:
	//		return Vulkan_MEMORY_POOL::Vulkan_MEMORY_POOL_UNKNOWN;
	//	}
	//}

	//forceinline Vulkan_RESOURCE_STATES FromResourceAccess(EResourceAccess ResourceAccess)
	//{
	//	if (ResourceAccess == EResourceAccess::None)
	//		return Vulkan_RESOURCE_STATE_COMMON;

	//	if (ResourceAccess == EResourceAccess::Common)
	//		return Vulkan_RESOURCE_STATE_COMMON;

	//	if (ResourceAccess == EResourceAccess::PipelineRead)
	//		return Vulkan_RESOURCE_STATE_GENERIC_READ;

	//	TEnumWrapper<Vulkan_RESOURCE_STATES> Result;
	//	Result.Set(Vulkan_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, ResourceAccess & (EResourceAccess::VertexBuffer | EResourceAccess::ConstBuffer));
	//	Result.Set(Vulkan_RESOURCE_STATE_INDEX_BUFFER, ResourceAccess & EResourceAccess::IndexBuffer);
	//	Result.Set(Vulkan_RESOURCE_STATE_RENDER_TARGET, ResourceAccess & EResourceAccess::RenderTarget);
	//	Result.Set(Vulkan_RESOURCE_STATE_UNORDERED_ACCESS, ResourceAccess & EResourceAccess::UnorderedAccess);
	//	Result.Set(Vulkan_RESOURCE_STATE_DEPTH_WRITE, ResourceAccess & EResourceAccess::DepthWrite);
	//	Result.Set(Vulkan_RESOURCE_STATE_DEPTH_READ, ResourceAccess & EResourceAccess::DepthRead);
	//	Result.Set(Vulkan_RESOURCE_STATE_INDIRECT_ARGUMENT, ResourceAccess & EResourceAccess::IndirectParameter);
	//	Result.Set(Vulkan_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE, ResourceAccess & (
	//		EResourceAccess::VertexShaderResource |
	//		EResourceAccess::HullShaderResource |
	//		EResourceAccess::DomainShaderResource |
	//		EResourceAccess::GeometryShaderResource |
	//		EResourceAccess::ComputeShaderResource));
	//	Result.Set(Vulkan_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, ResourceAccess & EResourceAccess::PixelShaderResource);
	//	Result.Set(Vulkan_RESOURCE_STATE_COPY_SOURCE, ResourceAccess & EResourceAccess::CopySource);
	//	Result.Set(Vulkan_RESOURCE_STATE_COPY_DEST, ResourceAccess & EResourceAccess::CopyDest);
	//	Result.Set(Vulkan_RESOURCE_STATE_RESOLVE_SOURCE, ResourceAccess & EResourceAccess::ResolveSource);
	//	Result.Set(Vulkan_RESOURCE_STATE_RESOLVE_DEST, ResourceAccess & EResourceAccess::ResolveDest);
	//	Result.Set(Vulkan_RESOURCE_STATE_PRESENT, ResourceAccess & EResourceAccess::Present);
	//	return Result;
	//}

	//forceinline Vulkan_SRV_DIMENSION FromResourceViewDimension(EResourceViewDimension ResourceViewDimension, bool MSAA = false)
	//{
	//	switch (ResourceViewDimension)
	//	{
	//	case EResourceViewDimension::None:
	//		return Vulkan_SRV_DIMENSION::Vulkan_SRV_DIMENSION_UNKNOWN;
	//	case EResourceViewDimension::Buffer:
	//		return Vulkan_SRV_DIMENSION::Vulkan_SRV_DIMENSION_BUFFER;
	//	case EResourceViewDimension::Texture1D:
	//		return Vulkan_SRV_DIMENSION::Vulkan_SRV_DIMENSION_TEXTURE1D;
	//	case EResourceViewDimension::Texture1DArray:
	//		return Vulkan_SRV_DIMENSION::Vulkan_SRV_DIMENSION_TEXTURE1DARRAY;
	//	case EResourceViewDimension::Texture2D:
	//		return MSAA ? Vulkan_SRV_DIMENSION::Vulkan_SRV_DIMENSION_TEXTURE2DMS : Vulkan_SRV_DIMENSION::Vulkan_SRV_DIMENSION_TEXTURE2D;
	//	case EResourceViewDimension::Texture2DArray:
	//		return MSAA ? Vulkan_SRV_DIMENSION::Vulkan_SRV_DIMENSION_TEXTURE2DARRAY : Vulkan_SRV_DIMENSION::Vulkan_SRV_DIMENSION_TEXTURE2DARRAY;
	//	case EResourceViewDimension::Texture3D:
	//		return Vulkan_SRV_DIMENSION::Vulkan_SRV_DIMENSION_TEXTURE3D;
	//	case EResourceViewDimension::TextureCube:
	//		return Vulkan_SRV_DIMENSION::Vulkan_SRV_DIMENSION_TEXTURECUBE;
	//	case EResourceViewDimension::TextureCubeArray:
	//		return Vulkan_SRV_DIMENSION::Vulkan_SRV_DIMENSION_TEXTURECUBEARRAY;
	//	default:
	//		return Vulkan_SRV_DIMENSION::Vulkan_SRV_DIMENSION_UNKNOWN;
	//	}
	//}

	//forceinline EResourceViewDimension ToResourceViewDimension(Vulkan_SRV_DIMENSION ResourceViewDimension, bool MSAA = false)
	//{
	//	switch (ResourceViewDimension)
	//	{
	//	case Vulkan_SRV_DIMENSION::Vulkan_SRV_DIMENSION_UNKNOWN:
	//		return EResourceViewDimension::None;
	//	case Vulkan_SRV_DIMENSION::Vulkan_SRV_DIMENSION_BUFFER:
	//		return EResourceViewDimension::Buffer;
	//	case Vulkan_SRV_DIMENSION::Vulkan_SRV_DIMENSION_TEXTURE1D:
	//		return EResourceViewDimension::Texture1D;
	//	case Vulkan_SRV_DIMENSION::Vulkan_SRV_DIMENSION_TEXTURE1DARRAY:
	//		return EResourceViewDimension::Texture1DArray;
	//	case Vulkan_SRV_DIMENSION::Vulkan_SRV_DIMENSION_TEXTURE2D:
	//	case Vulkan_SRV_DIMENSION::Vulkan_SRV_DIMENSION_TEXTURE2DMS:
	//		return EResourceViewDimension::Texture2D;
	//	case Vulkan_SRV_DIMENSION::Vulkan_SRV_DIMENSION_TEXTURE2DARRAY:
	//	case Vulkan_SRV_DIMENSION::Vulkan_SRV_DIMENSION_TEXTURE2DMSARRAY:
	//		return EResourceViewDimension::Texture2DArray;
	//	case Vulkan_SRV_DIMENSION::Vulkan_SRV_DIMENSION_TEXTURE3D:
	//		return EResourceViewDimension::Texture3D;
	//	case Vulkan_SRV_DIMENSION::Vulkan_SRV_DIMENSION_TEXTURECUBE:
	//		return EResourceViewDimension::TextureCube;
	//	case Vulkan_SRV_DIMENSION::Vulkan_SRV_DIMENSION_TEXTURECUBEARRAY:
	//		return EResourceViewDimension::TextureCubeArray;
	//	default:
	//		return EResourceViewDimension::None;
	//	}
	//}

	//forceinline Vulkan_RTV_DIMENSION FromResourceViewDimension_RenderTargetView(EResourceViewDimension ResourceViewDimension, bool MSAA = false)
	//{
	//	switch (ResourceViewDimension)
	//	{
	//	case EResourceViewDimension::None:
	//		return Vulkan_RTV_DIMENSION::Vulkan_RTV_DIMENSION_UNKNOWN;
	//	case EResourceViewDimension::Buffer:
	//		return Vulkan_RTV_DIMENSION::Vulkan_RTV_DIMENSION_BUFFER;
	//	case EResourceViewDimension::Texture1D:
	//		return Vulkan_RTV_DIMENSION::Vulkan_RTV_DIMENSION_TEXTURE1D;
	//	case EResourceViewDimension::Texture1DArray:
	//		return Vulkan_RTV_DIMENSION::Vulkan_RTV_DIMENSION_TEXTURE1DARRAY;
	//	case EResourceViewDimension::Texture2D:
	//		return MSAA ? Vulkan_RTV_DIMENSION::Vulkan_RTV_DIMENSION_TEXTURE2DMS : Vulkan_RTV_DIMENSION::Vulkan_RTV_DIMENSION_TEXTURE2D;
	//	case EResourceViewDimension::Texture2DArray:
	//		return MSAA ? Vulkan_RTV_DIMENSION::Vulkan_RTV_DIMENSION_TEXTURE2DMSARRAY : Vulkan_RTV_DIMENSION::Vulkan_RTV_DIMENSION_TEXTURE2DARRAY;
	//	case EResourceViewDimension::Texture3D:
	//		return Vulkan_RTV_DIMENSION::Vulkan_RTV_DIMENSION_TEXTURE3D;
	//	default:
	//		return Vulkan_RTV_DIMENSION::Vulkan_RTV_DIMENSION_UNKNOWN;
	//	}
	//}

	//forceinline Vulkan_DSV_DIMENSION FromResourceViewDimension_DepthStencilView(EResourceViewDimension ResourceViewDimension, bool MSAA = false)
	//{
	//	switch (ResourceViewDimension)
	//	{
	//	case EResourceViewDimension::None:
	//		return Vulkan_DSV_DIMENSION::Vulkan_DSV_DIMENSION_UNKNOWN;
	//	case EResourceViewDimension::Texture1D:
	//		return Vulkan_DSV_DIMENSION::Vulkan_DSV_DIMENSION_TEXTURE1D;
	//	case EResourceViewDimension::Texture1DArray:
	//		return Vulkan_DSV_DIMENSION::Vulkan_DSV_DIMENSION_TEXTURE1DARRAY;
	//	case EResourceViewDimension::Texture2D:
	//		return MSAA ? Vulkan_DSV_DIMENSION::Vulkan_DSV_DIMENSION_TEXTURE2D : Vulkan_DSV_DIMENSION::Vulkan_DSV_DIMENSION_TEXTURE2DMS;
	//	case EResourceViewDimension::Texture2DArray:
	//		return MSAA ? Vulkan_DSV_DIMENSION::Vulkan_DSV_DIMENSION_TEXTURE2DARRAY : Vulkan_DSV_DIMENSION::Vulkan_DSV_DIMENSION_TEXTURE2DMSARRAY;
	//	default:
	//		return Vulkan_DSV_DIMENSION::Vulkan_DSV_DIMENSION_UNKNOWN;
	//	}
	//}

	//forceinline Vulkan_UAV_DIMENSION FromResourceViewDimension_UAV(EResourceViewDimension ResourceViewDimension)
	//{
	//	switch (ResourceViewDimension)
	//	{
	//	case EResourceViewDimension::None:
	//		return Vulkan_UAV_DIMENSION::Vulkan_UAV_DIMENSION_UNKNOWN;
	//	case EResourceViewDimension::Buffer:
	//		return Vulkan_UAV_DIMENSION::Vulkan_UAV_DIMENSION_BUFFER;
	//	case EResourceViewDimension::Texture1D:
	//		return Vulkan_UAV_DIMENSION::Vulkan_UAV_DIMENSION_TEXTURE1D;
	//	case EResourceViewDimension::Texture1DArray:
	//		return Vulkan_UAV_DIMENSION::Vulkan_UAV_DIMENSION_TEXTURE1DARRAY;
	//	case EResourceViewDimension::Texture2D:
	//		return Vulkan_UAV_DIMENSION::Vulkan_UAV_DIMENSION_TEXTURE2D;
	//	case EResourceViewDimension::Texture2DArray:
	//		return Vulkan_UAV_DIMENSION::Vulkan_UAV_DIMENSION_TEXTURE2DARRAY;
	//	case EResourceViewDimension::Texture3D:
	//		return Vulkan_UAV_DIMENSION::Vulkan_UAV_DIMENSION_TEXTURE3D;
	//	default:
	//		return Vulkan_UAV_DIMENSION::Vulkan_UAV_DIMENSION_UNKNOWN;
	//	}
	//}

	//forceinline Vulkan_DESCRIPTOR_HEAP_TYPE FromDescriptorHeapType(EDescriptorHeapType DescriptorHeapType)
	//{
	//	switch (DescriptorHeapType)
	//	{
	//	case EDescriptorHeapType::ShaderResource:
	//		return Vulkan_DESCRIPTOR_HEAP_TYPE::Vulkan_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	//	case EDescriptorHeapType::RenderTarget:
	//		return Vulkan_DESCRIPTOR_HEAP_TYPE::Vulkan_DESCRIPTOR_HEAP_TYPE_RTV;
	//	case EDescriptorHeapType::DepthStencil:
	//		return Vulkan_DESCRIPTOR_HEAP_TYPE::Vulkan_DESCRIPTOR_HEAP_TYPE_DSV;
	//	case EDescriptorHeapType::Sampler:
	//		return Vulkan_DESCRIPTOR_HEAP_TYPE::Vulkan_DESCRIPTOR_HEAP_TYPE_SAMPLER;
	//	default:
	//		Assert(false, u8"Invalid DescriptorHeapType"V);
	//		return Vulkan_DESCRIPTOR_HEAP_TYPE::Vulkan_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	//	}
	//}

	//forceinline Vulkan_DESCRIPTOR_HEAP_FLAGS FromDescriptorHeapFlags(EDescriptorHeapFlag ResourcePacketFlags)
	//{
	//	TEnumWrapper<Vulkan_DESCRIPTOR_HEAP_FLAGS> Result;
	//	Result.Set(Vulkan_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, ResourcePacketFlags & EDescriptorHeapFlag::ShaderVisible);
	//	return Result;
	//}

	forceinline VkDescriptorType FromBindingType(EDescriptorType DescriptorType, EResourceDimension Dimension = EResourceDimension::None)
	{
		switch (DescriptorType)
		{
		case EDescriptorType::UniformBuffer: 
			return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

		case EDescriptorType::StorageBuffer:
			return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;

		case EDescriptorType::UniformTexelBuffer: 
			return VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;

		case EDescriptorType::StorageTexelBuffer: 
			return VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;

		case EDescriptorType::SampledTexture: 
			return VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;

		case EDescriptorType::StorageTexture: 
			return VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;

		case EDescriptorType::Sampler:
			return VK_DESCRIPTOR_TYPE_SAMPLER;

		default:
			AssertFailed(u8"Unkonwn DescriptorType."V);
			return VK_DESCRIPTOR_TYPE_MAX_ENUM;
		}
	}

	//forceinline Vulkan_ROOT_PARAMETER_TYPE FromRootParameterType(EVulkanRootParameterType RootDescriptorType)
	//{
	//	switch (RootDescriptorType)
	//	{
	//	case EVulkanRootParameterType::DescriptorTable:
	//		return Vulkan_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	//	case EVulkanRootParameterType::UInt32Constant:
	//		return Vulkan_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	//	case EVulkanRootParameterType::ConstBuffer:
	//		return Vulkan_ROOT_PARAMETER_TYPE_CBV;
	//	case EVulkanRootParameterType::ShaderResource:
	//		return Vulkan_ROOT_PARAMETER_TYPE_SRV;
	//	case EVulkanRootParameterType::UnorderedAccess:
	//		return Vulkan_ROOT_PARAMETER_TYPE_UAV;
	//	default:
	//		return Vulkan_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	//	}
	//}

	//forceinline Vulkan_SHADER_VISIBILITY FromShaderVisibility(EShaderVisibility ShaderVisibility)
	//{
	//	switch (ShaderVisibility)
	//	{
	//	case EShaderVisibility::All:
	//		return Vulkan_SHADER_VISIBILITY::Vulkan_SHADER_VISIBILITY_ALL;
	//	case EShaderVisibility::Vertex:
	//		return Vulkan_SHADER_VISIBILITY::Vulkan_SHADER_VISIBILITY_VERTEX;
	//	case EShaderVisibility::Hull:
	//		return Vulkan_SHADER_VISIBILITY::Vulkan_SHADER_VISIBILITY_HULL;
	//	case EShaderVisibility::Domain:
	//		return Vulkan_SHADER_VISIBILITY::Vulkan_SHADER_VISIBILITY_DOMAIN;
	//	case EShaderVisibility::Geoetry:
	//		return Vulkan_SHADER_VISIBILITY::Vulkan_SHADER_VISIBILITY_GEOMETRY;
	//	case EShaderVisibility::Pixel:
	//		return Vulkan_SHADER_VISIBILITY::Vulkan_SHADER_VISIBILITY_PIXEL;
	//	default:
	//		return Vulkan_SHADER_VISIBILITY::Vulkan_SHADER_VISIBILITY_ALL;
	//	}
	//}

	forceinline VkBlendFactor FromBlendFactor(EBlendFactor BlendFactor)
	{
		switch (BlendFactor)
		{
		default:
		case EBlendFactor::None:
		case EBlendFactor::Zero: return VK_BLEND_FACTOR_ZERO;
		case EBlendFactor::One: return VK_BLEND_FACTOR_ONE;
		case EBlendFactor::SrcColor: return VK_BLEND_FACTOR_SRC_COLOR;
		case EBlendFactor::SrcColorInv: return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
		case EBlendFactor::SrcAlpha: return VK_BLEND_FACTOR_SRC_ALPHA;
		case EBlendFactor::SrcAlphaInv: return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		case EBlendFactor::DestAlpha: return VK_BLEND_FACTOR_DST_ALPHA;
		case EBlendFactor::DestAlphaInv: return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
		case EBlendFactor::DestColor: return VK_BLEND_FACTOR_DST_COLOR;
		case EBlendFactor::DestColorInv: return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
		case EBlendFactor::BlendFactor: return VK_BLEND_FACTOR_CONSTANT_ALPHA;
		case EBlendFactor::BlendFactorInv: return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA;
		case EBlendFactor::Src1Color: return VK_BLEND_FACTOR_SRC1_COLOR;
		case EBlendFactor::Src1ColorInv: return VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR;
		case EBlendFactor::Src1Alpha: return VK_BLEND_FACTOR_SRC_ALPHA;
		case EBlendFactor::Src1AlphaInv: return VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA;
		}
	}

	forceinline VkBlendOp FromBlendOP(EBlendAlgorithm BlendAlgorithm)
	{
		switch (BlendAlgorithm)
		{
		default:
		case EBlendAlgorithm::Add: return VK_BLEND_OP_ADD;
		case EBlendAlgorithm::Subtract: return VK_BLEND_OP_SUBTRACT;
		case EBlendAlgorithm::Min: return VK_BLEND_OP_MIN;
		case EBlendAlgorithm::Max: return VK_BLEND_OP_MAX;
		}
	}

	forceinline VkCullModeFlagBits FromCullMode(ECullMode CullMode)
	{
		// VK_CULL_MODE_FRONT_BIT specifies that front-facing triangles are discarded
		// VK_CULL_MODE_BACK_BIT specifies that back-facing triangles are discarded
		switch (CullMode)
		{
		default:
		case ECullMode::None: return VK_CULL_MODE_NONE;
		case ECullMode::Front: return VK_CULL_MODE_FRONT_BIT;
		case ECullMode::Back: return VK_CULL_MODE_BACK_BIT;
		}
	}

	forceinline VkColorComponentFlags FromWriteMasks(EWriteMasks WriteMasks)
	{
		TEnumWrapper<VkColorComponentFlags> Result;
		Result.Set(VK_COLOR_COMPONENT_R_BIT, WriteMasks & EWriteMasks::Red);
		Result.Set(VK_COLOR_COMPONENT_G_BIT, WriteMasks & EWriteMasks::Green);
		Result.Set(VK_COLOR_COMPONENT_B_BIT, WriteMasks & EWriteMasks::Blue);
		Result.Set(VK_COLOR_COMPONENT_A_BIT, WriteMasks & EWriteMasks::Alpha);
		return Result;
	}

	forceinline VkPrimitiveTopology FromTopology(ETopology Topology, EPrimitiveTopology PrimitiveTopology)
	{
		if (PrimitiveTopology != EPrimitiveTopology::None)
		{
			switch (PrimitiveTopology)
			{
			default:
			case EPrimitiveTopology::None:
			case EPrimitiveTopology::Point: return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
			case EPrimitiveTopology::Line: return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
				//case ETopology::LineStrip:return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
			case EPrimitiveTopology::Triangle: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
				//case ETopology::TriangleStrip: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
			case EPrimitiveTopology::Patch :return VK_PRIMITIVE_TOPOLOGY_PATCH_LIST;
			}
		}

		AssertExpr(Topology != ETopology::None);
		switch (Topology)
		{
		default:
		case ETopology::None:
		case ETopology::PointList: return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
		case ETopology::LineList: return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
		case ETopology::LineStrip: return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
		case ETopology::TriangleList: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		case ETopology::TriangleStrip: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
		case ETopology::PatchList: return VK_PRIMITIVE_TOPOLOGY_PATCH_LIST;
		}
	}

	//forceinline VkPrimitiveTopology FromPrimitiveType(EPrimitiveType Geometry)
	//{
	//	switch (Geometry)
	//	{
	//	default:
	//	case EPrimitiveType::None:
	//	case EPrimitiveType::Point: return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
	//	case EPrimitiveType::Line:return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
	//	case EPrimitiveType::Triangle:return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
	//	case EPrimitiveType::Patch:return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
	//	}
	//}

	forceinline VkFilter FromFilter(ESampleFilter Filter)
	{
		switch (Filter)
		{
		default:
		case ESampleFilter::Point: return VK_FILTER_NEAREST;
		case ESampleFilter::Trilinear:
		case ESampleFilter::Linear: return VK_FILTER_LINEAR;
		}
	}

	forceinline VkSamplerMipmapMode FromMipmapMode(ESampleFilter Filter)
	{
		switch (Filter)
		{
		default:
		case ESampleFilter::Point: return VK_SAMPLER_MIPMAP_MODE_NEAREST;
		case ESampleFilter::Trilinear: return VK_SAMPLER_MIPMAP_MODE_LINEAR;
		}
	}

	forceinline VkSamplerAddressMode FromAddressMode(EAddressMode AddressMode)
	{
		switch (AddressMode)
		{
		default:
		case EAddressMode::Clamp: return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		case EAddressMode::Wrap: return VK_SAMPLER_ADDRESS_MODE_REPEAT;
		case EAddressMode::Mirror: return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
		case EAddressMode::Border: return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
		}
	}

	forceinline VkCompareOp FromComparison(EComparison Comparison)
	{
		switch (Comparison)
		{
		default:
		case EComparison::None: return VK_COMPARE_OP_ALWAYS;
		case EComparison::Never: return VK_COMPARE_OP_NEVER;
		case EComparison::Always: return VK_COMPARE_OP_ALWAYS;
		case EComparison::Less: return VK_COMPARE_OP_LESS;
		case EComparison::LessEqual: return VK_COMPARE_OP_LESS_OR_EQUAL;
		case EComparison::Equal: return VK_COMPARE_OP_EQUAL;
		case EComparison::GreaterEqual: return VK_COMPARE_OP_GREATER_OR_EQUAL;
		case EComparison::Greater: return VK_COMPARE_OP_GREATER_OR_EQUAL;
		case EComparison::NotEqual: return VK_COMPARE_OP_NOT_EQUAL;
		}
	}

	forceinline VkBorderColor FromBorderColor(EBorderColor BorderColor)
	{
		switch (BorderColor)
		{
		case EBorderColor::TransparentBlack:
			return VkBorderColor::VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
		case EBorderColor::OpaqueBlack:
			return VkBorderColor::VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;
		case EBorderColor::OpaqueWhite:
			return VkBorderColor::VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
		default:
			return VkBorderColor::VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
		}
	}

	forceinline EResourceAccess GetInitialAccess(const FResourceDesc & ResourceDesc)
	{
		if (ResourceDesc.Usage & EResourceUsage::DepthStencil)
			return EResourceAccess::DepthWrite;

		if (ResourceDesc.Usage & EResourceUsage::WriteMask)
		{
			if (ResourceDesc.Usage & EResourceUsage::VertexBuffer)
				return EResourceAccess::VertexBuffer;

			if (ResourceDesc.Usage & EResourceUsage::UniformBuffer)
				return EResourceAccess::UniformBuffer;

			if (ResourceDesc.Usage & EResourceUsage::StorageBuffer)
				return EResourceAccess::ComputeStorageBuffer;

			if (ResourceDesc.Usage & EResourceUsage::UniformTexelBuffer)
				return EResourceAccess::UniformTexelBuffer;

			if (ResourceDesc.Usage & EResourceUsage::StorageTexelBuffer)
				return EResourceAccess::StorageTexelBuffer;

			if (ResourceDesc.Usage & EResourceUsage::IndexBuffer)
				return EResourceAccess::IndexBuffer;

			if (ResourceDesc.Usage & EResourceUsage::IndirectBuffer)
				return EResourceAccess::IndirectBuffer;

			if (ResourceDesc.Usage & EResourceUsage::ResolveRead)
				return EResourceAccess::ResolveRead;

			if (ResourceDesc.Usage & EResourceUsage::ResolveWrite)
				return EResourceAccess::ResolveWrite;

			if (ResourceDesc.Usage & EResourceUsage::RenderTarget)
				return EResourceAccess::RenderTarget;

			if (ResourceDesc.Usage & EResourceUsage::StorageTexture)
				return EResourceAccess::StorageTexture;

			if (ResourceDesc.Usage & EResourceUsage::SampledTexture)
				return EResourceAccess::PixelSampledTexture;

			if (ResourceDesc.Usage & EResourceUsage::CopyRead)
				return EResourceAccess::CopyRead;

			if (ResourceDesc.Usage & EResourceUsage::CopyWrite)
				return EResourceAccess::CopyWrite;

			return EResourceAccess::None;
		}
		else if (ResourceDesc.Usage & EResourceUsage::BufferMask)
		{
			return EResourceAccess::None;
		}
		else
		{
			return EResourceAccess::None;
		}
	}

	//forceinline Vulkan_INDIRECT_ARGUMENT_TYPE FromIndirectParameterType(EIndirectParameterType IndirectParameterType)
	//{
	//	switch (IndirectParameterType)
	//	{
	//	default:
	//	case EIndirectParameterType::Draw: return Vulkan_INDIRECT_ARGUMENT_TYPE::Vulkan_INDIRECT_ARGUMENT_TYPE_DRAW;
	//	case EIndirectParameterType::DrawIndexed: return Vulkan_INDIRECT_ARGUMENT_TYPE::Vulkan_INDIRECT_ARGUMENT_TYPE_DRAW_INDEXED;
	//	case EIndirectParameterType::Dispatch: return Vulkan_INDIRECT_ARGUMENT_TYPE::Vulkan_INDIRECT_ARGUMENT_TYPE_DISPATCH;
	//	case EIndirectParameterType::VertexBufferView: return Vulkan_INDIRECT_ARGUMENT_TYPE::Vulkan_INDIRECT_ARGUMENT_TYPE_VERTEX_BUFFER_VIEW;
	//	case EIndirectParameterType::IndexBufferView: return Vulkan_INDIRECT_ARGUMENT_TYPE::Vulkan_INDIRECT_ARGUMENT_TYPE_INDEX_BUFFER_VIEW;
	//	case EIndirectParameterType::Constannt: return Vulkan_INDIRECT_ARGUMENT_TYPE::Vulkan_INDIRECT_ARGUMENT_TYPE_CONSTANT;
	//	case EIndirectParameterType::CBV: return Vulkan_INDIRECT_ARGUMENT_TYPE::Vulkan_INDIRECT_ARGUMENT_TYPE_CONSTANT_BUFFER_VIEW;
	//	case EIndirectParameterType::SRV: return Vulkan_INDIRECT_ARGUMENT_TYPE::Vulkan_INDIRECT_ARGUMENT_TYPE_SHADER_RESOURCE_VIEW;
	//	case EIndirectParameterType::UAV: return Vulkan_INDIRECT_ARGUMENT_TYPE::Vulkan_INDIRECT_ARGUMENT_TYPE_UNORDERED_ACCESS_VIEW;
	//	case EIndirectParameterType::DispatchRays: return Vulkan_INDIRECT_ARGUMENT_TYPE::Vulkan_INDIRECT_ARGUMENT_TYPE_DISPATCH_RAYS;
	//	case EIndirectParameterType::DispatchMesh: return Vulkan_INDIRECT_ARGUMENT_TYPE::Vulkan_INDIRECT_ARGUMENT_TYPE_DISPATCH_MESH;
	//	}
	//}

	//forceinline Vulkan_ROOT_DESCRIPTOR_FLAGS FromRootDescriptorFlags(EDescriptorFlag DescriptorFlags)
	//{
	//	TEnumWrapper<Vulkan_ROOT_DESCRIPTOR_FLAGS> Result;
	//	Result.Set(Vulkan_ROOT_DESCRIPTOR_FLAG_DATA_VOLATILE, DescriptorFlags & EDescriptorFlag::Volatile);
	//	Result.Set(Vulkan_ROOT_DESCRIPTOR_FLAG_DATA_STATIC, DescriptorFlags & EDescriptorFlag::Static);
	//	return Result;
	//}

	//forceinline Vulkan_DESCRIPTOR_RANGE_FLAGS FromDescriptorRangeFlags(EDescriptorRangeFlag DescriptorFlags)
	//{
	//	TEnumWrapper<Vulkan_DESCRIPTOR_RANGE_FLAGS> Result;
	//	Result.Set(Vulkan_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE, DescriptorFlags & EDescriptorRangeFlag::DescriptorVolatile);
	//	Result.Set(Vulkan_DESCRIPTOR_RANGE_FLAG_DATA_VOLATILE, DescriptorFlags & EDescriptorRangeFlag::Volatile);
	//	Result.Set(Vulkan_DESCRIPTOR_RANGE_FLAG_DATA_STATIC, DescriptorFlags & EDescriptorRangeFlag::Static);
	//	return Result;
	//}

	//forceinline Vulkan_FENCE_FLAGS FromFenceFlags(EFenceFlag FenceFlags)
	//{
	//	TEnumWrapper<Vulkan_FENCE_FLAGS> Result;
	//	Result.Set(Vulkan_FENCE_FLAG_SHARED, FenceFlags & EFenceFlag::Shared);
	//	return Result;
	//}

	forceinline VkViewport FromViewport(const FViewport & Viewport)
	{
		return VkViewport { Viewport.X, Viewport.Y, Viewport.Width, Viewport.Height, Viewport.NearZ, Viewport.FarZ };
	}

	forceinline VkRect2D FromRect2D(const RectI & Rect)
	{
		return VkRect2D { { Rect.X, Rect.Y }, { uint32(Rect.Width), uint32(Rect.Height) } };
	}

	struct FVulkanShaderRegisterRange
	{
		EShaderStage ShaderStage = EShaderStage::None;
		uint32 RegisterSpace = NullIndex;
		uint32 RegisterIndex = 0;
		uint32 RegisterCount = 0;

		auto operator <=>(const FVulkanShaderRegisterRange &) const = default;
		bool operator ==(const FVulkanShaderRegisterRange &) const = default;

		friend uintx Hash(const FVulkanShaderRegisterRange & This) { return HashCombine(This.ShaderStage, This.RegisterSpace, This.RegisterIndex, This.RegisterCount); }
	};

	constexpr int MaxShaderRegisterSpaceCount = 4;
	constexpr TArray<uint32, 4> RegisterMask8Bits = { 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000 };

	struct FVulkanShaderParameter
	{
		FName ParameterName;

		VkDescriptorType vkDescriptorType = VK_DESCRIPTOR_TYPE_MAX_ENUM;

		EShaderRelevance ShaderRelevance = EShaderRelevance::None;
		EFormat Format = EFormat::None;

		uint32 BindingIndex = NullIndex;
		uint32 BindingSpace = 0;

		uint32 DescriptorIndex = NullIndex;

		auto operator <=>(const FVulkanShaderParameter &) const = default;
		bool operator ==(const FVulkanShaderParameter &) const = default;
		explicit operator bool() const { return ShaderRelevance != EShaderRelevance::None; }
		friend uintx Hash(const FVulkanShaderParameter & This) { return HashCombine(This.ParameterName, This.vkDescriptorType, This.BindingIndex, This.BindingSpace); }

		RHI_VULKAN_VAR static FVulkanShaderParameter None;
	};

	constexpr uint32 MaxShaderBindings= 32;
	constexpr uint32 InvalidBindingIndex = NullIndex;

	struct FVulkanShaderBindingMask
	{
		struct FBindingMaskIndexProxy
		{
			FVulkanShaderBindingMask & BindingMask;
			uint32 Index;

			operator bool() const { return (BindingMask.BindingMask >> Index) & 0x1; }
			FBindingMaskIndexProxy & operator = (bool Value)
			{
				SetBitIndex(BindingMask.BindingMask, Index, Value);
				return *this;
			}
		};

		uint64 BindingMask = 0;

		auto operator <=>(const FVulkanShaderBindingMask &) const = default;
		bool operator ==(const FVulkanShaderBindingMask &) const = default;

		constexpr explicit operator bool() const { return BindingMask != 0; }
		constexpr bool operator [](uint32 BindingIndex) const { return (BindingMask >> BindingIndex) & 0x1; }
		constexpr FBindingMaskIndexProxy operator [](uint32 BindingIndex) { return {*this, BindingIndex}; }

		friend uintx Hash(const FVulkanShaderBindingMask & This) { return Hash(This.BindingMask); }
	};

	struct FVulkanShaderBindings
	{
		TArray<FVulkanShaderParameter, MaxVulkanShaderParameters> Parameters;
		TArray<FVulkanShaderBindingMask, MaxShaderStages> BindingMasks;
		TArray<FSamplerDesc, MaxStaticSamplers> StaticSamplers;

		friend uintx Hash(const FVulkanShaderBindings & This) { return HashCombine(This.Parameters, This.BindingMasks, This.StaticSamplers); }

		auto operator <=>(const FVulkanShaderBindings &) const = default;
		bool operator ==(const FVulkanShaderBindings &) const = default;

		auto & operator[](this auto && This, uint32 BindingIndex)
		{
			for (auto & Parameter : This.Parameters)
			{
				if (Parameter.BindingIndex == BindingIndex)
					return Parameter;
			}
			return (decltype(This.Parameters[0]) &)(FVulkanShaderParameter::None);
		}

		uint32 GetNumDescriptors(VkDescriptorType DescriptorType) const
		{
			uint32 NumDescriptors = 0;
			for (uintx ShaderStageIndex = 0; ShaderStageIndex < MaxShaderStages; ++ShaderStageIndex)
			{
				for (uint32 DescriptorIndex = 0; DescriptorIndex < MaxVulkanShaderDescriptors; ++DescriptorIndex)
				{
					const FVulkanShaderParameter & ShaderParameter = Parameters[DescriptorIndex];
					if (ShaderParameter.DescriptorIndex != NullIndex &&
						BindingMasks[ShaderStageIndex][ShaderParameter.BindingIndex] &&
						ShaderParameter.vkDescriptorType == DescriptorType)
						++NumDescriptors;
				}
			}

			if (DescriptorType == VK_DESCRIPTOR_TYPE_SAMPLER)
			{
				for (const FSamplerDesc & StaticSampler : StaticSamplers)
				{
					if (StaticSampler.ShaderRelevance != EShaderRelevance::None)
						++NumDescriptors;
				}
				
			}
			return NumDescriptors;
		}

		bool IsImmutableSampler(uint32 BindingIndex, uint32 BindingSpace) const
		{
			for (const FSamplerDesc & StaticSampler : StaticSamplers)
			{
				if (StaticSampler.BindingIndex == BindingIndex && StaticSampler.BindingSpace == BindingSpace)
					return true;
			}
			return false;
		}
	};

	inline void VkResultAssert(bool Success, FStringV Formal)
	{
		if (!Success)
			AssertFailed(Formal);
	}

	template<typename ...ArgTs>
	void VkResultAssert(bool Success, FStringV Formal, ArgTs && ... Args)
	{
		if (!Success)
			AssertForaml(false, Formal, Forward<ArgTs>(Args)...);
	}

	template<typename VulkanObjectT>
	void SetVulkanObjectName(VkDevice Device, const VulkanObjectT & Object, FStringV Name, VkObjectType ObjectType)
	{
#if XIN_VULKAN_DEBUG_UNTILS
		VkDebugUtilsObjectNameInfoEXT  vkDebugUtilsObjectNameInfo { VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT };
		vkDebugUtilsObjectNameInfo.pObjectName = (const chara *)Name.Data;
		vkDebugUtilsObjectNameInfo.objectType = ObjectType;
		vkDebugUtilsObjectNameInfo.objectHandle = uint64(Object);
		vkSetDebugUtilsObjectNameEXT(Device, &vkDebugUtilsObjectNameInfo);
#endif
	}
}
