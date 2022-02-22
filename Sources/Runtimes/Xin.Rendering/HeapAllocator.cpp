#include "PCH.h"
#include "HeapAllocator.h"

namespace Xin::Rendering
{
	static FHeapAllocatorDesc DynamicBufferAllocatorDesc =
	{
		.HeapSize = 32 * UnitMiB,
		.HeapAlignment = 256,
		.HeapUsage = EHeapUsage::HostWrite | EHeapUsage::AllowBuffers,

		.ResourceUsage = EResourceUsage::UniformBuffer | EResourceUsage::HostWrite,
		.InitialAccess = EResourceAccess::Generic,
	};

	static FHeapAllocatorDesc DefaultHeapAllocatorDesc =
	{
		.HeapSize = 64 * UnitMiB,
		.HeapAlignment = 256,
		.HeapUsage = EHeapUsage::AllowMask,
	};

	FGlobalHeapInstance::FGlobalHeapInstance(IRHIDevice & Device) : Device(Device)
	{
		
	}

	void FGlobalHeapInstance::SetDebugName(FStringV Name)
	{
		IHeapInstance::SetDebugName(Name);
		for (uintx HeapIndex = 0; HeapIndex < GlobalHeaps.Size; ++HeapIndex)
			GlobalHeaps[HeapIndex]->SetDebugName(u8"{}.Heap[{}]"V.Format(Name, HeapIndex));
	}

	FRHIMemory FGlobalHeapInstance::Allocate(EResourceUsage ResourceUsage, uint64 Size, uint32 Alignment)
	{
		FHeapDesc HeapDesc;
		if (ResourceUsage & EResourceUsage::HostWrite)
			HeapDesc.Usage = EHeapUsage::HostWrite | EHeapUsage::AllowBuffers;
		else
			HeapDesc.Usage = EHeapUsage::None;

		if (ResourceUsage & (EResourceUsage::UniformBuffer | EResourceUsage::IndexBuffer | EResourceUsage::VertexBuffer))
			HeapDesc.Usage |= EHeapUsage::AllowBuffers;

		if (ResourceUsage & EResourceUsage::SampledTexture)
			HeapDesc.Usage |= EHeapUsage::AllowTextures;

		if (ResourceUsage & (EResourceUsage::RenderTarget | EResourceUsage::DepthStencil))
			HeapDesc.Usage |= EHeapUsage::AllowRTsDSs;

		HeapDesc.Size = Size;
		HeapDesc.Alignment = Alignment;
		IRHIHeapRef PlaceHeap = Device.CreateHeap(HeapDesc);
		PlaceHeap->SetDebugName(u8"{}.Heap[{}]"V.Format(DebugName, GlobalHeaps.Size));

		FRHIMemory MemoryBlock { this, PlaceHeap, { 0, Size, Size, Alignment } };
		GlobalHeaps.Add(PlaceHeap);
		return MemoryBlock;
	}

	void FGlobalHeapInstance::Deallocate(const FRHIMemory & Block)
	{
		AssertExpr(GlobalHeaps.Contains(Block.PlaceHeap));
		GlobalHeaps.Remove(Block.PlaceHeap);
	}

	FBuddyHeapInstance::FBuddyHeapInstance(FBuddyMemoryAllocator & Allocator) : Allocator(Allocator)
	{
		AssertExpr(Allocator.AllocatorDesc.HeapSize % Allocator.AllocatorDesc.HeapAlignment == 0);
		FHeapDesc HeapDesc;
		HeapDesc.Usage = Allocator.AllocatorDesc.HeapUsage;
		HeapDesc.Size = Allocator.AllocatorDesc.HeapSize;

		PlaceHeap = Allocator.Device.CreateHeap(HeapDesc);

		uint64 NumOrders = BitFindReverse(Allocator.AllocatorDesc.HeapSize / Allocator.AllocatorDesc.HeapAlignment) + 1;
		FreeBlocks.Reset(NumOrders, TSet<uint64>{});
		FreeBlocks.Back().Insert(0);
	}

	FBuddyHeapInstance::~FBuddyHeapInstance()
	{
		
	}

	void FBuddyHeapInstance::SetDebugName(FStringV Name)
	{
		IHeapInstance::SetDebugName(Name);
		if (PlaceHeap)
			PlaceHeap->SetDebugName(u8"{}.PlaceHeap"V.Format(Name));
	}

	FRHIMemory FBuddyHeapInstance::Allocate(uint64 Size, uint32 Alignment)
	{
		Assert(BitCount1(Alignment) == 1, u8""V);

		uint32 AllocAlignment = Max(Alignment, Allocator.AllocatorDesc.HeapAlignment);
		uint64 AllocSize = AlignWith(Size, AllocAlignment);
		uint64 AllocRate = (AllocSize + Allocator.AllocatorDesc.HeapAlignment - 1) / Allocator.AllocatorDesc.HeapAlignment;
		uint32 AllocOrder = BitFindReverse(AllocRate + AllocRate - 1);

		AssertExpr(AllocSize <= Allocator.AllocatorDesc.HeapSize << 1);

		if (AllocOrder >= 64 - BitFindReverse(Allocator.AllocatorDesc.HeapAlignment))
			return {};

#if XIN_DEBUG
		ValidataBlocks();
#endif
		for (uint32 AllocOrderIndex = AllocOrder; AllocOrderIndex < FreeBlocks.Size; ++AllocOrderIndex)
		{
			if (FreeBlocks[AllocOrderIndex].Size() > 0)
			{
				static uintx EntryIndex = 0;
				++EntryIndex;

				FRHIBlockLocation BlockLocation = AllocateBlock(AllocOrder);
				if (!BlockLocation)
					return { };

				AssertExpr(FreeBlocks.Back().Size() == 0 || (FreeBlocks.Back().Size() == 1 && *FreeBlocks.Back().Begin() == 0));
				AssertExpr(BlockLocation.BlockSize <= Allocator.AllocatorDesc.HeapSize << 1);
				AssertExpr(BlockLocation.Offset + BlockLocation.BlockSize <= Allocator.AllocatorDesc.HeapSize);

#if XIN_DEBUG
				AssertExpr(!UsedBlocks.Contains(BlockLocation.Offset));
				UsedBlocks.Insert({ BlockLocation.Offset, nullptr });
				ValidataBlocks();
#endif

				FRHIMemory Block = { this, PlaceHeap };
				Block.Location.Offset = BlockLocation.Offset;
				Block.Location.Size = Size;
				Block.Location.BlockSize = BlockLocation.BlockSize;
				Block.Location.Alignment = Alignment;
				return Block;
			}
		}

		return { };
	}

	void FBuddyHeapInstance::Deallocate(const FRHIMemory & Block)
	{
#if XIN_DEBUG
		AssertExpr(UsedBlocks.Contains(Block.Location.Offset));
		UsedBlocks.Remove(Block.Location.Offset);
#endif
		DeallocateBlock({Block.Location.Offset, Block.Location.BlockSize});
#if XIN_DEBUG
		ValidataBlocks();
#endif
	}

	void FBuddyHeapInstance::SetBlockResource(uint64 BlockOffset, IRefer * Object)
	{
#if XIN_DEBUG
		AssertExpr(UsedBlocks.Contains(BlockOffset));
		UsedBlocks[BlockOffset] = Object;
#endif
	}

	uint64 FBuddyHeapInstance::GetMemoryUsed() const
	{
		uint64 MemoryUsed = Allocator.AllocatorDesc.HeapSize;
		for (uintx OrderIndex = 0; OrderIndex < FreeBlocks.Size; ++OrderIndex)
			MemoryUsed -= (Allocator.AllocatorDesc.HeapAlignment << OrderIndex) * FreeBlocks[OrderIndex].Size();
		return MemoryUsed;
	}

	FRHIBlockLocation FBuddyHeapInstance::AllocateBlock(uint32 AllocOrder)
	{
		if (AllocOrder >= FreeBlocks.Size)
			return {};

		if (FreeBlocks[AllocOrder].Size() > 0)
		{
			auto Iter = FreeBlocks[AllocOrder].Begin();
			uint64 BlockOffset = *Iter;
			FreeBlocks[AllocOrder].Erase(Iter);
			AssertExpr(!UsedBlocks.Contains(BlockOffset));
			return { BlockOffset, Allocator.AllocatorDesc.HeapAlignment << AllocOrder };
		}
		else
		{
			FRHIBlockLocation ParentBlock = AllocateBlock(AllocOrder + 1);
			AssertExpr(ParentBlock.BlockSize > 0);

			uint64 RightBlockOffset = ParentBlock.Offset + (Allocator.AllocatorDesc.HeapAlignment << AllocOrder);

			AssertExpr(!UsedBlocks.Contains(RightBlockOffset));
			FreeBlocks[AllocOrder].Insert(RightBlockOffset);
			return { ParentBlock.Offset, Allocator.AllocatorDesc.HeapAlignment << AllocOrder };
		}
	}

	void FBuddyHeapInstance::DeallocateBlock(const FRHIBlockLocation & Block)
	{
		AssertExpr(Block.BlockSize > 0);
		AssertExpr(BitFindReverse(Block.BlockSize % Allocator.AllocatorDesc.HeapAlignment) == 0);

		uint64 AllocOrder = BitFindReverse(Block.BlockSize / Allocator.AllocatorDesc.HeapAlignment);

		//uint64 BuddyOffset = Block.Offset ^ Block.BlockSize;
		AssertExpr(Block.BlockSize == Allocator.AllocatorDesc.HeapAlignment << AllocOrder);

		uint64 BuddyOffset = (Block.Offset % (Block.BlockSize << 1)) ? Block.Offset - Block.BlockSize : Block.Offset + Block.BlockSize;
		AssertExpr(BuddyOffset != Block.Offset);

		if (FreeBlocks[AllocOrder].Contains(BuddyOffset))
		{
			// Join with buddy
			FreeBlocks[AllocOrder].Erase(BuddyOffset);
			DeallocateBlock({ Min(Block.Offset, BuddyOffset), Block.BlockSize << 1 });
		}
		else
		{
			AssertExpr(!UsedBlocks.Contains(Block.Offset));
			FreeBlocks[AllocOrder].Insert(Block.Offset);
		}
	}

#if XIN_DEBUG
	void FBuddyHeapInstance::ValidataBlocks()
	{
		for (uint32 AllocOrderIndex = 0; AllocOrderIndex < FreeBlocks.Size; ++AllocOrderIndex)
		{
			for (uint64 FreeBlockOffset : FreeBlocks[AllocOrderIndex])
			{
				AssertExpr(!UsedBlocks.Contains(FreeBlockOffset));
				//AssertExpresion(!UsedBlocks.Contains(FreeBlockOffset + FreeBlockOffset / 2));
			}
		}
	}
#endif
	void FBuddyMemoryAllocator::SetDebugName(FStringV Name)
	{
		IRenderingObject::SetDebugName(Name);
		for (uintx InstanceIndex = 0; InstanceIndex < HeapInstances.Size; ++InstanceIndex)
			HeapInstances[InstanceIndex]->SetDebugName(u8"{}.Instance[{}]"V.Format(Name, InstanceIndex));
	}

	FRHIMemory FBuddyMemoryAllocator::Allocate(uint64 Size, uint32 Alignment)
	{
		if (Size > AllocatorDesc.HeapSize)
			return { };

		static int32 AllocateIndex = 0;
		++AllocateIndex;
		for (FBuddyHeapInstanceRef & HeapInstance : HeapInstances)
		{
			FRHIMemory Block = HeapInstance->Allocate(Size, Alignment);
			if (Block)
				return Block;
		}

		FBuddyHeapInstanceRef & HeapInstance = HeapInstances.Add(new FBuddyHeapInstance(*this));
		HeapInstance->SetDebugName(u8"{}.Instance[{}]"V.Format(DebugName, HeapInstanceIndex++));
		return HeapInstance->Allocate(Size, Alignment);
	}

	uintx FBuddyMemoryAllocator::GetMemoryUsed() const
	{
		uintx MemoryUsed = 0;
		for (const FBuddyHeapInstanceRef & HeapInstance : HeapInstances)
			MemoryUsed += HeapInstance->GetMemoryUsed();
		return MemoryUsed;
	}

	void FBuddyMemoryAllocator::UpdateContext()
	{
		
	}

	void FBuddyMemoryAllocator::ClearContext()
	{
		TPointerIterator<TReferPtr<FBuddyHeapInstance>, false> HeapInstanceIter = HeapInstances.Begin();
		while (HeapInstanceIter != HeapInstances.End())
		{
			if ((*HeapInstanceIter)->GetMemoryUsed() == 0)
				HeapInstanceIter = HeapInstances.Erase(HeapInstanceIter);
			else
				++HeapInstanceIter;
		}

	}

	void FHeapAllocator::ClearContext()
	{
		DynamicBufferAllocator.ClearContext();
		DefaultHeapAllocator.ClearContext();
	}

	FHeapAllocator::FHeapAllocator(IRHIDevice & Device)
		: Device(Device)
		, GlobalHeapInstance(Device)
		, DynamicBufferAllocator(Device, DynamicBufferAllocatorDesc)
		, DefaultHeapAllocator(Device, DefaultHeapAllocatorDesc)
	{
		
	}

	void FHeapAllocator::SetDebugName(FStringV Name)
	{
		IHeapAllocator::SetDebugName(Name);
		DynamicBufferAllocator.SetDebugName(u8"{}.DynamicBuffers"V.Format(Name));
		DefaultHeapAllocator.SetDebugName(u8"{}.Default"V.Format(Name));
		GlobalHeapInstance.SetDebugName(u8"{}.Global"V.Format(Name));
	}

	FRHIMemory FHeapAllocator::Allocate(EResourceUsage ResourceUsage, uint64 Size, uint32 Alignment)
	{
		Alignment = Max(Alignment, 0x100);

		if (ResourceUsage & EResourceUsage::HostMask)
		{
			//if (Size <= DynamicBufferAllocator.AllocatorDesc.HeapSize >> 1)
			if (Size <= DynamicBufferAllocator.AllocatorDesc.HeapSize)
				return DynamicBufferAllocator.Allocate(Size, Alignment);
		}
		else
		{
			//if (Size <= DefaultHeapAllocator.AllocatorDesc.HeapSize >> 1)
			if (Size <= DefaultHeapAllocator.AllocatorDesc.HeapSize)
				return DefaultHeapAllocator.Allocate(Size, Alignment);
		}

		return GlobalHeapInstance.Allocate(ResourceUsage, Size, Alignment);
	}

	void FHeapAllocator::Deallocate(const FRHIMemory & Block)
	{
		
	}

	void FHeapAllocator::UpdateContext()
	{
		DynamicBufferAllocator.UpdateContext();
		DefaultHeapAllocator.UpdateContext();
	}
}
