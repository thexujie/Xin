#pragma once

#include "Xin.RHI.D3D12.Config.h"

namespace Xin::RHI::D3D12
{
	RHID3D12_API IRHIDeviceRef CreateDevice(ECreateDeviceFlag CreateDeviceFlags = ECreateDeviceFlag::None);
}
