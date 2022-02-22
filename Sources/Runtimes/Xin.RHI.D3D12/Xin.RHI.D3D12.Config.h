#pragma once

#include "Xin.Core/Xin.Core.h"

#if XIN_LIB_DYNAMIC
#if _Xin_RHI_D3D12_Module_
#define RHID3D12_API XIN_EXPORT
#else
#define RHID3D12_API XIN_IMPORT
#endif
#else
#define RHID3D12_API
#endif
