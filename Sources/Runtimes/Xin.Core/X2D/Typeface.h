#pragma once

#include "X2D.Types.h"

namespace Xin
{
	enum class ETextWrap
	{
		None,
		Char,
		Word,
		WordNoBreaking,
	};

	enum class ETextFlags
	{
		None = 0x0000,
		
		WrapDefault = 0x0001,
		WrapChar = 0x0002,
		WrapWord = 0x0004,
		
		TrimmingNone = None,
		TrimmingChar =0x0010,
		TrimmingCharEllipsis = 0x0020,
		TrimmingWord = 0x0040,
		TrimmingWordEllipsis = 0x0080,
		TrimmingPathEllipsis = 0x0100,

		AlignLeading = 0x00000000,
		AlignCenter = 0x00010000,
		AlignTrailing = 0x00020000,
		AlignLeadingJustify = 0x00040000,

		LineAlignNear = 0x00000000,
		LineAlignCenter = 0x00100000,
		LineAlignFar = 0x00200000,
	};
	template class TEnumWrapper<ETextFlags>;

	enum class EFontFlags
	{
		None = 0,
		Italic = 0x0002,
		UnderLine = 0x0004,
		StrikeOut = 0x0008,
	};
	template class TEnumWrapper<EFontFlags>;

	enum ETextAntiAliasing
	{
		System = 0,
		ClearType,
		Gray,
		GrayGrid,
		AntiGray,
		AntiGrayGrid,
	};
	
	namespace FontWeights
	{
		enum
		{
			Invisible = 0,
			Thin = 100,
			ExtraLight = 200,
			Light = 300,
			Normal = 400,
			Medium = 500,
			SemiBold = 600,
			Bold = 700,
			ExtraBold = 800,
			BlackBold = 900,
			ExtraBlackBold = 1000,
		};
	}

	struct FTypeface
	{
		FTypeface() = default;
		FTypeface(const FTypeface &) = default;
		FTypeface(FTypeface &&) noexcept = default;
		FTypeface & operator =(const FTypeface &) = default;
		auto operator <=>(const FTypeface &) const = default;
		FTypeface(FStringV FontFamily, uint32_t FontWeight = FontWeights::Normal, EFontFlags FontFlags = EFontFlags::None) : FontFamily(FontFamily), FontWeight(FontWeight), FontFlags(FontFlags) {}
		
		FString FontFamily;
		uint32_t FontWeight = FontWeights::Normal;
		EFontFlags FontFlags = EFontFlags::None;
	};

	struct FTextFormat
	{
		FTextFormat() = default;
		FTextFormat(const FTextFormat &) = default;
		FTextFormat(FTextFormat &&) noexcept = default;
		FTextFormat & operator =(const FTextFormat &) = default;
		auto operator <=>(const FTextFormat &) const = default;

		FTextFormat(const FTypeface & Typeface, float32 FontSize, ETextFlags TextFlags = ETextFlags::None) : Typeface(Typeface), FontSize(FontSize), TextFlags(TextFlags) {}
		
		FTypeface Typeface;
		float32 FontSize = 0.0f;
		ETextFlags TextFlags = ETextFlags::None;
	};
	
}