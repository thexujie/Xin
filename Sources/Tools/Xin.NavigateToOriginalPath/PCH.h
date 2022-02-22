#pragma once

#include "Xin.Core/Xin.Core.h"
#include "Xin.Core/Platform/Win32/Win32.h"
#include "Xin.Core.Windows/Xin.Core.Windows.h"

using namespace Xin;
using namespace Xin::Win32;
using namespace Xin::Windows;

#include <shlobj.h>
#include <shlwapi.h>

#if XIN_WINDOWS
#ifdef _Xin_NavigateToOriginalPath_Module_
#define NavigateToOriginalPath_API __declspec(dllexport)
#else
#define NavigateToOriginalPath_API __declspec(dllimport)
#endif
#endif

extern Xin::uintx_t DllRefCount;
