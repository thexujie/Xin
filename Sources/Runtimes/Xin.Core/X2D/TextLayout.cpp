#include "PCH.h"
#include "TextLayout.h"

namespace Xin
{
	//static TTypeRegister<ITextShaper> TypeRegister_ITextShaper;

	static void ProduceBidiOrdering(TSlice<EBiDiLevel> BidiLevels, TSlice<uint32> BidiOrdering)
	{
		size_t RunIndex = 0;
		uint32 CurrentBidiLevel = uint32(BidiLevels[0]);

		// Rearrange each run to produced reordered spans.
		for (size_t BidiIndex = 0; BidiIndex < BidiLevels.Size; ++BidiIndex)
		{
			size_t RunEnd = BidiIndex + 1;
			uint32 NextBidiLevel = (RunEnd < BidiLevels.Size) ? uint32(BidiLevels[BidiOrdering[RunEnd]]) : 0; // past last element

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
					if (uint32(BidiLevels[BidiOrdering[--RunIndex]]) < CurrentBidiLevel)
					{
						previousLevel = uint32(BidiLevels[BidiOrdering[RunIndex]]);
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

	void FTextRectLayouter::Layout(const SizeF & LayoutSize, ETextAlignment TextAlignment)
	{
		// no clusters
		if (TextShape.ShapeRuns.Size == 0 || TextShape.ScriptClusters.Size == 0)
			return;

		{
			FLayoutLine & FirstLine = TextBlob.LayoutLines.AddDefault();
			FirstLine.RunIndex = 0;
		}

		for (uintx RunIndex = 0; RunIndex < TextShape.ShapeRuns.Size; ++RunIndex)
		{
 			const FShapeRun & ShapeRun = TextShape.ShapeRuns[RunIndex];

			float32 LineAdvance = 0.0f;
			uintx SoftBreakIndex = NullIndex;
			uintx BreakIndex = NullIndex;
			uintx FirstClusterIndex = ShapeRun.ClusterIndex;
			uintx ClusterIndex = ShapeRun.ClusterIndex;

			while (ClusterIndex < ShapeRun.ClusterIndex + ShapeRun.ClusterLength)
			{
				const FScriptCluster & Cluster = TextShape.ScriptClusters[ClusterIndex];
				AssertExpr(Cluster.RunIndex == RunIndex);

				if (Cluster.BreakCondition == EBreakCondition::Hard)
				{
					BreakIndex = ClusterIndex;

					AddClusterRun(FirstClusterIndex, ClusterIndex + 1 - FirstClusterIndex);

					FLayoutLine & LayoutLine = TextBlob.LayoutLines.AddDefault();
					LayoutLine.RunIndex = TextBlob.ClusterRuns.Size;
					LineAdvance = 0.0f;

				}
				else if (ClusterIndex + 1 == ShapeRun.ClusterIndex + ShapeRun.ClusterLength)
				{
					// Last cluster of this ShapeRun.
					AddClusterRun(FirstClusterIndex, ClusterIndex + 1 - FirstClusterIndex);

					// just break, we shouldn't add new line for last cluster.
					break;
				}
				else
				{
					if (Cluster.BreakCondition == EBreakCondition::Soft)
						SoftBreakIndex = FirstClusterIndex;

					LineAdvance += Cluster.Advance.Width;

					if ((TextBlob.LayoutLines.Back().RunLength > 0 || ClusterIndex > FirstClusterIndex) && LineAdvance + TextShape.ScriptClusters[ClusterIndex + 1].Advance.Width > LayoutSize.Width)
					{
						BreakIndex = SoftBreakIndex != NullIndex ? SoftBreakIndex : ClusterIndex;

						AddClusterRun(FirstClusterIndex, BreakIndex + 1 - FirstClusterIndex);

						FLayoutLine & LayoutLine = TextBlob.LayoutLines.AddDefault();
						LayoutLine.RunIndex = TextBlob.ClusterRuns.Size;
						LineAdvance = 0.0f;
					}
					else
					{
						++ClusterIndex;
					}
				}


				// Add cluster run on [ClusterIndex, BreakIndex]
				if (BreakIndex != NullIndex)
				{
					ClusterIndex = BreakIndex + 1;
					FirstClusterIndex = ClusterIndex;

					SoftBreakIndex = NullIndex;
					BreakIndex = NullIndex;
				}
			}
		}
#if 0
		for (uintx ClusterIndex = 0, RunClusterIndex = 0, LastRunIndex = 0; ClusterIndex < TextShape.ScriptClusters.Size; ++ClusterIndex)
		{
			const FScriptCluster & Cluster = TextShape.ScriptClusters[ClusterIndex];
			if (Cluster.RunIndex == NullIndexU32 || Cluster.RunIndex != LastRunIndex)
			{
				AddClusterRun(RunClusterIndex, ClusterIndex - RunClusterIndex);

				LastRunIndex = Cluster.RunIndex;
				RunClusterIndex = ClusterIndex;
				LayoutPosition.X += RunAdvance;
				RunAdvance = 0.0f;
			}

			// no spaces for this clutser.
			if (LayoutPosition.X + RunAdvance + Cluster.Advance.Width > LayoutSize.Width)
			{
				// there already some clusters on this line, so we break line, and add run.
				if (ClusterIndex > RunClusterIndex)
				{
					AddClusterRun(RunClusterIndex, ClusterIndex - RunClusterIndex);
					RunClusterIndex = ClusterIndex;
				}

				RunAdvance = 0.0f;

				LayoutPosition.X = 0.0f;
				LayoutPosition.Y += TextShape.FontSize;
				FLayoutLine & LayoutLine = TextBlob.LayoutLines.AddDefault();
				LayoutLine.RunIndex = TextBlob.ClusterRuns.Size;
			}

			RunAdvance += Cluster.Advance.Width;

			if (Cluster.BreakCondition == EBreakCondition::Hard || ClusterIndex + 1 == TextShape.ScriptClusters.Size)
			{
				AddClusterRun(RunClusterIndex, ClusterIndex + 1 - RunClusterIndex);
				RunClusterIndex = ClusterIndex + 1;
			}
			else
			{
			}
		}
#endif

		TextBlob.FinalSize = SizeF::Zero;
		for (uintx LineIndex = 0; LineIndex < TextBlob.LayoutLines.Size; ++LineIndex)
		{
			FLayoutLine & LayoutLine = TextBlob.LayoutLines[LineIndex];
			if (LayoutLine.RunLength == 0)
				continue;

			const FClusterRun & FirstRun = TextBlob.ClusterRuns[LayoutLine.RunIndex];
			const FClusterRun & LastRun = TextBlob.ClusterRuns[LayoutLine.RunIndex + LayoutLine.RunLength - 1];

			for (uint32 RunIndex = LayoutLine.RunIndex; RunIndex < LayoutLine.RunIndex + LayoutLine.RunLength; ++RunIndex)
			{
				const FClusterRun & ClusterRun = TextBlob.ClusterRuns[RunIndex];
				LayoutLine.Width += ClusterRun.Size.Width;

				LayoutLine.Ascent = Max(LayoutLine.Ascent, ClusterRun.FontFace.Ascent * ClusterRun.FontSize);
				LayoutLine.Descent = Max(LayoutLine.Descent, ClusterRun.FontFace.Descent * ClusterRun.FontSize);
				if (TextAlignment & ETextAlignment::Baseline)
				{
					LayoutLine.Height = LayoutLine.Ascent + LayoutLine.Descent;
				}
				else
				{
					LayoutLine.Height = Max(LayoutLine.Height, ClusterRun.FontFace.Height * ClusterRun.FontSize);
				}
			}

			const FScriptCluster & FirstCluster = TextShape.ScriptClusters[FirstRun.ClusterIndex];
			const FScriptCluster & LastCluster = TextShape.ScriptClusters[LastRun.ClusterIndex + LastRun.ClusterLength - 1];

			LayoutLine.TextIndex = FirstCluster.TextIndex;
			LayoutLine.TextLength = LastCluster.TextIndex + LastCluster.TextLength - FirstCluster.TextIndex;

			LayoutLine.ClusterIndex = FirstRun.ClusterIndex;
			LayoutLine.ClusterLength = LastRun.ClusterIndex + LastRun.ClusterLength - FirstRun.ClusterIndex;

			TextBlob.FinalSize.Width = Max(TextBlob.FinalSize.Width, LayoutLine.Width);
			TextBlob.FinalSize.Height += LayoutLine.Height;

#if XIN_DEBUG
			LayoutLine.String = TextShape.String.View(LayoutLine.TextIndex, LayoutLine.TextLength);
#endif
		}

		TextBlob.GlyphPositions.Resize(TextShape.GlyphIndices.Size);
		float32 LineY = 0.0f;
		for (uintx LineIndex = 0; LineIndex < TextBlob.LayoutLines.Size; ++LineIndex)
		{
			FLayoutLine & LayoutLine = TextBlob.LayoutLines[LineIndex];
			if (LayoutLine.RunLength == 0)
				continue;

			float32 RunX = 0.0f;

			TList<EBiDiLevel> BiDiLevels(LayoutLine.RunLength, EInitializeMode::Zero);
			TList<uint32> BiDiOrdering(LayoutLine.RunLength, EInitializeMode::Zero);
			for (uint32 RunIndex = 0; RunIndex < LayoutLine.RunLength; ++RunIndex)
			{
				BiDiLevels[RunIndex] = TextBlob.ClusterRuns[LayoutLine.RunIndex + RunIndex].BiDiLevel;
				BiDiOrdering[RunIndex] = RunIndex;
			}

			ProduceBidiOrdering(BiDiLevels, BiDiOrdering);

			for (uint32 RunIndex = 0; RunIndex < LayoutLine.RunLength; ++RunIndex)
			{
				FClusterRun & ClusterRun = TextBlob.ClusterRuns[LayoutLine.RunIndex + BiDiOrdering[RunIndex]];
				if (ClusterRun.GlyphLength == 0)
					continue;

				ClusterRun.Position.X = RunX;

				switch (TextAlignment & ETextAlignment::VerticalMask)
				{
				case ETextAlignment::Top:
					ClusterRun.Position.Y = LineY;
					break;
				case ETextAlignment::CenterY:
					ClusterRun.Position.Y = (LayoutLine.Height - ClusterRun.FontFace.Height * ClusterRun.FontSize) * 0.5f;;
					break;
				case ETextAlignment::Bottom:
					ClusterRun.Position.Y = LineY + LayoutLine.Height - ClusterRun.FontFace.Height * ClusterRun.FontSize;
					break;
				default:
					ClusterRun.Position.Y = LayoutLine.Ascent - ClusterRun.FontFace.Ascent * ClusterRun.FontSize;
					break;
				}

				RunX += ClusterRun.Size.Width;

				float32 RunAdvacne = 0.0f;
				TextBlob.GlyphPositions[ClusterRun.GlyphIndex] = { };
				for (uint32 GlyphIndex = ClusterRun.GlyphIndex; GlyphIndex < ClusterRun.GlyphIndex + ClusterRun.GlyphLength; ++GlyphIndex)
				{
					if (ClusterRun.BiDiLevel & EBiDiLevel::RTL)
						TextBlob.GlyphPositions[GlyphIndex].X = ClusterRun.Size.Width - TextShape.GlyphAdvances[GlyphIndex] - RunAdvacne;
					else
						TextBlob.GlyphPositions[GlyphIndex].X = RunAdvacne;

					TextBlob.GlyphPositions[GlyphIndex].X -= TextShape.GlyphOffsets[GlyphIndex].X;
					TextBlob.GlyphPositions[GlyphIndex].Y = TextShape.GlyphOffsets[GlyphIndex].Y;

					RunAdvacne += TextShape.GlyphAdvances[GlyphIndex];
				}

				ClusterRun.GlyphPositions = TextBlob.GlyphPositions.View(ClusterRun.GlyphIndex, ClusterRun.GlyphLength);
			}

			LineY += LayoutLine.Height;
		}

		AssertExpr(TextBlob.FinalSize.Width >= 0.0f);
		AssertExpr(TextBlob.FinalSize.Height >= 0.0f);

		AssertExpr(TextBlob.FinalSize.Width < 0xFFFF * 4.0f);
		AssertExpr(TextBlob.FinalSize.Height < 0xFFFF * 4.0f);

		TextBlob.LayoutLines.Back();
	}

	void FTextRectLayouter::AddClusterRun(uint32 ClusterIndex, uint32 ClusterLength)
	{
		if (ClusterLength == 0)
			return;

		FLayoutLine & CurrentLine = TextBlob.LayoutLines.Back();
		++CurrentLine.RunLength;

		const FScriptCluster & FirstCluster = TextShape.ScriptClusters[ClusterIndex];
		const FScriptCluster & LastCluster = TextShape.ScriptClusters[ClusterIndex + ClusterLength - 1];

		AssertExpr(FirstCluster.RunIndex == LastCluster.RunIndex);

		const FShapeRun & ShapeRun = TextShape.ShapeRuns[FirstCluster.RunIndex];

		FClusterRun & ClusterRun = TextBlob.ClusterRuns.Emplace(ShapeRun.FontFace);
		ClusterRun.FontSize = ShapeRun.FontSize;

		ClusterRun.LineIndex = TextBlob.LayoutLines.Size - 1;

		ClusterRun.TextIndex = FirstCluster.TextIndex;
		ClusterRun.TextLength = LastCluster.TextIndex + LastCluster.TextLength - FirstCluster.TextIndex;

		ClusterRun.ClusterIndex = ClusterIndex;
		ClusterRun.ClusterLength = ClusterLength;

		ClusterRun.GlyphIndex = FirstCluster.GlyphIndex;
		ClusterRun.GlyphLength = LastCluster.GlyphIndex + LastCluster.GlyphLength - FirstCluster.GlyphIndex;

		ClusterRun.ScriptCode = ShapeRun.ScriptCode;
		ClusterRun.BiDiLevel = ShapeRun.BiDiLevel;
		ClusterRun.Rotate90 = ShapeRun.Rotate90;

		ClusterRun.GlyphIndices = TextShape.GlyphIndices.View(ClusterRun.GlyphIndex, ClusterRun.GlyphLength);
		ClusterRun.GlyphAdvances = TextShape.GlyphAdvances.View(ClusterRun.GlyphIndex, ClusterRun.GlyphLength);
		ClusterRun.GlyphOffsets = TextShape.GlyphOffsets.View(ClusterRun.GlyphIndex, ClusterRun.GlyphLength);

		for (uint32 Index = ClusterIndex; Index < ClusterIndex + ClusterLength; ++Index)
		{
			ClusterRun.Size.Width += TextShape.ScriptClusters[Index].Advance.Width;
			ClusterRun.Size.Height = Max(ClusterRun.Size.Height, TextShape.ScriptClusters[Index].Advance.Height);
		}
#if XIN_DEBUG
		ClusterRun.String = TextShape.String.View(ClusterRun.TextIndex, ClusterRun.TextLength);
		ClusterRun.Clusters = TextShape.ScriptClusters.View(ClusterRun.ClusterIndex, ClusterRun.ClusterLength);
#endif
	}
}
