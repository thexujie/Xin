#pragma once

#include "Panel.h"

namespace Xin::UI
{
	class UI_API UOverlapPanel : public UPanel
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

	public:
		UOverlapPanel() = default;
		UOverlapPanel(FName Name) : UPanel(Name) {}
		~UOverlapPanel() = default;

	public:
		void OnInitialize() override;
		FSize OnMeasure(FSize ConstrainedSize) override;
		void OnArrange() override;

	public:
		PropertyMetadata(Inherits, Default(EElementAlignment::Fill));
		DependencyPropertyRW(EElementAlignment, ElementHorizontalAlignment) = EElementAlignment::Stretch;

		PropertyMetadata(Inherits);
		DependencyPropertyRW(EElementAlignment, ElementVerticalAlignment) = EElementAlignment::Stretch;
	};
	using UOverlapPanelRef = TReferPtr<UOverlapPanel>;
}
