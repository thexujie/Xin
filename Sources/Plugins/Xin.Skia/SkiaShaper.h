#pragma once

#include "SkiaTypes.h"

#include <hb.h>
#include <hb-ot.h>
#include <hb-icu.h>
#include <unicode/brkiter.h>
#include <unicode/locid.h>
#include <unicode/ubidi.h>
#include <unicode/uchriter.h>


#define XIN_SKIA_DEBUG XIN_DEBUG

namespace Xin::Skia
{
	class FSkiaShaper;

	class SKIA_API FSkiaFontFace : public IFontFace
	{
	public:
		FSkiaFontFace(const FFontFace & FontFace, SkFontStyle skFontStyle, SkTypeface * skTypeface);

	public:
		SkFontStyle skFontStyle = { };
		sk_sp<SkTypeface> skTypeface;
		TScopedPtr<hb_font_t, hb_font_destroy> hbFont;
	};
	using FSkiaFontFaceRef = TReferPtr<FSkiaFontFace>;

	struct FSkiaScriptRun
	{
		uint32 TextIndex = 0;
		uint32 TextLength = 0;

		uint32 Utf32Index = 0;
		uint32 Utf32Length = 0;

		EScriptCode ScriptCode = EScriptCode::None;
		EBiDiLevel BiDiLevel = EBiDiLevel::LTR;

#if XIN_SKIA_DEBUG
		FUStringV String;
		FU32StringV U32String;
#endif
	};

	class SKIA_API FSkiaFontFallbacker
	{
	public:
		FSkiaFontFallbacker(FSkiaShaper & Shaper);

		sk_sp<SkTypeface> MapCharacter(char32 Utf32Char, EScriptCode ScriptCode, const FSkiaFontFace & BaseFontFace);

	public:
		FSkiaShaper & Shaper;

		TMap<EScriptCode, FString> PrefferedFontFaces;
	};

	class SKIA_API FSkiaTextShape : public ITextShape
	{
	public:
		FSkiaTextShape(FSkiaShaper & Shaper) : Shaper(Shaper) {}

		void Shape(FUStringV String, const FFontFace & FontFace, float32 FontSize, EReadingDirection ReadingDirection);

	private:
		bool Itemize();

	public:
		FSkiaShaper & Shaper;

		FU16String U16String;
		FU32String U32String;
		TList<uint8> CodePointLength;


		// 0 ltr 1 rtl 0xfe auto but preffer ltf 0xff auto but preffer rtl
		uint8 DefaultBidiLevel = UBIDI_DEFAULT_LTR ;

		TList<FSkiaScriptRun> ScriptRuns;

		TList<uint32> GlyphClusters;
	};

	class SKIA_API FSkiaShaper : public ITextShaper
	{
	public:
		FSkiaShaper(FSkiaGraphics & SkiaGraphics);
		~FSkiaShaper();

	public:
		IFontFamilyRef CreateFontFamily(FStringV FamilyName) override;
		IFontFaceRef CreateFontFace(const FFontFace & FontFace) override;
		ITextBlobRef CreateTextBlob(FStringV String, const FFontFace & FontFace, float32 FontSize, const SizeF & LayoutSize, ETextLayoutFlags LayoutFlags) override;

	public:
		FSkiaFontFace * FetchFontFace(const FFontFace & FontFace);
		FSkiaFontFace * FetchFontFace(SkTypeface * skTypeface);

		TMap<FFontFace, FSkiaFontFaceRef> FontFaces;

	public:
		icu::Locale DefaultLocal;
		FSkiaFontFallbacker FontFallbacker { *this };

		// https://vimerzhao.top/articles/review-skia-upgrade-m104-to-m122/
		sk_sp<SkFontMgr> skDefaultFontMgr;
	};

	class SKIA_API FSkiaTextBlob: public ITextBlob
	{
	public:
		FSkiaTextBlob(FSkiaShaper & Shaper);

	public:
		void Shape(FUStringV String, const FFontFace & FontFace, float32 FontSize, EReadingDirection ReadingDirection);
		void Layout(const SizeF & LayoutSize, ETextLayoutFlags LayoutFlags = ETextLayoutFlags::None);

	public:
		FSkiaShaper & Shaper;
		FSkiaTextShape TextShape { Shaper };

	public:
		TList<float32> GlyphAdvancesJustified;
	};
}
