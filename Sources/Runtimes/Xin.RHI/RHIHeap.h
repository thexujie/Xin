#pragma once

#include "RHITypes.h"

namespace Xin::RHI
{
	class RHI_API IHeapInstance : public IRHIObject
	{
	public:
		virtual void Deallocate(const FRHIMemory & Block) = 0;
		virtual void SetBlockResource(uint64 BlockOffset, IRefer * Object) = 0;
	};
	using IHeapInstanceRef = TReferPtr<IHeapInstance>;

	class RHI_API IHeapAllocator : public IRHIObject
	{
	public:
		virtual FRHIMemory Allocate(EResourceUsage ResourceUsage, uint64 Size, uint32 Alignment) = 0;
		virtual void Deallocate(const FRHIMemory & Block) = 0;
	};
	using IHeapAllocatorRef = TReferPtr<IHeapAllocator>;

	class RHI_API IRHIHeap : public IRHIObject
	{
	public:
		IRHIHeap(FHeapDesc HeapDesc) : HeapDesc(HeapDesc) {}

	public:
		FHeapDesc HeapDesc;
		FRHIAddress BaseAddress;
	};
	using IRHIHeapRef = TReferPtr<IRHIHeap>;
}
