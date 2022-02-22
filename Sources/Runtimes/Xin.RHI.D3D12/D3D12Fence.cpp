#include "PCH.h"
#include "D3D12Fence.h"
#include "D3D12Device.h"

namespace Xin::RHI::D3D12
{
	FD3D12Fence::FD3D12Fence(FD3D12Device & D3D12Devcie)
	{
		HRESULT Result = D3D12Devcie.d3d12Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, __uuidof(ID3D12Fence), d3d12Fence.GetVV());
		if (FAILED(Result))
		{
			LogError(u8"ID3D12Device::CreateFence failed, {}"V, ErrorDescription(Result));
		}
		hEvent = ::CreateEventW(NULL, FALSE, FALSE, NULL);
	}

	void FD3D12Fence::SetDebugName(FStringV Name)
	{
		IRHIFence::SetDebugName(Name);
		SetD3D12ObjectName(d3d12Fence, Name);
	}

	void FD3D12Fence::WaitFence(uint64 Value)
	{
		if (Value == NullIndex)
			Value = SignalValue;

		AssertExpr(d3d12Fence);
		if (d3d12Fence->GetCompletedValue() < Value)
		{
			HRESULT Result = d3d12Fence->SetEventOnCompletion(Value, hEvent);
			if (Succeeded(Result))
				WaitForSingleObject(hEvent, INFINITE);
		}
	}

	//uint64 FD3D12Fence::Signal()
	//{
	//	D3D12Fence->Signal(++FenceValue);
	//	return FenceValue;
	//}
}
