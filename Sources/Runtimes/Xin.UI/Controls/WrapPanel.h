#pragma once

#include "Panel.h"

namespace Xin::UI
{
	class UI_API UWrapPanel : public UPanel
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

	public:
		UWrapPanel() = default;
		UWrapPanel(FName Name) : UPanel(Name) {}
		~UWrapPanel() = default;

	public:
		void OnInitialize() override;
		FSize OnMeasure(FSize ConstrainedSize) override;
		void OnArrange() override;

	public:
		DependencyPropertyRW(EOrientation, Orientation) = EOrientation::Horizontal;

		[[Meta(AffectsArrange)]]
		DependencyPropertyRW(EElementAlignment, ElementAlignment) = EElementAlignment::Near;

		DependencyPropertyRW(FDimen2, ItemSize);
	};
	using UWrapPanelRef = TReferPtr<UWrapPanel>;
}
