#pragma once

#include "Xin.UI.Types.h"
#include "Shape.h"

namespace Xin::UI
{
	class UI_API UPolyline : public UShape
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

	public:
		UPolyline() = default;
		UPolyline(FName Name) : UShape(Name) {}
		~UPolyline() = default;

	public:
		void OnInitialize() override;
		void OnFinalize() override;
		FSize OnMeasure(FSize ConstrainedSize) override;
		void OnPaint(IPainter & Painter) override;

	public:
		TList<FPoint> Points;
		EStretch Stretch = EStretch::None;

	private:
		IPathRef Path;
	};
	using UPolylineRef = TReferPtr<UPolyline>;
}
