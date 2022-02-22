#include "PCH.h"
#include "DWFontFallbacker.h"

#include "DWTextShaper.h"

namespace Xin::DWrite
{
	FDWFontFallbacker::FDWFontFallbacker(FDWTextShaper & Shaper) : Shaper(Shaper)
	{

	}

	void FDWFontFallbacker::Initialize()
	{
		MappedFontFamilys.InsertOrAssign(EScriptCode(MakeFourCC("Arab")), Shaper.FindFontFamily(u8"Times New Roman"V));

		PreferredFontFamilys.Add(Shaper.FindFontFamily(u8"Source Han Sans SC"V));
		PreferredFontFamilys.Add(Shaper.FindFontFamily(u8"Source Han Serif SC"V));
	}

	//TTuple<uint32, FDWFontRef, float32> FDWFontFallback::MapCharacters(FDWTextAnalysisSource & AnalysisSource, uint32 TextIndex, uint32 TextLength, FDWFontFamily & DWFontFamily)
	//{
	//	// Step 1. check unicode existing in base font.
	//	auto [Unicode, Utf16Length] = FEncoding::Utf16ToUtf32((const char16 *)AnalysisSource.String.Data + TextIndex, TextLength);
	//	if (!Utf16Length)
	//		return { 0, nullptr, 0.0f };

	//}

	TTuple<uint32, FDWFontFaceRef, float32> FDWFontFallbacker::MapCharacters(
		FDWTextAnalysisSource & AnalysisSource, uint32 Utf16Index, uint32 Utf16Length, EScriptCode ScriptCode, 
		const FDWFontFace & BaseFontFace, float32 FontSize)
	{
		//Shaper.FetchFontDesingScriptCode(Shaper.FindDWriteFontFamily(L"Segoe UI Emoji"));

		DWRITE_FONT_STRETCH FontStretch = DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL;
		HRESULT Result;

		BOOL CharacterExists = FALSE;
		FDWFontFaceRef MappedFont;
		float32 MappedFontSize = 0.0f;
		bool FontDesignForScriptCode = false;

		auto [Char32, NumUtf16s] = FEncoding::Utf16ToUtf32((const char16 *)AnalysisSource.String.Data + Utf16Index, Utf16Length);
		if (!NumUtf16s)
			return { 0, nullptr, 0.0f };

		auto ForwardUnicodes = [&](IDWriteFontRef & DWriteFont)->uint32
		{
			uint32 MappedLength = NumUtf16s;
			// Forward to next unkonwn char or char exists in base font.
			while (MappedLength < Utf16Length)
			{
				uintx UnicodeTextIndex = Utf16Index + MappedLength;
				auto [NextUnicode, NextUtf16Length] = FEncoding::Utf16ToUtf32((const char16 *)AnalysisSource.String.Data + UnicodeTextIndex, uint32(Utf16Length - UnicodeTextIndex));
				if (!NextUtf16Length)
					break;

				if (0xEF00 <= NextUnicode && NextUnicode <= 0xFE0F)
					CharacterExists = true;
				else
				{
					// char exists in base font.
					Result = DWriteFont->HasCharacter(NextUnicode, &CharacterExists);
				}
				if (CharacterExists)
					MappedLength += NextUtf16Length;
				else
					break;
			}

			if (!MappedLength)
				return 0;

			FFontFace FallbackFontFace;
			FallbackFontFace.FaceName = GetDWriteFontFamilyName(DWriteFont);
			FallbackFontFace.FontWeight = BaseFontFace.FontFace.FontWeight;
			FallbackFontFace.FontStyle = BaseFontFace.FontFace.FontStyle;
			MappedFont = Shaper.FindFontFace(FallbackFontFace);

			// Keep base line
			//FallbackFontInfo.FontSize = Min(BaseDWFont->Ascent / MappedFont->Ascent, BaseDWFont->Descent / MappedFont->Descent) * BaseFont.FontSize;
			// Keep font height
			//MappedFontSize = BaseFont.FontSize / MappedFont->Height;
			// Keep font ascent
			//MappedFontSize = BaseDWFont->Ascent / MappedFont->Ascent * BaseFont.FontSize;
			// Keep font descent
			//MappedFontSize = BaseDWFont->Descent / MappedFont->Descent * BaseFont.FontSize;
			MappedFontSize = FontSize;
			return MappedLength;
		};

		if (Shaper.dwSystemFontFallback /*&& false*/)
		{
			UINT32 FallbackLength;

			IDWriteFontRef MappedDWriteFont;
			FLOAT Scale;
			Result = Shaper.dwSystemFontFallback->MapCharacters(
				&AnalysisSource, Utf16Index, Utf16Length,
				Shaper.dwSystemFontCollection, BaseFontFace.FamilyNameW.Data /*L"Courier New"*/,
				DWRITE_FONT_WEIGHT(BaseFontFace.FontFace.FontWeight), FromFontStyle(BaseFontFace.FontFace.FontStyle), FontStretch, &FallbackLength, MappedDWriteFont.GetPP(), &Scale);

			if (FAILED(Result) || !MappedDWriteFont)
			{
				Result = Shaper.dwSystemFontFallback->MapCharacters(
					&AnalysisSource, Utf16Index, Utf16Length,
					Shaper.dwSystemFontCollection, /*baseFamilyName*/nullptr,
					DWRITE_FONT_WEIGHT(BaseFontFace.FontFace.FontWeight), FromFontStyle(BaseFontFace.FontFace.FontStyle), FontStretch, &FallbackLength, MappedDWriteFont.GetPP(), &Scale);
				AssertExpr(!MappedDWriteFont);
			}

			if (MappedDWriteFont)
			{
				uint32 MappedLength = FallbackLength;
				FFontFace FallbackFontFace;
				FallbackFontFace.FaceName = GetDWriteFontFamilyName(MappedDWriteFont);
				FallbackFontFace.FontWeight = BaseFontFace.FontFace.FontWeight;
				FallbackFontFace.FontStyle = BaseFontFace.FontFace.FontStyle;
				MappedFont = Shaper.FindFontFace(FallbackFontFace);

				while (MappedLength < Utf16Length)
				{
					IDWriteFontRef NextMappedDWriteFont;
					Result = Shaper.dwSystemFontFallback->MapCharacters(
						&AnalysisSource, Utf16Index + MappedLength, Utf16Length - MappedLength,
						nullptr, MappedFont->FamilyNameW.Data,
						DWRITE_FONT_WEIGHT(BaseFontFace.FontFace.FontWeight), FromFontStyle(BaseFontFace.FontFace.FontStyle), FontStretch, &FallbackLength, NextMappedDWriteFont.GetPP(), &Scale);

					if (SUCCEEDED(Result) && NextMappedDWriteFont && GetDWriteFontFamilyName(NextMappedDWriteFont) == FallbackFontFace.FaceName)
						MappedLength += FallbackLength;
					else
						break;
				}

				return { MappedLength, MappedFont, FontSize };
			}

			//if (FallbackLength && MappedDWriteFont)
			//{
			//	HeadUtf16Length = FallbackLength;

			//	uint32 MappedLength = ForwardUnicodes(MappedDWriteFont);
			//	return { MappedLength, MappedFont, FontSize };
			//}

			// fallback by myself.
		}

		// return mapped length in char16
		IDWriteFontRef BackupDWriteFont;
		auto MappingUnicodes = [&](IDWriteFontFamily * DWriteFontFamily, bool ForceDesignFor)->uint32
		{
			if (!DWriteFontFamily)
				return 0;

			TReferPtr<IDWriteFontList> FontList;
			//FontList = FontFamily;
			Result = DWriteFontFamily->GetMatchingFonts(DWRITE_FONT_WEIGHT(BaseFontFace.FontFace.FontWeight), FontStretch, FromFontStyle(BaseFontFace.FontFace.FontStyle), FontList.GetPP());
			if (FAILED(Result))
				return 0;

			IDWriteFontRef MappedDWriteFont;
			for (uint32 FontIndex = 0; FontIndex < FontList->GetFontCount(); ++FontIndex)
			{
				IDWriteFontRef DWriteFont;
				FontList->GetFont(FontIndex, DWriteFont.GetPP());

				// we have already GetMatchingFonts.
				//if (DWriteFont->GetWeight() != DWRITE_FONT_WEIGHT(FontFace.FontInfo.FontWeight))
				//	continue;

				//if (DWriteFont->GetStyle() != FromFontStyle(FontFace.FontInfo.FontStyle))
				//	continue;

				//if (DWriteFont->GetStretch() != FontStretch)
				//	continue;

				//if (0xEF00 <= HeadUnicode && HeadUnicode <= 0xFE0F)
				//	CharacterExists = TRUE;
				//else
					Result = DWriteFont->HasCharacter(Char32, &CharacterExists);
				if (FAILED(Result) || !CharacterExists)
					continue;

				MappedDWriteFont = DWriteFont;
				TView<EScriptCode> DesignScriptCodes = Shaper.FetchFontDesingScriptCode(DWriteFont);
				for (uintx ScriptCodeIndex = 0; ScriptCodeIndex < DesignScriptCodes.Size; ++ScriptCodeIndex)
				{
					if (DesignScriptCodes[ScriptCodeIndex] == ScriptCode)
					{
						FontDesignForScriptCode = true;
						break;
					}
				}

				if (FontDesignForScriptCode)
					break;
			}

			if (!MappedDWriteFont)
				return 0;

			if (ForceDesignFor && !FontDesignForScriptCode)
			{
				if (!FontDesignForScriptCode)
				{
					BackupDWriteFont = MappedDWriteFont;
					return 0;
				}
			}

			return ForwardUnicodes(MappedDWriteFont);
		};

		// Step 1. test base font.
		{
			uint32 MappedLength = MappingUnicodes(BaseFontFace.dwFontFamily, false);
			if (MappedLength > 0)
				return { MappedLength, MappedFont, MappedFontSize };
		}

		// Step 2. test existing mapped font family.
		if (auto Iter = MappedFontFamilys.Find(ScriptCode); Iter != MappedFontFamilys.End())
		{
			uint32 MappedLength = MappingUnicodes(Iter->Second->dwFontFamily, true);
			if (MappedLength > 0)
				return { MappedLength, MappedFont, MappedFontSize };
		}

		// Step 3. test preferred mapped font family.
		for (const FDWFontFamilyRef DWFontFamily : PreferredFontFamilys)
		{
			uint32 MappedLength = MappingUnicodes(DWFontFamily->dwFontFamily, true);
			if (MappedLength > 0)
				return { MappedLength, MappedFont, MappedFontSize };
		}

		// Step 4. check unicode existing in system font collection.
		for (uint32 FontFamilyIndex = 0; FontFamilyIndex < Shaper.dwSystemFontCollection->GetFontFamilyCount(); ++FontFamilyIndex)
		{
			IDWriteFontFamilyRef DWriteFontFamily;
			Shaper.dwSystemFontCollection->GetFontFamily(FontFamilyIndex, DWriteFontFamily.GetPP());
			//Shaper.SystemDWFontCollection->GetFontFamily(Shaper.SystemDWFontCollection->GetFontFamilyCount() - 1 - FontFamilyIndex, DWriteFontFamily.GetPP());

			uint32 MappedLength = MappingUnicodes(DWriteFontFamily, true);
			if (MappedLength > 0)
			{
				MappedFontFamilys.Insert({ ScriptCode, Shaper.FindFontFamily(MappedFont->FontFace.FaceName)});
				return { MappedLength, MappedFont, MappedFontSize };
			}
		}

		// Step 5. test preferred mapped font family.
		for (const FDWFontFamilyRef DWFontFamily : PreferredFontFamilys)
		{
			uint32 MappedLength = MappingUnicodes(DWFontFamily->dwFontFamily, false);
			if (MappedLength > 0)
				return { MappedLength, MappedFont, MappedFontSize };
		}

		// Step 5. use backup font.
		if (BackupDWriteFont)
		{
			uint32 MappedLength = ForwardUnicodes(BackupDWriteFont);
			return { MappedLength, MappedFont, MappedFontSize };
		}

		return { 0, nullptr, 0.0f };
	}

	intx FDWFontFallbacker::MeasureFontPriority(char32 Unicode, IDWriteFont & DWriteFont) const
	{
		Shaper.FetchFontDesingScriptCode(&DWriteFont);

		BOOL CharacterExists;
		HRESULT Result = DWriteFont.HasCharacter(Unicode, &CharacterExists);
		if (FAILED(Result) || !CharacterExists)
			return 0;

		return 1;
	}
}
