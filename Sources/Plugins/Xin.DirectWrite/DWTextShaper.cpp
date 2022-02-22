#include "PCH.h"
#include "DWTextShaper.h"

#include "DWTextShape.h"

namespace Xin::DWrite
{
	FDWTextShaper::FDWTextShaper()
	{
		if (!dwLibrary.Load(u8"dwrite.dll"V))
			return;

		auto DWriteCreateFactoryProc = dwLibrary.GetProc<decltype(DWriteCreateFactory)>("DWriteCreateFactory");
		if (!DWriteCreateFactoryProc)
			return;

		HRESULT Result = DWriteCreateFactoryProc(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), dwFactory.GetTT<IUnknown>());
		if (FAILED(Result))
			return;

		Result = dwFactory->GetSystemFontCollection(dwSystemFontCollection.GetPP(), false);
		AssertExpr(SUCCEEDED(Result));

		dwFactory2 = dwFactory.QueryInterface<IDWriteFactory2>();
		dwFactory3 = dwFactory.QueryInterface<IDWriteFactory3>();
		if (dwFactory2)
		{
			Result = dwFactory2->GetSystemFontFallback(dwSystemFontFallback.GetPP());
			AssertExpr(SUCCEEDED(Result));
		}

		Result = dwFactory->CreateTextAnalyzer(dwTextAnalyzer.GetPP());
		dwTextAnalyzer1 = dwTextAnalyzer.QueryInterface<IDWriteTextAnalyzer1>();
		//Result = DWriteFactory3->GetGdiInterop(DWriteGdiInterop.GetPP());

		DefaultLocalName.Resize(LOCALE_NAME_MAX_LENGTH);
		int32 LocalNameLength = ::GetUserDefaultLocaleName(DefaultLocalName.Data, DefaultLocalName.Capacity);
		DefaultLocalName.SetSize(LocalNameLength > 0 ? LocalNameLength - 1 : 0);

		HDC hdcScreen = ::GetDC(NULL);
		int32 DpiY = GetDeviceCaps(hdcScreen, LOGPIXELSY);
		ReleaseDC(NULL, hdcScreen);
		DeviceDPI = (float32)DpiY / USER_DEFAULT_SCREEN_DPI;

		FontFallbacker.Initialize();
	}

	IFontFamilyRef FDWTextShaper::CreateFontFamily(FStringV FamilyName)
	{
		return FindFontFamily(FamilyName ? FamilyName : FStringV { FFontFace::Default.FaceName });
	}

	IFontFaceRef FDWTextShaper::CreateFontFace(const FFontFace & FontFace)
	{
		FFontFace RealFontFace = FontFace;
		if (!RealFontFace.FaceName)
			RealFontFace.FaceName = FFontFace::Default.FaceName;
		return FindFontFace(RealFontFace);
	}

	ITextBlobRef FDWTextShaper::CreateTextBlob(FStringV String, const FFontFace & FontFace, float32 FontSize, const SizeF & LayoutSize, ETextLayoutFlags LayoutFlags)
	{
		TReferPtr<FDWTextBlob> DWTextLayout = new FDWTextBlob(*this);
		DWTextLayout->Shape(String, FontFace, FontSize, EReadingDirection::Default);
		DWTextLayout->Layout(LayoutSize, LayoutFlags);
		return DWTextLayout;
	}

	EScriptCode FDWTextShaper::FindIsoScriptCode(UINT16 DWriteScript)
	{
		if (auto Iter = MappedScriptCodes.Find(DWriteScript); Iter != MappedScriptCodes.End())
			return Iter->Second;

		DWRITE_SCRIPT_ANALYSIS ScriptAnalysis {DWriteScript, { DWRITE_SCRIPT_SHAPES_DEFAULT }};
		DWRITE_SCRIPT_PROPERTIES ScriptProperties;
		HRESULT Result = dwTextAnalyzer1->GetScriptProperties(ScriptAnalysis, &ScriptProperties);
		if (SUCCEEDED(Result))
		{
			EScriptCode ScriptCode = EScriptCode(BytesReverse(ScriptProperties.isoScriptCode));
			MappedScriptCodes[DWriteScript] = ScriptCode;
			return ScriptCode;
		}
		MappedScriptCodes[DWriteScript] = EScriptCode::None;
		return EScriptCode::None;
	}

	TView<EScriptCode> FDWTextShaper::FetchFontDesingScriptCode(IDWriteFontFamilyRef DWriteFontFamily)
	{
		FWString FamilyName = GetDWriteFontFamilyNameW(DWriteFontFamily);

		if (auto Iter = FontFamilyDesignScriptCodes.Find(FamilyName); Iter != FontFamilyDesignScriptCodes.End())
			return Iter->Second;

		TList<EScriptCode> ScriptCodes;
		for (uint32 FontIndex = 0; FontIndex < DWriteFontFamily->GetFontCount(); ++FontIndex)
		{
			IDWriteFontRef DWriteFont;
			DWriteFontFamily->GetFont(FontIndex, DWriteFont.GetPP());

			BOOL LanguageTagExists;
			TReferPtr<IDWriteLocalizedStrings> LanguageTagLocalizedStrings;
			HRESULT Result = DWriteFont->GetInformationalStrings(DWRITE_INFORMATIONAL_STRING_DESIGN_SCRIPT_LANGUAGE_TAG, LanguageTagLocalizedStrings.GetPP(), &LanguageTagExists);
			if (SUCCEEDED(Result) && LanguageTagExists && LanguageTagLocalizedStrings)
			{
				for (int32 StringIndex = 0; StringIndex < LanguageTagLocalizedStrings->GetCount(); ++StringIndex)
				{
					uint32 StringLength;
					LanguageTagLocalizedStrings->GetStringLength(StringIndex, &StringLength);
					FWString LanguageTag { StringLength };
					LanguageTagLocalizedStrings->GetString(StringIndex, LanguageTag.Data, LanguageTag.Capacity);

					FWStringV LanguageTagV = LanguageTag;
					if (LanguageTag[0] == '\'')
					{
						AssertExpr(LanguageTag[LanguageTagV.Size - 1] == '\'');
						LanguageTagV = LanguageTag.View(0, LanguageTagV.Size - 2);
					}

					if (LanguageTagV.Size > 4)
					{
						FString FamilyNameU = GetDWriteFontFamilyName(DWriteFontFamily);
						LogInfo(u8"Font {} has unormal language tag {}"V, FamilyNameU, FEncoding::ToUtf8(LanguageTagV));
					}

					uint32 FontIsoScriptCode = MakeFourCC(LanguageTagV[0], LanguageTagV[1], LanguageTagV[2], LanguageTagV[3]);
					ScriptCodes.AddUnique(EScriptCode(FontIsoScriptCode));
					//break;
				}
			}
		}

		return FontFamilyDesignScriptCodes.Insert({ FamilyName, ScriptCodes }).First->Second;
	}

	TView<EScriptCode> FDWTextShaper::FetchFontDesingScriptCode(IDWriteFontRef DWriteFont)
	{
		FWString FamilyName = GetDWriteFontFamilyNameW(DWriteFont);

		if (auto Iter = FontDesignScriptCodes.Find(FamilyName); Iter != FontDesignScriptCodes.End())
			return Iter->Second;


		TList<EScriptCode> ScriptCodes;
		{
			// DWRITE_INFORMATIONAL_STRING_SUPPORTED_SCRIPT_LANGUAGE_TAG
			BOOL LanguageTagExists;
			TReferPtr<IDWriteLocalizedStrings> LanguageTagLocalizedStrings;
			HRESULT Result = DWriteFont->GetInformationalStrings(DWRITE_INFORMATIONAL_STRING_SUPPORTED_SCRIPT_LANGUAGE_TAG, LanguageTagLocalizedStrings.GetPP(), &LanguageTagExists);
			if (SUCCEEDED(Result) && LanguageTagExists && LanguageTagLocalizedStrings)
			{
				for (int32 StringIndex = 0; StringIndex < LanguageTagLocalizedStrings->GetCount(); ++StringIndex)
				{
					uint32 StringLength;
					LanguageTagLocalizedStrings->GetStringLength(StringIndex, &StringLength);
					FWString LanguageTag { StringLength };
					LanguageTagLocalizedStrings->GetString(StringIndex, LanguageTag.Data, LanguageTag.Capacity);

					FWStringV LanguageTagV = LanguageTag;
					if (LanguageTag[0] == '\'')
					{
						AssertExpr(LanguageTag[LanguageTagV.Size - 1] == '\'');
						LanguageTagV = LanguageTag.View(0, LanguageTagV.Size - 2);
					}

					if (LanguageTagV.Size > 4)
					{
						FStringV FamilyNameU = FEncoding::ToUtf8(FamilyName);
						LogInfo(u8"Font {} has unormal language tag {}"V, FamilyNameU, FEncoding::ToUtf8(LanguageTagV));
					}

					uint32 FontIsoScriptCode = MakeFourCC(LanguageTagV[0], LanguageTagV[1], LanguageTagV[2], LanguageTagV[3]);
					ScriptCodes.Emplace(EScriptCode(FontIsoScriptCode));
					//break;
				}
			}
		}

		if (ScriptCodes.Size)
			return FontDesignScriptCodes.Insert({ FamilyName, ScriptCodes }).First->Second;
		else
		{
			FontDesignScriptCodes[FamilyName] = TView<EScriptCode>::None;
			return TView<EScriptCode>::None;
		}
	}

	IDWriteFontFamilyRef FDWTextShaper::FindDWriteFontFamily(FWStringV FamilyName)
	{
		if (!FamilyName)
			return nullptr;

		if (auto Iter = dwFamilies.Find(FamilyName); Iter != dwFamilies.End())
			return Iter->Second;

		BOOL Exists;
		uint32 FoundIndex;
		HRESULT Result = dwSystemFontCollection->FindFamilyName(FamilyName.Data, &FoundIndex, &Exists);

		IDWriteFontFamilyRef DWriteFontFamily;
		if (Succeeded(Result) && FoundIndex != NullIndex)
			Result = dwSystemFontCollection->GetFontFamily(FoundIndex, DWriteFontFamily.GetPP());

		dwFamilies[FamilyName] = DWriteFontFamily;
		return DWriteFontFamily;
	}

	IDWriteFontRef FDWTextShaper::FindDWriteFont(const FFontFace & FontFace)
	{
		auto Iter = dwFonts.Find(FontFace);
		if (Iter != dwFonts.End())
			return Iter->Second;

		IDWriteFontRef DWriteFont;
		FDWFontFamilyRef DWFontFamily = FindFontFamily(FontFace.FaceName);
		if (DWFontFamily)
		{
			HRESULT Result = DWFontFamily->dwFontFamily->GetFirstMatchingFont(
				(DWRITE_FONT_WEIGHT)FontFace.FontWeight,
				DWRITE_FONT_STRETCH_NORMAL,
				FromFontStyle(FontFace.FontStyle),
				DWriteFont.GetPP()
			);
		}
		dwFonts[FontFace] = DWriteFont;
		return DWriteFont;
	}

	IDWriteTextFormat * FDWTextShaper::FindDWriteTextFormat(const FTextFormat & TextFormat) const
	{
		FTextFormat RealTextFormat = TextFormat;
		if (!RealTextFormat.FamilyName)
			RealTextFormat.FamilyName = FFontFace::Default.FaceName;

		auto Iter = dwTextFormats.Find(RealTextFormat);
		if (Iter != dwTextFormats.End())
			return Iter->Second.Get();

		IDWriteTextFormatRef DWriteTextFormat;

		FWString FontNameW = FEncoding::ToWide(RealTextFormat.FamilyName);
		IDWriteFontCollection * DWriteFontCollection = nullptr;
		//HRESULT hr = m_pDWrite->GetSystemFontCollection(&DWriteFontCollection);
		//uint_32 uiIndex = 0;
		//BOOL bExist = FALSE;
		//pFontCollection->FindFamilyName(chbName, &uiIndex, &bExist);
		HRESULT Result = dwFactory->CreateTextFormat(FontNameW.Data, DWriteFontCollection,
			(DWRITE_FONT_WEIGHT)TextFormat.FontWeight,
			TextFormat.FontStyle & EFontStyle::Italic ? DWRITE_FONT_STYLE_ITALIC : DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_ULTRA_CONDENSED,
			(float32)TextFormat.FontSize,
			L"",
			DWriteTextFormat.GetPP());

		if (TextFormat.TextAlignment & ETextAlignment::Right)
			DWriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
		else if (TextFormat.TextAlignment & ETextAlignment::CenterX)
			DWriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		else
			DWriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);

		//if (TextFormat.TextAlignment & ETextAlignment::Bottom)
		//	DWriteTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
		//else if (TextFormat.TextAlignment & ETextAlignment::CenterY)
		//	DWriteTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		//else
		//	DWriteTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

		//IDWriteInlineObject * DWriteInlineObject = nullptr;
		//DWRITE_TRIMMING DwriteTrimming = {};
		//switch (format.trimming)
		//{
		//case TextTrimmingChar:
		//	DwriteTrimming.granularity = DWRITE_TRIMMING_GRANULARITY_CHARACTER;
		//	break;
		//case TextTrimmingCharEllipsis:
		//	DwriteTrimming.granularity = DWRITE_TRIMMING_GRANULARITY_CHARACTER;
		//	m_pDWrite->CreateEllipsisTrimmingSign(pFormat, &DWriteInlineObject);
		//	break;
		//case TextTrimmingWord:
		//	DwriteTrimming.granularity = DWRITE_TRIMMING_GRANULARITY_WORD;
		//	break;
		//case TextTrimmingWordEllipsis:
		//	DwriteTrimming.granularity = DWRITE_TRIMMING_GRANULARITY_WORD;
		//	m_pDWrite->CreateEllipsisTrimmingSign(pFormat, &DWriteInlineObject);
		//	break;
		//case TextTrimmingPathEllipsis:
		//	DwriteTrimming.granularity = DWRITE_TRIMMING_GRANULARITY_CHARACTER;
		//	DwriteTrimming.delimiter = L'/';
		//	m_pDWrite->CreateEllipsisTrimmingSign(pFormat, &DWriteInlineObject);
		//	break;
		//default:
		//	break;
		//}
		//DWriteTextFormat->SetTrimming(&DwriteTrimming, DWriteInlineObject);
		//DWriteTextFormat->SetWordWrapping(GetDwTextWrapping(format.wrap));
		dwTextFormats[TextFormat] = DWriteTextFormat;
		return DWriteTextFormat.Get();
	}

	FDWFontFamilyRef FDWTextShaper::FindFontFamily(FStringV FamilyName)
	{
		auto Iter = FontFamilys.Find(FamilyName);
		if (Iter != FontFamilys.End())
			return Iter->Second;

		FStringV RealFamilyName = FamilyName.Size ? FamilyName : FStringV { FFontFace::Default.FaceName };
		FWString FamilyNameW = FEncoding::ToWide(RealFamilyName);
		IDWriteFontFamilyRef DWriteFontFamily = FindDWriteFontFamily(FamilyNameW);
		FDWFontFamilyRef FontFamily = new FDWFontFamily(*this, RealFamilyName, FamilyNameW, DWriteFontFamily.Get());
		FontFamilys[FamilyName] = FontFamily;
		return FontFamily;
	}

	FDWFontFaceRef FDWTextShaper::FindFontFace(const FFontFace & FontFace)
	{
		auto Iter = FontFaces.Find(FontFace);
		if (Iter != FontFaces.End())
			return Iter->Second;

		FFontFace RealFontFace = FontFace;
		if (RealFontFace.FaceName.Empty())
			RealFontFace.FaceName = FFontFace::Default.FaceName;

		FDWFontFaceRef DWFont = new FDWFontFace(*this, RealFontFace, FindDWriteFont(RealFontFace));
		FontFaces[FontFace] = DWFont;
		return DWFont;
	}
}
