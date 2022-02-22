#include "PCH.h"
#include "VulkanPipelineState.h"

#include "VulkanDevice.h"
#include "VulkanShaderModule.h"

namespace Xin::RHI::Vulkan
{
	static void GetPipelineLayoutFromPipelineState(const FPipelineStateDesc & PipelineStateDesc, FVulkanShaderBindings & ShaderBindings)
	{
		ShaderBindings.StaticSamplers = PipelineStateDesc.StaticSamplers;

		uint32 BindingIndex = 0;
		uint32 DescriptorIndex = 0;
		for (uintx ShaderStageIndex = 0; ShaderStageIndex < MaxShaderStages; ++ShaderStageIndex)
		{
			if (PipelineStateDesc.Shaders[ShaderStageIndex] == nullptr)
				continue;

			const FReflectParameters & ReflectParameters = PipelineStateDesc.Shaders[ShaderStageIndex]->ShaderInstance.GetReflectParameters();

			for (const FReflectParameter & ReflectParameter : ReflectParameters.Parameters)
			{
				//if (ReflectParameter.BindingType == EDescriptorType::Sampler 
				//	&& PipelineStateDesc.IsStaticSampler(ReflectParameter.BindingIndex, ReflectParameter.BindingSpace))
				//	continue;

				if (FVulkanShaderParameter & ExistedShaderParameter = ShaderBindings[ReflectParameter.BindingIndex])
				{
					AssertExpr(ExistedShaderParameter.ParameterName == FName(ReflectParameter.Name));
					AssertExpr(ExistedShaderParameter.vkDescriptorType == FromBindingType(ReflectParameter.BindingType, ReflectParameter.ResourceDimension));
					ExistedShaderParameter.ShaderRelevance |= ShaderRelevanceFormShaderStage(EShaderStage(ShaderStageIndex));
				}
				else
				{
					FVulkanShaderParameter & ShaderParameter = ShaderBindings.Parameters[BindingIndex++];
					{
						ShaderParameter.ParameterName = FName(ReflectParameter.Name);
						ShaderParameter.vkDescriptorType = FromBindingType(ReflectParameter.BindingType, ReflectParameter.ResourceDimension);

						ShaderParameter.ShaderRelevance = ShaderRelevanceFormShaderStage(EShaderStage(ShaderStageIndex));
						ShaderParameter.Format = ReflectParameter.Format;
						ShaderParameter.BindingIndex = ReflectParameter.BindingIndex;
						ShaderParameter.BindingSpace = ReflectParameter.BindingSpace;

						if (ReflectParameter.BindingType == EDescriptorType::Sampler
							&& PipelineStateDesc.IsStaticSampler(ReflectParameter.BindingIndex, ReflectParameter.BindingSpace))
							ShaderParameter.DescriptorIndex = NullIndex;
						else
							ShaderParameter.DescriptorIndex = DescriptorIndex++;
					}

					ShaderBindings.BindingMasks[ShaderStageIndex][ReflectParameter.BindingIndex] = true;
				}
			}
		}
	}


	FVulkanPipelineState::FVulkanPipelineState(FVulkanDevice & Device, const FPipelineStateDesc & PipelineStateDesc)
		: Device(Device), PipelineStateDesc(PipelineStateDesc)
	{
		bool IsComputePipeline = false;
		for (uintx ShaderStageIndex = 0; ShaderStageIndex < MaxShaderStages; ++ShaderStageIndex)
		{
			if (IRHIShaderModule * Shader = PipelineStateDesc.Shaders[ShaderStageIndex]; 
				Shader && Shader->ShaderInstance.GetShaderStage() == EShaderStage::Compute)
			{
				IsComputePipeline = true;
				break;
			}
		}

		GetPipelineLayoutFromPipelineState(PipelineStateDesc, ShaderBindings);

		if (IsComputePipeline)
			CreateComputePipelineState();
		else
			CreateGraphicsPipelineState();
	}

	FVulkanPipelineState::~FVulkanPipelineState()
	{
		if (vkPipeline)
		{
			vkDestroyPipeline(Device.vkDevice, vkPipeline, nullptr);
			vkPipeline = nullptr;
		}

		if (vkPipelineLayout)
		{
			vkDestroyPipelineLayout(Device.vkDevice, vkPipelineLayout, nullptr);
			vkPipelineLayout = nullptr;
		}
	}

	void FVulkanPipelineState::CreateGraphicsPipelineState()
	{
		vkPipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		VkGraphicsPipelineCreateInfo GraphicsPipelineCI { VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO };
		GraphicsPipelineCI.stageCount = 0;


		IRHIShaderModule * VertexShader = nullptr;

		TArray<VkPipelineShaderStageCreateInfo, MaxShaderStages> ShaderStageCIs {};
		for (uintx ShaderStageIndex = 0; ShaderStageIndex < MaxShaderStages; ++ShaderStageIndex)
		{
			if (IRHIShaderModule * Shader = PipelineStateDesc.Shaders[ShaderStageIndex])
			{
				if (Shader->ShaderInstance.GetShaderStage() == EShaderStage::Vertex)
					VertexShader = Shader;

				VkPipelineShaderStageCreateInfo & ShaderStageCI = ShaderStageCIs[GraphicsPipelineCI.stageCount++];
				ShaderStageCI = { VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO };

				ShaderStageCI.stage = FromShaderStage_FlagBits(EShaderStage(ShaderStageIndex));
				ShaderStageCI.module = StaticCastRef<FVulkanShaderModule>(Shader).vkShaderModule;
				ShaderStageCI.pName = (const chara *)(Shader->ShaderInstance.GetEntryName().Data);
				ShaderStageCI.pSpecializationInfo = nullptr;
			}
			else {}
		}
		AssertExpr(VertexShader);
		GraphicsPipelineCI.pStages = ShaderStageCIs.Data;

		auto FindInputBindingIndex = [&](EInputSemantic InputSemantic)->uint32
		{
			for (const FReflectVariable & InputVarialbe : VertexShader->ShaderInstance.GetReflectParameters().InputVariables)
			{
				if (StringEqualIC(InputVarialbe.Name, InputSemanticNames[InputSemantic]))
					return InputVarialbe.BindingIndex;
			}
			return NullIndex;
		};

		TArray<VkVertexInputBindingDescription, MaxInputElements> VertexInputBindingDescriptions {};
		TArray<VkVertexInputAttributeDescription, MaxInputElements> VertexInputAttributeDescriptions {};
		VkPipelineVertexInputStateCreateInfo VertexInputStateCI { VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO };
		{
			TArray<uint32, MaxInputStreams> RequiredStreamStrides {};
			for (uintx InputElementIndex = 0; InputElementIndex < MaxInputElements; ++InputElementIndex)
			{
				const FInputElement & InputElement = PipelineStateDesc.InputFormat.Elements[InputElementIndex];
				if (InputElement.Format != EFormat::None)
				{
					RequiredStreamStrides[InputElement.StreamIndex] += GetFormatBytesPerPixel(InputElement.Format);

					uint32 BindingIndex = FindInputBindingIndex(InputElement.SemanticName);
					if (BindingIndex == NullIndex)
						continue;

					VertexInputStateCI.vertexAttributeDescriptionCount++;
					VkVertexInputAttributeDescription & VertexInputAttributeDescription = VertexInputAttributeDescriptions[BindingIndex];
					VertexInputAttributeDescription.location = BindingIndex;
					VertexInputAttributeDescription.binding = InputElement.StreamIndex;
					VertexInputAttributeDescription.format = FromFormat(InputElement.Format);
					VertexInputAttributeDescription.offset = InputElement.ByteOffset;

				}
			}

			for (uintx InputStreamIndex = 0; InputStreamIndex < MaxInputStreams; ++InputStreamIndex)
			{
				AssertExpr(PipelineStateDesc.InputFormat.Streams[InputStreamIndex].Stride == 0
					|| RequiredStreamStrides[InputStreamIndex] <= PipelineStateDesc.InputFormat.Streams[InputStreamIndex].Stride);
			}
			VertexInputStateCI.pVertexAttributeDescriptions = VertexInputAttributeDescriptions.Data;

			for (uintx InputStreamIndex = 0; InputStreamIndex < MaxInputStreams; ++InputStreamIndex)
			{
				const FInputStream & InputStream = PipelineStateDesc.InputFormat.Streams[InputStreamIndex];
				if (!InputStream.Stride && !RequiredStreamStrides[InputStreamIndex])
					break;

				VkVertexInputBindingDescription & VertexInputBindingDescription = VertexInputBindingDescriptions[VertexInputStateCI.vertexBindingDescriptionCount];
				VertexInputBindingDescription.binding = VertexInputStateCI.vertexBindingDescriptionCount++;
				VertexInputBindingDescription.stride = InputStream.Stride > 0 ? InputStream.Stride : RequiredStreamStrides[InputStreamIndex];
				VertexInputBindingDescription.inputRate = InputStream.Instanced ? VK_VERTEX_INPUT_RATE_INSTANCE : VK_VERTEX_INPUT_RATE_VERTEX;
			}
			VertexInputStateCI.pVertexBindingDescriptions = VertexInputBindingDescriptions.Data;
		}
		VkPipelineInputAssemblyStateCreateInfo InputAssemblyStateCI { VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO };
		{
			InputAssemblyStateCI.topology = FromTopology(PipelineStateDesc.Topology, PipelineStateDesc.InputFormat.PrimitiveTopology);
			InputAssemblyStateCI.primitiveRestartEnable = false;
		}
		VkPipelineTessellationStateCreateInfo TessellationStateCI { VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO };
		TessellationStateCI.patchControlPoints = 0;

		VkPipelineViewportStateCreateInfo ViewportStateCI { VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO };
		{
			ViewportStateCI.viewportCount = 1;
			ViewportStateCI.pViewports = nullptr;
			ViewportStateCI.scissorCount = 1;
			ViewportStateCI.pScissors = nullptr;
		}
		VkPipelineRasterizationStateCreateInfo RasterizationStateCI { VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO };
		{
			////RasterizationState.depthClampEnable = ;
			//RasterizationState.rasterizerDiscardEnable = ;
			RasterizationStateCI.polygonMode = PipelineStateDesc.RasterizeState.Wireframe ? VK_POLYGON_MODE_LINE : VK_POLYGON_MODE_FILL;
			RasterizationStateCI.cullMode = FromCullMode(PipelineStateDesc.RasterizeState.CullMode);
			// Vulkan is right handed, but we invert CW by set negative viewport height, so we do not need to invert frontface CCW.
			RasterizationStateCI.frontFace = PipelineStateDesc.RasterizeState.FrontCCW ? VK_FRONT_FACE_COUNTER_CLOCKWISE : VK_FRONT_FACE_CLOCKWISE;
			RasterizationStateCI.depthBiasEnable = PipelineStateDesc.RasterizeState.DepthBias != 0;
			RasterizationStateCI.depthBiasConstantFactor = PipelineStateDesc.RasterizeState.DepthBias ;
			RasterizationStateCI.depthBiasClamp = PipelineStateDesc.RasterizeState.DepthBiasClamp;
			RasterizationStateCI.depthBiasSlopeFactor = PipelineStateDesc.RasterizeState.SlopScaledDepthBias;
			RasterizationStateCI.lineWidth = 1.0f;
		}
		VkPipelineMultisampleStateCreateInfo MultisampleState { VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO };
		{
			MultisampleState.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		}
		VkPipelineDepthStencilStateCreateInfo DepthStencilStateCI { VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO };
		{
			DepthStencilStateCI.depthTestEnable = PipelineStateDesc.DepthStencilState.DepthTest;
			DepthStencilStateCI.depthWriteEnable = PipelineStateDesc.DepthStencilState.DepthWrite;
			DepthStencilStateCI.depthCompareOp = FromComparison(PipelineStateDesc.DepthStencilState.DepthComparison);
			//DepthStencilState.depthBoundsTestEnable = ;
			//DepthStencilState.stencilTestEnable = PipelineStateDesc.DepthStencilState.StencilTest;
		}
		TArray<VkPipelineColorBlendAttachmentState, MaxRenderTargets> ColorBlendAttachments {};
		VkPipelineColorBlendStateCreateInfo ColorBlendStateCI { VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO };
		{
			ColorBlendStateCI.flags = VkPipelineColorBlendStateCreateFlags(0);
			ColorBlendStateCI.pAttachments = ColorBlendAttachments.Data;
		}
		//ColorBlendState.logicOpEnable = BlendMethod.Enable;
		uint32 NumRenderTargets = 0;
		{
			for (uintx RenderTargetIndex = 0; RenderTargetIndex < MaxRenderTargets; ++RenderTargetIndex)
			{
				if (PipelineStateDesc.OutputFormat.RenderTargets[RenderTargetIndex] == EFormat::None)
					continue;

				++NumRenderTargets;
				ColorBlendStateCI.attachmentCount = RenderTargetIndex + 1;
				const FBlendMethod & BlendMethod = PipelineStateDesc.BlendState.RenderTargets[RenderTargetIndex];
				VkPipelineColorBlendAttachmentState & ColorBlendAttachment = ColorBlendAttachments[RenderTargetIndex];
				ColorBlendAttachment = { BlendMethod.Enable };

				if (BlendMethod.Enable)
				{
					ColorBlendAttachment.srcColorBlendFactor = FromBlendFactor(BlendMethod.SrcColor);
					ColorBlendAttachment.dstColorBlendFactor = FromBlendFactor(BlendMethod.DstColor);
					ColorBlendAttachment.colorBlendOp = FromBlendOP(BlendMethod.ColorAlgorithm);

					ColorBlendAttachment.srcAlphaBlendFactor = FromBlendFactor(BlendMethod.SrcAlpha);
					ColorBlendAttachment.dstAlphaBlendFactor = FromBlendFactor(BlendMethod.DstAlpha);
					ColorBlendAttachment.alphaBlendOp = FromBlendOP(BlendMethod.AlphaAlgorithm);
				}
				ColorBlendAttachment.colorWriteMask = FromWriteMasks(BlendMethod.WriteMasks);
			}
		}

		VkDynamicState EnabledDynamicStates[2] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
		VkPipelineDynamicStateCreateInfo DynamicStateCI { VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO };
		{
			DynamicStateCI.dynamicStateCount = ArraySize(EnabledDynamicStates);
			DynamicStateCI.pDynamicStates = EnabledDynamicStates;
		}

		TList<VkDescriptorSetLayout> DescriptorSetLayouts = Device.FetchDescriptorSetLayout(ShaderBindings);

		VkPipelineLayoutCreateInfo PipelineLayoutCI { VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO };
		{
			PipelineLayoutCI.setLayoutCount = uint32(DescriptorSetLayouts.Size);
			PipelineLayoutCI.pSetLayouts = DescriptorSetLayouts.Data;
		}

		vkCreatePipelineLayout(Device.vkDevice, &PipelineLayoutCI, nullptr, &vkPipelineLayout);

		GraphicsPipelineCI.pVertexInputState = &VertexInputStateCI;
		GraphicsPipelineCI.pInputAssemblyState = &InputAssemblyStateCI;
		GraphicsPipelineCI.pTessellationState = &TessellationStateCI;
		GraphicsPipelineCI.pViewportState = &ViewportStateCI;
		GraphicsPipelineCI.pRasterizationState = &RasterizationStateCI;
		GraphicsPipelineCI.pMultisampleState = &MultisampleState;
		GraphicsPipelineCI.pDepthStencilState = &DepthStencilStateCI;
		GraphicsPipelineCI.pColorBlendState = &ColorBlendStateCI;
		GraphicsPipelineCI.pDynamicState = &DynamicStateCI;
		GraphicsPipelineCI.layout = vkPipelineLayout;
		GraphicsPipelineCI.renderPass = nullptr;
		GraphicsPipelineCI.subpass = 0;
		GraphicsPipelineCI.basePipelineHandle = nullptr;
		GraphicsPipelineCI.basePipelineIndex = -1;

		TArray<VkFormat, MaxRenderTargets> ColorAttachmentFormats;

		for (uintx RenderTargetIndex = 0; RenderTargetIndex < MaxRenderTargets; ++RenderTargetIndex)
		{
			if (PipelineStateDesc.OutputFormat.RenderTargets[RenderTargetIndex] == EFormat::None)
			{
				ColorAttachmentFormats[RenderTargetIndex] = VK_FORMAT_UNDEFINED;
				continue;
			}

			ColorAttachmentFormats[RenderTargetIndex] = FromFormat(PipelineStateDesc.OutputFormat.RenderTargets[RenderTargetIndex]);
		}

		VkPipelineRenderingCreateInfo PipelineRenderingCI { VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO };
		PipelineRenderingCI.colorAttachmentCount = NumRenderTargets;
		PipelineRenderingCI.pColorAttachmentFormats = ColorAttachmentFormats.Data;
		PipelineRenderingCI.depthAttachmentFormat = FromFormatDSV(PipelineStateDesc.OutputFormat.DepthStencil);
		if (IsStencilFormat(PipelineStateDesc.OutputFormat.DepthStencil))
		{
			PipelineRenderingCI.stencilAttachmentFormat = PipelineRenderingCI.depthAttachmentFormat;
		}

		GraphicsPipelineCI.pNext = &PipelineRenderingCI;
		VkResult Result = vkCreateGraphicsPipelines(Device.vkDevice, nullptr, 1, &GraphicsPipelineCI, nullptr, &vkPipeline);
		AssertExpr(Result == VK_SUCCESS);
	}

	void FVulkanPipelineState::CreateComputePipelineState()
	{
		vkPipelineBindPoint = VK_PIPELINE_BIND_POINT_COMPUTE;

		VkComputePipelineCreateInfo ComputePipelineCI { VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO };

		for (uintx ShaderStageIndex = 0; ShaderStageIndex < MaxShaderStages; ++ShaderStageIndex)
		{
			if (IRHIShaderModule * Shader = PipelineStateDesc.Shaders[ShaderStageIndex])
			{
				AssertExpr(Shader->ShaderInstance.GetShaderStage() == EShaderStage::Compute);

				VkPipelineShaderStageCreateInfo & ShaderStageCI = ComputePipelineCI.stage;
				ShaderStageCI = { VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO };

				ShaderStageCI.stage = FromShaderStage_FlagBits(EShaderStage(ShaderStageIndex));
				ShaderStageCI.module = StaticCastRef<FVulkanShaderModule>(Shader).vkShaderModule;
				ShaderStageCI.pName = (const chara *)(Shader->ShaderInstance.GetEntryName().Data);
				ShaderStageCI.pSpecializationInfo = nullptr;
				break;
			}
			else {}
		}

		TList<VkDescriptorSetLayout> DescriptorSetLayouts = Device.FetchDescriptorSetLayout(ShaderBindings);

		VkPipelineLayoutCreateInfo PipelineLayoutCI { VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO };
		{
			PipelineLayoutCI.setLayoutCount = uint32(DescriptorSetLayouts.Size);
			PipelineLayoutCI.pSetLayouts = DescriptorSetLayouts.Data;
		}

		vkCreatePipelineLayout(Device.vkDevice, &PipelineLayoutCI, nullptr, &vkPipelineLayout);
		ComputePipelineCI.layout = vkPipelineLayout;

		VkResult Result = vkCreateComputePipelines(Device.vkDevice, nullptr, 1, &ComputePipelineCI, nullptr, &vkPipeline);
	}

	FVulkanIndirectSignature::FVulkanIndirectSignature(FVulkanDevice & VulkanDevice, const FIndirectSignature & IndirectSignature, FVulkanPipelineState * PipelineState)
		: VulkanDevice(VulkanDevice), IndirectSignature(IndirectSignature) {}
}
