#include "PCH.h"
#include "VulkanDevice.h"

#include "VulkanCommandList.h"
#include "VulkanFence.h"
#include "VulkanViewport.h"
#include "VulkanCommandQueue.h"
#include "VulkanPipelineState.h"
#include "VulkanShaderModule.h"

//#undef XIN_VULKAN_DEVICE_DEBUG

namespace Xin::RHI::Vulkan
{
	static FVulkanOfflineDescriptorAllocatorDesc DescriptorAllcaotorCommonConfiguration =
	{
		.HeapSize = 1024,
		.Type = EDescriptorHeapType::ShaderResource,
	};

	static FVulkanOfflineDescriptorAllocatorDesc DescriptorAllcaotorRTVConfiguration =
	{
		.HeapSize = 256,
		.Type = EDescriptorHeapType::RenderTarget,
	};

	static FVulkanOfflineDescriptorAllocatorDesc DescriptorAllcaotorDSVConfiguration =
	{
		.HeapSize = 256,
		.Type = EDescriptorHeapType::DepthStencil,
	};

	static VkBool32 VKAPI_CALL DebugUtils_MessengerCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT MessageSeverity, VkDebugUtilsMessageTypeFlagsEXT MessageType,
		const VkDebugUtilsMessengerCallbackDataEXT * CallbackData, void * UserData)
	{
		// Log debug message
		if (MessageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
		{
			LogWarning(u8"0x{:x} - {}: {}", CallbackData->messageIdNumber, FAStringV(CallbackData->pMessageIdName), FAStringV(CallbackData->pMessage));
		}
		else if (MessageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
		{
			LogError(u8"0x{:x} - {}: {}", CallbackData->messageIdNumber, FAStringV(CallbackData->pMessageIdName), FAStringV(CallbackData->pMessage));
			//FDebug::Break();
		}
		return VK_FALSE;
	}

	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugReport_Callback(
		VkDebugReportFlagsEXT ReportFlags, VkDebugReportObjectTypeEXT /*type*/,
		uint64_t /*object*/, size_t /*location*/, int32_t /*message_code*/,
		const char *LayerPrefix, const char *Message, void * /*user_data*/)
	{
		if (ReportFlags & VK_DEBUG_REPORT_ERROR_BIT_EXT)
		{
			LogError(u8"{}: {}", FAStringV(LayerPrefix), FAStringV(Message));
		}
		else if (ReportFlags & VK_DEBUG_REPORT_WARNING_BIT_EXT)
		{
			LogWarning(u8"{}: {}", FAStringV(LayerPrefix), FAStringV(Message));
		}
		else if (ReportFlags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT)
		{
			LogWarning(u8"{}: {}", FAStringV(LayerPrefix), FAStringV(Message));
		}
		else
		{
			LogInfo(u8"{}: {}", FAStringV(LayerPrefix), FAStringV(Message));
		}
		return VK_FALSE;
	}

	FVulkanDevice::FVulkanDevice(ECreateDeviceFlag Flags)
		: DescriptorAllocatorCommon(*this, DescriptorAllcaotorCommonConfiguration)
		, DescriptorAllocatorRTV(*this, DescriptorAllcaotorRTVConfiguration)
		, DescriptorAllocatorDSV(*this, DescriptorAllcaotorDSVConfiguration)
	{
		InitializeInstance(Flags);
		InitializeDevice(Flags);
	}

	FVulkanDevice::~FVulkanDevice()
	{
		FreeCommandAllocators.Clear();
		DescriptorHeaps.Clear();

		for (auto & Iter : DescriptorSetLayouts)
		{
			for (VkDescriptorSetLayout & vkDescriptorSetLayout : Iter.Second)
				vkDestroyDescriptorSetLayout(vkDevice, vkDescriptorSetLayout, nullptr);
		}
		DescriptorSetLayouts.Clear();

		for (auto & Iter : Samplers)
			vkDestroySampler(vkDevice, Iter.Second, nullptr);
		Samplers.Reset();

		if (vkDevice)
		{
			vkDestroyDevice(vkDevice, nullptr);
			vkDevice = nullptr;
		}

#if XIN_VULKAN_DEBUG_UNTILS
		if (DebugUtilsMessengerEXT)
		{
			vkDestroyDebugUtilsMessengerEXT(vkInstance, DebugUtilsMessengerEXT, nullptr);
			DebugUtilsMessengerEXT = nullptr;
		}

		if (DebugReportCallbackEXT)
		{
			vkDestroyDebugReportCallbackEXT(vkInstance, DebugReportCallbackEXT, nullptr);
			DebugReportCallbackEXT = nullptr;
		}
#endif

		if (vkInstance)
		{
			vkDestroyInstance(vkInstance, nullptr);
			vkInstance = nullptr;
		}
	}


	static bool ValidateLayers(TView<char const *> LayerNames, TView<VkLayerProperties> LayerProperties)
	{
		for (const char * LayerName : LayerNames)
		{
			if (!LayerName)
				break;

			bool Found = false;
			for (const VkLayerProperties & LayerPropertie : LayerProperties)
			{
				if (strcmp(LayerPropertie.layerName, LayerName) == 0)
				{
					Found = true;
					break;
				}
			}

			if (!Found)
				return false;
		}

		return true;
	}

	static bool TryEnableExtension(TList<const char *> & EnabledExtensions, const char * ExtensionName, TView<VkExtensionProperties> InstanceExtensionProperties)
	{
		for (const VkExtensionProperties & ExtensionProperties : InstanceExtensionProperties)
		{
			if (strcmp(ExtensionProperties.extensionName, ExtensionName) == 0)
			{
				EnabledExtensions.Add(ExtensionName);
				return true;
			}
		}
		return false;
	}

	void FVulkanDevice::InitializeInstance(ECreateDeviceFlag Flags)
	{
		VkResult Result = volkInitialize();

		uint32 NumInstanceLayerProperties;
		vkEnumerateInstanceLayerProperties(&NumInstanceLayerProperties, nullptr);
		InstanceLayerProperties.ResizeTo(NumInstanceLayerProperties);
		vkEnumerateInstanceLayerProperties(&NumInstanceLayerProperties, InstanceLayerProperties.Data);

		uint32 NumInstanceExtensionProperties;
		vkEnumerateInstanceExtensionProperties(nullptr, &NumInstanceExtensionProperties, nullptr);
		InstanceExtensionProperties.ResizeTo(NumInstanceExtensionProperties);
		vkEnumerateInstanceExtensionProperties(nullptr, &NumInstanceExtensionProperties, InstanceExtensionProperties.Data);

#if XIN_VULKAN_DEVICE_VALIDATION
		static const TArray<TArray<char const *, 3>, 4> ValidationLayersRequired =
		{
			// The preferred validation layer is "VK_LAYER_KHRONOS_validation"
			{"VK_LAYER_KHRONOS_validation"},

			// Otherwise we fallback to using the LunarG meta layer
			{"VK_LAYER_LUNARG_standard_validation"},

			// Otherwise we attempt to enable the individual layers that compose the LunarG meta layer since it doesn't exist
			{
				//"VK_LAYER_GOOGLE_threading",
				"VK_LAYER_LUNARG_parameter_validation",
				"VK_LAYER_LUNARG_object_tracker",
				"VK_LAYER_LUNARG_core_validation",
				//"VK_LAYER_GOOGLE_unique_objects",
			},

			// Otherwise as a last resort we fallback to attempting to enable the LunarG core layer
			{"VK_LAYER_LUNARG_core_validation"}
		};

		for (const TView<char const *> & Layers : ValidationLayersRequired)
		{
			if (ValidateLayers(Layers, InstanceLayerProperties))
			{
				EnabledLayers.ReserveTo(Layers.Size);
				for (const char * LayerName : Layers)
				{
					if (!LayerName)
						break;
					EnabledLayers.Add(LayerName);
				}
				LogInfo(u8"Enabled Validation Layers: {}"V, FEncoding::ToUtf8(FAStringV(EnabledLayers[0])));
				break;
			}
		}
#endif

		bool FullEntensions = true;

		FullEntensions &= TryEnableExtension(EnabledExtensions, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME, InstanceExtensionProperties);

		FullEntensions &= TryEnableExtension(EnabledExtensions, VK_KHR_WIN32_SURFACE_EXTENSION_NAME, InstanceExtensionProperties);

		const bool HeadlessSurfaceEnabled = TryEnableExtension(EnabledExtensions, VK_EXT_HEADLESS_SURFACE_EXTENSION_NAME, InstanceExtensionProperties);
		if (!HeadlessSurfaceEnabled)
		{
			FullEntensions |= TryEnableExtension(EnabledExtensions, VK_KHR_SURFACE_EXTENSION_NAME, InstanceExtensionProperties);
		}

		//FullEntensions &= TryEnableExtension(EnabledExtensions, VK_NV_PER_STAGE_DESCRIPTOR_SET_EXTENSION_NAME, InstanceExtensionProperties);

		//FullEntensions |= TryEnableExtension(EnabledExtensions, VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME, InstanceExtensionProperties);

#if XIN_VULKAN_DEBUG_UNTILS
		bool HasDebugUtils = TryEnableExtension(EnabledExtensions, VK_EXT_DEBUG_UTILS_EXTENSION_NAME, InstanceExtensionProperties);
		bool HasDebugReport = false;
		if (HasDebugUtils)
		{
			HasDebugReport = TryEnableExtension(EnabledExtensions, VK_EXT_DEBUG_REPORT_EXTENSION_NAME, InstanceExtensionProperties);
			if (!HasDebugReport)
			{
				LogInfo(u8"Neither of {} or {} are available; disabling debug reporting"V, FAStringV(VK_EXT_DEBUG_UTILS_EXTENSION_NAME), FAStringV(VK_EXT_DEBUG_REPORT_EXTENSION_NAME));
			}
		}
#endif

		AssertExpr(FullEntensions);

		uint32 ApiVersion = VK_API_VERSION_1_3;
		VkApplicationInfo ApplicationInfo { VK_STRUCTURE_TYPE_APPLICATION_INFO };
		{
			ApplicationInfo.pApplicationName   = "Vulkan RHI Device";
			ApplicationInfo.applicationVersion = 0;
			ApplicationInfo.pEngineName        = "RHI.Vulkan";
			ApplicationInfo.engineVersion      = 0;
			ApplicationInfo.apiVersion         = ApiVersion;
		}

		VkInstanceCreateInfo InstanceCI = { VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO };
		{
			InstanceCI.pApplicationInfo = &ApplicationInfo;

			InstanceCI.enabledExtensionCount   = uint32(EnabledExtensions.Size);
			InstanceCI.ppEnabledExtensionNames = EnabledExtensions.Data;

			InstanceCI.enabledLayerCount   = uint32(EnabledLayers.Size);
			InstanceCI.ppEnabledLayerNames = EnabledLayers.Data;
		}

#if XIN_VULKAN_DEBUG_UNTILS
		VkDebugUtilsMessengerCreateInfoEXT DebugUtilsCI = { VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT };
		VkDebugReportCallbackCreateInfoEXT DebugReportCI = { VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT };
		if (HasDebugUtils)
		{
			DebugUtilsCI.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
			DebugUtilsCI.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
			DebugUtilsCI.pfnUserCallback = DebugUtils_MessengerCallback;

			InstanceCI.pNext = &DebugUtilsCI;
		}
		else if (HasDebugReport)
		{
			DebugReportCI.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
			DebugReportCI.pfnCallback = DebugReport_Callback;

			InstanceCI.pNext = &DebugReportCI;
		}
#endif

		VkLayerSettingsCreateInfoEXT LayerSettingsCreateInfo { VK_STRUCTURE_TYPE_LAYER_SETTINGS_CREATE_INFO_EXT };
		//if (EnabledExtensions.Contains(VK_EXT_LAYER_SETTINGS_EXTENSION_NAME))
		if (auto Iter = EnabledExtensions.FindBy([](const char * ExtensionName) { return strcmp(ExtensionName, VK_EXT_LAYER_SETTINGS_EXTENSION_NAME) == 0; });
			Iter != EnabledExtensions.End())
		{
			//LayerSettingsCreateInfo.settingCount = static_cast<uint32_t>(required_layer_settings.size());
			//LayerSettingsCreateInfo.pSettings    = required_layer_settings.data();
			//LayerSettingsCreateInfo.pNext        = InstanceInfo.pNext;
			//InstanceInfo.pNext                  = &LayerSettingsCreateInfo;
		}

		Result = vkCreateInstance(&InstanceCI, nullptr, &vkInstance);
		if (Result != VK_SUCCESS)
		{
			LogError(u8"vkCreateInstance failed with {}"V, Result);
		}

		volkLoadInstance(vkInstance);

#if XIN_VULKAN_DEBUG_UNTILS
		if (HasDebugUtils)
		{
			Result = vkCreateDebugUtilsMessengerEXT(vkInstance, &DebugUtilsCI, nullptr, &DebugUtilsMessengerEXT);
			if (Result != VK_SUCCESS)
			{
				LogError(u8"Could not create debug utils messenger");
			}
		}
		else if (HasDebugReport)
		{
			Result = vkCreateDebugReportCallbackEXT(vkInstance, &DebugReportCI, nullptr, &DebugReportCallbackEXT);
			if (Result != VK_SUCCESS)
			{
				LogError(u8"Could not create debug report callback");
			}
		}
#endif

		uint32 NumPhysicalDevices;
		vkEnumeratePhysicalDevices(vkInstance, &NumPhysicalDevices, nullptr);
		TList<VkPhysicalDevice> VkPhysicalDevices(NumPhysicalDevices);
		vkEnumeratePhysicalDevices(vkInstance, &NumPhysicalDevices, VkPhysicalDevices.Data);

		PhysicalDevices.ReserveTo(NumPhysicalDevices);
		for (VkPhysicalDevice VkPhysicalDevice : VkPhysicalDevices)
			PhysicalDevices.Add(FPhysicalDevice::FromHandle(VkPhysicalDevice));
	}

	void FVulkanDevice::InitializeDevice(ECreateDeviceFlag Flags)
	{
		PhysicalDevice = PhysicalDevices[0];

		//DeviceEnabledExtensions.Add(VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME);
		//DeviceEnabledExtensions.Add(VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME);

		DeviceEnabledExtensions.Add(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

		DeviceEnabledExtensions.Add(VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME);
		DeviceEnabledExtensions.Add(VK_KHR_DEDICATED_ALLOCATION_EXTENSION_NAME);

		//DeviceEnabledExtensions.Add(VK_NV_PER_STAGE_DESCRIPTOR_SET_EXTENSION_NAME);

		//DeviceEnabledExtensions.Add(VK_KHR_MAINTENANCE_5_EXTENSION_NAME);

		uintx NumQueueFamilyProperties = PhysicalDevice.QueueFamilyProperties.Size;

		TList<VkDeviceQueueCreateInfo> QueueCIs(NumQueueFamilyProperties, { VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO }, EElementFill::Fill);
		TList<TList<float>>      QueuePriorities(NumQueueFamilyProperties);

		for (uint32_t QueueFamilyIndex = 0U; QueueFamilyIndex < NumQueueFamilyProperties; ++QueueFamilyIndex)
		{
			const VkQueueFamilyProperties & QueueFamilyProperty = PhysicalDevice.QueueFamilyProperties[QueueFamilyIndex];
			{
				QueuePriorities[QueueFamilyIndex].ResizeTo(QueueFamilyProperty.queueCount, 0.5f);
			}

			VkDeviceQueueCreateInfo & DeviceQueueCI = QueueCIs[QueueFamilyIndex];
			DeviceQueueCI.queueFamilyIndex = QueueFamilyIndex;
			DeviceQueueCI.queueCount       = QueueFamilyProperty.queueCount;
			DeviceQueueCI.pQueuePriorities = QueuePriorities[QueueFamilyIndex].Data;
		}

		VkPhysicalDeviceTimelineSemaphoreFeaturesKHR TimelineSemaphoreFeatures { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TIMELINE_SEMAPHORE_FEATURES_KHR, nullptr, true };
		VkPhysicalDeviceDynamicRenderingFeaturesKHR DynamicRenderingFeatures { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES_KHR, &TimelineSemaphoreFeatures, true};
		VkPhysicalDeviceSynchronization2Features Synchronization2Features { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SYNCHRONIZATION_2_FEATURES, &DynamicRenderingFeatures, true};
		VkPhysicalDeviceShaderDemoteToHelperInvocationFeatures  ShaderDemoteToHelperInvocationFeatures { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_DEMOTE_TO_HELPER_INVOCATION_FEATURES, &Synchronization2Features, true };
		//VkPhysicalDevicePerStageDescriptorSetFeaturesNV PerStageDescriptorSetFeaturesNv { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PER_STAGE_DESCRIPTOR_SET_FEATURES_NV, &Synchronization2Features, true, false };
		VkDeviceCreateInfo DeviceCI { VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };

		// Latest requested feature will have the pNext's all set up for device creation.
		DeviceCI.pNext = &ShaderDemoteToHelperInvocationFeatures;

		DeviceCI.pQueueCreateInfos       = QueueCIs.Data;
		DeviceCI.queueCreateInfoCount    = uint32(QueueCIs.Size);
		DeviceCI.enabledExtensionCount   = uint32(DeviceEnabledExtensions.Size);
		DeviceCI.ppEnabledExtensionNames = DeviceEnabledExtensions.Data;

		VkPhysicalDeviceFeatures PhysicalDeviceFeatures = { };
		//ArrayFill((uint32 *)(&PhysicalDeviceFeatures), sizeof(VkPhysicalDeviceFeatures) / 4, 0x1u);
		PhysicalDeviceFeatures.samplerAnisotropy = true;
		PhysicalDeviceFeatures.shaderImageGatherExtended = true;

		//const auto requested_gpu_features = gpu.get_requested_features();
		//PhysicalDeviceFeatures = PhysicalDevice.Features;
		DeviceCI.pEnabledFeatures = &PhysicalDeviceFeatures;

		VkResult Result = vkCreateDevice(PhysicalDevice.DeviceHandle, &DeviceCI, nullptr, &vkDevice);
		if (Result != VK_SUCCESS)
		{
			LogError(u8"vkCreateDevice failed with {}."V, Result);
			return;
		}

		vkPhysicalDevice = PhysicalDevice.DeviceHandle;

		if (DeviceId != 0)
		{
			uintx GraphicsQueueFamilyIndex = NullIndex;
			uintx PresentQueueFamilyIndex = NullIndex;
			//for (const VkQueueFamilyProperties & QueueFamilyProperties : PhysicalDevices[0].QueueFamilyProperties)
			for (uintx QueueFamilyIndex = 0;  QueueFamilyIndex < PhysicalDevices[0].QueueFamilyProperties.Size; ++QueueFamilyIndex)
			{
				const FPhysicalDevice & PhysicalDevice = PhysicalDevices[0];
				VkBool32 SupportsPresent;
				vkGetPhysicalDeviceSurfaceSupportKHR(PhysicalDevice.DeviceHandle, QueueFamilyIndex, nullptr, &SupportsPresent);

				const VkQueueFamilyProperties & QueueFamilyProperties = PhysicalDevices[0].QueueFamilyProperties[QueueFamilyIndex];
				if (QueueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT)
				{
					if (PresentQueueFamilyIndex == NullIndex)
					{
						DeviceId = PhysicalDevice.DeviceID;
						break;
					}
				}
			}
		}
	}

	IRHIViewportRef FVulkanDevice::CreateViewport(IRHICommandQueue * CommandQueue, voidp WindowHandle, SizeU Size, uint32 BackBufferCount, EFormat Format, EPresentMode SwapEffect, FMSAA MSAA)
	{
		return new FVulkanViewport(StaticCastRef<FVulkanCommandQueue>(CommandQueue), WindowHandle, Size, BackBufferCount, Format, SwapEffect, MSAA);
	}

	IRHIFenceRef FVulkanDevice::CreateFence()
	{
		return new FVulkanFence(*this);
	}

	IRHICommandListRef FVulkanDevice::CreateCommandList(ECommandType CommandType)
	{
		return new FVulkanCommandList(*this, CommandType);
	}

	IRHICommandQueueRef FVulkanDevice::CreateCommandQueue(const FCommandQueueDesc & CommandQueueDesc)
	{
		return new FVulkanCommandQueue(*this, CommandQueueDesc);
	}

	IRHIHeapRef FVulkanDevice::CreateHeap(const FHeapDesc & HeapDesc)
	{
		return new FVulkanHeap(*this, HeapDesc);
	}

	IRHIResourceRef FVulkanDevice::CreateCommitedResource(const FResourceDesc & ResourceDesc)
	{
		return new FVulkanResource(*this, ResourceDesc);
	}

	IRHIResourceRef FVulkanDevice::CreateReservedResource(const FResourceDesc & ResourceDesc)
	{
		return new FVulkanResource(*this, ResourceDesc);
	}

	IRHIResourceRef FVulkanDevice::CreatePlacedResource(const FResourceDesc & ResourceDesc, const FRHIMemory & Memory) 
	{
		return new FVulkanResource(*this, ResourceDesc, Memory);
	}

	IRHIResourceRef FVulkanDevice::CreatePlacedResource(const FResourceDesc & ResourceDesc, IHeapAllocator & HeapAllocator)
	{
		return new FVulkanResource(*this, ResourceDesc, HeapAllocator);
	}

	IRHIResourceViewRef FVulkanDevice::CreateResourceView(IRHIResource * RHIResource, const FResourceViewDesc & ResourceViewDesc)
	{
		return new FVulkanResourceView(StaticCastRef<FVulkanResource>(RHIResource), ResourceViewDesc);
	}

	IRHIIndirectSignatureRef FVulkanDevice::CreateIndirectSignature(const FIndirectSignature & IndirectSignature, IRHIPipelineState * RHIPipelineState)
	{
		return new FVulkanIndirectSignature(*this, IndirectSignature, StaticCast<FVulkanPipelineState>(RHIPipelineState));
	}

	IRHIShaderModuleRef FVulkanDevice::CreateShaderModule(IShaderInstance & ShaderInstance)
	{
		return new FVulkanShaderModule(*this, ShaderInstance);
	}

	IRHIPipelineStateRef FVulkanDevice::CreatePipelineState(const FPipelineStateDesc & PipelineStateDesc)
	{
		return new FVulkanPipelineState(*this, PipelineStateDesc);
	}

	uint64 FVulkanDevice::GetMemoryUsage() const
	{
		uint64 MemoryUsage = 0;
		VkPhysicalDeviceMemoryBudgetPropertiesEXT DeviceMemoryBudgetProperties { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_BUDGET_PROPERTIES_EXT };
		VkPhysicalDeviceMemoryProperties2 DeviceMemoryProperties2 { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_PROPERTIES_2 };
		DeviceMemoryProperties2.pNext = &DeviceMemoryBudgetProperties;

		vkGetPhysicalDeviceMemoryProperties2(vkPhysicalDevice, &DeviceMemoryProperties2);
		for (int32 MemoryHeapIndex = 0; MemoryHeapIndex < VK_MAX_MEMORY_HEAPS; ++MemoryHeapIndex)
		{
			if (DeviceMemoryProperties2.memoryProperties.memoryHeaps[MemoryHeapIndex].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT)
				MemoryUsage += DeviceMemoryBudgetProperties.heapUsage[MemoryHeapIndex];;
		}
		return MemoryUsage;
	}

	void FVulkanDevice::ClearContext()
	{
		FreeCommandAllocators.Reset();
	}

	void FVulkanDevice::UpdateContext()
	{
	}

	uint32 FVulkanDevice::GetMemoryType(uint32 TypeBits, VkMemoryPropertyFlags MemoryPropertyFlags) const
	{
		for (uint32 MemoryTyeIndex = 0; MemoryTyeIndex < PhysicalDevice.MemoryProperties.memoryTypeCount; ++MemoryTyeIndex)
		{
			if (TypeBits & 1)
			{
				const VkMemoryType & MemoryType = PhysicalDevice.MemoryProperties.memoryTypes[MemoryTyeIndex];
				if (MemoryType.propertyFlags == MemoryPropertyFlags)
					return MemoryTyeIndex;
			}
		}

		for (uint32 MemoryTyeIndex = 0; MemoryTyeIndex < PhysicalDevice.MemoryProperties.memoryTypeCount; ++MemoryTyeIndex)
		{
			if (TypeBits & 1)
			{
				const VkMemoryType & MemoryType = PhysicalDevice.MemoryProperties.memoryTypes[MemoryTyeIndex];
				if (AllBits(MemoryType.propertyFlags, MemoryPropertyFlags))
					return MemoryTyeIndex;
			}
		}
		return NullIndex;
	}

	EResourceAccess FVulkanDevice::GetForamtAccess(EFormat Format) const
	{
		VkFormatProperties FormatProperties {};
		vkGetPhysicalDeviceFormatProperties(vkPhysicalDevice, FromFormat(Format), &FormatProperties);

		EResourceAccess ResourceAccess = EResourceAccess::None;
		SetFlags(ResourceAccess, EResourceAccess::RenderTarget, FormatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT);
		SetFlags(ResourceAccess, EResourceAccess::DepthStencil, FormatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
		return ResourceAccess;
	}

	FAllocateInfo FVulkanDevice::GetAllocateInfo(const FResourceDesc & ResourceDesc, bool LocatedResource) const
	{
		FAllocateInfo AllocateInfo;
		VkMemoryRequirements2 MemoryRequirements2 { VK_STRUCTURE_TYPE_MEMORY_REQUIREMENTS_2 };
		if (ResourceDesc.Dimension == EResourceDimension::Buffer)
		{
			VkBufferCreateInfo BufferCI = BufferCreateInfoFromResourceDesc(ResourceDesc);
			VkDeviceBufferMemoryRequirements DeviceBufferMemoryRequirements { VK_STRUCTURE_TYPE_DEVICE_BUFFER_MEMORY_REQUIREMENTS };
			DeviceBufferMemoryRequirements.pCreateInfo = &BufferCI;

			vkGetDeviceBufferMemoryRequirements(vkDevice, &DeviceBufferMemoryRequirements, &MemoryRequirements2);
		}
		else
		{
			VkImageCreateInfo ImageCI = ImageCreateInfoFromResourceDesc(ResourceDesc, VK_IMAGE_LAYOUT_UNDEFINED);
			VkDeviceImageMemoryRequirements DeviceImageMemoryRequirements { VK_STRUCTURE_TYPE_DEVICE_IMAGE_MEMORY_REQUIREMENTS };
			DeviceImageMemoryRequirements.pCreateInfo = &ImageCI;

			vkGetDeviceImageMemoryRequirements(vkDevice, &DeviceImageMemoryRequirements, &MemoryRequirements2);
		}

		AllocateInfo.Size = MemoryRequirements2.memoryRequirements.size;
		AllocateInfo.Alignment = MemoryRequirements2.memoryRequirements.alignment;
		AllocateInfo.Type = MemoryRequirements2.memoryRequirements.memoryTypeBits;

		return AllocateInfo;
	}

	uint64 FVulkanDevice::GetCopyableFootprints(const FResourceDesc & ResourceDesc, TSlice<FVulkanCopyableFootprint> CopyableFootprints) const
	{
		uint32 NumCopyableFootprints = ResourceDesc.Mipmaps * ResourceDesc.Layers;
		AssertExpr(CopyableFootprints.Size >= NumCopyableFootprints);

		FFormatInfo FormatInfo = GetFormatInfo(ResourceDesc.Format);
		if (!FormatInfo.BlockBits)
			return 0;

		auto GetMipmapSize = [&](uint32 MipmapIndex)
		{
			uint32 MipmapWidth = ResourceDesc.Size.Width >> MipmapIndex;
			uint32 MipmapHeight = ResourceDesc.Size.Width >> MipmapIndex;

			uint32 BlockedWidth = Max(1, AlignWith(MipmapWidth, FormatInfo.BlockWidth));
			uint32 BlockedHeight = Max(1, AlignWith(MipmapHeight, FormatInfo.BlockHeight));
			uint32 NumBlockedBytes = BlockedWidth * BlockedHeight * FormatInfo.BlockBits / 8;
			return NumBlockedBytes;
		};

		uint64 CopyableFootprintOffset = 0;
		for (uintx LayerIndex = 0; LayerIndex < ResourceDesc.Layers; ++LayerIndex)
		{
			for (uintx MipmapIndex = 0; MipmapIndex < ResourceDesc.Mipmaps; ++MipmapIndex)
			{
				FVulkanCopyableFootprint & CopyableFootprint = CopyableFootprints[ResourceDesc.Mipmaps * LayerIndex + MipmapIndex];
				CopyableFootprint.Offset = CopyableFootprintOffset;
				CopyableFootprint.Format = ResourceDesc.Format;
				CopyableFootprint.Size = ResourceDesc.Size >> MipmapIndex;
				CopyableFootprint.Layer = LayerIndex;
				CopyableFootprint.NumBytes = GetMipmapSize(MipmapIndex);
				CopyableFootprintOffset += CopyableFootprint.NumBytes;
			}
		}

		return CopyableFootprintOffset;
	}

	FVulkanCommandAllocatorRef FVulkanDevice::AcquireCommandAllocator()
	{
		if (FreeCommandAllocators.Size)
			return FreeCommandAllocators.PopBack();

		return new FVulkanCommandAllocator(*this);
	}

	void FVulkanDevice::FreeCommandAllocator(FVulkanCommandAllocator * CommandAllocator)
	{
		CommandAllocator->ResetAllocator();
		FreeCommandAllocators.Add(CommandAllocator);
	}

	FVulkanDescriptorHeapRef FVulkanDevice::AquireOnlineDescriptorHeap(const FVulkanShaderBindings & ShaderBindings)
	{
		for (uintx Index = 0; Index < DescriptorHeaps.Size; ++Index)
		{
			FVulkanDescriptorHeapRef & DescriptorHeap = DescriptorHeaps[Index];
			if (DescriptorHeap->ShaderBindings == ShaderBindings)
			{
				FVulkanDescriptorHeapRef Result = DescriptorHeap;
				DescriptorHeaps.EraseAt(Index);
				return Result;
			}
		}

		constexpr uint32 MaxDescriptorSets = 2048;

		FVulkanDescriptorHeapRef VulkanDescriptorHeap = new FVulkanDescriptorHeap(*this, ShaderBindings, MaxDescriptorSets);
		VulkanDescriptorHeap->SetDebugName(Format(u8"{}.OnlineDescriptorHeap.{}"V, DebugName, OnlineDescriptorHeapIndex++));
		return VulkanDescriptorHeap;
	}

	void FVulkanDevice::ForfietDescriptorHeap(FVulkanDescriptorHeapRef DescriptorHeap)
	{
		DescriptorHeap->ResetAllocate();
		DescriptorHeaps.Add(DescriptorHeap);
	}

	void FVulkanDevice::AllocateOfflineDescriptors(EResourceAspect ResourceView, FVulkanDescriptorLocation & DescriptorLocation, uint32 DescriptorCount)
	{
		switch (ResourceView)
		{
		case EResourceAspect::ShaderResource:
		case EResourceAspect::TexelAccess:
			DescriptorAllocatorCommon.Allocate(DescriptorLocation, DescriptorCount);
			break;
		case EResourceAspect::RenderTarget:
			DescriptorAllocatorRTV.Allocate(DescriptorLocation, DescriptorCount);
			break;
		case EResourceAspect::DepthStencil:
			DescriptorAllocatorDSV.Allocate(DescriptorLocation, DescriptorCount);
			break;
		default:
			break;
		}
	}

	TList<VkDescriptorSetLayout> FVulkanDevice::FetchDescriptorSetLayout(const FVulkanShaderBindings & ShaderBindings) const
	{
		FHashedDescriptorSetLayout HashedDescriptorSetLayout { ShaderBindings };

		if (auto Iter = DescriptorSetLayouts.Find(HashedDescriptorSetLayout); Iter != DescriptorSetLayouts.End())
			return Iter->Second;

		TArray<VkSampler, MaxStaticSamplers> StaticSamplers {};
		// create a new VkDescriptorSetLayout
		uint32 NumShaderBindings = 0;
		TList<VkDescriptorSetLayout> vkDescriptorSetLayouts;
		TArray<VkDescriptorSetLayoutBinding, MaxVulkanShaderParameters> DescriptorSetLayoutBindings {};
		for (uint32 BindingIndex = 0; BindingIndex < MaxVulkanShaderBindingIndex; ++BindingIndex)
		{
			if (const FVulkanShaderParameter & ShaderParameter = ShaderBindings[BindingIndex])
			{
				VkDescriptorSetLayoutBinding & DescriptorSetLayoutBinding = DescriptorSetLayoutBindings[NumShaderBindings];
				DescriptorSetLayoutBinding.binding = ShaderParameter.BindingIndex;
				DescriptorSetLayoutBinding.descriptorType = ShaderParameter.vkDescriptorType;
				DescriptorSetLayoutBinding.descriptorCount = 1;
				DescriptorSetLayoutBinding.stageFlags = FromShaderRelevance(ShaderParameter.ShaderRelevance);

				for (uintx StaticSamplerIndex = 0; StaticSamplerIndex < MaxStaticSamplers; ++StaticSamplerIndex)
				{
					const FSamplerDesc & SamplerDesc = ShaderBindings.StaticSamplers[StaticSamplerIndex];
					if (SamplerDesc.BindingIndex == ShaderParameter.BindingIndex && SamplerDesc.BindingSpace == ShaderParameter.BindingSpace)
					{
						if (!StaticSamplers[ShaderParameter.BindingIndex])
							StaticSamplers[ShaderParameter.BindingIndex] = FetchSampler(SamplerDesc);

						DescriptorSetLayoutBinding.pImmutableSamplers = &(StaticSamplers[ShaderParameter.BindingIndex]);
						break;
					}
				}

				++NumShaderBindings;
			}
		}

		VkDescriptorSetLayoutCreateInfo DescriptorSetLayoutCI { VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO };
		{
			DescriptorSetLayoutCI.flags = VkDescriptorSetLayoutCreateFlagBits(0);
			DescriptorSetLayoutCI.bindingCount = NumShaderBindings;
			DescriptorSetLayoutCI.pBindings = DescriptorSetLayoutBindings.Data;
		}

		VkDescriptorSetLayout vkDescriptorSetLayout;
		vkCreateDescriptorSetLayout(vkDevice, &DescriptorSetLayoutCI, nullptr, &vkDescriptorSetLayout);
		vkDescriptorSetLayouts.Add(vkDescriptorSetLayout);

		DescriptorSetLayouts.Insert({ HashedDescriptorSetLayout, vkDescriptorSetLayouts });
		return vkDescriptorSetLayouts;
	}

	VkSampler FVulkanDevice::FetchSampler(const FSamplerDesc & SamplerDesc) const
	{
		FHashedSamplerDesc HashedSamplerDesc { SamplerDesc };

		if (auto Iter = Samplers.Find(HashedSamplerDesc); Iter != Samplers.End())
			return Iter->Second;

		// create a new VkDescriptorSetLayout
		VkSamplerCreateInfo SamplerCI = { VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO };
		SamplerCI.magFilter = FromFilter(SamplerDesc.SampleFilter);
		SamplerCI.minFilter = FromFilter(SamplerDesc.SampleFilter);;
		SamplerCI.mipmapMode = FromMipmapMode(SamplerDesc.SampleFilter);
		SamplerCI.addressModeU = FromAddressMode(SamplerDesc.AddressModeU);
		SamplerCI.addressModeV = FromAddressMode(SamplerDesc.AddressModeV);
		SamplerCI.addressModeW = FromAddressMode(SamplerDesc.AddressModeW);
		SamplerCI.mipLodBias = SamplerDesc.MipLODBias;
		SamplerCI.compareEnable = SamplerDesc.Comparison != EComparison::None;
		SamplerCI.compareOp = FromComparison(SamplerDesc.Comparison);
		SamplerCI.minLod = SamplerDesc.MinLOD;
		SamplerCI.maxLod = SamplerDesc.MaxLOD;
		SamplerCI.maxAnisotropy = SamplerDesc.MaxAnisotropy;
		SamplerCI.anisotropyEnable = SamplerDesc.MaxAnisotropy > 0;
		SamplerCI.borderColor = FromBorderColor(SamplerDesc.BorderColor);

		VkSampler Sampler;
		VkResult Result = vkCreateSampler(vkDevice, &SamplerCI, nullptr, &Sampler);

		Samplers.Insert({ HashedSamplerDesc, Sampler });
		return Sampler;
		
	}
	//IVulkanHeapRef FVulkanDevice::CreateResourceHeap(const FHeapDesc & HeapDesc)
	//{
	//	return nullptr;
	//}


	FVulkanDevice::FPhysicalDevice FVulkanDevice::FPhysicalDevice::FromHandle(VkPhysicalDevice VkPhysicalDevice)
	{
		FPhysicalDevice PhysicalDevice { VkPhysicalDevice };

		vkGetPhysicalDeviceFeatures(VkPhysicalDevice, &PhysicalDevice.Features);
		vkGetPhysicalDeviceProperties(VkPhysicalDevice, &PhysicalDevice.Properties);
		vkGetPhysicalDeviceMemoryProperties(VkPhysicalDevice, &PhysicalDevice.MemoryProperties);

		{
			uint32 NumQueueFamilyProperties = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(VkPhysicalDevice, &NumQueueFamilyProperties, nullptr);
			PhysicalDevice.QueueFamilyProperties.ResizeTo(NumQueueFamilyProperties, EInitializeMode::None);
			vkGetPhysicalDeviceQueueFamilyProperties(VkPhysicalDevice, &NumQueueFamilyProperties, PhysicalDevice.QueueFamilyProperties.Data);
		}

		{
			uint32 NumExtensionProperties = 0;
			vkEnumerateDeviceExtensionProperties(VkPhysicalDevice, nullptr, &NumExtensionProperties, nullptr);
			PhysicalDevice.ExtensionProperties.ResizeTo(NumExtensionProperties, EInitializeMode::None);
			vkEnumerateDeviceExtensionProperties(VkPhysicalDevice, nullptr, &NumExtensionProperties, PhysicalDevice.ExtensionProperties.Data);
		}

		PhysicalDevice.DeviceName = FEncoding::ToUtf8(FAStringV(PhysicalDevice.Properties.deviceName));
		PhysicalDevice.DeviceType = PhysicalDevice.Properties.deviceType;

		PhysicalDevice.VendorID = PhysicalDevice.Properties.vendorID;
		PhysicalDevice.DeviceID = PhysicalDevice.Properties.deviceID;

		PhysicalDevice.ApiVersion = PhysicalDevice.Properties.apiVersion;
		PhysicalDevice.DriverVersion = PhysicalDevice.Properties.driverVersion;

		return PhysicalDevice;
	}
}
