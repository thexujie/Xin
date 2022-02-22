#pragma once

#include "VulkanTypes.h"

namespace Xin::RHI::Vulkan
{
	class FVulkanDevice;

	enum EVulkanStaticSamplerRegisterIndex
	{
		Vulkan_SSRI_PointWrap = 5,
		Vulkan_SSRI_PointClamp,
		Vulkan_SSRI_PointBorder,
		Vulkan_SSRI_LinearWrap,
		Vulkan_SSRI_LinearClamp,
		Vulkan_SSRI_LinearBorder,
		Vulkan_SSRI_TrilinearWrap,
		Vulkan_SSRI_TrilinearClamp,
		Vulkan_SSRI_TrilinearBorder,
		Vulkan_SSRI_Shadow,
	};

	constexpr int RegisterRangeUnit_CBV = 4;
	constexpr int RegisterRangeUnit_SRV = 4;
	constexpr int RegisterRangeUnit_UAV = 4;
	constexpr int RegisterRangeUnit_SAM = 4;

	constexpr bool IsVulkanStaticSamplerRegisterIndex(uint32 RegisterIndex)
	{
		return Vulkan_SSRI_PointWrap <= RegisterIndex && RegisterIndex <= Vulkan_SSRI_Shadow;
	}

	class RHI_VULKAN_API FVulkanPipelineState : public IRHIPipelineState
	{
	public:
		FVulkanPipelineState(FVulkanDevice & Device, const FPipelineStateDesc & PipelineStateDesc);
		~FVulkanPipelineState();

	private:
		void CreateGraphicsPipelineState();
		void CreateComputePipelineState();

	public:
		FVulkanDevice & Device;
		FPipelineStateDesc PipelineStateDesc;

		FVulkanShaderBindings ShaderBindings;

		VkPipelineBindPoint vkPipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		VkPipelineLayout vkPipelineLayout = nullptr;
		VkPipeline vkPipeline = nullptr;
	};
	using IRHIPipelineStateRef = TReferPtr<IRHIPipelineState>;


	class RHI_VULKAN_API FVulkanIndirectSignature : public IRHIIndirectSignature
	{
	public:
		FVulkanIndirectSignature(FVulkanDevice & VulkanDevice, const FIndirectSignature & IndirectSignature, FVulkanPipelineState * PipelineState);

	public:
		FVulkanDevice & VulkanDevice;
		FIndirectSignature IndirectSignature;

		//IVulkanRootSignatureRef VulkanRootSignature;
		//IVulkanCommandSignatureRef VulkanCommandSignature;
	};
	using FVulkanIndirectSignatureRef = TReferPtr<FVulkanIndirectSignature>;
}
