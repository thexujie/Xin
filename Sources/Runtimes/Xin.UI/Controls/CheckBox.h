#pragma once

#include "Xin.UI.Types.h"
#include "ToggleButton.h"

namespace Xin::UI
{
	class UI_API UCheckBox : public UToggleButton
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

		UCheckBox();
		UCheckBox(FName Name) : UToggleButton(Name) {}
		~UCheckBox() = default;

	public:
		void OnToggle() final;
		void OnCheck(FRoutedEventArgs & Args) final;

	public:
		DependencyPropertyRW(FName, GroupName) = FName::None;
	};
	using UCheckBoxRef = TReferPtr<UCheckBox>;
}
