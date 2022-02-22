#include "PCH.h"
#include "VulkanFence.h"
#include "VulkanDevice.h"

namespace Xin::RHI::Vulkan
{
	FVulkanFence::FVulkanFence(FVulkanDevice & Device) : Device(Device)
	{
		//HRESULT Result = RHIDevice.InnerDevice->CreateFence(0, Vulkan_FENCE_FLAG_NONE, __uuidof(IVulkanFence), InnerFence.GetVV());
		//if (FAILED(Result))
		//{
		//	LogError(u8"IVulkanDevice::CreateFence failed, {}"V, ErrorDescription(Result));
		//}

		VkSemaphoreCreateInfo SemaphoreCreateInfo = { VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };
		VkSemaphoreTypeCreateInfoKHR TimelineSemaphoreCreateInfo { VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO };

		TimelineSemaphoreCreateInfo.semaphoreType = VK_SEMAPHORE_TYPE_TIMELINE;
		TimelineSemaphoreCreateInfo.initialValue = 0;
		SemaphoreCreateInfo.pNext = &TimelineSemaphoreCreateInfo;
		
		vkCreateSemaphore(Device.vkDevice, &SemaphoreCreateInfo, nullptr, &vkSemaphore);
	}

	FVulkanFence::~FVulkanFence()
	{
		if (vkSemaphore)
		{
			vkDestroySemaphore(Device.vkDevice, vkSemaphore, nullptr);
			vkSemaphore = nullptr;
		}
	}
	void FVulkanFence::SetDebugName(FStringV ResourceLabel)
	{
		//SetVulkanObjectName(InnerFence, ResourceLabel);
	}

	void FVulkanFence::WaitFence(uint64 Value)
	{
		if (Value == NullIndex)
			Value = SignalValue;

		//AssertExpresion(InnerFence);
		//if (InnerFence->GetCompletedValue() < Value)
		//{
		//	HRESULT Result = InnerFence->SetEventOnCompletion(Value, hEvent);
		//	if (Succeeded(Result))
		//		WaitForSingleObject(hEvent, INFINITE);
		//}
	}

	uint64 FVulkanFence::GetCompletedValue() const
	{
		if (!vkSemaphore)
			return 0;

		uint64 CompletedValue;
		vkGetSemaphoreCounterValue(Device.vkDevice, vkSemaphore, &CompletedValue);
		return CompletedValue;
	}

	//uint64 FVulkanFence::Signal()
	//{
	//	VulkanFence->Signal(++FenceValue);
	//	return FenceValue;
	//}
}
