#include "PCH.h"
#include "RepeatButton.h"

namespace Xin::UI
{
	void URepeatButton::OnConstruct()
	{
		ClickMode = EClickMode::Press;
		DispatcherTimer.Tick += { this, &URepeatButton::DispatcherTimer_OnTick };
		UButtonControl::OnConstruct();
	}

	void URepeatButton::OnMouseDown(FMouseButtonEventArgs & Args)
	{
		if (Args.Button == EMouseButton::Left)
		{
			RepeatIndex = 0;
			DispatcherTimer.Interval = Delay;
			DispatcherTimer.Start();
		}
		UButtonControl::OnMouseDown(Args);
	}

	void URepeatButton::OnMouseUp(FMouseButtonEventArgs & Args)
	{
		if (Args.Button == EMouseButton::Left && DispatcherTimer.Ticking)
		{
			DispatcherTimer.Stop();
		}
		UButtonControl::OnMouseUp(Args);
	}

	void URepeatButton::OnMouseLeave(FMouseEventArgs & Args)
	{
		if (DispatcherTimer.Ticking)
			DispatcherTimer.Stop();
		UButtonControl::OnMouseLeave(Args);
	}

	void URepeatButton::DispatcherTimer_OnTick()
	{
		if (!RepeatIndex)
		{
			++RepeatIndex;
			DispatcherTimer.Interval = Interval;
			DispatcherTimer.Start();
		}
		OnClick();
	}
}
