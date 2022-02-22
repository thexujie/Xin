#pragma once

#include "DWTypes.h"
#include "DWTextShape.h"

namespace Xin::DWrite
{
	struct FClusterPosition
	{
		uint32 ClusterIndex = 0;
		uint32 Utf16Index = 0;
		uint32 RunIndex = 0;
		uint32 RunUtf16EndIndex = 0;
	};

	class DWRITE_API FDWTextLayouter
	{
	public:
		FDWTextLayouter(FDWTextShape & TextShape, FDWTextBlob_Old & TextLayout, bool AdvancesJustify = true);

		void RectLayout(const ITextLayoutRectArea & LayoutRects);

	private:
		void SetClusterPosition(FClusterPosition & Cluster, uint32 Utf16Index) const;
		void AdvanceClusterPosition(FClusterPosition & Cluster) const;

		forceinline uint32 GetClusterGlyphStart(const FClusterPosition & Cluster) const
		{
			uint32 GlyphIndex = Shape.ShapeRuns[Cluster.RunIndex].GlyphIndex;
			if (Cluster.Utf16Index < Shape.Utf16Clusters.Size)
				return Shape.Utf16Clusters[Cluster.Utf16Index];
			else
				return GlyphIndex + Shape.ShapeRuns[Cluster.RunIndex].GlyphLength;
		}

		forceinline float32 GetGlyphsWidth(uint32 GlyphIndex, uint32 GlyphEnd, const float32 * GlyphAdvances) const
		{
			return std::accumulate(GlyphAdvances + GlyphIndex, GlyphAdvances + GlyphEnd, 0.0f);
		}

		forceinline float32 GetClustersWidth(const FClusterPosition & ClusterIndex, const FClusterPosition & ClusterEnd) const
		{
			uint32 GlyphIndex = GetClusterGlyphStart(ClusterIndex);
			uint32 GlyphEnd = GetClusterGlyphStart(ClusterEnd);
			return GetGlyphsWidth(GlyphIndex, GlyphEnd, Shape.GlyphAdvances.Data);
		}

		FClusterPosition BreakLine(const FClusterPosition & FirstCluster, uint32 Utf16Length, float32 LineWidth);

		void ProduceBidiOrdering(uint32 StartRunIndex, TSlice<uint32> BidiOrdering) const;
		void ProduceJustifiedAdvances( const RectF & LayoutRect, const FClusterPosition & HeadCluster, const FClusterPosition & TailCluster) const;
		void ProduceGlyphRuns(const RectF & LayoutRect, const FClusterPosition & FirstCluster, const FClusterPosition & LastCluster ) const;

	public:
		FDWTextShape & Shape;
		FDWTextBlob_Old & TextLayout;
		bool AdvancesJustify = true;

	private:
		mutable TList<uint32> RunBidiOrdering { 1024, EInitializeMode::None };
	};

}
