#include "PCH.h"
#include "Popup.h"

namespace Xin::UI
{
	UPopup::UPopup() : UWindow()
	{
		WindowStyle = EWindowStyle::Frameless;
	}

	UPopup::~UPopup()
	{
		
	}

	void UPopup::OnInitialize()
	{
		UWindow::OnInitialize();
	}

	void UPopup::OnFinalize()
	{
		TrackElement = nullptr;

		UWindow::OnFinalize();
	}

	void UPopup::OnDesiredSizeChanged(FSize DesiredSizeBefore)
	{
		UWindow::OnDesiredSizeChanged(DesiredSizeBefore);
		UpdateWindowLocation();
	}

	void UPopup::OnShowing(FEventArgs & Args)
	{
		UWindow::OnShowing(Args);
		UpdateWindowLocation();
	}

	void UPopup::UpdateWindowLocation()
	{
		FPoint Location = FPoint::NaN;

		switch (PopupPosition)
		{
		case EPopupPosition::Bottom:
			if (UElementRef Element = TrackElement.Lock())
			{
				FPoint ShowLocation = Element->LocalToGlobal({ (Element->FinalSize.Width - DesiredSize.Width) * 0.5dim, Element->FinalSize.Height });
				Location = { ShowLocation.X, ShowLocation.Y + PlacementOffset.Y };
			}
			break;
		case EPopupPosition::Right:
			if (UElementRef Element = TrackElement.Lock())
			{
				FPoint ShowLocation = Element->LocalToGlobal({ Element->FinalSize.Width, 0dim});
				Location = { ShowLocation.X, ShowLocation.Y + PlacementOffset.Y };
			}
			break;
		case EPopupPosition::Mouse:
			Location = Desktop.GetCursorPosition();
			break;
		case EPopupPosition::Cursor:
		{
			FPoint MousePosition = Desktop.GetCursorPosition();
			FSize CursorSize = Desktop.GetCursorSize();
			Location = FPoint { MousePosition.X, MousePosition.Y + CursorSize.Height } + PlacementOffset;
			break;
		}
		default:
			break;
		}
		if (Location.AnyNaN())
			return;

		UWindowRef OwnerWindow = Owner.Lock();
		if (!OwnerWindow)
		{
			// god bless you !!!
			WindowLocation = Location;
			return;
		}

		FRect WorkingRect = FRect(Desktop.GetWorkingRect());

		if (Location.X < WorkingRect.X)
			Location.X = WorkingRect.X;
		else if (Location.X + DesiredSize.Width > WorkingRect.Right)
			Location.X = WorkingRect.Right - DesiredSize.Width;
		else { }

		if (Location.Y < WorkingRect.Y)
			Location.Y = WorkingRect.Y;
		else if (Location.Y + DesiredSize.Height > WorkingRect.Bottom)
			Location.Y = WorkingRect.Bottom - DesiredSize.Height;
		else { }

		WindowLocation = Location;
	}
}
