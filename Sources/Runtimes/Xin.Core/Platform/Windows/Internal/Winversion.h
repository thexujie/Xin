#pragma once

#include "Internal.Types.h"

namespace Xin::Windows::Internal
{
	struct CORE_API FWinversion
	{
		uint32 Major;
		uint32 Minor;
		uint32 Build;

		bool operator==(const FWinversion &) const = default;
		auto operator<=>(const FWinversion &) const = default;

		CORE_VAR static FWinversion Current;
	};

	enum class EWinversion
	{
		None = 0,

		Windows6,
		Windows7,
		Windows8,
		Windows8_1,
		Windows10,
		Windows10_TH1,
		Windows10_TH2,
		Windows10_RS1,
		Windows10_RS2,
		Windows10_RS3,
		Windows10_RS4,
		Windows10_RS5,
		Windows10_19H1,
		Windows10_19H2,
		Windows10_20H1,
		Windows10_20H2,
		Windows10_21H1,
		Windows10_21H2,
		Windows10_22H2,

		Windows11,
		Windows11_22H2,
		Windows11_23H2,
	};

	inline FWinversion Winversion_6 = { 6, 0, 0 };
	inline FWinversion Winversion_7 = { 6, 1, 0 };
	inline FWinversion Winversion_8 = { 6, 2, 0 };
	inline FWinversion Winversion_8_1 = { 6, 3, 0 };

	inline FWinversion Winversion_10 = { 10, 0, 0 };
	inline FWinversion Winversion_10_TH1 = { 10, 0, 10240 };
	inline FWinversion Winversion_10_TH2 = { 10, 0, 10586 };
	inline FWinversion Winversion_10_RS1 = { 10, 0, 14393 };
	inline FWinversion Winversion_10_RS2 = { 10, 0, 15063 };
	inline FWinversion Winversion_10_RS3 = { 10, 0, 16299 };
	inline FWinversion Winversion_10_RS4 = { 10, 0, 17134 };
	inline FWinversion Winversion_10_RS5 = { 10, 0, 17763 };
	inline FWinversion Winversion_10_19H1 = { 10, 0, 18362 };
	inline FWinversion Winversion_10_19H2 = { 10, 0, 18363 };
	inline FWinversion Winversion_10_20H1 = { 10, 0, 19041 };
	inline FWinversion Winversion_10_20H2 = { 10, 0, 19042 };
	inline FWinversion Winversion_10_21H1 = { 10, 0, 19043 };
	inline FWinversion Winversion_10_21H2 = { 10, 0, 19044 };
	inline FWinversion Winversion_10_22H2 = { 10, 0, 19045 };

	inline FWinversion Winversion_11 = { 10, 0, 22000};
	inline FWinversion Winversion_11_22H2 = { 10, 0, 22621 };
	inline FWinversion Winversion_11_23H2 = { 10, 0, 22631 };
	inline FWinversion Winversion_New = { UInt32Max, UInt32Max, UInt32Max };
}
