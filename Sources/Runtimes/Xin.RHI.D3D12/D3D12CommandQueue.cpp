#include "PCH.h"
#include "D3D12CommandQueue.h"
#include "D3D12Device.h"
#include "D3D12Fence.h"

namespace Xin::RHI::D3D12
{
	FD3D12CommandQueue::FD3D12CommandQueue(FD3D12Device & D3D12Devcie, const FCommandQueueDesc & QueueDesc)
		: IRHICommandQueue(D3D12Devcie, QueueDesc), D3D12Devcie(D3D12Devcie), QueueFence(D3D12Devcie)
	{
		D3D12_COMMAND_QUEUE_DESC D3D12CommandQueueDesc = {};
		D3D12CommandQueueDesc.Type = FromCommandType(QueueDesc.Type);
		HRESULT Result = D3D12Devcie.d3d12Device->CreateCommandQueue(&D3D12CommandQueueDesc, IID_PPV_ARGS(d3d12CommandQueue.GetPP()));
	}

	FD3D12CommandQueue::~FD3D12CommandQueue()
	{
	}

	void FD3D12CommandQueue::SetMarker(FStringV Event, const FColor & Color)
	{
#if XIN_D3D12_PIX
		::PIXSetMarker(d3d12CommandQueue.Get(), Color.ToARGB(), (const char *)Event.Data);
#endif
	}

	void FD3D12CommandQueue::BeginEvent(FStringV Event, const FColor & Color)
	{

#if XIN_D3D12_PIX
		::PIXBeginEvent(d3d12CommandQueue.Get(), Color.ToARGB(), (const char *)Event.Data);
#endif
	}

	void FD3D12CommandQueue::EndEvent()
	{
#if XIN_D3D12_PIX
		::PIXEndEvent(d3d12CommandQueue.Get());
#endif
	}

	void FD3D12CommandQueue::UpdateContext()
	{
		UpdateFencedResources();
	}

	void FD3D12CommandQueue::WaitFinish(uint32 TimeoutMS)
	{
		QueueFence.WaitFence(NullIndex);
	}

	void FD3D12CommandQueue::FenceCommandAllocator(FD3D12CommandAllocator * RHICommandAllocator, ECommandType CommandType)
	{
		FencedCommandAllocators[CommandType].Add({ QueueFence.SignalValue, RHICommandAllocator });
	}

	void FD3D12CommandQueue::FenceResource(IRHIResource * RHIResource)
	{
		if (!RHIResource)
			return;

		FencedResources.Emplace(TPair<uint64, IRHIResourceRef>{ QueueFence.SignalValue, RHIResource });
	}

	void FD3D12CommandQueue::TileMapping(IRHIResource * Resource, const FRHIMemory & Memory)
	{
		FD3D12Resource & D3D12Resource = StaticCastRef<FD3D12Resource>(Resource);
		FD3D12Heap & D3D12Heap = StaticCastRef<FD3D12Heap>(Memory.PlaceHeap);
		AssertExpr(D3D12Heap.d3d12Heap);

		D3D12_TILED_RESOURCE_COORDINATE StartCoordinates = { 0, 0, 0, 0 };
		D3D12_TILE_REGION_SIZE RegionSizes = { 1, false };
		D3D12_TILE_RANGE_FLAGS TileRangeFlags = D3D12_TILE_RANGE_FLAGS::D3D12_TILE_RANGE_FLAG_NONE;
		uint32 RangeOffsets = 0;
		uint32 RangetTileCounts = 1;
		d3d12CommandQueue->UpdateTileMappings(D3D12Resource.d3d12Resource, 1, &StartCoordinates, &RegionSizes, D3D12Heap.d3d12Heap, 1, &TileRangeFlags, &RangeOffsets, &RangetTileCounts, D3D12_TILE_MAPPING_FLAG_NONE);
	}

	void FD3D12CommandQueue::FenceCommandList(ID3D12GraphicsCommandListRef CommandList, FD3D12CommandAllocatorRef CommandAllocator)
	{
		FencedCommandList.Add({ QueueFence.SignalValue, { CommandAllocator, CommandList } });
	}

	void FD3D12CommandQueue::FenceOnlineDescriptorHeap(FD3D12DescriptorHeapRef DescriptorHeap)
	{
		FencedOnlineDescriptorHeaps.Add({ QueueFence.SignalValue, DescriptorHeap });
	}

	uint64 FD3D12CommandQueue::UpdateFencedResources() const
	{
		uint64 FenceCompletedValue = QueueFence.GetCompletedValue();

		{
			auto ResourceIter = FencedResources.Begin();
			while (ResourceIter != FencedResources.End())
			{
				if (ResourceIter->First <= FenceCompletedValue)
				{
					ResourceIter = FencedResources.Erase(ResourceIter);
				}
				else
					++ResourceIter;
			}
		}

		{
			auto Iter = FencedCommandList.Begin();
			while (Iter != FencedCommandList.End())
			{
				if (Iter->First <= FenceCompletedValue)
				{
					Iter->Second.First->FreeCommandList(Iter->Second.Second);
					Iter = FencedCommandList.Erase(Iter);
				}
				else
					++Iter;
			}
		}

		TList<TPair<uint64, FD3D12CommandAllocatorRef>> & FencedCommandAllocators_Typed = FencedCommandAllocators[ECommandType::Direct];
		auto CommandAllocatorIter = FencedCommandAllocators_Typed.Begin();
		while (CommandAllocatorIter != FencedCommandAllocators_Typed.End())
		{
			if (CommandAllocatorIter->First <= FenceCompletedValue)
			{
				FD3D12CommandAllocatorRef D3D12CommandAllocator = CommandAllocatorIter->Second;
				CommandAllocatorIter = FencedCommandAllocators_Typed.Erase(CommandAllocatorIter);

				D3D12Devcie.FreeCommandAllocator(D3D12CommandAllocator, ECommandType::Direct);
			}
			else
				++CommandAllocatorIter;
		}

		for (auto DescriptorHeapIter = FencedOnlineDescriptorHeaps.Begin(); DescriptorHeapIter != FencedOnlineDescriptorHeaps.End(); /**/)
		{
			if (DescriptorHeapIter->First <= FenceCompletedValue)
			{
				D3D12Devcie.FreeOnlineDescriptorHeap(DescriptorHeapIter->Second);
				DescriptorHeapIter = FencedOnlineDescriptorHeaps.Erase(DescriptorHeapIter);
			}
			else
				++DescriptorHeapIter;
		}
		return FenceCompletedValue;
	}
}
