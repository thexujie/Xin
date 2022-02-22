#include "PCH.h"
#include "ContextMenu.h"

#include "ListControl.h"

namespace Xin::UI
{
	UContextMenu::UContextMenu() : UPopup()
	{
		WindowShowMode = EWindowShowMode::NoneActivate;
		MouseActivatable = false;
		Title = u8"CONTEXT MENU"T;
	}

	UContextMenu::~UContextMenu()
	{
	}

	void UContextMenu::OnConstruct()
	{
		UPopup::OnConstruct();
	}

	void UContextMenu::OnInitialize()
	{
		UPopup::OnInitialize();
	}

	void UContextMenu::OnFinalize()
	{
		UPopup::OnFinalize();
	}

	void UContextMenu::OnHidden()
	{
		Content = nullptr;
		UPopup::OnHidden();
	}

	void UContextMenu::ShowMenu(UElement * Element, const FVariant & Menu, EPopupPosition PlacementMode, TFunction<void()> OnClosed)
	{
		if (!Element || !Menu)
		{
			if (Shown)
			{
				Hide();
				TrackElement = nullptr;
			}
			return;
		}

		if (Shown && TrackElement != Element)
		{
			Hide();
			TrackElement = nullptr;
		}

		Closed = OnClosed;
		PopupPosition = PlacementMode;
		TrackElement = Element;
		//ResetValue(UWindow::WindowSizeProperty());
		WindowSize = FSize::NaN;
		Size = { FDimen::Auto, FDimen::Auto };
		Content = Menu;
		Show();
	}
}
