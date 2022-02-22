#include "PCH.h"
#include "DWTextLayouter.h"

#include "DWTextShaper.h"

namespace Xin::DWrite
{
	FDWTextLayouter::FDWTextLayouter(FDWTextShape & TextShape, FDWTextBlob_Old & TextLayout, bool AdvancesJustify)
		: Shape(TextShape), TextLayout(TextLayout), AdvancesJustify(AdvancesJustify)
	{

	}

	void FDWTextLayouter::RectLayout(const ITextLayoutRectArea & LayoutRects)
	{
		if (!Shape.Utf16Clusters.Size)
			return;

		FClusterPosition CurrCluster;
		SetClusterPosition(CurrCluster, 0);

		while (CurrCluster.Utf16Index < Shape.Utf16Clusters.Size)
		{
			RectF LayoutRect = LayoutRects.NextRect();
			if (LayoutRect.Width <= 0)
				break;

			// Fit as many clusters between breakpoints that will go in.
			FClusterPosition TailCluster = BreakLine(CurrCluster, Shape.U16String.Length, LayoutRect.Width);

			if (TailCluster.Utf16Index <= CurrCluster.Utf16Index)
				break;

			FLayoutLine & LayoutLine = TextLayout.LayoutLines.Add({ uint32(TextLayout.ClusterRuns.Size), 0 });
			LayoutLine.RunIndex = uint32(TextLayout.ClusterRuns.Size);
			// Push the glyph runs to the sink.
			ProduceGlyphRuns(LayoutRect, CurrCluster, TailCluster);

			LayoutLine.RunLength = uint32(TextLayout.ClusterRuns.Size) - LayoutLine.RunIndex;

			for (sizet SpanIndex = 0; SpanIndex < LayoutLine.RunLength; ++SpanIndex)
			{
				const FClusterRun & ClusterRun = TextLayout.ClusterRuns[LayoutLine.RunIndex + SpanIndex];
				LayoutLine.Ascent = Max(LayoutLine.Ascent, ClusterRun.FontFace.Ascent * ClusterRun.FontSize);
				LayoutLine.Descent = Max(LayoutLine.Descent, ClusterRun.FontFace.Descent * ClusterRun.FontSize);
				LayoutLine.Width += ClusterRun.Size.Width;

				//LayoutLine.Height = Max(LayoutLine.Height, LayoutSpan.Font.Height * LayoutSpan.FontSize);
			}
			LayoutLine.Height = LayoutLine.Ascent + LayoutLine.Descent;

			CurrCluster = TailCluster;
		}


		for (uintx LineIndex = 0; LineIndex < TextLayout.LayoutLines.Size; ++LineIndex)
		{
			FLayoutLine & LayoutLine = TextLayout.LayoutLines[LineIndex];

			const FClusterRun & FirstRun = TextLayout.ClusterRuns[LayoutLine.RunIndex];
			const FClusterRun & LastRun = TextLayout.ClusterRuns[LayoutLine.RunIndex + LayoutLine.RunLength - 1];

			const FScriptCluster & FirstCluster = TextLayout.TextShape.ScriptClusters[FirstRun.ClusterIndex];
			const FScriptCluster & LastCluster = TextLayout.TextShape.ScriptClusters[LastRun.ClusterIndex + LastRun.ClusterLength];

			LayoutLine.TextIndex = FirstRun.TextIndex;
			LayoutLine.TextLength = LastRun.TextIndex + LastRun.TextLength - FirstRun.TextIndex;

			LayoutLine.ClusterIndex = FirstRun.ClusterIndex;
			LayoutLine.ClusterLength = LastRun.ClusterIndex + LastRun.ClusterLength - FirstRun.ClusterIndex;

#if XIN_DEBUG
			LayoutLine.String = TextLayout.TextShape.String.View(LayoutLine.TextIndex, LayoutLine.TextLength);
#endif
		}

		TextLayout.LayoutLines.Back();
	}

	void FDWTextLayouter::SetClusterPosition(FClusterPosition & Cluster, uint32 Utf16Index) const
	{
		Cluster.Utf16Index = Utf16Index;

		if (Utf16Index < Cluster.RunUtf16EndIndex && IsTextIndexWithinTextRun(Utf16Index, Shape.ShapeRuns[Cluster.RunIndex]))
			return;

		// Search next run start from current cluster run.
		uint32 RunIndex = 0;
		if (Utf16Index >= Shape.ShapeRuns[Cluster.RunIndex].Utf16Index)
			RunIndex = Cluster.RunIndex;

		auto GlyphRunIter = FindIf(Shape.ShapeRuns.Begin() + RunIndex, Shape.ShapeRuns.End(), [&](const FShapeRun & ShapeRun) { return IsTextIndexWithinTextRun(Utf16Index, ShapeRun); });
		if (GlyphRunIter == Shape.ShapeRuns.End())
		{
			if (Utf16Index != Shape.ShapeRuns.Back().Utf16Index + Shape.ShapeRuns.Back().Utf16Length)
			{
				LogError(u8"Utf16Index {} is not within any text range of TextRuns.", Utf16Index);
				AssertFalse();
				return;
			}
			RunIndex = Shape.ShapeRuns.Size - 1;
		}
		else
			RunIndex = (uint32)Distance(Shape.ShapeRuns.Begin(), GlyphRunIter);

		Cluster.RunIndex = RunIndex;
		Cluster.RunUtf16EndIndex = Shape.ShapeRuns[RunIndex].Utf16Index + Shape.ShapeRuns[RunIndex].Utf16Length;
	}

	void FDWTextLayouter::AdvanceClusterPosition(FClusterPosition & Cluster) const
	{
		uint32 Utf16Index = Cluster.Utf16Index;
		uint32 ClusterIndex = Shape.Utf16Clusters[Utf16Index];

		for (Utf16Index = Utf16Index + 1; Utf16Index < Cluster.RunUtf16EndIndex; ++Utf16Index)
		{
			if (Shape.Utf16Clusters[Utf16Index] != ClusterIndex)
			{
				// advance to next cluster in this run.
				Cluster.Utf16Index = Utf16Index;
				return;
			}
		}

		AssertExpr(Utf16Index == Cluster.RunUtf16EndIndex);
		// advance to next cluster in next run.
		if (Utf16Index == Cluster.RunUtf16EndIndex)
			SetClusterPosition(Cluster, Cluster.RunUtf16EndIndex);
	}

	FClusterPosition FDWTextLayouter::BreakLine(const FClusterPosition & FirstCluster, uint32 Utf16Length, float32 LineWidth)
	{
		FClusterPosition CurrCluster { FirstCluster };
		FClusterPosition NextCluster { FirstCluster };

		uint32 ValidBreakPosition = FirstCluster.Utf16Index;
		uint32 BestBreakPosition = FirstCluster.Utf16Index;
		float32 TextWidth = 0;

		while (CurrCluster.Utf16Index < Utf16Length)
		{
			// One cluster at least.
			AdvanceClusterPosition(NextCluster);
			TextWidth += GetClustersWidth(CurrCluster, NextCluster);

			DWRITE_LINE_BREAKPOINT LineBreakpoint = Shape.TextBreakpoints[NextCluster.Utf16Index - 1];
			if (TextWidth >= LineWidth && !LineBreakpoint.isWhitespace)
			{
				// Want a minimum of one cluster.
				if (ValidBreakPosition > FirstCluster.Utf16Index)
					break;
			}

			ValidBreakPosition = NextCluster.Utf16Index;

			// See if we can break after this character cluster, and if so,
			// mark it as the new potential break point.
			if (LineBreakpoint.breakConditionAfter != DWRITE_BREAK_CONDITION_MAY_NOT_BREAK)
			{
				BestBreakPosition = NextCluster.Utf16Index;
				if (LineBreakpoint.breakConditionAfter == DWRITE_BREAK_CONDITION_MUST_BREAK)
					break; // we have a hard return, so we've fit all we can.
			}
			CurrCluster = NextCluster;
		}

		// Want last best position that didn't break a word, but if that's not available,
		// fit at least one cluster (emergency line breaking).
		SetClusterPosition(CurrCluster, BestBreakPosition != FirstCluster.Utf16Index ? BestBreakPosition : ValidBreakPosition);
		return CurrCluster;
	}

	void FDWTextLayouter::ProduceBidiOrdering(uint32 StartRunIndex, TSlice<uint32> BidiOrdering) const
	{
		// Fill all entries with initial indices
		for (uint32 SpanIndex = 0; SpanIndex < BidiOrdering.Size; ++SpanIndex)
			BidiOrdering[SpanIndex] = StartRunIndex + SpanIndex;

		if (BidiOrdering.Size <= 1)
			return;

		size_t RunIndex = 0;
		uint32 CurrentBidiLevel = uint32(Shape.ShapeRuns[StartRunIndex].BiDiLevel);

		// Rearrange each run to produced reordered spans.
		for (size_t BidiIndex = 0; BidiIndex < BidiOrdering.Size; ++BidiIndex)
		{
			size_t RunEnd = BidiIndex + 1;
			uint32 NextBidiLevel = (RunEnd < BidiOrdering.Size)
				? uint32(Shape.ShapeRuns[BidiOrdering[RunEnd]].BiDiLevel)
				: 0; // past last element

			// We only care about transitions, particularly high to low,
			// because that means we have a run behind us where we can
			// do something.

			if (CurrentBidiLevel <= NextBidiLevel) // This is now the beginning of the next run.
			{
				if (CurrentBidiLevel < NextBidiLevel)
				{
					CurrentBidiLevel = NextBidiLevel;
					RunIndex = BidiIndex + 1;
				}
				continue; // Skip past equal levels, or increasing stairsteps.
			}

			do // currentLevel > nextLevel
			{
				// Recede to find start of the run and previous level.
				uint32 previousLevel;
				for (;;)
				{
					if (RunIndex <= 0) // reached front of index list
					{
						previousLevel = 0; // position before string has bidi level of 0
						break;
					}
					if (uint32(Shape.ShapeRuns[BidiOrdering[--RunIndex]].BiDiLevel) < CurrentBidiLevel)
					{
						previousLevel = uint32(Shape.ShapeRuns[BidiOrdering[RunIndex]].BiDiLevel);
						++RunIndex; // compensate for going one element past
						break;
					}
				}

				// Reverse the indices, if the difference between the current and
				// next/previous levels is odd. Otherwise, it would be a no-op, so
				// don't bother.
				if ((Min(CurrentBidiLevel - NextBidiLevel, CurrentBidiLevel - previousLevel) & 1) != 0)
				{
					std::reverse(BidiOrdering.Data + RunIndex, BidiOrdering.Data + RunEnd);
				}

				// Descend to the next lower level, the greater of previous and next
				CurrentBidiLevel = std::max(previousLevel, NextBidiLevel);
			}
			while (CurrentBidiLevel > NextBidiLevel); // Continue until completely flattened.
		}
	}

	void FDWTextLayouter::ProduceJustifiedAdvances(const RectF & LayoutRect, const FClusterPosition & HeadCluster, const FClusterPosition & TailCluster) const
	{
		// Performs simple inter-word justification
		// using the breakpoint analysis whitespace property.

		// Copy out default, unjustified advances.-
		uint32 GlyphStart = GetClusterGlyphStart(HeadCluster);
		uint32 GlyphEnd = GetClusterGlyphStart(TailCluster);

		// No glyphs to modify.
		if (GlyphEnd - GlyphStart == 0)
			return;

		ArrayCopy(TextLayout.GlyphAdvancesJustified.Data + GlyphStart, Shape.GlyphAdvances.Data + GlyphStart, GlyphEnd - GlyphStart);

		if (LayoutRect.Width <= 0)
			return;

		////////////////////////////////////////
		// First, count how many spaces there are in the text range.

		FClusterPosition CurrCluster(HeadCluster);
		uint32 WhitespaceCount = 0;

		while (CurrCluster.Utf16Index < TailCluster.Utf16Index)
		{
			if (Shape.TextBreakpoints[CurrCluster.Utf16Index].isWhitespace)
				++WhitespaceCount;
			AdvanceClusterPosition(CurrCluster);
		}
		if (WhitespaceCount <= 0)
			return; // Can't justify using spaces, since none exist.


		////////////////////////////////////////
		// Second, determine the needed contribution to each space.

		float LineWidth = GetGlyphsWidth(GlyphStart, GlyphEnd, Shape.GlyphAdvances.Data);
		float JustificationPerSpace = (LayoutRect.Width - LineWidth) / WhitespaceCount;

		if (JustificationPerSpace <= 0)
			return; // Either already justified or would be negative justification.

		constexpr float32 MaxSpaceWidth = 8.0f;
		if (JustificationPerSpace > MaxSpaceWidth)
			return; // Avoid justification if it would space the line out awkwardly far.


		////////////////////////////////////////
		// Lastly, adjust the advance widths, adding the difference to each space character.

		CurrCluster = HeadCluster;
		while (CurrCluster.Utf16Index < TailCluster.Utf16Index)
		{
			if (Shape.TextBreakpoints[CurrCluster.Utf16Index].isWhitespace)
				TextLayout.GlyphAdvancesJustified[GetClusterGlyphStart(CurrCluster)] += JustificationPerSpace;

			AdvanceClusterPosition(CurrCluster);
		}
	}

	void FDWTextLayouter::ProduceGlyphRuns(const RectF & LayoutRect, const FClusterPosition & FirstCluster, const FClusterPosition & LastCluster) const
	{
		uint32 LastRunIndex = LastCluster.RunIndex;
		AssertExpr(LastCluster.Utf16Index <= Shape.ShapeRuns[LastRunIndex].Utf16Index + Shape.ShapeRuns[LastRunIndex].Utf16Length);
		if (LastCluster.Utf16Index > Shape.ShapeRuns[LastRunIndex].Utf16Index)
			++LastRunIndex; // Only partially cover the run, so round up.

		uint32 NumRuns = LastRunIndex - FirstCluster.RunIndex;
		RunBidiOrdering.Resize(NumRuns, EInitializeMode::None);
		ProduceBidiOrdering(FirstCluster.RunIndex, RunBidiOrdering);

		// Ignore any trailing whitespace
		// ...

		// Look backwards from end until we find non-space.
		uint32 TailTextPosition = LastCluster.Utf16Index;
		for (; TailTextPosition > FirstCluster.Utf16Index; --TailTextPosition)
		{
			if (!Shape.TextBreakpoints[TailTextPosition - 1].isWhitespace)
				break; // Encountered last significant character.
		}

		// Set the glyph run's ending cluster to the last whitespace.
		FClusterPosition TailCluster(FirstCluster);
		SetClusterPosition(TailCluster, TailTextPosition);

		// Produce justified advances to reduce the jagged edge.
		if (AdvancesJustify)
			ProduceJustifiedAdvances(LayoutRect, FirstCluster, TailCluster);

		TList<float32> & GlyphAdvances = AdvancesJustify ? TextLayout.GlyphAdvancesJustified : Shape.GlyphAdvances;

		////////////////////////////////////////
		// Determine starting point for alignment.

		float LayoutX = LayoutRect.X;

		if (Shape.ReadingDirection == EReadingDirection::RightToLeft)
		{
			// For RTL, we neeed the run width to adjust the origin
			// so it starts on the right side.
			uint32 GlyphIndex = GetClusterGlyphStart(FirstCluster);
			uint32 GlyphEndIndex = GetClusterGlyphStart(TailCluster);

			if (GlyphIndex < GlyphEndIndex)
			{
				float LineWidth = GetGlyphsWidth(GlyphIndex, GlyphEndIndex, GlyphAdvances.Data);
				LayoutX = LayoutRect.Right - LineWidth;
			}
		}

		////////////////////////////////////////
		// Send each glyph run to the sink.
		for (size_t SpanIndex = 0; SpanIndex < NumRuns; ++SpanIndex)
		{
			const FShapeRun & ShapeRun = Shape.ShapeRuns[RunBidiOrdering[SpanIndex]];

			// If the run is only partially covered, we'll need to find
			// the subsection of glyphs that were fit.
			uint32 SpanGlyphIndex = (ShapeRun.Utf16Index < FirstCluster.Utf16Index) ? GetClusterGlyphStart(FirstCluster) : ShapeRun.GlyphIndex;
			uint32 SpanGlyphEndIndex = (TailCluster.Utf16Index < ShapeRun.Utf16Index + ShapeRun.Utf16Length) ? GetClusterGlyphStart(TailCluster) : (ShapeRun.GlyphIndex + ShapeRun.GlyphLength);

			if (SpanGlyphIndex >= SpanGlyphEndIndex || ShapeRun.Invisible)
			{
				// The itemizer told us not to draw this character,
				// either because it was a formatting, control, or other hidden character.
				continue;
			}

			// The run width is needed to know how far to move forward,
			// and to flip the origin for right-to-left text.
			float32 SpanWidth = GetGlyphsWidth(SpanGlyphIndex, SpanGlyphEndIndex, GlyphAdvances.Data);


			// Flush this glyph run.
			uint32 GlyphCount = SpanGlyphEndIndex - SpanGlyphIndex;
			TextLayout.AddGlyphSpan(
				FirstCluster.Utf16Index, LastCluster.Utf16Index - FirstCluster.Utf16Index,
				{ LayoutX, LayoutRect.Y },
				SpanWidth,
				Shape.GlyphIndices.View(SpanGlyphIndex, GlyphCount),
				GlyphAdvances.View(SpanGlyphIndex, GlyphCount),
				Shape.GlyphOffsets.View(SpanGlyphIndex, GlyphCount),
				ShapeRun.FontFace, ShapeRun.FontSize,
				ShapeRun.BiDiLevel, ShapeRun.Rotate90
			);

			LayoutX += SpanWidth;
		}

	}
}

