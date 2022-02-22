#include "PCH.h"
#include "RadioButton.h"

#include "View.h"

namespace Xin::UI
{
	URadioButton::URadioButton()
	{
	}

	void URadioButton::OnInitialized()
	{
		AssertExpr(Placed);
		View.RegisterRadioButton(this);
		UToggleButton::OnInitialized();
	}

	void URadioButton::OnFinalized()
	{
		AssertExpr(Placed);
		View.UnregisterRadioButton(this);
		UToggleButton::OnFinalized();
	}

	void URadioButton::OnPaint(IPainter & Painter)
	{
		//Painter.FillRect({ FPoint::Zero, ArrangeRect.Size }, Colors::Red);
		//FString Text = Content.Get<FString>();
	}

	void URadioButton::OnToggle()
	{
		if (Checked != true)
			Checked = true;
	}

	void URadioButton::OnCheck(FRoutedEventArgs & Args)
	{
		if (Placed)
			View.CheckRadioButton(this);
	}
}
