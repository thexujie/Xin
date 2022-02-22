#include "PCH.h"
#include "ToggleButton.h"

namespace Xin::UI
{
	void UToggleButton::OnConstruct()
	{
		Focusable = true;
		ClickMode = EClickMode::Press;
		UButtonControl::OnConstruct();
	}

	void UToggleButton::OnClick(FRoutedEventArgs & Args)
	{
		UButtonControl::OnClick(Args);
		if (!Args.Handled)
			OnToggle();
	}

	void UToggleButton::OnToggle()
	{
		// true false null
		TOptional<bool> NextValue = Checked;
		if (not NextValue.HasValue)
			NextValue = true;
		else if (NextValue.Value)
			NextValue = false;
		else
			NextValue = IsThreeState ? noval : TOptional<bool>(true);
		Checked = NextValue;
	}

	void UToggleButton::OnCheckedChanged(TOptional<bool> ValueBefore)
	{
		TOptional<bool> Value = Checked;

		FRoutedEventArgs RoutedEventArgs { *this };
		if (not Value.HasValue)
			OnIndeterminate(RoutedEventArgs);
		else if (Value.Value)
			OnCheck(RoutedEventArgs);
		else
			OnUnCheck(RoutedEventArgs);
	}
}
