#include "PCH.h"
#include "DWFontFace.h"
#include "DWTextShaper.h"

namespace Xin::DWrite
{
	FDWFontFamily::FDWFontFamily(FDWTextShaper & DWShaper, FStringV FamilyName, FWStringV FamilyNameW, IDWriteFontFamily * dwFontFamily)
		: IFontFamily(FamilyName), DWShaper(DWShaper), dwFontFamily(dwFontFamily), FamilyNameW(FamilyNameW)
	{
		
	}

	static FFontMetrics FontMetricsFromIDWriteFont(IDWriteFont * dwFont, float32 FontSize)
	{
		DWRITE_FONT_METRICS DWriteFontMetrics {};
		dwFont->GetMetrics(&DWriteFontMetrics);

		FFontMetrics FontMetrics;
		FontMetrics.Size = FontSize;
		FontMetrics.Ascent = FontSize * DWriteFontMetrics.ascent / DWriteFontMetrics.designUnitsPerEm;
		FontMetrics.Descent = FontSize * DWriteFontMetrics.descent / DWriteFontMetrics.designUnitsPerEm;
		FontMetrics.LineSpace = FontSize * DWriteFontMetrics.lineGap / DWriteFontMetrics.designUnitsPerEm;
		FontMetrics.UnitsPerEm = DWriteFontMetrics.designUnitsPerEm;
		return FontMetrics;
	}

	FDWFontFace::FDWFontFace(FDWTextShaper & DWShaper, const FFontFace & FontFace, IDWriteFontRef dwFont)
		: IFontFace(FontFace, FontMetricsFromIDWriteFont(dwFont, 1.0f)), DWShaper(DWShaper), dwFont(dwFont)
	{
		FamilyNameW = FEncoding::ToWide(FontFace.FaceName);
		HRESULT Result = dwFont->CreateFontFace(dwFontFace.GetPP());
		AssertExpr(SUCCEEDED(Result));
		Result = dwFont->GetFontFamily(dwFontFamily.GetPP());
	}

	FString GetDWriteFontFamilyName(IDWriteFontRef DWriteFont)
	{
		HRESULT Result;

		IDWriteFontFamilyRef DWriteFontFamily;
		Result = DWriteFont->GetFontFamily(DWriteFontFamily.GetPP());

		TReferPtr<IDWriteLocalizedStrings> DWriteLocalizedStrings;
		Result = DWriteFontFamily->GetFamilyNames(DWriteLocalizedStrings.GetPP());

		uint32 StringLength;
		DWriteLocalizedStrings->GetStringLength(0, &StringLength);
		FWString FontFamilyName { StringLength };
		DWriteLocalizedStrings->GetString(0, FontFamilyName.Data, FontFamilyName.Length + 1);

		return FEncoding::ToUtf8(FontFamilyName);
	}

	FString GetDWriteFontFamilyName(IDWriteFontFamilyRef DWriteFontFamily)
	{
		return FEncoding::ToUtf8(GetDWriteFontFamilyNameW(DWriteFontFamily));
	}

	FWString GetDWriteFontFamilyNameW(IDWriteFontRef DWriteFont)
	{
		HRESULT Result;

		IDWriteFontFamilyRef DWriteFontFamily;
		Result = DWriteFont->GetFontFamily(DWriteFontFamily.GetPP());

		TReferPtr<IDWriteLocalizedStrings> DWriteLocalizedStrings;
		Result = DWriteFontFamily->GetFamilyNames(DWriteLocalizedStrings.GetPP());

		uint32 StringLength;
		DWriteLocalizedStrings->GetStringLength(0, &StringLength);
		FWString FontFamilyName { StringLength };
		DWriteLocalizedStrings->GetString(0, FontFamilyName.Data, FontFamilyName.Length + 1);

		return FontFamilyName;
	}

	FWString GetDWriteFontFamilyNameW(IDWriteFontFamilyRef DWriteFontFamily)
	{
		TReferPtr<IDWriteLocalizedStrings> DWriteLocalizedStrings;
		HRESULT Result = DWriteFontFamily->GetFamilyNames(DWriteLocalizedStrings.GetPP());

		uint32 StringLength;
		DWriteLocalizedStrings->GetStringLength(0, &StringLength);
		FWString FontFamilyName { StringLength };
		DWriteLocalizedStrings->GetString(0, FontFamilyName.Data, FontFamilyName.Length + 1);

		return FontFamilyName;
	}
}
