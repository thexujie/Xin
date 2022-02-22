#include "PCH.h"
#include "CheckBox.h"

namespace Xin::UI
{
	UCheckBox::UCheckBox()
	{
	}

	void UCheckBox::OnToggle()
	{
		UToggleButton::OnToggle();
	}

	void UCheckBox::OnCheck(FRoutedEventArgs & Args)
	{
		UToggleButton::OnCheck(Args);
	}
}
