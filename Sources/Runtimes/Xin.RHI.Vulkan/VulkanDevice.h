#pragma once

#include "VulkanTypes.h"
#include "VulkanShaderCompiler.h"
#include "VulkanCommandAllocator.h"
#include "VulkanHeap.h"

#define XIN_VULKAN_DEVICE_VALIDATION (XIN_VULKAN_DEBUG_UNTILS/* && XIN_DEBUG*/)

namespace Xin::RHI::Vulkan
{
	class RHI_VULKAN_API FVulkanDevice : public IRHIDevice
	{
	public:
		FVulkanDevice(ECreateDeviceFlag Flags);
		~FVulkanDevice();

		void InitializeInstance(ECreateDeviceFlag Flags);
		void InitializeDevice(ECreateDeviceFlag Flags);

	public:
		EDeviceRHI GetDeviceRHI() const override { return EDeviceRHI::Vulkan; }
		ETextureCompression GetTextureCompression() const override { return ETextureCompression::ASTC; }

		IRHIShaderCompiler & GetShaderCompiler() override { return VulkanShaderCompiler; }

		IRHIFenceRef CreateFence() override;
		IRHIShaderModuleRef CreateShaderModule(IShaderInstance & ShaderInstance) override;
		IRHIPipelineStateRef CreatePipelineState(const FPipelineStateDesc & PipelineStateDesc) override;

		IRHICommandListRef CreateCommandList(ECommandType CommandType) override;
		IRHICommandQueueRef CreateCommandQueue(const FCommandQueueDesc & CommandQueueDesc) override;

		IRHIViewportRef CreateViewport(IRHICommandQueue * CommandQueue, voidp WindowHandle, SizeU Size, uint32 BackBufferCount,
			EFormat Format = EFormat::RGBA, EPresentMode SwapEffect = EPresentMode::FlipDiscard, FMSAA MSAA = {}) override;

		IRHIHeapRef CreateHeap(const FHeapDesc & HeapDesc) override;
		IRHIResourceRef CreateCommitedResource(const FResourceDesc & ResourceDes) override;
		IRHIResourceRef CreateReservedResource(const FResourceDesc & ResourceDesc) override;
		IRHIResourceRef CreatePlacedResource(const FResourceDesc & ResourceDesc, const FRHIMemory & Memory) override;
		IRHIResourceRef CreatePlacedResource(const FResourceDesc & ResourceDesc, IHeapAllocator & HeapAllocator) override;
		IRHIResourceViewRef CreateResourceView(IRHIResource * RHIResource, const FResourceViewDesc & ResourceViewDesc) override;
		IRHIIndirectSignatureRef CreateIndirectSignature(const FIndirectSignature & IndirectSignature, IRHIPipelineState * RHIPipelineState) override;

	public:
		void ClearContext() override;
		void UpdateContext() override;
		uint64 GetMemoryUsage() const override;

	public:
		uint32 GetMemoryType(uint32 TypeBits, VkMemoryPropertyFlags MemoryPropertyFlags) const;
		EResourceAccess GetForamtAccess(EFormat Format) const override;
		FAllocateInfo GetAllocateInfo(const FResourceDesc & ResourceDesc, bool LocatedResource = false) const;
		// return RequiredSize;
		uint64 GetCopyableFootprints(const FResourceDesc & ResourceDesc, TSlice<FVulkanCopyableFootprint> CopyableFootprints) const;

	public:
		FVulkanCommandAllocatorRef AcquireCommandAllocator();
		void FreeCommandAllocator(FVulkanCommandAllocator * CommandAllocator);

		TList<FVulkanCommandAllocatorRef> FreeCommandAllocators;

	public:
		FVulkanDescriptorHeapRef AquireOnlineDescriptorHeap(const FVulkanShaderBindings & ShaderBindings);
		void ForfietDescriptorHeap(FVulkanDescriptorHeapRef DescriptorHeap);

		TList<FVulkanDescriptorHeapRef> DescriptorHeaps;

	public:
		void AllocateOfflineDescriptors(EResourceAspect ResourceView, FVulkanDescriptorLocation & DescriptorLocation, uint32 DescriptorCount);
		FVulkanOfflineDescriptorAllocator DescriptorAllocatorCommon;
		FVulkanOfflineDescriptorAllocator DescriptorAllocatorRTV;
		FVulkanOfflineDescriptorAllocator DescriptorAllocatorDSV;

	private:
		uint32 OnlineDescriptorHeapIndex = 0;

	private:
		class FHashedDescriptorSetLayout
		{
		public:
			uintx HashCode = 0;
			FVulkanShaderBindings ShaderBindings;

			FHashedDescriptorSetLayout() = default;
			FHashedDescriptorSetLayout(const FVulkanShaderBindings & ShaderBindings) :HashCode(Hash(ShaderBindings)), ShaderBindings(ShaderBindings) {}

			bool operator ==(const FHashedDescriptorSetLayout & Another) const { return HashCode == Another.HashCode; }
			auto operator <=>(const FHashedDescriptorSetLayout & Another) const noexcept { return HashCode <=> Another.HashCode; }
			friend uintx Hash(const FHashedDescriptorSetLayout & Index) { return Index.HashCode; }
		};
		mutable TMap<FHashedDescriptorSetLayout, TList<VkDescriptorSetLayout>> DescriptorSetLayouts;

	public:
		TList<VkDescriptorSetLayout> FetchDescriptorSetLayout(const FVulkanShaderBindings & ShaderBindings) const;

	private:
		class FHashedSamplerDesc
		{
		public:
			uintx HashCode = 0;
			FSamplerDesc SamplerDesc;

			FHashedSamplerDesc() = default;
			FHashedSamplerDesc(const FSamplerDesc & SamplerDesc) :HashCode(Hash(SamplerDesc)), SamplerDesc(SamplerDesc) {}

			bool operator ==(const FHashedSamplerDesc & Another) const { return HashCode == Another.HashCode; }
			auto operator <=>(const FHashedSamplerDesc & Another) const noexcept { return HashCode <=> Another.HashCode; }
			friend uintx Hash(const FHashedSamplerDesc & Index) { return Index.HashCode; }
		};
		mutable TMap<FHashedSamplerDesc, VkSampler> Samplers;

	public:
		VkSampler FetchSampler(const FSamplerDesc & SamplerDesc) const;

	public:
		FString DeviceName;
		uint64 VendorId = 0;
		uint64 DeviceId = 0;

		FVulkanShaderCompiler VulkanShaderCompiler { *this };

		TList<VkLayerProperties> InstanceLayerProperties;
		TList<VkExtensionProperties> InstanceExtensionProperties;

		TList<char const *> EnabledLayers;
		TList<const char *> EnabledExtensions;

		uint32 CommomResourceIncrement = 0;

		VkInstance vkInstance = nullptr;

#if XIN_VULKAN_DEBUG_UNTILS
		VkDebugUtilsMessengerEXT DebugUtilsMessengerEXT = nullptr;
		VkDebugReportCallbackEXT DebugReportCallbackEXT = nullptr;
#endif

		struct FPhysicalDevice
		{
			VkPhysicalDevice DeviceHandle;

			VkPhysicalDeviceFeatures Features;
			VkPhysicalDeviceProperties Properties;
			VkPhysicalDeviceMemoryProperties MemoryProperties;

			TList<VkQueueFamilyProperties> QueueFamilyProperties;
			TList<VkExtensionProperties> ExtensionProperties;

			//VkPhysicalDeviceLimits PhysicalDeviceLimits;
			//VkPhysicalDeviceSparseProperties PhysicalDeviceSparseProperties;

			FString DeviceName;
			VkPhysicalDeviceType DeviceType = VK_PHYSICAL_DEVICE_TYPE_OTHER;
			uint32_t VendorID = {};
			uint32_t DeviceID = {};

			uint32_t ApiVersion = {};
			uint32_t DriverVersion = {};

			static FPhysicalDevice FromHandle(VkPhysicalDevice PhysicalDevice);
		};
		TList<FPhysicalDevice> PhysicalDevices;
		FPhysicalDevice PhysicalDevice;

		TList<const char *> DeviceEnabledExtensions;

		VkPhysicalDevice vkPhysicalDevice = nullptr;
		VkDevice vkDevice = nullptr;
	};
	using FVulkanDeviceRef = TReferPtr<FVulkanDevice>;
}
