#pragma once

#include "Panel.h"

namespace Xin::UI
{
	class UI_API UStackPanel : public UPanel
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

	public:
		UStackPanel() = default;
		UStackPanel(FName Name) : UPanel(Name) {}
		~UStackPanel() = default;

	public:
		void OnInitialize() override;
		FSize OnMeasure(FSize ConstrainedSize) override;
		void OnArrange() override;

	public:
		DependencyPropertyRW(EOrientation, Orientation) = EOrientation::Horizontal;

		[[Meta(AffectsArrange)]]
		ProxyPropertyRW(FVec2, ScrollOffset) = FVec2::Zero;

		[[Meta(AffectsArrange)]]
		DependencyPropertyRW(EElementAlignment, ElementAlignment) = EElementAlignment::Stretch;
	};
	using UStackPanelRef = TReferPtr<UStackPanel>;
}
