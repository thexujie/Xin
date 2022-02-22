#pragma once

#include "Xin.RHI.Vulkan.Config.h"

namespace Xin::RHI::Vulkan
{
	RHI_VULKAN_API IRHIDeviceRef CreateDevice(ECreateDeviceFlag CreateDeviceFlags = ECreateDeviceFlag::None);
}
