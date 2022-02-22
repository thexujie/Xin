#pragma once

#include "DWTypes.h"
#include "DWFontFace.h"
#include "DWTextAnalysisSource.h"

namespace Xin::DWrite
{
	class FDWFontFace;

	struct FDWFontFallbackString
	{
		FDWTextAnalysisSource & AnalysisSource;
		uint32 TextIndex;
		uint32 TextLength;
	};

	/**
	 * @link: https://learn.microsoft.com/en-us/dotnet/api/system.windows.media.fontfamily?view=windowsdesktop-8.0#composite-fonts
	 * @link: https://www.unicode.org/mail-arch/unicode-ml/y2013-m07/0068.html
	 *     DirectWrite has fallback data that is not read from the registry or in any way configurable.
	 *     In Windows 8.1, though, APIs have been introduced that allow an app to specify its own fallback.
	 *     (It's similar to the WPF APIs for creating a composite font definition.)
	 */
	class DWRITE_API FDWFontFallbacker : public ISharedRefer
	{
	public:
        FDWFontFallbacker(FDWTextShaper & Shaper);
		virtual ~FDWFontFallbacker() = default;

		void Initialize();

	public:
		TTuple<uint32, FDWFontFaceRef, float32> MapCharacters(
			FDWTextAnalysisSource & AnalysisSource, uint32 Utf16Index, uint32 Utf16Length, EScriptCode ScriptCode, 
			const FDWFontFace & BaseFontFace, float32 FontSize);

	private:
		intx MeasureFontPriority(char32 Unicode, IDWriteFont & DWriteFont) const;

	public:
        FDWTextShaper & Shaper;
		TMap<EScriptCode, FDWFontFamilyRef> MappedFontFamilys;

		TList<FDWFontFamilyRef> PreferredFontFamilys;
	};
    using FDWFontFallbackRef = TReferPtr<FDWFontFallbacker>;
}
