#pragma once

#include "../Basic/Basic.h"

namespace Xin
{
	enum class EFileAttribute
	{
		None = 0,
		Normal = 0x0001,
		Hidden = 0x0002,
		System = 0x0004,
		ReadOnly = 0x00010000,

		Directory = 0x10000000,
	};
	template class TEnumWrapper<EFileAttribute>;
}
