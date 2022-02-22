#pragma once

#include "Xin.Core/Xin.Core.h"

#if XIN_LIB_DYNAMIC
#if _Xin_Skia_Module_
#define SKIA_API XIN_EXPORT
#else
#define SKIA_API XIN_IMPORT
#endif
#else
#define SKIA_API
#endif
