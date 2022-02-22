#pragma once

#include "Xin.UI.Types.h"
#include "Presenter.h"

namespace Xin::UI
{
	class UI_API UBorder : public UPresenter
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

	public:
		UBorder();
		UBorder(FName Name) : UPresenter(Name) {}
		~UBorder() = default;

	public:
		void OnConstruct() override;
		FSize OnMeasure(FSize ConstrainedSize) override;
		void OnArrange() override;
		void OnPaint(IPainter & Painter) override;

	public:
		[[Meta(AffectsRender)]]
		DependencyPropertyRW(IBrushRef, Fill) = nullptr;

		[[Meta(AffectsRender)]]
		DependencyPropertyRW(IBrushRef, Stroke) = nullptr;

		[[Meta(AffectsMeasure, FriendlyName("Border Thickness"), AffectsRender)]]
		DependencyPropertyRW(FThickness, Thickness) = 0.0;
	};
	using UBorderRef = TReferPtr<UBorder>;
}
