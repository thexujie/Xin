#pragma once

#include "Xin.UI.Types.h"
#include "Shape.h"

namespace Xin::UI
{
	class UI_API UEllipse : public UShape
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

	public:
		UEllipse() = default;
		UEllipse(FName Name) : UShape(Name) {}
		~UEllipse() = default;

	public:
		void OnPaint(IPainter & Painter) override;

	public:
		[[Meta(AffectsMeasure, AffectsRender)]]
		ProxyPropertyRW(FSize, Radius) = FSize::Zero;
	};
	using UEllipseRef = TReferPtr<UEllipse>;
}
