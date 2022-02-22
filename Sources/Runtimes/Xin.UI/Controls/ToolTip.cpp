#include "PCH.h"
#include "ToolTip.h"

namespace Xin::UI
{
	UToolTip::UToolTip() : UPopup()
	{
		WindowShowMode = EWindowShowMode::NoneActivate;
		HitInside = EHitResult::Ignore;
		Title = u8"CONTEXT MENU"T;

		ToolTipTimer.Tick += {this, & UToolTip::OnTimerTick };
	}

	UToolTip::~UToolTip()
	{
		
	}

	void UToolTip::OnConstruct()
	{
		UPopup::OnConstruct();
	}

	void UToolTip::OnInitialize()
	{
		UPopup::OnInitialize();
	}

	void UToolTip::OnFinalize()
	{
		ToolTipTimer.Stop();
		ToolTipElement = nullptr;

		UPopup::OnFinalize();
	}

	void UToolTip::OnHidden()
	{
		UPopup::OnHidden();
	}

	void UToolTip::OnTrackElementChanged(UElementObj TrackElementBefore)
	{
		UPopup::OnTrackElementChanged(TrackElementBefore);

		if (UElementRef Element = TrackElementBefore.Lock())
			Element->MouseMove -= { this, &UToolTip::OnHostMouseMove };
		if (UElementRef Element = TrackElement.Lock())
			Element->MouseMove += { this, &UToolTip::OnHostMouseMove };
	}

	void UToolTip::ShowElementToolTip(UElement * Element)
	{
		if (ToolTipElement == Element)
			return;

		if (Element && !Element->ToolTip)
		{
			if (Shown)
			{
				Hide();
				TrackElement = nullptr;
			}
			ToolTipElement = nullptr;
			return;
		}

		if (Shown && TrackElement != Element)
		{
			Hide();
			TrackElement = nullptr;
		}

		ToolTipElement = Element;
		if (Element)
		{
			ToolTipTimer.Interval = Element->GetValueT(FToolTip::DelayProperty(), DefaultToolTipDelay);
			ToolTipTimer.Start();
		}
	}

	void UToolTip::OnTimerTick()
	{
		AssertExpr(ToolTipTimer.StartTime > 0);
		ToolTipTimer.Stop();

		if (UElementRef Element = ToolTipElement.Lock())
		{
			if (TrackElement == Element.Get())
			{
				TrackElement = nullptr;
				Hide();
			}
			else
			{
				TrackElement = Element;

				Size = { FDimen::Auto, FDimen::Auto };
				Content = Element->ToolTip;
				PopupPosition = Element->GetValueT(FToolTip::PlacementProperty(), EPopupPosition::Bottom);
				Show();

				uint32 Duration = Element->GetValueT(FToolTip::DurationProperty(), DefaultToolTipDuration);
				if (Duration > 0)
				{
					ToolTipTimer.Interval = Duration;
					ToolTipTimer.Start();
				}
			}
		}
		else
			Hide();
	}

	void UToolTip::OnHostMouseMove(FMouseEventArgs & EventArgs)
	{
		switch (PopupPosition)
		{
		case EPopupPosition::Mouse:
		case EPopupPosition::Cursor:
			UpdateWindowLocation();
			break;
		default:
			break;
		}
	}
}
