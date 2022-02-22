#pragma once

#include "X2D.Types.h"

namespace Xin
{
	enum EFontWeight
	{
		Thin = 100,
		ExtraLight = 200,
		Light = 300,
		SemiLight = 350,
		Regular = 400,
		Medium = 500,
		SemiBold = 600,
		Bold = 700,
		ExtraBold = 800,
		Heavy = 900,
	};

	enum class EFontStyle
	{
		None = 0,
		Italic = 0x0001,
	};

	struct CORE_API FFontFace
	{
		FString FaceName;
		EFontWeight FontWeight = EFontWeight::Regular;
		EFontStyle FontStyle = EFontStyle::None;

		bool operator==(const FFontFace &) const noexcept = default;
		auto operator <=>(const FFontFace &) const noexcept = default;

		friend uintx Hash(const FFontFace & Self)
		{
			return HashCombine(Self.FaceName, Self.FontWeight, Self.FontStyle);
		}

		CORE_VAR static FFontFace Default;
	};

	struct CORE_API FFontFormat
	{
		FFontFace FontFace;
		// The logical size of the font in DIPs (equals 1/96 inch)
		float32 FontSize = 18.0f;

		CORE_VAR static FFontFormat Default;
	};

	enum class ETextAlignment
	{
		Left = 0,
		CenterX = 0x0001,
		Right = 0x0002,
		Baseline = 0,
		Top = 0x0010,
		CenterY = 0x0020,
		Bottom = 0x0040,
		None = Left | Baseline,

		HorizontalMask = Left | CenterX | Right,
		VerticalMask = Baseline | Top | CenterY | Bottom
	};

	struct FTextFormat
	{
		FString FamilyName;
		uint32 FontSize = 16;
		uint32 FontWeight = EFontWeight::Regular;
		EFontStyle FontStyle = EFontStyle::None;
		ETextAlignment TextAlignment = ETextAlignment::None;

		bool operator ==(const FTextFormat &) const noexcept = default;
		auto operator <=>(const FTextFormat &) const noexcept = default;

		friend uintx Hash(const FTextFormat & Self)
		{
			return HashCombine(Self.FamilyName, Self.FontSize, Self.FontWeight, Self.FontStyle);
		}
	};

	struct FFontMetrics
	{
		float32 Size;
		float32 Ascent;
		float32 Descent;
		float32 LineSpace;
		uint32 UnitsPerEm;

		PropertyR(float32, GetHeight) Height;
		PropertyR(float32, GetLineHeight) LineHeight;

		float32 GetHeight() const { return Ascent + Descent; }
		float32 GetLineHeight() const { return Ascent + Descent + LineSpace; }

		FFontMetrics operator *(float32 Size) const
		{
			FFontMetrics FontMetrics;
			FontMetrics.Size = Size;
			FontMetrics.Ascent = Ascent * Size;
			FontMetrics.Descent = Descent * Size;
			FontMetrics.LineSpace = LineSpace * Size;
			FontMetrics.UnitsPerEm = UnitsPerEm;
			return FontMetrics;
		}
	};

	class IFontFamily : public IX2DObject
	{
	public:
		IFontFamily() = default;
		IFontFamily(FStringV FamilyName) : FamilyName(FamilyName) {}

	public:
		FString FamilyName;
	};
	using IFontFamilyRef = TReferPtr<IFontFamily>;


	class IFontFace : public ISharedRefer
	{
	public:
		IFontFace() = default;

		IFontFace(const FFontFace & FontFace, const FFontMetrics & FontMetrics)
			: FontFace(FontFace), Ascent(FontMetrics.Ascent), Descent(FontMetrics.Descent) {}

	public:
		FFontFace FontFace;
		float32 Ascent = 0.0f;
		float32 Descent = 0.0f;

		PropertyR(float32, GetHeight) Height;
		float32 GetHeight() const { return Ascent + Descent; }
	};

	using IFontFaceRef = TReferPtr<IFontFace>;
}
