#pragma once

#include "Xin.Core/Xin.Core.h"

#if XIN_LIB_DYNAMIC
#if _Xin_Direct2D_Module_
#define D2D_API XIN_EXPORT
#else
#define D2D_API XIN_IMPORT
#endif
#else
#define D2D_API
#endif
