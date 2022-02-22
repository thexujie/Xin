#pragma once

#include "Xin.Core/Xin.Core.h"
#include "Xin.Core/Platform/Windows/Internal/Internal.h"

#if XIN_LIB_DYNAMIC
#if _Xin_Windows_Informer_Module_
#define INFORMER_API XIN_EXPORT
#else
#define INFORMER_API XIN_IMPORT
#endif
#else
#define INFORMER_API
#endif

namespace Xin::Windows::Informer
{
}
