#pragma once

#include "Xin.UI.Types.h"
#include "ToggleButton.h"

namespace Xin::UI
{
	class UI_API URadioButton : public UToggleButton
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

		URadioButton();
		URadioButton(FName Name) : UToggleButton(Name) {}
		~URadioButton() = default;

	public:
		void OnInitialized() override;
		void OnFinalized() override;

		void OnPaint(IPainter & Painter) override;

	public:
		void OnToggle() final;
		void OnCheck(FRoutedEventArgs & Args) final;

	public:
		DependencyPropertyRW(FName, GroupName) = FName::None;
	};
	using URadioButtonRef = TReferPtr<URadioButton>;
}
