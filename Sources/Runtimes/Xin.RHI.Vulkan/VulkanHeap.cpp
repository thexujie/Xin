#include "PCH.h"
#include "VulkanHeap.h"
#include "VulkanDevice.h"

namespace Xin::RHI::Vulkan
{
	FVulkanHeap::FVulkanHeap(FVulkanDevice & VulkanDevice, const FHeapDesc & HeapDesc) : IRHIHeap(HeapDesc), VulkanDevice(VulkanDevice)
	{
		if (HeapDesc.Usage & EHeapUsage::HostMask)
		{
			VkMemoryPropertyFlagBits vkMemoryPropertyFlags = VkMemoryPropertyFlagBits(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
			VkBufferCreateInfo BufferCI { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
			BufferCI.size = HeapDesc.Size;
			BufferCI.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
			BufferCI.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			BufferCI.queueFamilyIndexCount = 0;
			VkResult Result = vkCreateBuffer(VulkanDevice.vkDevice, &BufferCI, nullptr, &vkBuffer);

			VkMemoryRequirements MemoryRequirements;
			vkGetBufferMemoryRequirements(VulkanDevice.vkDevice, vkBuffer, &MemoryRequirements);

			VkMemoryAllocateInfo MemoryAllocateInfo = { VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO };
			MemoryAllocateInfo.allocationSize = MemoryRequirements.size;
			MemoryAllocateInfo.memoryTypeIndex = VulkanDevice.GetMemoryType(MemoryRequirements.memoryTypeBits, vkMemoryPropertyFlags);

			Result = vkAllocateMemory(VulkanDevice.vkDevice, &MemoryAllocateInfo, nullptr, &vkMemory);
			VkResultAssert(!Result, u8"vkAllocateMemory failed."V);

			Result = vkBindBufferMemory(VulkanDevice.vkDevice, vkBuffer, vkMemory, 0);
			VkResultAssert(!Result, u8"vkBindBufferMemory failed."V);

			uint8_t * MappedPointer;
			Result = vkMapMemory(VulkanDevice.vkDevice, vkMemory, 0, VK_WHOLE_SIZE, 0, (void **)&MappedPointer);
			VkResultAssert(!Result, u8"vkMapMemory failed."V);

			BaseAddress.CPUAddress = MappedPointer;
			BaseAddress.GPUAddress = FGPUAddress(vkBuffer);
		}
		else
		{
			VkMemoryAllocateInfo MemoryAllocateInfo = { VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO };
			MemoryAllocateInfo.allocationSize = HeapDesc.Size;
			MemoryAllocateInfo.memoryTypeIndex = VulkanDevice.GetMemoryType(0x1, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

			VkResult Result = vkAllocateMemory(VulkanDevice.vkDevice, &MemoryAllocateInfo, nullptr, &vkMemory);
			VkResultAssert(!Result, u8"vkAllocateMemory failed."V);
		}
	}

	FVulkanHeap::~FVulkanHeap()
	{
		if (vkBuffer)
		{
			vkDestroyBuffer(VulkanDevice.vkDevice, vkBuffer, nullptr);
			vkBuffer = nullptr;
		}
		if (vkMemory)
		{
			vkFreeMemory(VulkanDevice.vkDevice, vkMemory, nullptr);
			vkMemory = nullptr;
		}
	}

	void FVulkanHeap::SetDebugName(FStringV ResourceLabel)
	{
		IRHIHeap::SetDebugName(ResourceLabel);
		//SetVulkanObjectName(D3DHeap.Get(), ResourceLabel);
	}

	byte * FVulkanHeap::Lock(ERHILockMode LockMode)
	{
		uint8 * MappedPointer;
		VkResult Result = vkMapMemory(VulkanDevice.vkDevice, vkMemory, 0, VK_WHOLE_SIZE, 0, (void **)&MappedPointer);
		if (Result)
			return nullptr;
		return MappedPointer;
	}

	void FVulkanHeap::Unlock()
	{
		vkUnmapMemory(VulkanDevice.vkDevice, vkMemory);
	}

	FVulkanDescriptorLocation::~FVulkanDescriptorLocation()
	{
		Reset();
	}

	FVulkanDescriptorLocation::FVulkanDescriptorLocation(FVulkanDescriptorLocation && That) noexcept
		: HeapAllocator(That.HeapAllocator), DescriptorHeap(That.DescriptorHeap)
		, BlockLocation(That.BlockLocation)
		, Address(That.Address)
	{
		That.Detatch();
	}

	FVulkanDescriptorLocation & FVulkanDescriptorLocation::operator =(FVulkanDescriptorLocation && That) noexcept
	{
		HeapAllocator = That.HeapAllocator;
		DescriptorHeap = That.DescriptorHeap;
		BlockLocation = That.BlockLocation;
		Address = That.Address;
		That.Detatch();
		return *this;
	}

	void FVulkanDescriptorLocation::Detatch()
	{
		HeapAllocator = nullptr;
		Reset();
	}

	void FVulkanDescriptorLocation::Reset()
	{
		if (HeapAllocator)
			HeapAllocator->Deallocate(BlockLocation);

		HeapAllocator = nullptr;
		DescriptorHeap = nullptr;
		BlockLocation = { };
		Address = { };
	}

	FVulkanDescriptorHeap::FVulkanDescriptorHeap(FVulkanDevice & VulkanDevice, const FVulkanShaderBindings & ShaderBindings, uint32 MaxDescriptorSets)
		: VulkanDevice(VulkanDevice), ShaderBindings(ShaderBindings)
	{
		DescriptorSetLayout = VulkanDevice.FetchDescriptorSetLayout(ShaderBindings);

		VkDescriptorPoolSize DescriptorPoolSizes[ArraySize(UsedDescriptorTypes)] = { };
		uintx NumDescriptorPoolSizes = 0;
		for (VkDescriptorType DescriptorType : UsedDescriptorTypes)
		{
			uint32 NumBindingSlots = ShaderBindings.GetNumDescriptors(DescriptorType);
			if (NumBindingSlots > 0)
			{
				VkDescriptorPoolSize & DescriptorPoolSize = DescriptorPoolSizes[NumDescriptorPoolSizes++];
				DescriptorPoolSize.type = DescriptorType;
				DescriptorPoolSize.descriptorCount = NumBindingSlots;
			}
		}

		VkDescriptorPoolCreateInfo DescriptorPoolCI = { VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO };
		{
			DescriptorPoolCI.poolSizeCount = NumDescriptorPoolSizes;
			DescriptorPoolCI.pPoolSizes = DescriptorPoolSizes;
			DescriptorPoolCI.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
			DescriptorPoolCI.maxSets = MaxDescriptorSets;
		}
		VkResult Result = vkCreateDescriptorPool(VulkanDevice.vkDevice, &DescriptorPoolCI, nullptr, &vkDescriptorPool);
	}

	FVulkanDescriptorHeap::~FVulkanDescriptorHeap()
	{
		if (vkDescriptorSets.Size)
		{
			vkFreeDescriptorSets(VulkanDevice.vkDevice, vkDescriptorPool, uint32(vkDescriptorSets.Size), vkDescriptorSets.Data);
			vkDescriptorSets.Clear();
		}

		if (vkDescriptorPool)
		{
			vkDestroyDescriptorPool(VulkanDevice.vkDevice, vkDescriptorPool, nullptr);
			vkDescriptorPool = nullptr;
		}
	}

	void FVulkanDescriptorHeap::SetDebugName(FStringV ResourceLabel)
	{
		IRHIObject::SetDebugName(ResourceLabel);
		//SetVulkanObjectName(VulkanDescriptorHeap.Get(), ResourceLabel);
	}

	VkDescriptorSet FVulkanDescriptorHeap::Allocate()
	{
		if (DescriptorSetIndex < vkDescriptorSets.Size)
			return vkDescriptorSets[DescriptorSetIndex++];

		VkDescriptorSetAllocateInfo DescriptorSetAI { VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO };
		DescriptorSetAI.descriptorPool = vkDescriptorPool;
		DescriptorSetAI.descriptorSetCount = uint32(DescriptorSetLayout.Size);
		DescriptorSetAI.pSetLayouts = DescriptorSetLayout.Data;

		VkDescriptorSet DescriptorSet;
		VkResult Result = vkAllocateDescriptorSets(VulkanDevice.vkDevice, &DescriptorSetAI, &DescriptorSet);
		if (Result)
			return nullptr;

		vkDescriptorSets.Add(DescriptorSet);
		++DescriptorSetIndex;
		return DescriptorSet;
	}

	void FVulkanDescriptorHeap::ResetAllocate()
	{
		DescriptorSetIndex = 0;
	}

	bool FVulkanOfflineDescriptorAllocator::Allocate(FVulkanDescriptorLocation & DescriptorLocation, uint32 DescriptorCount)
	{
		AssertExpr(DescriptorCount <= AllocatorDesc.HeapSize);

		uint32 DescriptorIndex = NullIndex;
		for (uintx Index = 0; Index + DescriptorCount <= DescriptorBits.Size; ++Index)
		{
			bool Found = true;
			for (uintx TestIndex = 0; TestIndex < DescriptorCount; ++TestIndex)
			{
				if (DescriptorBits[Index + TestIndex])
				{
					Found = false;
					break;
				}

				if (Found)
					break;
			}

			if (Found)
			{
				DescriptorIndex = uint32(Index);
				break;
			}
		}

		if (DescriptorIndex == NullIndex)
		{
			FDescriptorHeapDesc DescriptorHeapDesc;
			DescriptorHeapDesc.Type = AllocatorDesc.Type;
			DescriptorHeapDesc.Size = AllocatorDesc.HeapSize;
			FVulkanDescriptorHeapRef DescriptorHeap = new FVulkanDescriptorHeap(VulkanDevice, {});
			DescriptorHeap->SetDebugName(Format(u8"{}.{}"V, DebugName, uintx(DescriptorHeaps.Size)));
			DescriptorHeaps.Add(DescriptorHeap);
			DescriptorIndex = uint32(DescriptorHeaps.Size - 1) * AllocatorDesc.HeapSize;
			DescriptorBits.AddN(false, AllocatorDesc.HeapSize);
#if Vulkan_HEAP_DEBUG
			BlockObjects.AddN(nullptr, AllocatorDesc.HeapSize);
			BlockObjectsLast.AddN(nullptr, AllocatorDesc.HeapSize);
#endif
		}

		uint32 DescriptorHeapIndex = DescriptorIndex / AllocatorDesc.HeapSize;
		uint32 LocalDescriptorIndex = DescriptorIndex % AllocatorDesc.HeapSize;
		FVulkanDescriptorHeap * DescriptorHeap = DescriptorHeaps[DescriptorHeapIndex];

		DescriptorLocation.HeapAllocator = this;
		DescriptorLocation.DescriptorHeap = DescriptorHeap;
		DescriptorLocation.BlockLocation.Offset = DescriptorIndex;
		DescriptorLocation.BlockLocation.Size = DescriptorCount;
		DescriptorLocation.Address.CPUAddress = DescriptorHeap->Address.CPUAddress + DescriptorHeap->HandleSize * LocalDescriptorIndex;
		DescriptorLocation.Address.GPUAddress = DescriptorHeap->Address.GPUAddress + DescriptorHeap->HandleSize * LocalDescriptorIndex;

		for (uintx TestIndex = 0; TestIndex < DescriptorCount; ++TestIndex)
		{
			AssertExpr(DescriptorBits[DescriptorIndex + TestIndex] == false);
#if Vulkan_HEAP_DEBUG
			AssertExpr(BlockObjects[DescriptorIndex + TestIndex] == nullptr);
#endif
		}

		DescriptorBits.SetAt(DescriptorIndex, true, DescriptorCount);
		for (uintx TestIndex = 0; TestIndex < DescriptorCount; ++TestIndex)
			AssertExpr(DescriptorBits[DescriptorIndex + TestIndex] == true);
		return true;
	}

	void FVulkanOfflineDescriptorAllocator::Deallocate(const FVulkanDescriptorBlockLocation & BlockLocation)
	{
#if Vulkan_HEAP_DEBUG
		AssertExpr(BlockObjects[BlockLocation.Offset]);
		BlockObjects[BlockLocation.Offset] = nullptr;
#endif
		for (uintx TestIndex = 0; TestIndex < BlockLocation.Size; ++TestIndex)
			AssertExpr(DescriptorBits[BlockLocation.Offset + TestIndex]);
		DescriptorBits.SetAt(BlockLocation.Offset, false, BlockLocation.Size);
	}
}
