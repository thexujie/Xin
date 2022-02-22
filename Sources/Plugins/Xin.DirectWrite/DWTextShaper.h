#pragma once

#include "DWTypes.h"
#include "DWFontFace.h"
#include "DWFontFallbacker.h"
#include "DWTextShape.h"

namespace Xin::DWrite
{
	class DWRITE_API FDWTextShaper : public ITextShaper
	{
	public:
		FDWTextShaper();

	public:
		IFontFamilyRef CreateFontFamily(FStringV FamilyName) override;
		IFontFaceRef CreateFontFace(const FFontFace & FontFace) override;

		ITextBlobRef CreateTextBlob(FStringV String, const FFontFace & FontFace, float32 FontSize, const SizeF & LayoutSize, ETextLayoutFlags LayoutFlags = ETextLayoutFlags::None) override;

	public:
		EScriptCode FindIsoScriptCode(UINT16 DWriteScript);
		TView<EScriptCode> FetchFontDesingScriptCode(IDWriteFontFamilyRef DWriteFontFamily);
		TView<EScriptCode> FetchFontDesingScriptCode(IDWriteFontRef DWriteFont);

		IDWriteFontFamilyRef FindDWriteFontFamily(FWStringV FamilyName);
		IDWriteFontRef FindDWriteFont(const FFontFace & FontFace);
		IDWriteTextFormat * FindDWriteTextFormat(const FTextFormat & TextFormat) const;

		FDWFontFamilyRef FindFontFamily(FStringV FamilyName);
		FDWFontFaceRef FindFontFace(const FFontFace & FontFace);

	public:
		FModule dwLibrary;
		IDWriteFactoryRef dwFactory;
		IDWriteFactory2Ref dwFactory2;
		IDWriteFactory2Ref dwFactory3;

		IDWriteFontCollectionRef dwSystemFontCollection;
		IDWriteFontFallbackRef dwSystemFontFallback;

		IDWriteTextAnalyzerRef dwTextAnalyzer;
		IDWriteTextAnalyzer1Ref dwTextAnalyzer1;
		IDWriteTextAnalyzer2Ref dwTextAnalyzer2;

		FDWFontFallbacker FontFallbacker { *this };

		//IDWriteGdiInteropRef DWriteGdiInterop;

		TMap<UINT16, EScriptCode> MappedScriptCodes;
		TMap<FWString, TList<EScriptCode>> FontFamilyDesignScriptCodes;
		TMap<FWString, TList<EScriptCode>> FontDesignScriptCodes;

		TMap<FWString, IDWriteFontFamilyRef> dwFamilies;
		TMap<FFontFace, IDWriteFontRef> dwFonts;

		mutable TMap<FTextFormat, IDWriteTextFormatRef> dwTextFormats;

		TMap<FString, FDWFontFamilyRef> FontFamilys;
		TMap<FFontFace, FDWFontFaceRef> FontFaces;

		FWString DefaultLocalName;

	public:
		float32 DeviceDPI = 1.0f;
	};
}
