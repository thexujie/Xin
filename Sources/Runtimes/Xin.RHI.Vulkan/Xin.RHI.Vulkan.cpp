#include "PCH.h"
#include "Xin.RHI.Vulkan.h"
#include "Xin.RHI.Vulkan.Private.h"

namespace Xin::RHI::Vulkan
{
	IRHIDeviceRef CreateDevice(ECreateDeviceFlag CreateDeviceFlags)
	{
		return CreateVulkanRHIDevice(CreateDeviceFlags);
	}

	IRHIDeviceRef CreateVulkanRHIDevice(ECreateDeviceFlag CreateDeviceFlags)
	{
		return new FVulkanDevice(CreateDeviceFlags);
	}
}
