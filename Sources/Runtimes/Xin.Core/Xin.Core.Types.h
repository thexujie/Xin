#pragma once

#include "Xin.Base/Xin.Base.h"

#ifdef _Xin_Core_Module_
#define CORE_API XIN_EXPORT
#define CORE_VAR
#else
#define CORE_API
#define CORE_VAR XIN_IMPORT
#endif

namespace Xin
{
	using dimenx = float64;
	using FPoint = TVec2<dimenx>;
	using FSize = TVec2<dimenx>;
	using FRect = TVec4<dimenx>;
	using FVec2 = TVec2<dimenx>;
	using FVec4 = TVec4<dimenx>;
	using FThickness = TThickness<dimenx>;
}
