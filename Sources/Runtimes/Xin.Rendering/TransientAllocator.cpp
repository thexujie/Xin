#include "PCH.h"
#include "TransientAllocator.h"

namespace Xin::Rendering
{
	static FHeapAllocatorDesc TransientHeapAllocatorDesc =
	{
		.HeapSize = 64 * UnitMiB,
		.HeapAlignment = 256,
		.HeapUsage = EHeapUsage::AllowMask,
	};

	FTransientAllocator::FTransientAllocator(IRHIDevice & Device)
		: Device(Device), DefaultHeapAllocator(Device, TransientHeapAllocatorDesc)
	{
	}

	FTransientAllocator::~FTransientAllocator()
	{
		FTransientAllocator::ResetAllocator();
	}

	void FTransientAllocator::SetDebugName(FStringV Name)
	{
		IRenderingObject::SetDebugName(Name);
		DefaultHeapAllocator.SetDebugName(u8"{}.Default"V.Format(Name));
	}

	void FTransientAllocator::TransientBegin()
	{
		EnsureExpr(DefaultHeapAllocator.GetMemoryUsed() == 0);
	}

	void FTransientAllocator::TransientEnd()
	{
		
	}

	void FTransientAllocator::ResetAllocator()
	{
		TransientResources.Clear();
		HeapRanges.Clear();
		//DefaultHeapAllocator.Reset();
	}

	FTransientResource * FTransientAllocator::AcquireResource(const FResourceDesc & ResourceDesc)
	{
		EResourceAccess InitialAccess = GetDefaultResourceAccess(ResourceDesc.Usage);

		FAllocateInfo AllocateInfo = Device.GetAllocateInfo(ResourceDesc);

		FRHIMemory Memory = DefaultHeapAllocator.Allocate(AllocateInfo.Size, AllocateInfo.Alignment);

		FTransientResourceRef TransientResource;
		FTransientResourceKey TransientResourceKey { Memory.PlaceHeap, Memory.Location.Offset, ResourceDesc };
		if (auto Iter = TransientResources.Find(TransientResourceKey); Iter != TransientResources.End())
		{
			TransientResource = Iter->Second.Get();
			//Resources.Erase(Iter);

			TransientResource->ResourcesBefore.Clear();
			TransientResource->ResourcesAfter.Clear();
		}
		else
		{
			TransientResource = new FTransientResource(Device, ResourceDesc, Memory);
			TransientResources.Insert({ TransientResourceKey, TransientResource });
		}

		TransientOverlap(TransientResource);
		return TransientResource;
	}

	void FTransientAllocator::ForfietResource(FTransientResource * TransientResource)
	{
		TransientResource->Memory.Instance->Deallocate(TransientResource->Memory);
		FTransientResourceKey TransientResourceKey { TransientResource->Memory.PlaceHeap, TransientResource->Memory.Location.Offset, TransientResource->Resource->ResourceDesc };
		TransientResources.Insert({ TransientResourceKey, TransientResource });
	}

	ullint2 FTransientAllocator::GetMemoryUsage() const
	{
		ullint2 Result = {};
		//Result += DefaultHeapAllocator.GetMemoryUsage();
		return Result;
	}

	ullint2 FTransientAllocator::GetOverlapInfo() const
	{
		ullint2 Result = {};
		for (auto Iter : TransientResources)
			Result.X += Iter.Second->Memory.Location.Size;
		for (const FHeapRange & HeapRange : HeapRanges)
			Result.Y += HeapRange.Max - HeapRange.Min;
		return Result;
	}

	void FTransientAllocator::TransientOverlap(FTransientResource * TransientResource)
	{
		static int DebugIndex = 0;
		++DebugIndex;

		FHeapRange HeapRangeNew =
		{
			TransientResource->Memory.Location.Offset,
			TransientResource->Memory.Location.Offset + TransientResource->Memory.Location.Size,
			TransientResource->Memory.PlaceHeap,
			TransientResource
		};

		AssertExpr(TransientResource->Memory.Location.Size <= TransientResource->Memory.Location.BlockSize);
		AssertExpr(TransientResource->ResourcesBefore.Size == 0);

		uintx FirstIndex = NullIndex;
		for (uintx RangeIndex = 0; RangeIndex < HeapRanges.Size; ++RangeIndex)
		{
			const FHeapRange & HeapRange = HeapRanges[RangeIndex];
			if (HeapRange.Heap == HeapRangeNew.Heap)
			{
				FirstIndex = RangeIndex;
				break;
			}
		}

		if (FirstIndex == NullIndex)
		{
			HeapRanges.Add(HeapRangeNew);
			return;
		}

		uintx InsertIndex = NullIndex;
		for(uintx RangeIndex = FirstIndex; RangeIndex < HeapRanges.Size; /**/)
		{
			FHeapRange & HeapRange = HeapRanges[RangeIndex];

			// Last Transient Resource
			if (HeapRange.TransientResource == HeapRangeNew.TransientResource)
				return;

			if (HeapRange.Heap != HeapRangeNew.Heap)
			{
				InsertIndex = RangeIndex;
				break;
			}

			// -----------XXXXXX----------- HeapRange
			// --------OOO----------------- HeapRangeNew
			// -----OOO-------------------- HeapRangeNew
			if (HeapRangeNew.Max <= HeapRange.Min)
			{
				InsertIndex = RangeIndex;
				break;
			}

			// -----------XXXXXX----------- HeapRange
			// -----------------OOO-------- HeapRangeNew
			// --------------------OOO----- HeapRangeNew
			if (HeapRange.Max <= HeapRangeNew.Min)
			{
				++RangeIndex;
				InsertIndex = RangeIndex;
				continue;
			}

			TransientResource->ResourcesBefore.Add(HeapRange.TransientResource);
			HeapRange.TransientResource->ResourcesAfter.Add(TransientResource);

			if (HeapRangeNew.Min <= HeapRange.Min)
			{
				if (HeapRange.Max <= HeapRangeNew.Max)
				{
					// -----------XXXXXX----------- HeapRange
					// --------OOOOOOOOO----------- HeapRangeNew
					// -----------OOOOOOOOO-------- HeapRangeNew
					// --------OOOOOOOOOOOO-------- HeapRangeNew
					HeapRanges.EraseAt(RangeIndex, 1);
					InsertIndex = RangeIndex;
				}
				else
				{
					// -----------XXXXXX----------- HeapRange
					// -----------OOO-------------- HeapRangeNew
					// --------OOOOOO-------------- HeapRangeNew
					HeapRange.Min = HeapRangeNew.Max;
					InsertIndex = RangeIndex;
					break;
				}
			}
			else/* if(HeapRangeNew.Min > HeapRange.Min)*/
			{
				if (HeapRangeNew.Max < HeapRange.Max)
				{
					// -----------XXXXXXXXX----------- HeapRange
					// --------------OOO----------- HeapRangeNew
					// -----------XXXOOOXXX-------- HeapRangeNew
					FHeapRange HeapRangeRight = { HeapRangeNew.Max, HeapRange.Max, HeapRange.Heap, HeapRange.TransientResource };
					HeapRange.Max = HeapRangeNew.Min;
					HeapRanges.Insert(RangeIndex + 1, HeapRangeRight);
					InsertIndex = RangeIndex + 1;
					break;
				}
				else
				{
					// -----------XXXXXXXXX----------- HeapRange
					// --------------OOOOOO----------- HeapRangeNew
					// --------------OOOOOOOOO-------- HeapRangeNew
					HeapRange.Max = HeapRangeNew.Min;
					InsertIndex = RangeIndex + 1;
					++RangeIndex;
				}
			}
		}
		AssertExpr(InsertIndex != NullIndex);
		if (InsertIndex > 0)
			AssertExpr(HeapRanges[InsertIndex - 1].Heap != HeapRangeNew.Heap || HeapRanges[InsertIndex - 1].Max <= HeapRangeNew.Min);
		if (InsertIndex < HeapRanges.Size)
			AssertExpr(HeapRanges[InsertIndex].Heap != HeapRangeNew.Heap || HeapRangeNew.Max <= HeapRanges[InsertIndex].Min);

#if XIN_DEBUG
		{
			for (uintx RangeIndex = 0; RangeIndex < HeapRanges.Size; ++RangeIndex)
			{
				FHeapRange & HeapRange = HeapRanges[RangeIndex];
				if (HeapRange.Heap == HeapRangeNew.Heap
					&& HeapRange.Min == HeapRangeNew.Min
					&& HeapRange.Max == HeapRangeNew.Max)
					AssertFalse(HeapRange == HeapRangeNew);
			}
		}
#endif

		HeapRanges.Insert(InsertIndex, HeapRangeNew);

#if XIN_DEBUG
		{
			TSet<IRHIHeap *> Heaps;
			IRHIHeap * Heap = nullptr;
			uint64 Position = 0;
			for (uintx RangeIndex = 0; RangeIndex < HeapRanges.Size; ++RangeIndex)
			{
				FHeapRange & HeapRange = HeapRanges[RangeIndex];
				if (HeapRange.Heap != Heap)
				{
					AssertExpr(!Heaps.Contains(HeapRange.Heap));
					Heap = HeapRange.Heap;
					Heaps.Insert(HeapRange.Heap);
				}
				else
				{
					AssertExpr(Position <= HeapRange.Min);
					AssertExpr(Position < HeapRange.Max);
				}
				Position = HeapRange.Max;
			}

			for (FTransientResource * TransientBefore : TransientResource->ResourcesBefore)
			{
				uint64 AddressMin = Max(TransientBefore->Memory.Location.Offset, TransientResource->Memory.Location.Offset);
				uint64 AddressMax = Min(TransientBefore->Memory.Location.Offset + TransientBefore->Memory.Location.BlockSize
					, TransientResource->Memory.Location.Offset + TransientResource->Memory.Location.BlockSize);
				AssertExpr(AddressMin < AddressMax);
			}
		}
#endif
	}
}
