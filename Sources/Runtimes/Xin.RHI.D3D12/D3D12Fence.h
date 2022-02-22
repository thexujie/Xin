#pragma once

#include "D3D12Types.h"

namespace Xin::RHI::D3D12
{
	class FD3D12Device;

	class FD3D12Fence : public IRHIFence
	{
	public:
		FD3D12Fence(FD3D12Device & D3D12Devcie);
		void SetDebugName(FStringV Name) override;

	public:
		void WaitFence(uint64 Value) override;
		uint64 GetCompletedValue() const override { return d3d12Fence->GetCompletedValue(); }

	public:
		ID3D12FenceRef d3d12Fence;
		HANDLE hEvent = NULL;
	};
}
