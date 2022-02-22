#pragma once

#include "VulkanTypes.h"

#define Vulkan_HEAP_DEBUG 1

namespace Xin::RHI::Vulkan
{
	class FVulkanDevice;

	class FVulkanHeap: public IRHIHeap
	{
	public:
		FVulkanHeap(FVulkanDevice & VulkanDevice, const FHeapDesc & HeapDesc);
		~FVulkanHeap();

		void SetDebugName(FStringV ResourceLabel) override;

	public:
		byte * Lock(ERHILockMode LockMode);
		void Unlock();

	public:
		FVulkanDevice & VulkanDevice;
		VkBuffer vkBuffer = nullptr;
		VkDeviceMemory vkMemory = nullptr;
	};

	class FVulkanDescriptorHeap : public IRHIObject
	{
	public:
		FVulkanDescriptorHeap(FVulkanDevice & VulkanDevice, const FVulkanShaderBindings & ShaderBindings, uint32 MaxDescriptorSets = 2048);
		~FVulkanDescriptorHeap();

		void SetDebugName(FStringV ResourceLabel) override;

	public:
		FVulkanDevice & VulkanDevice;
		FVulkanShaderBindings ShaderBindings;

		TList<VkDescriptorSetLayout> DescriptorSetLayout;
		VkDescriptorPool vkDescriptorPool = nullptr;
		TList<VkDescriptorSet> vkDescriptorSets;

		FVulkanDescriptorAddress Address;
		uint32 HandleSize = 0;

	public:
		bool CanAllocate() const { return DescriptorSetIndex < MaxDescriptorSets; }
		VkDescriptorSet Allocate();
		void ResetAllocate();

	private:
		uint32 DescriptorSetIndex = 0;
		uint32 MaxDescriptorSets = 0;

	public:
		static constexpr VkDescriptorType UsedDescriptorTypes[] = { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, VK_DESCRIPTOR_TYPE_SAMPLER };
	};
	using FVulkanDescriptorHeapRef = TReferPtr<FVulkanDescriptorHeap>;


	struct FVulkanOfflineDescriptorAllocatorDesc
	{
		uint32 HeapSize = 0;
		EDescriptorHeapType Type = EDescriptorHeapType::ShaderResource;
	};


	struct FVulkanDescriptorBlockLocation
	{
		uint64 Offset = 0;
		uint64 Size = 0;

		explicit operator bool() const { return !!Size; }
		bool operator ==(const FVulkanDescriptorBlockLocation &) const = default;
		auto operator <=>(const FVulkanDescriptorBlockLocation &) const = default;
		friend uintx Hash(const FVulkanDescriptorBlockLocation & BuddyBlock) { return HashCombine(BuddyBlock.Offset, BuddyBlock.Size); }
	};

	struct FVulkanDescriptorLocation
	{
		class FVulkanDescriptorAllocator * HeapAllocator = nullptr;
		FVulkanDescriptorHeap * DescriptorHeap = nullptr;
		FVulkanDescriptorBlockLocation BlockLocation;
		FVulkanDescriptorAddress Address;

	public:
		FVulkanDescriptorLocation() = default;
		~FVulkanDescriptorLocation();
		FVulkanDescriptorLocation(const FVulkanDescriptorLocation &) = delete;
		FVulkanDescriptorLocation & operator =(const FVulkanDescriptorLocation &) = delete;

		FVulkanDescriptorLocation(FVulkanDescriptorLocation && That) noexcept;
		FVulkanDescriptorLocation & operator =(FVulkanDescriptorLocation && That) noexcept;

		void Detatch();
		void Reset();
	};

	class FVulkanDescriptorAllocator : public IRHIObject
	{
	public:
		virtual void Deallocate(const FVulkanDescriptorBlockLocation & BlockLocation) = 0;
		virtual void SetBlockObject(const FVulkanDescriptorBlockLocation & DescriptorBlockLocation, IRHIObject * Object) {}
	};

	class FVulkanOfflineDescriptorAllocator final : public FVulkanDescriptorAllocator
	{
	public:
		FVulkanOfflineDescriptorAllocator(FVulkanDevice & VulkanDevice, const FVulkanOfflineDescriptorAllocatorDesc & AllocatorDesc)
			: VulkanDevice(VulkanDevice), AllocatorDesc(AllocatorDesc) {}

		bool Allocate(FVulkanDescriptorLocation & DescriptorLocation, uint32 DescriptorCount);
		void Deallocate(const FVulkanDescriptorBlockLocation & BlockLocation) override;
		void SetBlockObject(const FVulkanDescriptorBlockLocation & BlockLocation, IRHIObject * Object) override
		{
#if Vulkan_HEAP_DEBUG
			AssertExpr(BlockLocation.Size);
			AssertExpr(BlockObjects[BlockLocation.Offset] == nullptr);
			BlockObjects[BlockLocation.Offset] = Object;
			BlockObjectsLast[BlockLocation.Offset] = Object;
#endif
		}
	public:
		FVulkanDevice & VulkanDevice;
		FVulkanOfflineDescriptorAllocatorDesc AllocatorDesc;

	private:
		TList<FVulkanDescriptorHeapRef> DescriptorHeaps;
		TList<bool> DescriptorBits;
#if Vulkan_HEAP_DEBUG
		TList<IRHIObject *> BlockObjects;
		TList<IRHIObject *> BlockObjectsLast;
#endif
	};
}
