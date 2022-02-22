#pragma once

#include "RHITypes.h"
#include "RHIResource.h"
#include "RHIPipelineState.h"

namespace Xin::RHI
{
	class IRHIDevice;
	class IRHIFence;
	class IRHICommandQueue;

	struct FFrameStatics
	{
		uint32 DrawCalls = 0;
		uint32 DispatchCalls = 0;
		uint32 IndirectDispatchCalls = 0;
		uint32 Primitives = 0;
		uint32 RootSignatures = 0;
		uint32 PipelineStates = 0;
	};

	struct FTextureUpdateData
	{
		uint32 Width;
		uint32 Height;
		const byte * Data;
		uint64 Size;
	};

	class RHI_API IRHICommandAllocator : public IRHIObject
	{
	public:
		virtual void Reset() = 0;

	public:
		ECommandType CommandType = ECommandType::Direct;
	};
	using IRHICommandAllocatorRef = TReferPtr<IRHICommandAllocator>;

	struct FRenderPassTarget
	{
		IRHIResourceView * ResourceView = nullptr;
		EResourceLoad LoadAction = EResourceLoad::None;
		FClearValue ClearValue = { };

		FRenderPassTarget() = default;

		FRenderPassTarget(nullptr_t) {}

		FRenderPassTarget(IRHIResource * Resource, EResourceLoad LoadAction = EResourceLoad::None, FClearValue ClearValue = {})
			: ResourceView(Resource->GetResourceView()), LoadAction(LoadAction), ClearValue(ClearValue) { }

		FRenderPassTarget(IRHIResourceView * ResourceView, EResourceLoad LoadAction = EResourceLoad::None, FClearValue ClearValue = {})
			: ResourceView(ResourceView), LoadAction(LoadAction), ClearValue(ClearValue) { }

		constexpr operator bool() const { return !!ResourceView; }
	};

	struct FRenderPassState
	{
		TArray<FRenderPassTarget, MaxRenderTargets> RenderTargets;
		FRenderPassTarget DepthStencil;
		TArray<FViewport, MaxViewports> Viewports;
	};

	class RHI_API IRHICommandList : public IRHIObject
	{
	public:
		virtual void SetMarker(FStringV Marker, const FColor & Color = {}) = 0;
		virtual void BeginLabel(FStringV Label, const FColor & Color = {}) = 0;
		virtual void EndLabel() = 0;

		virtual void BeginCommand(IRHICommandQueue * CommandQueue) = 0;
		virtual void EndCommand(IRHICommandQueue * CommandQueue) = 0;
		virtual void ExecuteCommands(IRHICommandQueue * CommandQueue) = 0;
		virtual void FlushCommands(IRHICommandQueue * CommandQueue) = 0;

	public:
		virtual FFrameStatics GetFrameStatics() const = 0;

		virtual void FlushBarriers() = 0;

		virtual void BeginPass(const FRenderPassState & RenderPassState) = 0;
		virtual void EndPass() = 0;

		virtual void DiscardResource(IRHIResource * Resource, TView<RectU> Rects = {}) = 0;
		virtual void ClearDepthStencil(IRHIResourceView * DepthStencilView, float32 Depth, uint8 Stencial) = 0;
		virtual void ClearRenderTarget(IRHIResourceView * RenderTargetView, FColorF Color) = 0;
		virtual void ClearUnorderedAccess(IRHIResourceView * UnorderedAccessView, uint32 ClearValue) = 0;

		virtual void SetViewPorts(TView<FViewport> ViewPorts) = 0;
		virtual void SetScissorRects(TView<RectI> ScissorRects) = 0;

		virtual void SetPipelineState(IRHIPipelineState * PipelineState) = 0;

		virtual void SetParameter(FName ParameterName, const FRHIBlock & Block) = 0;
		virtual void SetParameter(FName ParameterName, IRHIPageable * Pageable) = 0;

		virtual void IASetVertexBuffers(uint32 BaseSlotIndex, TView<IRHIResource *> VertexBuffers, TView<uint32> Sizes, TView<uint32> Strides, uint32 VertexCount) = 0;
		virtual void IASetIndexBuffer(IRHIResource * IndexBuffer, EIndexFormat IndexFormat, uint32 IndexCount) = 0;

		virtual void DrawInstanced(uint32 VertexOffset, uint32 VertexCount, uint32 InstanceOffset = 0, uint32 InstanceCount = 1) = 0;
		virtual void DrawIndexedInstanced(uint32 IndexOffset, uint32 IndexCount, uint32 VertexOffset, uint32 InstanceOffset = 0, uint32 InstanceCount = 1) = 0;

		virtual void Dispatch(Vec3U GroupCount) = 0;
		virtual void DispatchIndirect(IRHIIndirectSignature * IndirectSignature, IRHIResource * IndirectBuffer, uint32 IndirectBufferOffset, uint32 MaxCommandCount) = 0;

		virtual void ResourceAccess(IRHIResource * Resource, EResourceAccess AccessAfter) = 0;
		virtual void ResourceBarrierUAV(IRHIResource * Resource) = 0;
		virtual void ResourceBarrierAlias(IRHIResource * ResourceBefore, IRHIResource * ResourceAfter) = 0;

		virtual void CopyResource(IRHIResource * TargetResource, IRHIResource * SourceResource) = 0;
		virtual void CopyBuffer(IRHIResource * TargetResource, uint64 TargetOffset, IRHIResource * SourceResource, uint64 SourceOffset, uint64 NumBytes) = 0;
		virtual void UpdateBuffer(IRHIResource * Resource, TView<byte> ResourceBytes, TFunction<FRHIMemory(uint64, uint32)> MemoryAllocator) = 0;
		virtual void UpdateTexture(IRHIResource * Resource, const FTextureSource & TextureSource, TFunction<FRHIMemory(uint64, uint32)> MemoryAllocator) = 0;

	public:
		class FScopedEvent
		{
		public:
			FScopedEvent(IRHICommandList & RHICommandList, FStringV EventName, FColor Color = Colors::None) : RHICommandList(RHICommandList)
			{
				if (EventName.Size)
				{
					RHICommandList.BeginLabel(EventName, Color);
					HasBegin = true;
				}
			}

			~FScopedEvent()
			{
				if (HasBegin)
					RHICommandList.EndLabel();
			}

		public:
			IRHICommandList & RHICommandList;
			bool HasBegin = false;
		};

	};
	using IRHICommandListRef = TReferPtr<IRHICommandList>;

	class RHI_API IRHICommandQueue: public IRHIObject
	{
	public:
		IRHICommandQueue(IRHIDevice & Device, const FCommandQueueDesc & CommandExecutorDesc) : Device(Device), QueueDesc(CommandExecutorDesc) {}

	public:
		virtual void SetMarker(FStringV Event, const FColor & Color) = 0;
		virtual void BeginEvent(FStringV Event, const FColor & Color) = 0;
		virtual void EndEvent() = 0;

	public:
		virtual IRHIFence & GetFence() const = 0;
		virtual void UpdateContext() = 0;
		virtual void WaitFinish(uint32 TimeoutMS = 0) = 0;

	public:
		virtual void FenceResource(IRHIResource * Resource) = 0;
		virtual void TileMapping(IRHIResource * Resource, const FRHIMemory & Memory) = 0;

	public:
		class FScopedEvent
		{
		public:
			FScopedEvent(IRHICommandQueue & RHICommandQueue, FStringV EventName, FColor Color = Colors::None) : RHICommandQueue(RHICommandQueue)
			{
				if (EventName.Size)
				{
					RHICommandQueue.BeginEvent(EventName, Color);
					HasBegin = true;
				}
			}

			~FScopedEvent()
			{
				if (HasBegin)
					RHICommandQueue.EndEvent();
			}

		public:
			IRHICommandQueue & RHICommandQueue;
			bool HasBegin = false;
		};

	public:
		IRHIDevice & Device;
		FCommandQueueDesc QueueDesc;
	};
	using IRHICommandQueueRef = TReferPtr<IRHICommandQueue>;
}
