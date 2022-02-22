#pragma once

#include "Xin.UI.Types.h"
#include "Controls/Element.h"

namespace Xin::UI
{
	class UI_API UGraph : public UElement
	{
		META_DECL

	public:
		UGraph() = default;
		~UGraph() = default;

	public:
		void OnInitialize() override;
		void OnFinalize() override;
		FSize OnMeasure(FSize ConstrainedSize) override;
		void OnPaint(IPainter & Painter) override;

	public:
		void LoadSvg(FStringV FilePath);

	private:
		struct FShape
		{
			IPathRef Path;

			IBrushRef FillBrush;
			IBrushRef StrokeBrush;
			float32 StrokeThickness;
		};
		TList<FShape> Shapes;

	public:
		ProxyPropertyRW(FString, SvgFilePath);
		ProxyPropertyRW(FSize, SvgSize);
		ProxyPropertyRW(Vec4F, SvgBounds);
		ProxyPropertyRW(Vec2F, SvgScale) = Vec2F::One;
	};
	using UVectorGraphRef = TReferPtr<UGraph>;


	inline UVectorGraphRef VectorGraph(FStringV SvgFilePath, TView<TPair<const FProperty &, FVariant>> Properties = { })
	{
		UVectorGraphRef Path = MakeRefer<UGraph>();
		Path->SvgFilePath = SvgFilePath;
		for (auto & Pair : Properties)
			Path->SetPropertyValue(Pair.First, Pair.Second);
		return Path;
	}
}
