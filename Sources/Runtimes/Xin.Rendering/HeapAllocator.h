#pragma once

#include "Xin.Rendering.Types.h"

namespace Xin::Rendering
{
	struct FHeapAllocatorDesc
	{
		uint64 HeapSize = 0;
		uint32 HeapAlignment = 0;

		EHeapUsage HeapUsage = EHeapUsage::None;

		EResourceUsage ResourceUsage = EResourceUsage::None;
		EResourceAccess InitialAccess = EResourceAccess::None;
	};

	struct FRHIBlockLocation
	{
		uint64 Offset = 0;
		uint64 BlockSize = 0;

		explicit operator bool() const { return BlockSize > 0; }
	};

	class RENDERING_API FGlobalHeapInstance : public IHeapInstance
	{
	public:
		FGlobalHeapInstance(IRHIDevice & Device);
		~FGlobalHeapInstance() = default;
		void SetDebugName(FStringV Name) override;

		FRHIMemory Allocate(EResourceUsage ResourceUsage, uint64 Size, uint32 Alignment);
		void Deallocate(const FRHIMemory & Block) override;
		void SetBlockResource(uint64 BlockOffset, IRefer * Object) override {}

	public:
		IRHIDevice & Device;
		TList<IRHIHeapRef> GlobalHeaps;
	};
	using FGlobalHeapInstanceRef = TReferPtr<FGlobalHeapInstance>;

	class RENDERING_API FBuddyHeapInstance : public IHeapInstance
	{
	public:
		explicit FBuddyHeapInstance(class FBuddyMemoryAllocator & Allocator);
		~FBuddyHeapInstance();
		void SetDebugName(FStringV Name) override;

		FRHIMemory Allocate(uint64 Size, uint32 Alignment);
		void Deallocate(const FRHIMemory & Block) override;
		void SetBlockResource(uint64 BlockOffset, IRefer * Object) override;

		uint64 GetMemoryUsed() const;

	private:
		FRHIBlockLocation AllocateBlock(uint32 AllocOrder);
		void DeallocateBlock(const FRHIBlockLocation & Block);

	public:
		class FBuddyMemoryAllocator & Allocator;
		IRHIHeapRef PlaceHeap;
		IRHIResourceRef LocateResource;

		TList<TSet<uint64>> FreeBlocks;
#if XIN_DEBUG
		TMap<uint64, IRefer *> UsedBlocks;
		void ValidataBlocks();
#endif
	};
	using FBuddyHeapInstanceRef = TReferPtr<FBuddyHeapInstance>;


	class RENDERING_API FBuddyMemoryAllocator : public IRenderingObject
	{
	public:
		FBuddyMemoryAllocator(IRHIDevice & Device, const FHeapAllocatorDesc & AllocatorDesc) : Device(Device), AllocatorDesc(AllocatorDesc){}
		void SetDebugName(FStringV Name) override;

		FRHIMemory Allocate(uint64 Size, uint32 Alignment);
		uintx GetMemoryUsed() const;

	public:
		void UpdateContext();
		void ClearContext();

	public:
		IRHIDevice & Device;

		FHeapAllocatorDesc AllocatorDesc;
		TList<FBuddyHeapInstanceRef> HeapInstances;
		uint32 HeapInstanceIndex = 0;
	};

	class RENDERING_API FHeapAllocator : public IHeapAllocator
	{
	public:
		FHeapAllocator(IRHIDevice & Device);
		void SetDebugName(FStringV Name) override;

		FRHIMemory Allocate(EResourceUsage ResourceUsage, uint64 Size, uint32 Alignment) override;
		void Deallocate(const FRHIMemory & Block) override;

	public:
		void UpdateContext();
		void ClearContext();

	public:
		IRHIDevice & Device;

		FGlobalHeapInstance GlobalHeapInstance;
		FBuddyMemoryAllocator DynamicBufferAllocator;
		FBuddyMemoryAllocator DefaultHeapAllocator;
	};
}
