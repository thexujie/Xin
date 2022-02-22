#pragma once

#include "Xin.Core/Xin.Core.h"

#if XIN_LIB_DYNAMIC
#if _Xin_RHI_UI_Module_
#define RHIUI_API XIN_EXPORT
#else
#define RHIUI_API XIN_IMPORT
#endif
#else
#define RHIUI_API
#endif
