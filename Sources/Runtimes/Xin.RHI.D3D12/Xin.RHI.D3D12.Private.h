#pragma once

#include "D3D12Types.h"
#include "D3D12Resource.h"
#include "D3D12ResourceView.h"
#include "D3D12PipelineState.h"
#include "D3D12CommandQueue.h"
#include "D3D12Device.h"

namespace Xin::RHI::D3D12
{
	RHID3D12_API FD3D12DeviceRef CreateD3D12RHIDevice(ECreateDeviceFlag CreateDeviceFlags = ECreateDeviceFlag::None);
}
