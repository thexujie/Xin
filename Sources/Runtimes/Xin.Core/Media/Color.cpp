#include "PCH.h"
#include "Color.h"

namespace Xin
{
#define COLORS_ITEM(ColorName, ColorValue) case  EColor::ColorName: return u8 ## #ColorName ## V;
	FStringV ColorName(EColor Color)
	{
		switch (Color)
		{
#include "Colors.inl"
		default:
			return FStringV::None;
		}
	}
#undef COLORS_ITEM

#define COLORS_ITEM(ColorName, ColorValue) if (StringEqualIC(Name, u8 ## #ColorName ## V)) return EColor::ColorName;
	EColor ColorValue(FStringV Name)
	{
#include "Colors.inl"
		return EColor::None;
	}
#undef COLORS_ITEM
}