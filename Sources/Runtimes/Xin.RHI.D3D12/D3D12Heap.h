#pragma once

#include "D3D12Types.h"

#define D3D12_HEAP_DEBUG 1

namespace Xin::RHI::D3D12
{
	class FD3D12Device;

	class FD3D12Heap: public IRHIHeap
	{
	public:
		FD3D12Heap(FD3D12Device & Device, const FHeapDesc & HeapDesc);

		void SetDebugName(FStringV Name) override;

	public:
		FD3D12Device & Device;

		ID3D12HeapRef d3d12Heap;
		ID3D12ResourceRef d3d12Resource;
	};

	class FD3D12DescriptorHeap : public IRHIObject
	{
	public:
		FD3D12DescriptorHeap(FD3D12Device & Device, const FDescriptorHeapDesc & DescriptorHeapDesc);

		void SetDebugName(FStringV Name) override;

	public:
		FD3D12Device & Device;
		ID3D12DescriptorHeapRef D3D12DescriptorHeap;

		FDescriptorHeapDesc Desc;
		FD3D12DescriptorAddress Address;
		uint32 HandleSize = 0;

	public:
		bool CanAllocate(uint32 DescriptorCount) const { return DescriptorIndex + DescriptorCount <= Desc.Size; }
		FD3D12AddressHandle Allocate(uint32 DescriptorCount);
		void ResetAllocate() { DescriptorIndex = 0; }

	private:
		uint32 DescriptorIndex = 0;
	};
	using FD3D12DescriptorHeapRef = TReferPtr<FD3D12DescriptorHeap>;


	struct FD3D12OfflineDescriptorAllocatorDesc
	{
		uint32 HeapSize = 0;
		EDescriptorHeapType Type = EDescriptorHeapType::ShaderResource;
	};


	struct FD3D12DescriptorBlockLocation
	{
		uint64 Offset = 0;
		uint64 Count = 0;

		explicit operator bool() const { return !!Count; }
		bool operator ==(const FD3D12DescriptorBlockLocation &) const = default;
		auto operator <=>(const FD3D12DescriptorBlockLocation &) const = default;
		friend uintx Hash(const FD3D12DescriptorBlockLocation & BuddyBlock) { return HashCombine(BuddyBlock.Offset, BuddyBlock.Count); }
	};

	struct FD3D12DescriptorLocation
	{
		class FD3D12DescriptorAllocator * Allocator = nullptr;
		FD3D12DescriptorHeap * DescriptorHeap = nullptr;
		FD3D12DescriptorBlockLocation BlockLocation;
		FD3D12DescriptorAddress Address;

	public:
		FD3D12DescriptorLocation() = default;
		~FD3D12DescriptorLocation();
		FD3D12DescriptorLocation(const FD3D12DescriptorLocation &) = delete;
		FD3D12DescriptorLocation & operator =(const FD3D12DescriptorLocation &) = delete;

		FD3D12DescriptorLocation(FD3D12DescriptorLocation && That) noexcept;
		FD3D12DescriptorLocation & operator =(FD3D12DescriptorLocation && That) noexcept;

		void Detatch();
		void Reset();
	};

	class FD3D12DescriptorAllocator : public IRHIObject
	{
	public:
		virtual void Deallocate(const FD3D12DescriptorBlockLocation & BlockLocation) = 0;
		virtual void SetBlockObject(const FD3D12DescriptorBlockLocation & DescriptorBlockLocation, IRHIObject * Object) {}
	};

	class FD3D12OfflineDescriptorAllocator final : public FD3D12DescriptorAllocator
	{
	public:
		FD3D12OfflineDescriptorAllocator(FD3D12Device & D3D12Devcie, const FD3D12OfflineDescriptorAllocatorDesc & AllocatorDesc)
			: D3D12Devcie(D3D12Devcie), AllocatorDesc(AllocatorDesc) {}

		bool Allocate(FD3D12DescriptorLocation & DescriptorLocation, uint32 DescriptorCount);
		void Deallocate(const FD3D12DescriptorBlockLocation & BlockLocation) override;
		void SetBlockObject(const FD3D12DescriptorBlockLocation & BlockLocation, IRHIObject * Object) override
		{
#if D3D12_HEAP_DEBUG
			AssertExpr(BlockLocation.Count);
			AssertExpr(BlockObjects[BlockLocation.Offset] == nullptr);
			BlockObjects[BlockLocation.Offset] = Object;
			BlockObjectsLast[BlockLocation.Offset] = Object;
#endif
		}
	public:
		FD3D12Device & D3D12Devcie;
		FD3D12OfflineDescriptorAllocatorDesc AllocatorDesc;

	private:
		TList<FD3D12DescriptorHeapRef> DescriptorHeaps;
		TList<bool> DescriptorBits;
#if D3D12_HEAP_DEBUG
		TList<IRHIObject *> BlockObjects;
		TList<IRHIObject *> BlockObjectsLast;
#endif
	};
}
