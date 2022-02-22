#include "PCH.h"
#include "D3D12CommandAllocator.h"

#include "D3D12Device.h"

namespace Xin::RHI::D3D12
{
	FD3D12CommandAllocator::FD3D12CommandAllocator(FD3D12Device & D3D12Devcie) : D3D12Devcie(D3D12Devcie)
	{
		HRESULT Result = D3D12Devcie.d3d12Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(d3d12CommandAllocator.GetPP()));
		AssertExpr(Succeeded(Result));
	}

	FD3D12CommandAllocator::~FD3D12CommandAllocator()
	{
		[[assume(d3d12CommandAllocator)]]
		d3d12CommandLists.Reset();
		d3d12CommandAllocator = nullptr;
	}

	void FD3D12CommandAllocator::SetDebugName(FStringV Name)
	{
		IRHIObject::SetDebugName(Name);
		SetD3D12ObjectName(d3d12CommandAllocator, Name);
	}

	ID3D12GraphicsCommandListRef FD3D12CommandAllocator::AcquireNextCommandList()
	{
		// Sometimes it is not been reset in time.
		AssertExpr(d3d12CommandLists.Size <= 256);

		if (d3d12CommandLists.Size)
		{
			ID3D12GraphicsCommandListRef d3d12CommandList = d3d12CommandLists.PopBack();
			d3d12CommandList->Reset(d3d12CommandAllocator, nullptr);
			return d3d12CommandList;
		}

		ID3D12GraphicsCommandListRef d3d12CommandList;
		D3D12Devcie.d3d12Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, d3d12CommandAllocator, nullptr, IID_PPV_ARGS(d3d12CommandList.GetPP()));
		SetD3D12ObjectName(d3d12CommandList, u8"{}.ComandList[{}]"V.Format(DebugName, NumCommandBuffers));
		++NumCommandBuffers;
		return d3d12CommandList;
	}

	void FD3D12CommandAllocator::ResetAllocator()
	{
		AssertForaml(NumCommandBuffers == d3d12CommandLists.Size, u8"{} of {} VkCommandBuffers of this VkCommandPool is online."V, NumCommandBuffers - d3d12CommandLists.Size, d3d12CommandLists.Size);
		d3d12CommandAllocator->Reset();
	}
}
