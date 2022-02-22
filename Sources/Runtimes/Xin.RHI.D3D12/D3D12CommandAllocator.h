#pragma once

#include "D3D12Types.h"

namespace Xin::RHI::D3D12
{
	class RHID3D12_API FD3D12CommandAllocator : public IRHIObject
	{
	public:
		FD3D12CommandAllocator(FD3D12Device & D3D12Devcie);
		~FD3D12CommandAllocator();
		void SetDebugName(FStringV Name) override;

		ID3D12GraphicsCommandListRef AcquireNextCommandList();
		void ResetAllocator();

		void FreeCommandList(ID3D12GraphicsCommandListRef d3d12CommandList)
		{
			d3d12CommandLists.Add(d3d12CommandList);
		}

	public:
		FD3D12Device & D3D12Devcie;
		ID3D12CommandAllocatorRef d3d12CommandAllocator;
		TList<ID3D12GraphicsCommandListRef> d3d12CommandLists;
		uintx NumCommandBuffers = 0;
	};
	using FD3D12CommandAllocatorRef = TReferPtr<FD3D12CommandAllocator>;
}
