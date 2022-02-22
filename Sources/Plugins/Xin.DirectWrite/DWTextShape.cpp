#include "PCH.h"
#include "DWTextShape.h"
#include "DWTextShaper.h"
#include "DWTextAnalyzer.h"
#include "DWTextLayouter.h"

namespace Xin::DWrite
{
	static uint32 EstimateGlyphCount(uint32 TextLength)
	{
		return 3 * TextLength / 2 + 16;
	}

	FDWTextShape::FDWTextShape(FDWTextShaper & Shaper)
		: Shaper(Shaper) {}


	void FDWTextShape::Shape(FUStringV String, const FFontFace & FontFace, float32 FontSize, EReadingDirection ReadingDirection)
	{
		this->String = String;
		this->FontFace = FontFace;
		this->FontSize = FontSize;
		this->ReadingDirection = ReadingDirection;

		Itemize();
	}

	void FDWTextShape::Itemize()
	{
		if (String.Length == 0)
			return;

		TList<uint8> NumUtf8sOfUtf16s;
		TList<uint8> NumUtf8sOfUtf32s;
		TList<uint32> Utf8IndicesOfUtf32s;
		TList<uint32> Utf32IndicesOfUtf8s;

		FU32String U32String;

		U16String.Reserve(String.Length / 2 + 1);
		U32String.Reserve(String.Length / 2 + 1);

		{
			uint32 Utf8Index = 0;
			while (Utf8Index < uint32(String.Length))
			{
				char32 Utf32;
				uint32 NumUtf8sOfUtf32 = FEncoding::Utf8ToUtf32(String.Data + Utf8Index, uint32(String.Length - Utf8Index), Utf32);
				AssertExpr(NumUtf8sOfUtf32 > 0);

				U32String.Append(Utf32);
				NumUtf8sOfUtf32s.Add(uint8(NumUtf8sOfUtf32));
				Utf8IndicesOfUtf32s.Add(Utf8Index);

				Utf32IndicesOfUtf8s.Add(uint32(U32String.Length - 1));
				Utf32IndicesOfUtf8s.AddN(0, NumUtf8sOfUtf32 - 1);

				char16 Utf16[2];
				size_t NumUtf16sOfUtf32 = FEncoding::Utf32ToUtf16(Utf32, Utf16, 2);
				AssertExpr(NumUtf16sOfUtf32 > 0);
				U16String.Append({ Utf16, NumUtf16sOfUtf32 });
				Utf8IndicesOfUtf16s.Add(Utf8Index);
				Utf8IndicesOfUtf16s.AddN(0, NumUtf16sOfUtf32 - 1);

				NumUtf8sOfUtf16s.Add(uint8(NumUtf16sOfUtf32));
				NumUtf8sOfUtf16s.AddN(0, NumUtf16sOfUtf32 - 1);

				Utf8Index += NumUtf8sOfUtf32;
			}

			Utf8IndicesOfUtf16s.Add(uint32(String.Length));
			Utf8IndicesOfUtf32s.Add(uint32(String.Length));
			Utf32IndicesOfUtf8s.Add(uint32(U32String.Length));
		}

#if XIN_DEBUG
		FWString WString = FEncoding::ToWide(String);
		AssertExpr(U16String.Length == WString.Length);
		for (uintx WideIndex = 0; WideIndex < WString.Length; ++WideIndex)
		{
			AssertExpr(U16String[WideIndex] == WString[WideIndex]);
		}
#endif

		FDWTextAnalysisSource AnalysisSource { Shaper, U16String, ReadingDirection };
		// 1. String -> Run(s)
		FDWTextAnalyzer TextAnalyzer { Shaper, AnalysisSource, ScriptRuns, TextBreakpoints };
		TextAnalyzer.Analyze();

		// 2. Font fallback
		FDWFontFace * BaseFontFace = Shaper.FindFontFace(FontFace);

		uint32 ScriptRunIndex = 0;
		while (ScriptRunIndex != NullIndex)
		{
			FDWScriptRun & ScriptRun = ScriptRuns[ScriptRunIndex];

			if (ScriptRun.ScriptAnalysis.shapes == DWRITE_SCRIPT_SHAPES_NO_VISUAL)
			{
				FShapeRun & ShapeRun = ShapeRuns.Emplace(*BaseFontFace, FontSize, ScriptRunIndex);

				ShapeRun.TextIndex = Utf8IndicesOfUtf16s[ScriptRun.Utf16Index];
				ShapeRun.TextLength = Utf8IndicesOfUtf16s[ScriptRun.Utf16Index + ScriptRun.Utf16Length] - ShapeRun.TextIndex;

				ShapeRun.Utf16Index = ScriptRun.Utf16Index;
				ShapeRun.Utf16Length = ScriptRun.Utf16Length;

				ShapeRun.ScriptCode = ScriptRun.ScriptCode;
				ShapeRun.BiDiLevel = ScriptRun.BiDiLevel;
				ShapeRun.Rotate90 = false;
				ShapeRun.Invisible = true;
#if XIN_DWRITE_DEBUG
				ShapeRun.String = String.View(ShapeRun.TextIndex, ShapeRun.TextLength);
#endif
				ScriptRunIndex = ScriptRun.NextRunIndex;
				continue;
			}

			uint32 Utf16Index = ScriptRun.Utf16Index;
			while (Utf16Index < ScriptRun.Utf16Index + ScriptRun.Utf16Length)
			{
				uint32 UnmapedUtf16Length = ScriptRun.Utf16Index + ScriptRun.Utf16Length - Utf16Index;
				auto [MappedUtf16Length, MappedFont, MappedFontSize] = Shaper.FontFallbacker.MapCharacters(AnalysisSource, Utf16Index, UnmapedUtf16Length, ScriptRun.ScriptCode, *BaseFontFace, FontSize);
				AssertExpr(MappedUtf16Length <= ScriptRun.Utf16Length);

				// fallback font failed, we skip whole analyzed run.
				if (!MappedUtf16Length || !MappedFont)
				{
					FShapeRun & ShapeRun = ShapeRuns.Emplace(*BaseFontFace, FontSize, ScriptRunIndex);

					ShapeRun.TextIndex = Utf8IndicesOfUtf16s[Utf16Index];
					ShapeRun.TextLength = Utf8IndicesOfUtf16s[Utf16Index + UnmapedUtf16Length] - ShapeRun.TextIndex;
					ShapeRun.Utf16Index = Utf16Index;
					ShapeRun.Utf16Length = UnmapedUtf16Length;
					ShapeRun.ScriptCode = ScriptRun.ScriptCode;
					ShapeRun.BiDiLevel = ScriptRun.BiDiLevel;
					ShapeRun.Rotate90 = false;
					ShapeRun.Invisible = true;
#if XIN_DWRITE_DEBUG
					ShapeRun.String = String.View(ShapeRun.TextIndex, ShapeRun.TextLength);
#endif
					break;
				}

				// use fallback font for MappedLength utf16(s).
				{
					FShapeRun & ShapeRun = ShapeRuns.Emplace(*MappedFont, MappedFontSize, ScriptRunIndex);
					ShapeRun.TextIndex = Utf8IndicesOfUtf16s[Utf16Index];
					ShapeRun.TextLength = Utf8IndicesOfUtf16s[Utf16Index + MappedUtf16Length] - ShapeRun.TextIndex;
					ShapeRun.Utf16Index = Utf16Index;
					ShapeRun.Utf16Length = MappedUtf16Length;
					ShapeRun.ScriptCode = ScriptRun.ScriptCode;
					ShapeRun.BiDiLevel = ScriptRun.BiDiLevel;
					ShapeRun.Rotate90 = false;

#if XIN_DWRITE_DEBUG
					ShapeRun.String = String.View(ShapeRun.TextIndex, ShapeRun.TextLength);
#endif
					Utf16Index += MappedUtf16Length;
				}
			}

			ScriptRunIndex = ScriptRun.NextRunIndex;
		}

		// 3. Glyph(s)
		TList<DWRITE_SHAPING_GLYPH_PROPERTIES> GlyphProperties;
		Utf16Clusters.ResizeTo(U16String.Length, EInitializeMode::Zero);

		uint32 TotalGlyphCountMax = EstimateGlyphCount(uint32(AnalysisSource.String.Size));
		GlyphIndices.ReserveTo(TotalGlyphCountMax);
		GlyphProperties.ReserveTo(TotalGlyphCountMax);
		ScriptClusters.ReserveTo(String.Length + 1);

		for (uintx RunIndex = 0; RunIndex < ShapeRuns.Size; ++RunIndex)
		{
			FShapeRun & ShapeRun = ShapeRuns[RunIndex];
			FDWScriptRun & ScriptRun = ScriptRuns[ShapeRun.ScriptIndex];

			ShapeRun.GlyphIndex = GlyphIndices.Size;
			ShapeRun.GlyphLength = 0;

			if (ShapeRun.Invisible)
			{
				if (ShapeRun.Utf16Index > 0)
				{
					for (uint32 Utf16Index = ShapeRun.Utf16Index; Utf16Index < ShapeRun.Utf16Index + ShapeRun.Utf16Length; ++Utf16Index)
					{
						Utf16Clusters[Utf16Index] += uint16(ShapeRun.Utf16Index);
					}
				}
				continue;
			}

			TList<DWRITE_SHAPING_TEXT_PROPERTIES> TextProperties { ShapeRun.TextLength };

			uint32 MaxGlyphs = EstimateGlyphCount(ShapeRun.TextLength);
			uint32 NumGlyphs = 0;
			GlyphIndices.Reserve(ShapeRun.GlyphIndex + MaxGlyphs);
			GlyphProperties.Reserve(ShapeRun.GlyphIndex + MaxGlyphs);

			AssertExpr(ShapeRun.Utf16Index + ShapeRun.Utf16Length <= U16String.Length);
			HRESULT Result = Shaper.dwTextAnalyzer->GetGlyphs(
				AnalysisSource.String.Data + ShapeRun.Utf16Index, ShapeRun.Utf16Length,
				StaticCast<FDWFontFace>(ShapeRun.FontFace).dwFontFace,
				ShapeRun.Rotate90, ShapeRun.BiDiLevel & EBiDiLevel::RTL,
				&ScriptRun.ScriptAnalysis,
				Shaper.DefaultLocalName.Data, nullptr,
				nullptr, nullptr, 0,
				MaxGlyphs,
				Utf16Clusters.Data + ShapeRun.Utf16Index,
				TextProperties.Data,
				GlyphIndices.Data + ShapeRun.GlyphIndex,
				GlyphProperties.Data + ShapeRun.GlyphIndex,
				&NumGlyphs);
			AssertExpr(NumGlyphs <= MaxGlyphs);

			if (Result == E_NOT_SUFFICIENT_BUFFER)
			{
				MaxGlyphs = EstimateGlyphCount(MaxGlyphs);
				GlyphIndices.Reserve(ShapeRun.GlyphIndex + MaxGlyphs);
				GlyphProperties.Reserve(ShapeRun.GlyphIndex + MaxGlyphs);

				AssertExpr(ShapeRun.Utf16Index + ShapeRun.Utf16Length <= U16String.Length);
				Result = Shaper.dwTextAnalyzer->GetGlyphs(
					AnalysisSource.String.Data + ShapeRun.Utf16Index, ShapeRun.Utf16Length,
					StaticCast<FDWFontFace>(ShapeRun.FontFace).dwFontFace,
					ShapeRun.Rotate90, ShapeRun.BiDiLevel & EBiDiLevel::RTL,
					&ScriptRun.ScriptAnalysis,
					Shaper.DefaultLocalName.Data, nullptr,
					nullptr, nullptr, 0,
					MaxGlyphs,
					Utf16Clusters.Data + ShapeRun.Utf16Index,
					TextProperties.Data,
					GlyphIndices.Data + ShapeRun.GlyphIndex,
					GlyphProperties.Data + ShapeRun.GlyphIndex,
					&NumGlyphs);
				AssertExpr(NumGlyphs <= MaxGlyphs);
			}

			if (FAILED(Result))
				break;

			GlyphOffsets.Reserve(ShapeRun.GlyphIndex + NumGlyphs);
			GlyphAdvances.Reserve(ShapeRun.GlyphIndex + NumGlyphs);

			Result = Shaper.dwTextAnalyzer->GetGlyphPlacements(
				AnalysisSource.String.Data + ShapeRun.Utf16Index,
				Utf16Clusters.Data + ShapeRun.Utf16Index,
				TextProperties.Data,
				ShapeRun.Utf16Length,
				GlyphIndices.Data + ShapeRun.GlyphIndex,
				GlyphProperties.Data + ShapeRun.GlyphIndex,
				NumGlyphs,
				StaticCast<FDWFontFace>(ShapeRun.FontFace).dwFontFace,
				ShapeRun.FontSize,
				ShapeRun.Rotate90, ShapeRun.BiDiLevel & EBiDiLevel::RTL,
				&ScriptRun.ScriptAnalysis,
				Shaper.DefaultLocalName.Data,
				nullptr, nullptr, 0,
				GlyphAdvances.Data + ShapeRun.GlyphIndex,
				(DWRITE_GLYPH_OFFSET *)GlyphOffsets.Data + ShapeRun.GlyphIndex
			);

			AssertExpr(SUCCEEDED(Result));

			ShapeRun.GlyphLength = NumGlyphs;
			GlyphIndices.Size = ShapeRun.GlyphIndex + NumGlyphs;
			GlyphOffsets.Size = ShapeRun.GlyphIndex + NumGlyphs;
			GlyphAdvances.Size = ShapeRun.GlyphIndex + NumGlyphs;
			GlyphProperties.Size = ShapeRun.GlyphIndex + NumGlyphs;

			if (ShapeRun.Utf16Index > 0)
			{
				for (uint32 Utf16Index = ShapeRun.Utf16Index; Utf16Index < ShapeRun.Utf16Index + ShapeRun.Utf16Length; ++Utf16Index)
				{
					Utf16Clusters[Utf16Index] += uint16(ShapeRun.Utf16Index);
				}
			}
		}


		// Utf16Clusters -> ScriptClusters.TextIndex, TextLength
		for (uint32 RunIndex = 0; RunIndex < uint32(ShapeRuns.Size); ++RunIndex)
		{
			FShapeRun & ShapeRun = ShapeRuns[RunIndex];
			if (ShapeRun.TextLength == 0)
				continue;

			ShapeRun.ClusterIndex = uint32(ScriptClusters.Size);

			uint32 LastClusterIndex = NullIndex;
			for (uintx Utf16Index = ShapeRun.Utf16Index; Utf16Index < ShapeRun.Utf16Index + ShapeRun.Utf16Length; ++Utf16Index)
			{
				DWRITE_LINE_BREAKPOINT & LineBreakpoint = TextBreakpoints[Utf16Index];
				uint32 ClusterIndex = Utf16Clusters[Utf16Index];
				if (LastClusterIndex != ClusterIndex /*|| LineBreakpoint.isWhitespace*/)
				{
					if (ScriptClusters.Size > 0)
					{
						FScriptCluster & LastCluster = ScriptClusters.Back();
						LastCluster.TextLength = Utf8IndicesOfUtf16s[Utf16Index] - LastCluster.TextIndex;
					}

					LastClusterIndex = ClusterIndex;
					ScriptClusters.Add({ RunIndex, Utf8IndicesOfUtf16s[Utf16Index], 0, 0, 0, EBreakCondition::None });
				}

				EBreakCondition BreakCondition = (LineBreakpoint.breakConditionAfter == DWRITE_BREAK_CONDITION_MUST_BREAK) ? EBreakCondition::Hard :
					(LineBreakpoint.breakConditionAfter == DWRITE_BREAK_CONDITION_CAN_BREAK ? EBreakCondition::Soft : EBreakCondition::None);

				FScriptCluster & LastCluster = ScriptClusters.Back();
				LastCluster.BreakCondition = Max(LastCluster.BreakCondition, BreakCondition);
			}

			if (ScriptClusters.Size > 0)
			{
				FScriptCluster & LastCluster = ScriptClusters.Back();
				LastCluster.TextLength = ShapeRun.TextIndex + ShapeRun.TextLength - LastCluster.TextIndex;
			}

			ShapeRun.ClusterLength = uint32(ScriptClusters.Size) - ShapeRun.ClusterIndex;
		}

		for (uintx ClusterIndex = 0; ClusterIndex < ScriptClusters.Size; ++ClusterIndex)
		{
			FScriptCluster & Cluster = ScriptClusters[ClusterIndex];
#if XIN_DEBUG
			Cluster.String = String.View(Cluster.TextIndex, Cluster.TextLength);
#endif
		}

		// GlyphProperties -> ScriptClusters.GlyphIndex, GlyphLength
		for (uint32 RunIndex = 0; RunIndex < uint32(ShapeRuns.Size); ++RunIndex)
		{
			FShapeRun & ShapeRun = ShapeRuns[RunIndex];
			if (ShapeRun.Invisible)
			{
				for (uintx ClusterIndex = ShapeRun.ClusterIndex; ClusterIndex < ShapeRun.ClusterIndex + ShapeRun.ClusterLength; ++ClusterIndex)
				{
					ScriptClusters[ClusterIndex].GlyphIndex = ShapeRun.GlyphIndex;
					ScriptClusters[ClusterIndex].GlyphLength= 0;
				}
				continue;
			}

			uint32 ClusterIndex = NullIndex;
			for (uint32 GlyphIndex = ShapeRun.GlyphIndex; GlyphIndex < ShapeRun.GlyphIndex + ShapeRun.GlyphLength; ++GlyphIndex)
			{
				const DWRITE_SHAPING_GLYPH_PROPERTIES & GlyphPropertiy = GlyphProperties[GlyphIndex];
				if (GlyphPropertiy.isClusterStart)
				{
					if (ClusterIndex == NullIndex)
						ClusterIndex = ShapeRun.ClusterIndex;
					else
						++ClusterIndex;

					AssertExpr(ClusterIndex < ScriptClusters.Size);
					ScriptClusters[ClusterIndex].GlyphIndex = GlyphIndex;
				}

				++ScriptClusters[ClusterIndex].GlyphLength;
				ScriptClusters[ClusterIndex].Advance.Width += GlyphAdvances[GlyphIndex];
			}
		}

		ScriptClusters.Back();

#if 0
		for (uintx GlyphIndex = 0, ClusterIndex = NullIndex; GlyphIndex < GlyphIndices.Size; ++GlyphIndex)
		{
			const DWRITE_SHAPING_GLYPH_PROPERTIES & GlyphPropertiy = GlyphProperties[GlyphIndex];
			if (GlyphPropertiy.isClusterStart)
			{
				if (ClusterIndex == NullIndex)
					ClusterIndex = 0;
				else
					++ClusterIndex;

				AssertExpresion(ClusterIndex < ScriptClusters.Size);
				ScriptClusters[ClusterIndex].GlyphIndex = GlyphIndex;
			}

			if (!GlyphPropertiy.isZeroWidthSpace)
				++ScriptClusters[ClusterIndex].GlyphLength;
			ScriptClusters[ClusterIndex].Advance.Width += GlyphAdvances[GlyphIndex];
		}
#endif
	}

	FDWTextBlob_Old::FDWTextBlob_Old(FDWTextShaper & Shaper) : ITextBlob(), Shaper(Shaper) {}

	void FDWTextBlob_Old::Shape(FUStringV String, const FFontFace & FontFace, float32 FontSize, EReadingDirection ReadingDirection)
	{
		TextShape.Shape(String, FontFace, FontSize, ReadingDirection);
	}

	void FDWTextBlob_Old::Layout(const SizeF & LayoutSize_, ETextLayoutFlags LayoutFlags)
	{
		ClusterRuns.Clear();
		LayoutLines.Clear();

		GlyphAdvancesJustified.ResizeTo(TextShape.GlyphAdvances.Size, EInitializeMode::None);

		FDWTextLayouter DWTextLayouter { TextShape, *this, !(LayoutFlags & ETextLayoutFlags::DontJustifyAdvances) };

		DWTextLayouter.RectLayout(FTextLayoutRectArea({ PointF::Zero, LayoutSize_ }, TextShape.FontSize));
		LayoutSize = LayoutSize_;

		FinalSize = SizeF::Zero;
		for (uintx LineIndex = 0; LineIndex < LayoutLines.Size; ++LineIndex)
		{
			const FLayoutLine & LayoutLine = LayoutLines[LineIndex];
			FinalSize.Width = Max(FinalSize.Width, LayoutLine.Width);
			FinalSize.Height += LayoutLine.Height;
		}
	}

	void FDWTextBlob_Old::AddGlyphSpan(
		uint32 Utf16Index, uint32 Utf16Length, PointF Position,
		float32 Width, TView<uint16> GlyphIndices, TView<float32> GlyphAdvances,
		TView<PointF> GlyphOffsets,
		IFontFace & Font, float32 FontSize, EBiDiLevel BiDiLevel, bool Rotate90
	)
	{
		FClusterRun & ClusterRun = ClusterRuns.Emplace(Font, FontSize);
		ClusterRun.Position = Position;
		ClusterRun.Size = { Width, FontSize };

		ClusterRun.TextIndex = TextShape.Utf8IndicesOfUtf16s[Utf16Index];
		ClusterRun.TextLength = TextShape.Utf8IndicesOfUtf16s[Utf16Index + Utf16Length] - ClusterRun.TextIndex;

		ClusterRun.Utf16Index = Utf16Index;
		ClusterRun.Utf16Length = Utf16Length;

		ClusterRun.GlyphIndices = GlyphIndices;
		ClusterRun.GlyphAdvances = GlyphAdvances;
		ClusterRun.GlyphOffsets = GlyphOffsets;
		//ClusterRun.ScriptCode = ;
		ClusterRun.BiDiLevel = BiDiLevel;
		ClusterRun.Rotate90 = Rotate90;
#if XIN_DEBUG
		//LayoutSpan.StringW = String.SubView(TextRange.X, TextRange.Y);
#endif
	}

	FDWTextBlob::FDWTextBlob(FDWTextShaper & Shaper) : Shaper(Shaper) {}

	void FDWTextBlob::Shape(FUStringV String, const FFontFace & FontFace, float32 FontSize, EReadingDirection ReadingDirection)
	{
		TextShape.Shape(String, FontFace, FontSize, ReadingDirection);
	}

	void FDWTextBlob::Layout(const SizeF & LayoutSize, ETextLayoutFlags LayoutFlags)
	{
		FTextRectLayouter TextRectLayouter { TextShape, *this };
		TextRectLayouter.Layout(LayoutSize);
	}
}
