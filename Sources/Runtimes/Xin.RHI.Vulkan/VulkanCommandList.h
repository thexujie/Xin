#pragma once

#include "VulkanTypes.h"
#include "VulkanHeap.h"
#include "VulkanCommandAllocator.h"
#include "VulkanResource.h"
#include "VulkanPipelineState.h"

namespace Xin::RHI::Vulkan
{
	class FVulkanCommandQueue;
	class FVulkanResource;
	class FVulkanResourceView;


	class RHI_VULKAN_API FVulkanCommandList : public IRHICommandList
	{
	public:
		FVulkanCommandList(FVulkanDevice & VulkanDevice, ECommandType CommandType);
		~FVulkanCommandList();

		void SetDebugName(FStringV ResourceLabel) override;

	public:
		void BeginCommand(IRHICommandQueue * CommandQueue) override;
		void EndCommand(IRHICommandQueue * CommandQueue) override;
		void ExecuteCommands(IRHICommandQueue * CommandQueue) override;

		void SetMarker(FStringV Marker, const FColor & Color) override;
		void BeginLabel(FStringV Label, const FColor & Color) override;
		void EndLabel() override;
		FFrameStatics GetFrameStatics() const override { return FrameStatics; }

	public:
		void FlushBarriers() override;

		void BeginPass(const FRenderPassState & RenderPassState) override;
		void EndPass() override;

		void DiscardResource(IRHIResource * Resource, TView<RectU> Rects) override;
		void ClearDepthStencil(IRHIResourceView * DepthStencilView, float32 Depth, uint8 Stencial) override;
		void ClearRenderTarget(IRHIResourceView * RenderTargetView, FColorF Color) override;
		void ClearUnorderedAccess(IRHIResourceView * UnorderedAccessView, uint32 ClearValue) override;

		void SetViewPorts(TView<FViewport> ViewPorts) override;
		void SetScissorRects(TView<RectI> ScissorRects) override;

		void SetPipelineState(IRHIPipelineState * PipelineState) override;

		void SetParameter(FName ParameterName, const FRHIBlock & Block) override;
		void SetParameter(FName ParameterName, IRHIPageable * Pageable) override;
		
		void IASetVertexBuffers(uint32 BaseSlotIndex, TView<IRHIResource *> VertexBuffers, TView<uint32> Sizes, TView<uint32> Strides, uint32 VertexCount) override;
		void IASetIndexBuffer(IRHIResource * IndexBuffer, EIndexFormat IndexFormat, uint32 IndexCount) override;

		void DrawInstanced(uint32 VertexOffset, uint32 VertexCount, uint32 InstanceOffset = 0, uint32 InstanceCount = 1) override;
		void DrawIndexedInstanced(uint32 IndexOffset, uint32 IndexCount, uint32 VertexOffset, uint32 InstanceOffset = 0, uint32 InstanceCount = 1) override;

		void Dispatch(Vec3U GroupCount) override;
		void DispatchIndirect(IRHIIndirectSignature * IndirectSignature, IRHIResource * IndirectBuffer, uint32 IndirectBufferOffset, uint32 MaxCommandCount) override;

		void ResourceAccess(IRHIResource * Resource, EResourceAccess AccessAfter) override;
		void ResourceBarrierUAV(IRHIResource * Resource) override;
		void ResourceBarrierAlias(IRHIResource * ResourceBefore, IRHIResource * ResourceAfter) override;

		void CopyResource(IRHIResource * TargetResource, IRHIResource * SourceResource) override;
		void CopyBuffer(IRHIResource * TargetResource, uint64 TargetOffset, IRHIResource * SourceResource, uint64 SourceOffset, uint64 NumBytes) override;

		void UpdateBuffer(IRHIResource * Resource, TView<byte> ResourceBytes, TFunction<FRHIMemory(uint64, uint32)> MemoryAllocator) override;
		void UpdateTexture(IRHIResource * Resource, const FTextureSource & TextureSource, TFunction<FRHIMemory(uint64, uint32)> MemoryAllocator) override;

	public:
		void BeginRecord();
		void EndRecord();
		void AfterExecute();
		void StagingForward(FVulkanCommandQueue & VulkanCommandQueue);
		void ResetRenderStates(FVulkanCommandQueue & VulkanCommandQueue);

	public:
		FVulkanDevice & VulkanDevice;
		ECommandType CommandType;

		VkCommandBuffer vkCommandBuffer { nullptr };
		FVulkanCommandAllocatorRef CommandAllocator { nullptr };

		TList<VkBuffer> StagingBuffers;
		TList<VkDeviceMemory> StagingMemorys;

	public:
		FFrameStatics FrameStatics;
		int64 IsPassRendering = 0;
		uint64 NumCommands = 0;

	private: // StateCache
		void ApplyRenderStates();
		void ResetStates();

		bool PipelineStateChanged : 1 = false;
		bool RenderTargetsChanged : 1 = false;
		bool ViewPortsChanged : 1 = false;
		bool ScissorRectsChanged : 1 = false;
		bool DescriptorHeapsChanged : 1 = false;

		FVulkanPipelineState * PipelineState = nullptr;
		FRenderPassState RenderPassState;

		uint32 NumViewports = 0;
		TArray<VkViewport, MaxViewports> vkViewports = {};

		uint32 NumScissorRects = 0;
		TArray<VkRect2D, MaxScissorRects> vkScissorRects = {};

	private: // Draw
		struct FPrimitiveResources
		{
			uint32 VertexBufferViewCount = 0;
			TArray<FVulkanResource *, MaxVertexBuffers> VertexBuffers = {};
			FVulkanResource * IndexBuffer = nullptr;
		};
		FPrimitiveResources PrimitiveResources;
		//Vulkan_INDEX_BUFFER_VIEW IndexBufferView {};

		//Vulkan_PRIMITIVE_TOPOLOGY PrimitiveTopology { Vulkan_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_UNDEFINED };

	public: // Resources
		VkDescriptorSet AllocateDescriptorSet();
		void ApplyResources();

	public:
		TList<FVulkanDescriptorHeapRef> OnlineDescriptorHeaps;

		bool ParametersChanged = false;

		TArray<FVulkanResource *, 512> ParameterResources = { };

		struct FResourceDescriptor
		{
			FResourceDescriptor() = default;
			FResourceDescriptor(const VkDescriptorBufferInfo & Buffer) : Buffer(Buffer) {}
			FResourceDescriptor(const VkDescriptorImageInfo & Image) : Image(Image) {}
			FResourceDescriptor(const VkBufferView & BufferView) : BufferView(BufferView) {}

			union
			{
				VkDescriptorBufferInfo Buffer;
				VkBufferView BufferView;
				VkDescriptorImageInfo Image;
			};

			IRHIPageable * Pageable = nullptr;
		};
		TArray<FResourceDescriptor, MaxVulkanShaderParameters> ParameterDescriptors;

		FVulkanResourceRef IndexBuffer;
		EIndexFormat IndexFormat = EIndexFormat::None;

		uint32 NumVertexBuffers = 0;
		TArray<FVulkanResourceRef, MaxVertexBuffers> VertexBuffers = {};
	};
	using FVulkanCommandListRef = TReferPtr<FVulkanCommandList>;
}
