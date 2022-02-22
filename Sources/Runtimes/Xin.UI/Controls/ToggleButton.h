#pragma once

#include "ButtonControl.h"

namespace Xin::UI
{
	class UI_API UToggleButton : public UButtonControl
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

		UToggleButton() = default;
		UToggleButton(FName Name) : UButtonControl(Name) {}
		~UToggleButton() = default;
		FStringV Describe() const override { return Format(u8"{}, Checked:{}"V, Content, Checked); }

	public:
		void OnConstruct() override;

	public:
		void OnClick(FRoutedEventArgs & Args) override;

	public:
		virtual void OnToggle();
		virtual void OnCheck(FRoutedEventArgs & Args) { Check(Args); }
		virtual void OnUnCheck(FRoutedEventArgs & Args) { UnCheck(Args); }
		virtual void OnIndeterminate(FRoutedEventArgs & Args) { Indeterminate(Args); }

	public:
		DependencyPropertyRW(bool, IsThreeState) = false;

		//[[Meta(AffectsState)]]
		DependencyPropertyRW(TOptional<bool>, Checked) = false;

	private:
		void OnCheckedChanged(TOptional<bool> ValueBefore);

	public:
		FRoutedEvent Check;
		FRoutedEvent UnCheck;
		FRoutedEvent Indeterminate;
	};
	using UToggleButtonRef = TReferPtr<UToggleButton>;
}
