#include "PCH.h"
#include "VulkanCommandList.h"

#include "VulkanDevice.h"
#include "VulkanResource.h"
#include "VulkanCommandQueue.h"
#include "VulkanHeap.h"
#include "VulkanResourceView.h"

#define Vulkan_COMMANDLIST_DEBUG_SINGLE_COMMAND 0

namespace Xin::RHI::Vulkan
{
	FVulkanCommandList::FVulkanCommandList(FVulkanDevice & VulkanDevice, ECommandType CommandType)
		: VulkanDevice(VulkanDevice), CommandType(CommandType) {}

	FVulkanCommandList::~FVulkanCommandList()
	{
		if (vkCommandBuffer)
		{
			vkEndCommandBuffer(vkCommandBuffer);
			vkCommandBuffer = nullptr;
		}

		if (CommandAllocator)
		{
			VulkanDevice.FreeCommandAllocator(CommandAllocator);
			CommandAllocator = nullptr;
		}
	}

	void FVulkanCommandList::SetDebugName(FStringV ResourceLabel)
	{
		IRHICommandList::SetDebugName(ResourceLabel);

		SetVulkanObjectName(VulkanDevice.vkDevice, vkCommandBuffer, DebugName, VkObjectType::VK_OBJECT_TYPE_COMMAND_BUFFER);
		//if (InnerCommandList)
		//	SetVulkanObjectName(InnerCommandList.Get(), ResourceLabel);
	}

	void FVulkanCommandList::BeginCommand(IRHICommandQueue * CommandQueue)
	{
		AssertExpr(!vkCommandBuffer);

		if (!CommandAllocator)
			CommandAllocator = VulkanDevice.AcquireCommandAllocator();

		vkCommandBuffer = CommandAllocator->AcquireCommandBuffer();
		FrameStatics = { };
	}

	void FVulkanCommandList::EndCommand(IRHICommandQueue * CommandQueue)
	{
		AssertExpr(vkCommandBuffer);
		AssertExpr(CommandAllocator);

		FVulkanCommandQueue & VulkanCommandQueue = StaticCastRef<FVulkanCommandQueue>(CommandQueue);

		vkEndCommandBuffer(vkCommandBuffer);

		VulkanCommandQueue.FenceCommandBuffer(vkCommandBuffer, CommandAllocator);
		VulkanCommandQueue.FenceCommandAllocator(CommandAllocator);

		vkCommandBuffer = nullptr;
		CommandAllocator = nullptr;
	}

	void FVulkanCommandList::FlushCommands(IRHICommandQueue * CommandQueue)
	{
		FlushBarriers();

		if (!NumCommands)
			return;

		NumCommands = 0;

		vkEndCommandBuffer(vkCommandBuffer);

		FVulkanCommandQueue & VulkanCommandQueue = StaticCastRef<FVulkanCommandQueue>(CommandQueue);

		VkSubmitInfo2 SubmitInfo2 { VK_STRUCTURE_TYPE_SUBMIT_INFO_2 };

		VkCommandBufferSubmitInfo CommandBufferSubmitInfo { VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO };
		{
			CommandBufferSubmitInfo.commandBuffer = vkCommandBuffer;
			CommandBufferSubmitInfo.deviceMask = 0;
		}

		VkSemaphoreSubmitInfo TimelineSemaphoreSubmitInfo { VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO };
		{
			TimelineSemaphoreSubmitInfo.pNext = NULL;
			TimelineSemaphoreSubmitInfo.semaphore = VulkanCommandQueue.QueueFence.vkSemaphore;
			TimelineSemaphoreSubmitInfo.value = ++VulkanCommandQueue.QueueFence.SignalValue;
			TimelineSemaphoreSubmitInfo.stageMask = VK_PIPELINE_STAGE_2_ALL_GRAPHICS_BIT;
		}
		SubmitInfo2.commandBufferInfoCount = 1;
		SubmitInfo2.pCommandBufferInfos = &CommandBufferSubmitInfo;
		SubmitInfo2.signalSemaphoreInfoCount = 1;
		SubmitInfo2.pSignalSemaphoreInfos = &TimelineSemaphoreSubmitInfo;

		VkResult Result = vkQueueSubmit2(VulkanCommandQueue.PrimaryQueue, 1, &SubmitInfo2, VK_NULL_HANDLE);

		VulkanCommandQueue.FenceCommandBuffer(vkCommandBuffer, CommandAllocator);
		VulkanCommandQueue.FenceCommandAllocator(CommandAllocator);

		CommandAllocator = VulkanDevice.AcquireCommandAllocator();
		vkCommandBuffer = CommandAllocator->AcquireCommandBuffer();

		StagingForward(VulkanCommandQueue);

		PipelineStateChanged = !!PipelineState;

		RenderTargetsChanged = true;

		ViewPortsChanged = true;
		NumScissorRects = true;
		ParametersChanged = true;
	}

	void FVulkanCommandList::ExecuteCommands(IRHICommandQueue * CommandQueue)
	{
		FlushBarriers();

		if (!NumCommands)
			return;

		NumCommands = 0;

		vkEndCommandBuffer(vkCommandBuffer);

		FVulkanCommandQueue & VulkanCommandQueue = StaticCastRef<FVulkanCommandQueue>(CommandQueue);

		VkSubmitInfo2 SubmitInfo2 { VK_STRUCTURE_TYPE_SUBMIT_INFO_2 };

		VkCommandBufferSubmitInfo CommandBufferSubmitInfo { VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO };
		{
			CommandBufferSubmitInfo.commandBuffer = vkCommandBuffer;
			CommandBufferSubmitInfo.deviceMask = 0;
		}

		VkSemaphoreSubmitInfo TimelineSemaphoreSubmitInfo { VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO };
		{
			TimelineSemaphoreSubmitInfo.pNext = NULL;
			TimelineSemaphoreSubmitInfo.semaphore = VulkanCommandQueue.QueueFence.vkSemaphore;
			TimelineSemaphoreSubmitInfo.value = ++VulkanCommandQueue.QueueFence.SignalValue;
			TimelineSemaphoreSubmitInfo.stageMask = VK_PIPELINE_STAGE_2_ALL_GRAPHICS_BIT;
		}
		SubmitInfo2.commandBufferInfoCount = 1;
		SubmitInfo2.pCommandBufferInfos = &CommandBufferSubmitInfo;
		SubmitInfo2.signalSemaphoreInfoCount = 1;
		SubmitInfo2.pSignalSemaphoreInfos = &TimelineSemaphoreSubmitInfo;

		VkResult Result = vkQueueSubmit2(VulkanCommandQueue.PrimaryQueue, 1, &SubmitInfo2, VK_NULL_HANDLE);

		VulkanCommandQueue.FenceCommandBuffer(vkCommandBuffer, CommandAllocator);
		vkCommandBuffer = CommandAllocator->AcquireCommandBuffer();

		StagingForward(VulkanCommandQueue);
		ResetRenderStates(VulkanCommandQueue);
	}


	void FVulkanCommandList::SetMarker(FStringV Marker, const FColor & Color)
	{
#if XIN_VULKAN_DEBUG_UNTILS
#endif
	}

	void FVulkanCommandList::BeginLabel(FStringV Label, const FColor & Color)
	{
#if XIN_VULKAN_DEBUG_UNTILS
		VkDebugUtilsLabelEXT DebugUtilsLabelEXT { VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT };
		DebugUtilsLabelEXT.pLabelName = (const chara *)Label.Data;
		vkCmdBeginDebugUtilsLabelEXT(vkCommandBuffer, &DebugUtilsLabelEXT);
		++NumCommands;
#endif
	}

	void FVulkanCommandList::EndLabel()
	{
#if XIN_VULKAN_DEBUG_UNTILS
		vkCmdEndDebugUtilsLabelEXT(vkCommandBuffer);
		++NumCommands;
#endif
	}

	void FVulkanCommandList::BeginRecord()
	{
		FrameStatics = {};
	}

	void FVulkanCommandList::EndRecord() {}

	void FVulkanCommandList::AfterExecute() {}

	void FVulkanCommandList::StagingForward(FVulkanCommandQueue & VulkanCommandQueue)
	{
		for (FVulkanDescriptorHeapRef & DescriptorHeap : OnlineDescriptorHeaps)
			VulkanCommandQueue.FenceOnlineDescriptorHeap(DescriptorHeap);
		OnlineDescriptorHeaps.Clear();

		for (VkBuffer Buffer : StagingBuffers)
			VulkanCommandQueue.FenceBuffer(Buffer);
		StagingBuffers.Clear();

		for (VkDeviceMemory Memory : StagingMemorys)
			VulkanCommandQueue.FenceMemory(Memory);
		StagingMemorys.Clear();
	}

	void FVulkanCommandList::ResetRenderStates(FVulkanCommandQueue & VulkanCommandQueue)
	{
		PipelineStateChanged = false;
		PipelineState = nullptr;

		RenderTargetsChanged = false;
		RenderPassState = {};

		ViewPortsChanged = false;
		vkViewports = {};

		NumScissorRects = false;
		vkScissorRects = {};

		ParameterDescriptors = {};

		IndexBuffer = nullptr;

		NumVertexBuffers = 0;
		VertexBuffers = {};

		ParametersChanged = false;
		ParameterResources = {};
	}

	inline void FVulkanCommandList::FlushBarriers()
	{
		++NumCommands;
	}

	void FVulkanCommandList::BeginPass(const FRenderPassState & RenderPassState)
	{
		this->RenderPassState = RenderPassState;

		VkRenderingAttachmentInfo ColorRenderingAttachmentInfos[MaxRenderTargets];

		uint32 NumRenderTargets = 0;
		for (uint32 RenderTargetIndex = 0; RenderTargetIndex < MaxRenderTargets; ++RenderTargetIndex)
		{
			VkRenderingAttachmentInfo & ColorRenderingAttachmentInfo = ColorRenderingAttachmentInfos[RenderTargetIndex];
			ColorRenderingAttachmentInfo = { VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO };

			const FRenderPassTarget & RenderPassTarget = RenderPassState.RenderTargets[RenderTargetIndex];
			if (RenderPassTarget.ResourceView)
			{
				++NumRenderTargets;

				FVulkanResourceView & ResourceView = StaticCastRef<FVulkanResourceView>(RenderPassTarget.ResourceView);

				VkClearValue ColorClearValue = {};
				if (RenderPassTarget.LoadAction == EResourceLoad::Clear)
				{
					if (RenderPassTarget.ClearValue)
						ColorClearValue = FromClearValue(RenderPassTarget.ClearValue);
					else if (ResourceView.VulkanResource.ResourceDesc.ClearValue)
						ColorClearValue = FromClearValue(ResourceView.VulkanResource.ResourceDesc.ClearValue);
					else
						ColorClearValue = {};
				}

				ColorRenderingAttachmentInfo.pNext = nullptr;
				ColorRenderingAttachmentInfo.imageView = ResourceView.vkImageView;
				ColorRenderingAttachmentInfo.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
				ColorRenderingAttachmentInfo.resolveMode = VK_RESOLVE_MODE_NONE;
				ColorRenderingAttachmentInfo.resolveImageView = nullptr;
				ColorRenderingAttachmentInfo.resolveImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				ColorRenderingAttachmentInfo.loadOp = FromLoadAction(RenderPassTarget.LoadAction);
				ColorRenderingAttachmentInfo.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
				ColorRenderingAttachmentInfo.clearValue = ColorClearValue;
			}
			else
			{
				ColorRenderingAttachmentInfo = {};
			}
		}

		VkRenderingAttachmentInfo DepthStencilRenderingAttachmentInfo { VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO };
		if (RenderPassState.DepthStencil.ResourceView)
		{
			constexpr /*VkImageLayout*/uint32 DepthStencilImageLayoutMask =
				VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL |
				VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL |
				VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL |
				VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL |
				VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL;

			FVulkanResourceView & ResourceView = StaticCastRef<FVulkanResourceView>(RenderPassState.DepthStencil.ResourceView);
			AssertExpr(ResourceView.VulkanResource.ImageLayout != VK_IMAGE_LAYOUT_UNDEFINED);
			AssertExpr(ResourceView.VulkanResource.ImageLayout != VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
			AssertExpr(ResourceView.VulkanResource.ImageLayout != VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
			AssertExpr(ResourceView.VulkanResource.ImageLayout != VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
			AssertExpr(ResourceView.VulkanResource.ImageLayout != VK_IMAGE_LAYOUT_PREINITIALIZED);

			VkClearValue DepthStencilClearValue {};
			if (RenderPassState.DepthStencil.LoadAction == EResourceLoad::Clear)
			{
				if (RenderPassState.DepthStencil.ClearValue)
					DepthStencilClearValue = FromClearValue(RenderPassState.DepthStencil.ClearValue);
				else if (ResourceView.VulkanResource.ResourceDesc.ClearValue)
					DepthStencilClearValue = FromClearValue(ResourceView.VulkanResource.ResourceDesc.ClearValue);
				else
					DepthStencilClearValue = {};
			}
			DepthStencilRenderingAttachmentInfo.pNext = nullptr;
			DepthStencilRenderingAttachmentInfo.imageView = ResourceView.vkImageView;
			DepthStencilRenderingAttachmentInfo.imageLayout = ResourceView.VulkanResource.ImageLayout;
			DepthStencilRenderingAttachmentInfo.resolveMode = VK_RESOLVE_MODE_NONE;
			DepthStencilRenderingAttachmentInfo.resolveImageView = nullptr;
			DepthStencilRenderingAttachmentInfo.resolveImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			DepthStencilRenderingAttachmentInfo.loadOp = FromLoadAction(RenderPassState.DepthStencil.LoadAction);
			DepthStencilRenderingAttachmentInfo.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			DepthStencilRenderingAttachmentInfo.clearValue = DepthStencilClearValue;
		}

		VkRenderingInfo RenderingInfo { VK_STRUCTURE_TYPE_RENDERING_INFO };
		{
			const IRHIResourceView * FirstResourceView = RenderPassState.RenderTargets[0].ResourceView ? RenderPassState.RenderTargets[0].ResourceView : RenderPassState.DepthStencil.ResourceView;
			const FVulkanResourceView & ResourceView = StaticCastRef<FVulkanResourceView>(FirstResourceView);

			RenderingInfo.renderArea = VkRect2D { 0, 0, ResourceView.VulkanResource.ResourceDesc.Size.Width, ResourceView.VulkanResource.ResourceDesc.Size.Height };
			RenderingInfo.layerCount = 1;
			RenderingInfo.colorAttachmentCount = NumRenderTargets;
			RenderingInfo.pColorAttachments = ColorRenderingAttachmentInfos;
			RenderingInfo.pDepthAttachment = &DepthStencilRenderingAttachmentInfo;
			RenderingInfo.pStencilAttachment = IsStencilFormat(ResourceView.VulkanResource.ResourceDesc.Format) ? &DepthStencilRenderingAttachmentInfo : nullptr;
		}

		vkCmdBeginRendering(vkCommandBuffer, &RenderingInfo);
		++IsPassRendering;
		++NumCommands;

		NumViewports = 0;
		for (uintx ViewportIndex = 0; ViewportIndex < RenderPassState.Viewports.Size; ++ViewportIndex)
		{
			const FViewport & Viewport = RenderPassState.Viewports[ViewportIndex];
			if (Viewport.Width == 0 || Viewport.Height == 0)
				break;

			// VK_KHR_Maintenance1
			vkViewports[ViewportIndex] = FromViewport(RenderPassState.Viewports[ViewportIndex]);

			vkViewports[ViewportIndex].height = -Viewport.Height;
			vkViewports[ViewportIndex].y = Viewport.Y + Viewport.Height;

			++NumViewports;
		}

		if (!NumViewports)
		{
			const IRHIResourceView * FirstResourceView = RenderPassState.RenderTargets[0].ResourceView ? RenderPassState.RenderTargets[0].ResourceView : RenderPassState.DepthStencil.ResourceView;
			const FVulkanResourceView & ResourceView = StaticCastRef<FVulkanResourceView>(FirstResourceView);

			NumViewports = 1;
			FViewport Viewport { 0.0f, 0.0f, float32(ResourceView.VulkanResource.ResourceDesc.Size.Width), float32(ResourceView.VulkanResource.ResourceDesc.Size.Height), 0.0f, 1.0f };

			vkViewports[0] = FromViewport(Viewport);
			vkViewports[0].height = -Viewport.Height;
			vkViewports[0].y = Viewport.Y + Viewport.Height;
		}

		ViewPortsChanged = false;
		vkCmdSetViewport(vkCommandBuffer, 0, NumViewports, vkViewports.Data);

		const FViewport & Viewport = RenderPassState.Viewports[0];
		NumScissorRects = 1;
		vkScissorRects[0] = { { int32(Viewport.X), int32(Viewport.Y) }, { uint32(Viewport.X + Viewport.Width), uint32(Viewport.Y + Viewport.Height) } };
		ScissorRectsChanged = false;
		vkCmdSetScissor(vkCommandBuffer, 0, NumScissorRects, vkScissorRects.Data);
	}

	void FVulkanCommandList::EndPass()
	{
		if (RenderTargetsChanged)
			ApplyRenderStates();

		--IsPassRendering;
		vkCmdEndRendering(vkCommandBuffer);
	}

	//void FVulkanCommandList::FlushCommands(IRHICommandExecutor * CommandQueue)
	//{
	//	FlushBarriers();
	//	if (!CommandCount)
	//		return;

	//	FVulkanCommandExecutor & VulkanCommandQueue = StaticCastRef<FVulkanCommandExecutor>(CommandQueue);
	//	VulkanCommandQueue.Execute(this);
	//}


	void FVulkanCommandList::DiscardResource(IRHIResource * Resource, TView<RectU> Rects)
	{
		FVulkanResource & VulkanResource = StaticCastRef<FVulkanResource>(Resource);
		FlushBarriers();
		AssertExpr(Rects.Size == 0);
		//InnerCommandList->DiscardResource(VulkanResource.InnerResource, nullptr);
	}

	void FVulkanCommandList::ClearDepthStencil(IRHIResourceView * DepthStencilView, float32 Depth, uint8 Stencial)
	{
		FVulkanResourceView & VulkanResourceView = StaticCastRef<FVulkanResourceView>(DepthStencilView);
		//ResourceBarrier(VulkanResourceView.VulkanResource, Vulkan_RESOURCE_STATE_DEPTH_WRITE, true);

		//if (VulkanResourceView.DescriptorLocation.Address.Offline())
		{
			//Vulkan_CLEAR_FLAGS ClearFlags = Vulkan_CLEAR_FLAG_DEPTH | Vulkan_CLEAR_FLAG_STENCIL;
			//InnerCommandList->ClearDepthStencilView(CPUDescriptorHandle(VulkanResourceView.DescriptorLocation.Address.CPUAddress), ClearFlags, Depth, Stencial, 0, nullptr);
		}
	}

	void FVulkanCommandList::ClearRenderTarget(IRHIResourceView * RenderTargetView, FColorF Color)
	{
		uintx RenderTargetIndex = RenderPassState.RenderTargets.FindIndexBy([&](const FRenderPassTarget & RenderPassTarget) { return RenderPassTarget.ResourceView == RenderTargetView; });
		AssertExpr(RenderTargetIndex != NullIndex);
		if (RenderTargetIndex == NullIndex)
			return;

		ApplyRenderStates();

		VkClearRect vkClearRect = { { 0, 0, RenderTargetView->GetResource()->ResourceDesc.Size.Width, RenderTargetView->GetResource()->ResourceDesc.Size.Height }, 0, 1 };
		VkClearAttachment vkClearAttachment { };
		vkClearAttachment.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		vkClearAttachment.colorAttachment = RenderTargetIndex;
		vkClearAttachment.clearValue = FromClearValue(Color);
		vkCmdClearAttachments(vkCommandBuffer, 1, &vkClearAttachment, 1, &vkClearRect);
	}

	void FVulkanCommandList::ClearUnorderedAccess(IRHIResourceView * UnorderedAccessView, uint32 ClearValue)
	{
		FVulkanResourceView & VulkanResourceView = StaticCastRef<FVulkanResourceView>(UnorderedAccessView);
		AssertExpr(VulkanResourceView.VulkanResource.ResourceDesc.Size.Y == 4);
		FlushBarriers();
		vkCmdFillBuffer(vkCommandBuffer, VulkanResourceView.VulkanResource.vkBuffer, 0, VulkanResourceView.VulkanResource.ResourceDesc.Size.X * VulkanResourceView.VulkanResource.ResourceDesc.Size.Y, ClearValue);
	}

	void FVulkanCommandList::SetViewPorts(TView<FViewport> ViewPorts)
	{
		NumViewports = (uint32)ViewPorts.Size;
		for (uintx ViewportIndex : IRange(ViewPorts.Size))
		{
			// VK_KHR_Maintenance1
			this->vkViewports[ViewportIndex] = FromViewport(ViewPorts[ViewportIndex]);

			this->vkViewports[ViewportIndex].height = -ViewPorts[ViewportIndex].Height;
			this->vkViewports[ViewportIndex].y = ViewPorts[ViewportIndex].Y + ViewPorts[ViewportIndex].Height;
		}
		ViewPortsChanged = true;
	}

	void FVulkanCommandList::SetScissorRects(TView<RectI> ScissorRects)
	{
		NumScissorRects = ScissorRects.Size;
		for (uintx ViewportIndex : IRange(ScissorRects.Size))
			this->vkScissorRects[ViewportIndex] = FromRect2D(ScissorRects[ViewportIndex]);
		ScissorRectsChanged = true;
	}

	void FVulkanCommandList::SetPipelineState(IRHIPipelineState * PipelineState)
	{
		if (this->PipelineState != PipelineState)
		{
			++FrameStatics.PipelineStates;
			this->PipelineState = StaticCast<FVulkanPipelineState>(PipelineState);
			PipelineStateChanged = true;
		}
	}

	static inline FVulkanAddress GetPageableAddress(IRHIPageable * RHIPageable, EDescriptorType DescriptorType)
	{
		return {};
	}

	static FVulkanCommandList::FResourceDescriptor ResourceDescriptorFromPageable(FVulkanDevice & Device, const FVulkanShaderParameter & ShaderParameter, IRHIPageable * Pageable)
	{
		FVulkanCommandList::FResourceDescriptor ResourceDescriptor;
		switch (Pageable->Pageable)
		{
		case EPageable::ResourceView:
		{
			FVulkanResourceView & VulkanResourceView = StaticCastRef<FVulkanResourceView>(Pageable);
			//if (!VulkanResourceView.VulkanResource.InnerResource)
			//	break;

			switch (ShaderParameter.vkDescriptorType)
			{
			case VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER:
			{
				ResourceDescriptor.BufferView = VulkanResourceView.vkBufferView;
				break;
			}
			case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
				//ResourceBarrier(VulkanResourceView.VulkanResource, Vulkan_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, true);
				break;
			case VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE:
			{
				ResourceDescriptor.Image.sampler = Device.FetchSampler(FSamplerDesc {});
				ResourceDescriptor.Image.imageView = VulkanResourceView.vkImageView;
				ResourceDescriptor.Image.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				break;
			}
			case VK_DESCRIPTOR_TYPE_STORAGE_IMAGE:
			{
				ResourceDescriptor.Image.sampler = nullptr;
				ResourceDescriptor.Image.imageView = VulkanResourceView.vkImageView;
				ResourceDescriptor.Image.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
				break;
			}
			default:
				AssertForaml(false, u8"Invalid resource view {}, Type={}."V, ShaderParameter.ParameterName, ShaderParameter.vkDescriptorType);
				break;
			}

			//PipelineParameters[ParameterIndex] = VulkanResourceView.DescriptorLocation.Address;
			break;
		}
		default:
		{
			FVulkanResource & VulkanResource = StaticCastRef<FVulkanResource>(Pageable);
			//if (ShaderParameter.DescriptorType != EDescriptorType::ConstBuffer && !VulkanResource.InnerResource)
			//{
			//	AssertForaml(false, u8"Invalid shader parameter, DescriptorType={}."V, ShaderParameter.DescriptorType);
			//	break;
			//}

			switch (ShaderParameter.vkDescriptorType)
			{
			case VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE:
			{
				ResourceDescriptor.Image.sampler = Device.FetchSampler(FSamplerDesc {});
				ResourceDescriptor.Image.imageView = VulkanResource.ResourceView->vkImageView;
				ResourceDescriptor.Image.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				break;
			}
			case VK_DESCRIPTOR_TYPE_STORAGE_IMAGE:
			{
				if (true || ShaderParameter.Format == EFormat::None ||
					VulkanResource.ResourceDesc.Format == ShaderParameter.Format)
				{
					ResourceDescriptor.Image.sampler = nullptr;
					ResourceDescriptor.Image.imageView = VulkanResource.ResourceView->vkImageView;
					ResourceDescriptor.Image.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
				}
				else
				{
					ResourceDescriptor.Image.sampler = nullptr;
					ResourceDescriptor.Image.imageView = VulkanResource.ResourceView->GetImageViewMutable(ShaderParameter.Format);
					ResourceDescriptor.Image.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
				}
				break;
			}
			case VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER:
			{
				ResourceDescriptor.BufferView = VulkanResource.ResourceView->vkBufferView;
				break;
			}
			case VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER:
			{
				ResourceDescriptor.BufferView = VulkanResource.ResourceView->vkBufferView;
				break;
			}
			case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
			{
				ResourceDescriptor.Buffer.buffer = VulkanResource.vkBuffer;
				ResourceDescriptor.Buffer.offset = 0;
				ResourceDescriptor.Buffer.range = VulkanResource.ResourceDesc.Size.X;
				break;
			}
			case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER:
				//ResourceBarrier(VulkanResource, Vulkan_RESOURCE_STATE_UNORDERED_ACCESS, true);
				//PipelineParameters[ParameterIndex] = VulkanResource.ResourceViews.UAV->DescriptorLocation.Address;
				break;
			default:
				AssertForaml(false, u8"Invalid resource {}, Type={}."V, ShaderParameter.ParameterName, ShaderParameter.vkDescriptorType);
				break;
			}
		}
		}

		return ResourceDescriptor;
	}

	void FVulkanCommandList::SetParameter(FName ParameterName, const FRHIBlock & Block)
	{
		for (uintx ParameterIndex = 0; ParameterIndex < MaxVulkanShaderParameters; ++ParameterIndex)
		{
			if (const FVulkanShaderParameter & ShaderParameter = PipelineState->ShaderBindings.Parameters[ParameterIndex])
			{
				if (ShaderParameter.ParameterName == ParameterName)
				{
					if (ShaderParameter.vkDescriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
					{
						VkDescriptorBufferInfo DescriptorBufferInfo;

						DescriptorBufferInfo.buffer = VkBuffer(Block.BaseAddress.GPUAddress);
						DescriptorBufferInfo.offset = Block.Location.Offset;
						DescriptorBufferInfo.range = Block.Location.Size;
						ParameterDescriptors[ShaderParameter.DescriptorIndex] = { DescriptorBufferInfo };
						ParametersChanged = true;
					}
					else{}
				}
			}
		}
	}

	void FVulkanCommandList::SetParameter(FName ParameterName, IRHIPageable * Pageable)
	{
		for (uintx ParameterIndex = 0; ParameterIndex < MaxVulkanShaderParameters; ++ParameterIndex)
		{
			if (const FVulkanShaderParameter & ShaderParameter = PipelineState->ShaderBindings.Parameters[ParameterIndex])
			{
				if (ShaderParameter.ParameterName == ParameterName)
				{
					ParameterDescriptors[ShaderParameter.DescriptorIndex] = ResourceDescriptorFromPageable(VulkanDevice, ShaderParameter, Pageable);
					ParameterDescriptors[ShaderParameter.DescriptorIndex].Pageable = Pageable;
					ParametersChanged = true;
					break;
				}
			}
		}
	}

	void FVulkanCommandList::IASetVertexBuffers(uint32 BaseSlotIndex, TView<IRHIResource *> VertexBuffers, TView<uint32> Sizes, TView<uint32> Strides, uint32 VertexCount)
	{
		AssertExpr(VertexBuffers.Size <= Sizes.Size);
		AssertExpr(VertexBuffers.Size <= Strides.Size);

		for (uint32 VertexBufferIndex = 0; VertexBufferIndex < BaseSlotIndex; ++VertexBufferIndex)
		{
			//VertexBufferViews[VertexBufferIndex] = {};
		}

		NumVertexBuffers = Min(VertexBuffers.Size, MaxVertexBuffers);
		for (uint32 VertexBufferIndex = 0; BaseSlotIndex + VertexBufferIndex < NumVertexBuffers; ++VertexBufferIndex)
		{
			FVulkanResource & VulkanResource = StaticCastRef<FVulkanResource>(VertexBuffers[VertexBufferIndex]);
			this->VertexBuffers[VertexBufferIndex] = &VulkanResource;
		}
	}

	void FVulkanCommandList::IASetIndexBuffer(IRHIResource * IndexBuffer, EIndexFormat IndexFormat, uint32 IndexCount)
	{
		if (!IndexBuffer)
		{
			//IndexBufferView = {};
			return;
		}

		FVulkanResource & VulkanResource = StaticCastRef<FVulkanResource>(IndexBuffer);
		this->IndexBuffer = &VulkanResource;
		this->IndexFormat = IndexFormat;
	}

	void FVulkanCommandList::DrawInstanced(uint32 VertexOffset, uint32 VertexCount, uint32 InstanceOffset, uint32 InstanceCount)
	{
		++FrameStatics.DrawCalls;
		//switch (PrimitiveTopology)
		//{
		//case D3D_PRIMITIVE_TOPOLOGY_POINTLIST:
		//	FrameStatics.Primitives += VertexCount;
		//	break;
		//case D3D_PRIMITIVE_TOPOLOGY_LINELIST:
		//	FrameStatics.Primitives += VertexCount / 2;
		//	break;
		//case D3D_PRIMITIVE_TOPOLOGY_LINESTRIP:
		//	FrameStatics.Primitives += (VertexCount + 1) / 2;
		//	break;
		//case D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST:
		//	FrameStatics.Primitives += VertexCount / 3;
		//	break;
		//case D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP:
		//	FrameStatics.Primitives += (VertexCount + 2) / 3;
		//	break;
		//default:
		//	break;
		//}

		FlushBarriers();
		ApplyRenderStates();
		ApplyResources();

		vkCmdDraw(vkCommandBuffer, VertexCount, InstanceCount, VertexOffset, InstanceOffset);
		//InnerCommandList->IASetPrimitiveTopology(PrimitiveTopology);
		//InnerCommandList->IASetVertexBuffers(0, VertexBufferViewCount, VertexBufferViews.Data);
		//InnerCommandList->IASetIndexBuffer(&IndexBufferView);
		//InnerCommandList->DrawInstanced(VertexCount, InstanceCount, VertexBaseIndex, InstanceBaseIndex);

		++NumCommands;
	}

	void FVulkanCommandList::DrawIndexedInstanced(uint32 IndexOffset, uint32 IndexCount, uint32 VertexOffset, uint32 InstanceOffset, uint32 InstanceCount)
	{
		++FrameStatics.DrawCalls;
		switch (PipelineState->PipelineStateDesc.Topology)
		{
		case ETopology::PointList:
			FrameStatics.Primitives += IndexCount;
			break;
		case ETopology::LineList:
			FrameStatics.Primitives += IndexCount / 2;
			break;
		case ETopology::LineStrip:
			FrameStatics.Primitives += (IndexCount + 1) / 2;
			break;
		case ETopology::TriangleList:
			FrameStatics.Primitives += IndexCount / 3;
			break;
		case ETopology::TriangleStrip:
			FrameStatics.Primitives += (IndexCount + 2) / 3;
			break;
		default:
			break;
		}

		ApplyRenderStates();
		ApplyResources();
		FlushBarriers();
		vkCmdDrawIndexed(vkCommandBuffer, IndexCount, InstanceCount, IndexOffset, VertexOffset, 0);
		++NumCommands;
	}

	void FVulkanCommandList::Dispatch(Vec3U GroupCount)
	{
		++FrameStatics.DispatchCalls;
		FlushBarriers();
		ApplyRenderStates();
		ApplyResources();
		vkCmdDispatch(vkCommandBuffer, GroupCount.X, GroupCount.Y, GroupCount.Z);
	}

	void FVulkanCommandList::DispatchIndirect(IRHIIndirectSignature * IndirectSignature, IRHIResource * IndirectBuffer, uint32 IndirectBufferOffset, uint32 MaxCommandCount)
	{
		FVulkanIndirectSignature & VulkanIndirectSignature = StaticCastRef<FVulkanIndirectSignature>(IndirectSignature);
		FVulkanResource & VulkanIndirectBuffer = StaticCastRef<FVulkanResource>(IndirectBuffer);

		FlushBarriers();
		ApplyRenderStates();
		ApplyResources();
		//InnerCommandList->ExecuteIndirect(VulkanIndirectSignature.VulkanCommandSignature.Get(), MaxCommandCount, VulkanIndirectBuffer.InnerResource, IndirectBufferOffset, nullptr, 0);
		vkCmdDispatchIndirect(vkCommandBuffer, VulkanIndirectBuffer.vkBuffer, IndirectBufferOffset);
		++FrameStatics.IndirectDispatchCalls;
	}

	static VkPipelineStageFlags getLayoutPipelineStageFlags(VkImageLayout layout)
	{
		switch (layout)
		{
		case VK_IMAGE_LAYOUT_UNDEFINED:
			return VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		case VK_IMAGE_LAYOUT_PREINITIALIZED:
			return VK_PIPELINE_STAGE_HOST_BIT;
		case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
		case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
			return VK_PIPELINE_STAGE_TRANSFER_BIT;
		case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
			return VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		case VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL:
			return VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
		case VK_IMAGE_LAYOUT_FRAGMENT_SHADING_RATE_ATTACHMENT_OPTIMAL_KHR:
			return VK_PIPELINE_STAGE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR;
		case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
			return VK_PIPELINE_STAGE_VERTEX_SHADER_BIT | VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR:
			return VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
		case VK_IMAGE_LAYOUT_GENERAL:
			assert(false && "Don't know how to get a meaningful VkPipelineStageFlags for VK_IMAGE_LAYOUT_GENERAL! Don't use it!");
			return 0;
		default:
			assert(false);
			return 0;
		}
	}

	void FVulkanCommandList::ResourceAccess(IRHIResource * Resource, EResourceAccess AccessAfter)
	{
		FVulkanResource & VulkanResource = StaticCastRef<FVulkanResource>(Resource);
		AssertExpr(VulkanResource.Memory.PlaceHeap || VulkanResource.ExternalImage);

		if (AccessAfter == EResourceAccess::None)
			AccessAfter = VulkanResource.InitialAccess;

		VulkanResource.ResourceAccess = AccessAfter;

		VkPipelineStageFlags PipelineStagesAfter = FromResourceAccess_PipelineStage(AccessAfter);
		VkAccessFlags AccessFlagsAfter = FromResourceAccess_AccessFlags(AccessAfter);

		if (VulkanResource.ResourceDesc.Dimension == EResourceDimension::Buffer)
		{
			VkBufferMemoryBarrier BufferMemoryBarrier { VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER };
			BufferMemoryBarrier.srcAccessMask = VulkanResource.AccessFlags;
			BufferMemoryBarrier.dstAccessMask = AccessFlagsAfter;
			BufferMemoryBarrier.buffer = VulkanResource.vkBuffer;
			BufferMemoryBarrier.offset = VulkanResource.Location.Offset;
			BufferMemoryBarrier.size = VulkanResource.Location.Size;

			vkCmdPipelineBarrier(vkCommandBuffer, VulkanResource.PipelineStages, PipelineStagesAfter, VkDependencyFlags(0),
				0, nullptr,
				1, &BufferMemoryBarrier,
				0, nullptr);
			++NumCommands;

			VulkanResource.PipelineStages = PipelineStagesAfter;
			VulkanResource.AccessFlags = AccessFlagsAfter;
		}
		else
		{
			VkImageLayout ImageLayoutAfter = FromResourceAccess_Layout(AccessAfter);

			// Create an image barrier object
			VkImageMemoryBarrier ImageMemoryBarrier { VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER };
			ImageMemoryBarrier.srcAccessMask = VulkanResource.AccessFlags;
			ImageMemoryBarrier.dstAccessMask = FromResourceAccess_AccessFlags(AccessAfter);
			ImageMemoryBarrier.oldLayout = VulkanResource.ImageLayout;
			ImageMemoryBarrier.newLayout = ImageLayoutAfter;
			ImageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			ImageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			ImageMemoryBarrier.image = VulkanResource.vkImage;

			VkImageSubresourceRange SubresourceRange;
			if (AccessAfter & EResourceAccess::DepthStencil)
			{
				SubresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
				if (IsStencilFormat(VulkanResource.ResourceDesc.Format))
					SubresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
			}
			else
			{
				if (IsDepthStencilFormat(VulkanResource.ResourceDesc.Format))
					SubresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
				else
					SubresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			}

			SubresourceRange.baseMipLevel = 0;
			SubresourceRange.levelCount = VK_REMAINING_MIP_LEVELS;
			SubresourceRange.baseArrayLayer = 0;
			SubresourceRange.layerCount = VK_REMAINING_ARRAY_LAYERS;
			ImageMemoryBarrier.subresourceRange = SubresourceRange;

			vkCmdPipelineBarrier(vkCommandBuffer, VulkanResource.PipelineStages, PipelineStagesAfter, VkDependencyFlags(0),
				0, nullptr,
				0, nullptr,
				1, &ImageMemoryBarrier);
			++NumCommands;

			VulkanResource.PipelineStages = PipelineStagesAfter;
			VulkanResource.AccessFlags = AccessFlagsAfter;
			VulkanResource.ImageLayout = ImageLayoutAfter;
		}
	}

	void FVulkanCommandList::ResourceBarrierUAV(IRHIResource * Resource)
	{
		AssertExpr(Resource);
		if (!Resource)
			return;

		FVulkanResource & VulkanResource = StaticCastRef<FVulkanResource>(Resource);
		//AssertExpresion(VulkanResource.InnerResource);

		//Vulkan_RESOURCE_BARRIER ResourceBarrier;
		//ResourceBarrier.Type = Vulkan_RESOURCE_BARRIER_TYPE_UAV;
		//ResourceBarrier.Flags = Vulkan_RESOURCE_BARRIER_FLAG_NONE;
		//ResourceBarrier.UAV.pResource = VulkanResource.InnerResource;
		//Barriers.Add(ResourceBarrier);
	}

	void FVulkanCommandList::ResourceBarrierAlias(IRHIResource * ResourceBefore, IRHIResource * ResourceAfter)
	{
		AssertExpr(ResourceAfter);
		if (!ResourceAfter)
			return;

		//AssertExpresion(!ResourceBefore || StaticCastRef<FVulkanResource>(ResourceBefore).InnerResource);
		//AssertExpresion(StaticCastRef<FVulkanResource>(ResourceAfter).InnerResource);
		//Vulkan_RESOURCE_BARRIER ResourceBarrier;
		//ResourceBarrier.Type = Vulkan_RESOURCE_BARRIER_TYPE_ALIASING;
		//ResourceBarrier.Flags = Vulkan_RESOURCE_BARRIER_FLAG_NONE;
		//ResourceBarrier.Aliasing.pResourceBefore = ResourceBefore ? StaticCastRef<FVulkanResource>(ResourceBefore).InnerResource.Get() : nullptr;
		//ResourceBarrier.Aliasing.pResourceAfter = StaticCastRef<FVulkanResource>(ResourceAfter).InnerResource;
		//Barriers.Add(ResourceBarrier);
	}

	void FVulkanCommandList::CopyResource(IRHIResource * DestinationResource, IRHIResource * SourceResource)
	{
		throw 0;
		//AssertExpresion(DestinationResource->Dimension == SourceResource->Dimension);
		//if (DestinationResource->Dimension != SourceResource->Dimension)
		//	return;

		//FlushBarriers();
		//if (DestinationResource->Dimension == EResourceDimension::Texture2D)
		//{
		//	VulkanCommandList->CopyResource(
		//		StaticCast<FVulkanResource>(DestinationResource)->D3DResource.Get(),
		//		StaticCast<FVulkanResource>(SourceResource)->D3DResource.Get());
		//}
		//++CommandCount;
	}

	void FVulkanCommandList::CopyBuffer(IRHIResource * TargetResource, uint64 TargetOffset, IRHIResource * SourceResource, uint64 SourceOffset, uint64 NumBytes)
	{
		throw 0;
		//FlushBarriers();
		//FVulkanResource & VulkanDestinationBuffer = StaticCastRef<FVulkanResource>(DestinationBuffer);
		//FVulkanResource & VulkanSourceBuffer = StaticCastRef<FVulkanResource>(SourceBuffer);
		//AssertExpresion(DestinationBuffer->Dimension == SourceBuffer->Dimension);
		//VulkanCommandList->CopyBufferRegion(VulkanDestinationBuffer.D3DResource.Get(), DestinationOffset, VulkanSourceBuffer.D3DResource.Get(), SourceOffset, ByteCount);
		//++CommandCount;
	}

	static void UpdateTextureDataOnResourceLocation(const FRHIAddress & Address, const FTextureDesc & TextureDesc, TView<byte> InitialData, TView<FVulkanCopyableFootprint> CopyableFootprints)
	{
		AssertExpr(CopyableFootprints.Size == TextureDesc.Depth * TextureDesc.MipLevels);
		uint64 SourceOffset = 0;
		for (uint32 DepthIndex = 0, SubresourceIndex = 0; DepthIndex < TextureDesc.Depth; ++DepthIndex)
		{
			for (uint32 MipmapIndex = 0; MipmapIndex < TextureDesc.MipLevels; ++MipmapIndex, ++SubresourceIndex)
			{
				FVulkanCopyableFootprint SubresourceFootPrint = CopyableFootprints[SubresourceIndex];

				SizeU MipmapSize = TextureDesc.Size >> MipmapIndex;
				//if (MipmapSize.X == 0) MipmapSize.X = 1;
				//if (MipmapSize.Y == 0) MipmapSize.Y = 1;
				FImageInfo FormatInfo = GetImageInfo(TextureDesc.Format, MipmapSize);
				//AssertExpresion(FormatInfo.RowCount == SubresourceFootPrint.RowCount);
				//AssertExpresion(FormatInfo.RowPitch <= SubresourceFootPrint.RowPitch);
				if (FormatInfo.RowPitch == 0)
					break;

				AssertExpr(SourceOffset + FormatInfo.TotalBytes <= InitialData.Size);
				for (uint32 RowIndex = 0; RowIndex < FormatInfo.RowCount; ++RowIndex)
				{
					AssertExpr(SubresourceFootPrint.Format != EFormat::None);
					//MemoryCopy(Address.CPUAddress + SubresourceFootPrint.Offset + SubresourceFootPrint.RowPitch * RowIndex,
					//	InitialData.Data + SourceOffset + FormatInfo.RowPitch * RowIndex, FormatInfo.RowPitch);
				}
				SourceOffset += FormatInfo.TotalBytes;
			}
		}
	}

	void FVulkanCommandList::UpdateBuffer(IRHIResource * Resource, TView<byte> ResourceBytes, TFunction<FRHIMemory(uint64, uint32)> MemoryAllocator) 
	{
		FVulkanResource & VulkanResource = StaticCastRef<FVulkanResource>(Resource);
		AssertExpr(VulkanResource.vkImage || VulkanResource.vkBuffer);
		if (VulkanResource.ResourceDesc.Dimension == EResourceDimension::Buffer)
		{
			if (VulkanResource.ResourceDesc.Usage & EResourceUsage::HostWrite)
			{
				MemoryCopy(VulkanResource.BaseAddress.CPUAddress + VulkanResource.Location.Offset, ResourceBytes.Data, ResourceBytes.Size);
			}
			else
			{
				FRHIMemory StagingBlock = MemoryAllocator(ResourceBytes.Size, 0);
				FVulkanHeap & VulkanHeap = StaticCastRef<FVulkanHeap>(StagingBlock.PlaceHeap);
				AssertExpr(VulkanHeap.vkBuffer);

				MemoryCopy(VulkanHeap.BaseAddress.CPUAddress + StagingBlock.Location.Offset, ResourceBytes.Data, ResourceBytes.Size);

				VkBufferCopy BufferCopy { StagingBlock.Location.Offset, VulkanResource.Location.Offset, ResourceBytes.Size };
				vkCmdCopyBuffer(vkCommandBuffer, VulkanHeap.vkBuffer, VulkanResource.vkBuffer, 1, &BufferCopy);
				NumCommands += 1;
			}
		}
		else
		{
			if (VulkanResource.ResourceDesc.Usage & EResourceUsage::HostWrite)
			{
				MemoryCopy(VulkanResource.BaseAddress.CPUAddress + VulkanResource.Location.Offset, ResourceBytes.Data, ResourceBytes.Size);
			}
			else
			{
				FRHIMemory StagingBlock = MemoryAllocator(ResourceBytes.Size, 0);
				FVulkanHeap & VulkanHeap = StaticCastRef<FVulkanHeap>(StagingBlock.PlaceHeap);
				AssertExpr(VulkanHeap.vkBuffer);
				MemoryCopy(VulkanHeap.BaseAddress.CPUAddress + StagingBlock.Location.Offset, ResourceBytes.Data, ResourceBytes.Size);

				const FResourceDesc & ResourceDesc = VulkanResource.ResourceDesc;
				TList<FVulkanCopyableFootprint> CopyableFootprints(ResourceDesc.Mipmaps * ResourceDesc.Layers);
				uint64 RequiredSize = VulkanDevice.GetCopyableFootprints(ResourceDesc, CopyableFootprints);

				// Setup buffer copy regions for each mip level
				TList<VkBufferImageCopy> BufferCopyRegions;
				{
					uint32_t BufferOffset = StagingBlock.Location.Offset;

					for (uint32_t MipmapIndex = 0; MipmapIndex < ResourceDesc.Mipmaps; MipmapIndex++)
					{
						const FVulkanCopyableFootprint & CopyableFootprint = CopyableFootprints[MipmapIndex];
						AssertExpr(CopyableFootprint.Size.Width > 0);
						AssertExpr(CopyableFootprint.Size.Height > 0);

						// Calculate offset into staging buffer for the current mip level
						VkBufferImageCopy BufferImageCopy = {};
						BufferImageCopy.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
						BufferImageCopy.imageSubresource.mipLevel = MipmapIndex;
						BufferImageCopy.imageSubresource.baseArrayLayer = 0;
						BufferImageCopy.imageSubresource.layerCount = 1;
						BufferImageCopy.imageExtent.width = CopyableFootprint.Size.Width;
						BufferImageCopy.imageExtent.height = CopyableFootprint.Size.Height;
						BufferImageCopy.imageExtent.depth = 1;
						BufferImageCopy.bufferOffset = BufferOffset;
						BufferCopyRegions.Add(BufferImageCopy);
					}
				}

				vkCmdCopyBufferToImage(vkCommandBuffer, VulkanHeap.vkBuffer, VulkanResource.vkImage,
					VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
					static_cast<uint32_t>(BufferCopyRegions.Size),
					BufferCopyRegions.Data);

				NumCommands += BufferCopyRegions.Size;
			}
		}
	}

	void FVulkanCommandList::UpdateTexture(IRHIResource * Resource, const FTextureSource & TextureSource, TFunction<FRHIMemory(uint64, uint32)> MemoryAllocator)
	{
		FVulkanResource & VulkanResource = StaticCastRef<FVulkanResource>(Resource);
		AssertExpr(VulkanResource.vkImage);

		FRHIMemory StagingBlock = MemoryAllocator(TextureSource.Bytes.Size, 0);
		FVulkanHeap & VulkanHeap = StaticCastRef<FVulkanHeap>(StagingBlock.PlaceHeap);
		AssertExpr(VulkanHeap.vkBuffer);
		MemoryCopy(VulkanHeap.BaseAddress.CPUAddress + StagingBlock.Location.Offset, TextureSource.Bytes.Data, TextureSource.Bytes.Size);

		// Setup buffer copy regions for each mip level
		TList<VkBufferImageCopy> BufferCopyRegions;
		{
			uint32_t BufferOffset = StagingBlock.Location.Offset;

			for (uint32_t MipmapIndex = 0; MipmapIndex < VulkanResource.ResourceDesc.Mipmaps; MipmapIndex++)
			{
				const FTextureSlice & TextureSlice = TextureSource.Slices[MipmapIndex];

				// Calculate offset into staging buffer for the current mip level
				VkBufferImageCopy BufferImageCopy = {};
				BufferImageCopy.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				BufferImageCopy.imageSubresource.mipLevel = MipmapIndex;
				BufferImageCopy.imageSubresource.baseArrayLayer = 0;
				BufferImageCopy.imageSubresource.layerCount = 1;
				BufferImageCopy.imageExtent.width = TextureSlice.Width;
				BufferImageCopy.imageExtent.height = TextureSlice.Height;
				BufferImageCopy.imageExtent.depth = 1;
				BufferImageCopy.bufferOffset = BufferOffset;
				BufferCopyRegions.Add(BufferImageCopy);

				BufferOffset += TextureSlice.SlicePitch;
			}
		}

		vkCmdCopyBufferToImage(vkCommandBuffer, VulkanHeap.vkBuffer, VulkanResource.vkImage,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			static_cast<uint32_t>(BufferCopyRegions.Size),
			BufferCopyRegions.Data);

		NumCommands += BufferCopyRegions.Size;
	}

	VkDescriptorSet FVulkanCommandList::AllocateDescriptorSet()
	{
		FVulkanDescriptorHeapRef OnlineDescriptorHeap;
		if (OnlineDescriptorHeaps.Size == 0 || !OnlineDescriptorHeaps.Back()->CanAllocate())
		{
			OnlineDescriptorHeap = VulkanDevice.AquireOnlineDescriptorHeap(PipelineState->ShaderBindings);
			OnlineDescriptorHeaps.Add(OnlineDescriptorHeap);
			DescriptorHeapsChanged = true;
		}
		else
			OnlineDescriptorHeap = OnlineDescriptorHeaps.Back();
		return OnlineDescriptorHeap->Allocate();
	}

	void FVulkanCommandList::ApplyResources()
	{
		if (!ParametersChanged)
			return;

		ParametersChanged = false;

		VkDescriptorSet DescriptorSet = AllocateDescriptorSet();
		AssertExpr(DescriptorSet);

		if (DescriptorHeapsChanged)
		{
			DescriptorHeapsChanged = false;
			FVulkanDescriptorHeapRef DescriptorHeap = OnlineDescriptorHeaps.Back();
		}

		TArray<VkWriteDescriptorSet, MaxVulkanShaderParameters> WriteDescriptorSets;

		uint32 NumUpdateDescriptorSets = 0;
		for (uintx ParameterIndex = 0; ParameterIndex < MaxVulkanShaderDescriptors; ++ParameterIndex)
		{
			const FVulkanShaderParameter & ShaderParameter = PipelineState->ShaderBindings.Parameters[ParameterIndex];
			if (ShaderParameter.DescriptorIndex == NullIndex || ShaderParameter.vkDescriptorType == VK_DESCRIPTOR_TYPE_MAX_ENUM)
				continue;

			//FResourceDescriptor & ResourceDescriptor = ResourceDescriptors[DesriptorIndex];
			FResourceDescriptor & ResourceDescriptor = ParameterDescriptors[ShaderParameter.DescriptorIndex];

			if (ShaderParameter.vkDescriptorType == VK_DESCRIPTOR_TYPE_SAMPLER)
			{
				if (!PipelineState->ShaderBindings.IsImmutableSampler(ShaderParameter.BindingIndex, ShaderParameter.BindingSpace))
				{
					LogError(u8"Mutable sampler has not implemented."V);
					ResourceDescriptor.Image.sampler = VulkanDevice.FetchSampler(FSamplerDesc {});
				}
				continue;
			}

			VkWriteDescriptorSet & WriteDescriptorSet = WriteDescriptorSets[ShaderParameter.DescriptorIndex];
			WriteDescriptorSet = { VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET };
			WriteDescriptorSet.dstSet = DescriptorSet;
			WriteDescriptorSet.dstBinding = ShaderParameter.BindingIndex;


			WriteDescriptorSet.descriptorType = ShaderParameter.vkDescriptorType;
			switch (ShaderParameter.vkDescriptorType)
			{
			case VK_DESCRIPTOR_TYPE_SAMPLER:
			{
				WriteDescriptorSet.pImageInfo = &ResourceDescriptor.Image;
				WriteDescriptorSet.descriptorCount = 1;
				break;
			}
			case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
			case VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE:
			case VK_DESCRIPTOR_TYPE_STORAGE_IMAGE:
			{
				if (ResourceDescriptor.Pageable->Pageable == EPageable::Resource)
				{
					FVulkanResource & VulkanResource = StaticCastRef<FVulkanResource>(ResourceDescriptor.Pageable);
					ResourceDescriptor.Image.imageLayout = VulkanResource.ImageLayout;
				}
				else if (ResourceDescriptor.Pageable->Pageable == EPageable::ResourceView)
				{
					FVulkanResourceView & VulkanResourceView = StaticCastRef<FVulkanResourceView>(ResourceDescriptor.Pageable);
					ResourceDescriptor.Image.imageLayout = VulkanResourceView.VulkanResource.ImageLayout;
				}
				else{}
				WriteDescriptorSet.pImageInfo = &ResourceDescriptor.Image;
				WriteDescriptorSet.descriptorCount = 1;
				break;
			}
			//case VK_DESCRIPTOR_TYPE_STORAGE_IMAGE:
			//{
			//	WriteDescriptorSet.pImageInfo = &ResourceDescriptor.Image;
			//	WriteDescriptorSet.descriptorCount = 1;
			//	break;
			//}
			case VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER:
			{
				WriteDescriptorSet.pTexelBufferView = &ResourceDescriptor.BufferView;
				WriteDescriptorSet.descriptorCount = 1;
				break;
			}
			case VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER:
			{
				WriteDescriptorSet.pTexelBufferView = &ResourceDescriptor.BufferView;
				WriteDescriptorSet.descriptorCount = 1;
				break;
			}
			case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
			{
				WriteDescriptorSet.pBufferInfo = &ResourceDescriptor.Buffer;
				WriteDescriptorSet.descriptorCount = 1;
				break;
			}
			default:
				AssertForaml(false, u8"Missing resource binding for {}"V, ShaderParameter.ParameterName);
				break;
			}
			++NumUpdateDescriptorSets;
		}

		vkUpdateDescriptorSets(VulkanDevice.vkDevice, NumUpdateDescriptorSets, WriteDescriptorSets.Data, 0, nullptr);

		vkCmdBindDescriptorSets(vkCommandBuffer, PipelineState->vkPipelineBindPoint, PipelineState->vkPipelineLayout, 0, 1, &DescriptorSet, 0, nullptr);

		if (NumVertexBuffers)
		{
			TArray<VkBuffer, MaxVertexBuffers> Buffers;
			TArray<VkDeviceSize, MaxVertexBuffers> Offsets;
			for (uint32 VertexBufferIndex = 0; VertexBufferIndex < NumVertexBuffers; ++VertexBufferIndex)
			{
				Buffers[VertexBufferIndex] = VertexBuffers[VertexBufferIndex]->vkBuffer;
				Offsets[VertexBufferIndex] = 0;
			}
			vkCmdBindVertexBuffers(vkCommandBuffer, 0, NumVertexBuffers, Buffers.Data, Offsets.Data);
		}

		if (IndexBuffer)
		{
			vkCmdBindIndexBuffer(vkCommandBuffer, IndexBuffer->vkBuffer, 0, FromIndexFormat(IndexFormat));
		}
	}

	void FVulkanCommandList::ApplyRenderStates()
	{
		// Pipeline State
		if (PipelineStateChanged)
		{
			PipelineStateChanged = false;
			vkCmdBindPipeline(vkCommandBuffer, PipelineState->vkPipelineBindPoint, PipelineState->vkPipeline);
		}

		if (RenderTargetsChanged)
		{
			RenderTargetsChanged = false;
		}
		else {}

		if (ViewPortsChanged)
		{
			ViewPortsChanged = false;
			vkCmdSetViewport(vkCommandBuffer, 0, NumViewports, vkViewports.Data);
		}

		if (ScissorRectsChanged)
		{
			ScissorRectsChanged = false;
			vkCmdSetScissor(vkCommandBuffer, 0, NumScissorRects, vkScissorRects.Data);
			//InnerCommandList->RSSetScissorRects(ScissorRectsCount, VulkanScissorRects.Data);
		}
	}

	void FVulkanCommandList::ResetStates()
	{
		RenderTargetsChanged = false;
		ViewPortsChanged = false;
		ScissorRectsChanged = false;
		RenderPassState = {};
		NumViewports = 0;
		//VulkanViewPorts = {};
		//ScissorRectsCount = 0;
		//VulkanScissorRects = {};
	}
}
