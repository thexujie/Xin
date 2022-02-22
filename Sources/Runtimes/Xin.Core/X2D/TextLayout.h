#pragma once

#include "X2D.Types.h"

#include "Font.h"

namespace Xin
{
	constexpr float32 TextBlobSizeMax = 65536.0f * 4;

	struct FShapeRun
	{
		IFontFace & FontFace;
		float32 FontSize;

		uint32 ScriptIndex = 0;

		EScriptCode ScriptCode = EScriptCode::None;
		EBiDiLevel BiDiLevel = EBiDiLevel::LTR;

		SizeF Advance;

		uint32 TextIndex = 0;
		uint32 TextLength = 0;

		uint32 Utf16Index = 0;
		uint32 Utf16Length = 0;

		uint32 Utf32Index = 0;
		uint32 Utf32Length = 0;

		bool Rotate90 = false;
		bool Invisible = false;

		uint32 GlyphIndex = 0;
		uint32 GlyphLength = 0;

		uint32 ClusterIndex = 0;
		uint32 ClusterLength = 0;

#if XIN_DEBUG
		FUStringV String;
		FU32StringV U32String;
#endif
	};

	enum class EBreakCondition
	{
		None = 0,
		Soft,
		Hard,
	};

	struct FScriptCluster
	{
		uint32 RunIndex = 0;
		uint32 TextIndex = 0;
		uint32 TextLength = 0;
		uint32 GlyphIndex = 0;
		uint32 GlyphLength = 0;
		EBreakCondition BreakCondition = EBreakCondition::None;
		SizeF Advance;
#if XIN_DEBUG
		FUStringV String;
#endif
	};

	enum class ETextLayoutFlags
	{
		None = 0,
		DontJustifyAdvances = 0x0001,
	};

	struct FClusterRun
	{
		IFontFace & FontFace;
		float32 FontSize;

		PointF Position;
		SizeF Size;

		uint32 LineIndex = 0;

		uint32 TextIndex = 0;
		uint32 TextLength = 0;

		uint32 Utf16Index = 0;
		uint32 Utf16Length = 0;

		uint32 ClusterIndex = 0;
		uint32 ClusterLength = 0;

		uint32 GlyphIndex = 0;
		uint32 GlyphLength = 0;

		TView<uint16> GlyphIndices;
		TView<float32> GlyphAdvances;
		TView<PointF> GlyphOffsets;
		TView<PointF> GlyphPositions;

		EScriptCode ScriptCode = EScriptCode::None;
		EBiDiLevel BiDiLevel = EBiDiLevel::LTR;
		bool Rotate90;

#if XIN_DEBUG
		FString String;
		TView<FScriptCluster> Clusters;
#endif
	};

	class CORE_API ITextLayoutArea : public IX2DObject
	{
	public:
	};

	class CORE_API ITextLayoutRectArea : public ITextLayoutArea
	{
	public:
		virtual void ResetIter() const = 0;
		virtual RectF NextRect() const = 0;
	};

	class CORE_API FTextLayoutRectArea : public ITextLayoutRectArea
	{
	public:
		FTextLayoutRectArea(const RectF & Rect, float32 LineHeight) : Rect(Rect), LineHeight(LineHeight) {}

		void ResetIter() const override
		{
			LineIndex = 0;
		}

		RectF NextRect() const override
		{
			if (LineHeight * LineIndex >= Rect.Height)
				return RectF::Zero;

			RectF Result { Rect.X, Rect.Y + LineHeight * LineIndex, Rect.Width, LineHeight };
			++LineIndex;
			return Result;
		}

	public:
		RectF Rect;
		float32 LineHeight;
		mutable uint32 LineIndex = 0;
	};

	struct FLayoutLine
	{
		uint32 RunIndex = 0;
		uint32 RunLength = 0;

		uint32 TextIndex = 0;
		uint32 TextLength = 0;

		uint32 ClusterIndex = 0;
		uint32 ClusterLength = 0;

		float32 Ascent = 0.0f;
		float32 Descent = 0.0f;

		float32 Width = 0.0f;
		float32 Height = 0.0f;

#if XIN_DEBUG
		FUStringV String;
#endif
	};

	class CORE_API ITextShape : public ISharedRefer
	{
	public:
		ITextShape() = default;

		ITextShape(const FFontFace & FontFace, float32 FontSize, EReadingDirection ReadingDirection)
			: FontFace(FontFace), FontSize(FontSize), ReadingDirection(ReadingDirection) {}

		~ITextShape() = default;

	public:
		FFontFace FontFace;
		float32 FontSize = 0.0f;
		EReadingDirection ReadingDirection = EReadingDirection::Default;

	public:
		FU8String String;

		TList<uint16> GlyphIndices;
		TList<PointF> GlyphOffsets;
		TList<float32> GlyphAdvances;

		TList<FShapeRun> ShapeRuns;
		TList<FScriptCluster> ScriptClusters;
	};

	class CORE_API ITextBlob : public IX2DObject
	{
	public:
		ITextBlob() = default;
		~ITextBlob() = default;

	public:
		TList<FClusterRun> ClusterRuns;
		TList<FLayoutLine> LayoutLines;

		TList<PointF> GlyphPositions;

		SizeF LayoutSize = SizeF::NaN;
		SizeF FinalSize = SizeF::Zero;
	};

	using ITextBlobRef = TReferPtr<ITextBlob>;

	class CORE_API FTextRectLayouter
	{
	public:
		FTextRectLayouter(const ITextShape & TextShape, ITextBlob & TextBlob) : TextShape(TextShape), TextBlob(TextBlob) {}
		~FTextRectLayouter() = default;

		void Layout(const SizeF & LayoutSize, ETextAlignment TextAlignment = ETextAlignment::Baseline);
		void AddClusterRun(uint32 ClusterIndex, uint32 ClusterLength);

	public:
		const ITextShape & TextShape;
		ITextBlob & TextBlob;
	};
}
