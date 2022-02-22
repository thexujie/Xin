#pragma once

#include "Xin.UI.Types.h"
#include "Shape.h"

namespace Xin::UI
{
	class UI_API URectangle : public UShape
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

	public:
		URectangle() = default;
		URectangle(FName Name) : UShape(Name) {}
		~URectangle() = default;

	public:
		void OnPaint(IPainter & Painter) override;

	public:
		[[Meta(AffectsMeasure, AffectsRender)]]
		ProxyPropertyRW(FSize, Radius) = FSize::Zero;
	};
	using URectangleRef = TReferPtr<URectangle>;
}
