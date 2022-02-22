#pragma once

#include "DWTypes.h"

namespace Xin::DWrite
{
	class FDWFontFamily : public IFontFamily
	{
	public:
		FDWFontFamily(FDWTextShaper & DWShaper, FStringV FamilyName, FWStringV FamilyNameW, IDWriteFontFamily * dwFontFamily);

	public:
		FDWTextShaper & DWShaper;
		IDWriteFontFamilyRef dwFontFamily;

		FWString FamilyNameW;
	};
	using FDWFontFamilyRef = TReferPtr<FDWFontFamily>;


	class FDWFontFace : public IFontFace
	{
	public:
		FDWFontFace(FDWTextShaper & DWShaper, const FFontFace & FontFace, IDWriteFontRef dwFont);

	public:
		FDWTextShaper & DWShaper;
		IDWriteFontRef dwFont;

		FWString FamilyNameW;
		IDWriteFontFaceRef dwFontFace;
		IDWriteFontFamilyRef dwFontFamily;
	};
	using FDWFontFaceRef = TReferPtr<FDWFontFace>;

	DWRITE_API FString GetDWriteFontFamilyName(IDWriteFontRef DWriteFont);
	DWRITE_API FWString GetDWriteFontFamilyNameW(IDWriteFontRef DWriteFont);
	DWRITE_API FString GetDWriteFontFamilyName(IDWriteFontFamilyRef DWriteFontFamily);
	DWRITE_API FWString GetDWriteFontFamilyNameW(IDWriteFontFamilyRef DWriteFontFamily);
}
