#pragma once

#include "Xin.Core/Xin.Core.h"

#if XIN_LIB_DYNAMIC
#if _Xin_RHI_Vulkan_Module_
#define RHI_VULKAN_API XIN_EXPORT
#define RHI_VULKAN_VAR XIN_EXPORT
#else
#define RHI_VULKAN_API
#define RHI_VULKAN_VAR XIN_IMPORT
#endif
#else
#define RHI_VULKAN_API
#define RHI_VULKAN_VAR
#endif

#if XIN_DEBUG
#define XIN_VULKAN_DEBUG 1
#else
#define XIN_VULKAN_DEBUG 0
#endif