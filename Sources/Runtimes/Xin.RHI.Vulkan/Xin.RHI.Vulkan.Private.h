#pragma once

#include "Xin.RHI.Vulkan.Config.h"
#include "VulkanDevice.h"

namespace Xin::RHI::Vulkan
{
	RHI_VULKAN_API IRHIDeviceRef CreateVulkanRHIDevice(ECreateDeviceFlag CreateDeviceFlags = ECreateDeviceFlag::None);
}
