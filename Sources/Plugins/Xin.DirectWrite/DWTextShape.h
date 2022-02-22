#pragma once

#include "DWTypes.h"
#include "DWFontFace.h"
#include "DWTextAnalysisSource.h"
#include "DWTextRun.h"

namespace Xin::DWrite
{
	class DWRITE_API FDWTextShape : public ITextShape
	{
		static_assert(sizeof(DWRITE_GLYPH_OFFSET) == sizeof(PointF));

	public:
		FDWTextShape(FDWTextShaper & Shaper);

		void Shape(FUStringV String, const FFontFace & FontFace, float32 FontSize, EReadingDirection ReadingDirection);

		void Itemize();

	public:
		FDWTextShaper & Shaper;

	public:
		FU16String U16String;
		TList<uint32> Utf8IndicesOfUtf16s;
		TList<uint16> Utf16Clusters;

	public:
		TList<FDWScriptRun> ScriptRuns;
		TList<DWRITE_LINE_BREAKPOINT> TextBreakpoints;
	};

	using FDWTextShapeRef = TReferPtr<FDWTextShape>;

	class DWRITE_API FDWTextBlob_Old : public ITextBlob
	{
	public:
		FDWTextBlob_Old(FDWTextShaper & Shaper);

	public:
		void Shape(FUStringV String, const FFontFace & FontFace, float32 FontSize, EReadingDirection ReadingDirection);
		void Layout(const SizeF & LayoutSize, ETextLayoutFlags LayoutFlags = ETextLayoutFlags::None);

	public:
		void AddGlyphSpan(
			uint32 Utf16Index, uint32 Utf16Length, PointF Position,
			float32 Width, TView<uint16> GlyphIndices, TView<float32> GlyphAdvances,
			TView<PointF> GlyphOffsets,
			IFontFace & Font, float32 FontSize,
			EBiDiLevel BiDiLevel, bool Rotate90);

	public:
		FDWTextShaper & Shaper;
		FDWTextShape TextShape { Shaper };

	public:
		TList<float32> GlyphAdvancesJustified;
	};

	class DWRITE_API FDWTextBlob: public ITextBlob
	{
	public:
		FDWTextBlob(FDWTextShaper & Shaper);

	public:
		void Shape(FUStringV String, const FFontFace & FontFace, float32 FontSize, EReadingDirection ReadingDirection);
		void Layout(const SizeF & LayoutSize, ETextLayoutFlags LayoutFlags = ETextLayoutFlags::None);

	public:
		FDWTextShaper & Shaper;
		FDWTextShape TextShape { Shaper };

	public:
		TList<float32> GlyphAdvancesJustified;
	};
}
