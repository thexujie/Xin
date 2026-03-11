#pragma once

#include "D3D12Types.h"
#include "D3D12Heap.h"
#include "D3D12Resource.h"
#include "D3D12PipelineState.h"
#include "D3D12CommandAllocator.h"

namespace Xin::RHI::D3D12
{
	class FD3D12CommandQueue;
	class FD3D12Resource;
	class FD3D12ResourceView;


	class RHID3D12_API FD3D12CommandList : public IRHICommandList
	{
	public:
		FD3D12CommandList(FD3D12Device & D3D12Devcie, ECommandType CommandType);
		~FD3D12CommandList();
		void SetDebugName(FStringV Name) override;

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
		void CopyDescriptors(D3D12_DESCRIPTOR_RANGE_TYPE DescriptorRangeType, FCPUAddress DestinationDescriptor, FCPUAddress SourceDescriptor, uint32 DescriptorCount);

	public:
		FD3D12Device & D3D12Devcie;
		ECommandType CommandType;

	public:
		ID3D12GraphicsCommandListRef d3d12CommandList;
#if XIN_DEBUG
		intx Debug_CommandListOpening = 0;
#endif
		FD3D12CommandAllocatorRef CommandAllocator;

		uint32 DescriptorAddressStride = 0;

	public:
		TList<FD3D12Resource *> ResourceBarriers;
		TList<D3D12_RESOURCE_BARRIER> Barriers;

	public:
		FFrameStatics FrameStatics;
		uint64 NumCommands = 0;

	private: // StateCache
		void ApplyRenderStates();
		void ResetPipelineStates(FD3D12CommandQueue & D3D12CommandQueue);

		bool PipelineStateChanged : 1 = false;
		bool RenderPassStateChanged : 1 = false;
		bool ViewPortsChanged : 1 = false;
		bool ScissorRectsChanged : 1 = false;
		bool DescriptorHeapsChanged : 1 = false;

		FD3D12PipelineState * PipelineState = nullptr;

		enum class ED3D12DescriptorHeapType
		{
			Resources = 0,
			Samplers,
		};
		static constexpr uintx ED3D12DescriptorHeapTypeCount = 2;
		TArray<ID3D12DescriptorHeap *, ED3D12DescriptorHeapTypeCount> DescriptorHeaps = {};

		FRenderPassState RenderPassState { };
		uint32 NumViewports = 0;
		TArray<D3D12_VIEWPORT, MaxViewports> D3D12ViewPorts = {};

		uint32 NumScissorRects = 0;
		TArray<D3D12_RECT, MaxScissorRects> D3D12ScissorRects = {};

	public:
		FRHIAddress AllocateOnlineDescriptors(uint32 DescriptorsCount);
		void ApplyResources();

		bool ParametersChanged = false;

		TList<FD3D12DescriptorHeapRef> OnlineDescriptorHeaps;

		TArray<FRHIAddress, 512> BindingParameters = { };

		EIndexFormat IndexFormat = EIndexFormat::None;
		FD3D12ResourceRef IndexBuffer;

		uint32 NumVertexBuffers = 0;
		TArray<FD3D12ResourceRef, MaxVertexBuffers> VertexBuffers = {};
		TArray<uint32, MaxVertexBuffers> VertexBufferSizes = {};
		TArray<uint32, MaxVertexBuffers> VertexBufferStrides = {};
	};
	using FD3D12CommandListRef = TReferPtr<FD3D12CommandList>;
}
