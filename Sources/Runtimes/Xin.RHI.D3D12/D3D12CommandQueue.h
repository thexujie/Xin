#pragma once

#include "D3D12Types.h"
#include "D3D12CommandList.h"
#include "D3D12Fence.h"
#include "D3D12Heap.h"

namespace Xin::RHI::D3D12
{
	class RHID3D12_API FD3D12CommandQueue: public IRHICommandQueue
	{
	public:
		FD3D12CommandQueue(FD3D12Device & D3D12Devcie, const FCommandQueueDesc & QueueDesc);
		~FD3D12CommandQueue();

	public:
		void SetMarker(FStringV Event, const FColor & Color) override;
		void BeginEvent(FStringV Event, const FColor & Color) override;
		void EndEvent() override;

	public:
		IRHIFence & GetFence() const override { return ConstCast(QueueFence); }
		void UpdateContext() override;
		void WaitFinish(uint32 TimeoutMS) override;

	public:
		FD3D12Device & D3D12Devcie;
		ID3D12CommandQueueRef d3d12CommandQueue;

		FD3D12Fence QueueFence;
		TList<FD3D12CommandListRef> FreeCommandLists;

	public:
		// FD3D12Context::AquireCommandAllocator
		//ID3D12CommandAllocatorRef AquireCommandAllocator(ECommandType CommandType);
		void FenceCommandAllocator(FD3D12CommandAllocator * D3D12CommandAllocator, ECommandType CommandType);

	public:
		void FenceResource(IRHIResource * RHIResource) override;
		void TileMapping(IRHIResource * Resource, const FRHIMemory & Memory) override;

	private:
		//mutable TArray<TList<ID3D12CommandAllocatorRef>, ECommandTypeCount> FreeCommandAllocators;
		mutable TArray<TList<TPair<uint64, FD3D12CommandAllocatorRef>>, ECommandTypeCount> FencedCommandAllocators;

		mutable TList<TPair<uint64, IRHIResourceRef>> FencedResources;

	public:
		void FenceCommandList(ID3D12GraphicsCommandListRef CommandList, FD3D12CommandAllocatorRef CommandAllocator);

		mutable TList<TPair<uint64, TPair<FD3D12CommandAllocatorRef, ID3D12GraphicsCommandListRef>>> FencedCommandList;

	public:
		uint64 UpdateFencedResources() const;

	public:
		// AquireOnlineDescriptorHeap in FD3D12Context
		//FD3D12DescriptorHeapRef AquireOnlineDescriptorHeap();
		void FenceOnlineDescriptorHeap(FD3D12DescriptorHeapRef DescriptorHeap);

		//TList<FD3D12DescriptorHeapRef> FreeOnlineDescriptorHeaps;
		mutable TList<TPair<uint64, FD3D12DescriptorHeapRef>> FencedOnlineDescriptorHeaps;
	};
	using FD3D12CommandExecutorRef = TReferPtr<FD3D12CommandQueue>;
}
