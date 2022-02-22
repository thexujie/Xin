#pragma once

#include "Xin.Rendering.Types.h"
#include "HeapAllocator.h"

namespace Xin::Rendering
{
	class RENDERING_API FTransientResource : public IRenderingObject
	{
	public:
		FTransientResource(IRHIDevice & Device, const FResourceDesc & ResourceDesc, const FRHIMemory & Memory)
			: Device(Device), Memory(Memory)
		{
			FResourceDesc TransientResourceDesc = ResourceDesc;
			TransientResourceDesc.Usage |= EResourceUsage::Transisent;
			Resource = Device.CreatePlacedResource(TransientResourceDesc, Memory);
		}

		void SetDebugName(FStringV Name) override
		{
			IRenderingObject::SetDebugName(Name);
			Resource->SetDebugName(Name);
		}

		void TileMapping()
		{
		}

	public:
		IRHIDevice & Device;
		IRHIResourceRef Resource;

		FRHIMemory Memory;
		TList<FTransientResource *> ResourcesBefore;
		TList<FTransientResource *> ResourcesAfter;
	};
	using FTransientResourceRef = TReferPtr<FTransientResource>;


	class RENDERING_API FTransientAllocator : public IRenderingObject
	{
	public:
		FTransientAllocator(IRHIDevice & Device);
		~FTransientAllocator();
		void SetDebugName(FStringV Name) override;

	public:
		void TransientBegin();
		void TransientEnd();
		void ResetAllocator();
		FTransientResource * AcquireResource(const FResourceDesc & ResourceDesc);
		void ForfietResource(FTransientResource * TransientResource);

	public:
		ullint2 GetMemoryUsage() const;
		ullint2 GetOverlapInfo() const;

	public:
		IRHIDevice & Device;

		FBuddyMemoryAllocator DefaultHeapAllocator;

		struct FTransientResourceKey
		{
			IRHIHeap * Heap;
			uint64 Offset;
			FResourceDesc ResourceDesc;
			auto operator<=>(const FTransientResourceKey &) const noexcept = default;
		};
		TMap<FTransientResourceKey, FTransientResourceRef> TransientResources;
		uint64 FenceValue = 0;

	private:
		struct FHeapRange
		{
			uint64 Min = 0;
			uint64 Max = 0;

			IRHIHeap * Heap;
			FTransientResource * TransientResource = nullptr;
			auto operator<=>(const FHeapRange &) const noexcept = default;
		};
		TList<FHeapRange> HeapRanges;

		void TransientOverlap(FTransientResource * TransientResource);
	};
	using FTransientAllocatorRef = TReferPtr<FTransientAllocator>;
}
