#include "PCH.h"
#include "Menu.h"

#include "StackPanel.h"
#include "Window.h"

namespace Xin::UI
{
	void UMenuItem::OnConstruct()
	{
		UContentControl::OnConstruct();
	}

	void UMenuItem::UpdateState()
	{
		if (Selected)
			SetState(u8"Selected"N);
		else if (Indicating)
			SetState(u8"Indicating"N);
		else
			SetState(u8"Normal"N);
	}

	void UMenuItem::OnMouseEnter(FMouseEventArgs & Args)
	{
		UContentControl::OnMouseEnter(Args);
		if (!Args.Handled)
		{
			if (UMenuRef Menu = Owner.Lock())
				Menu->OnItemMouseEnter(*this, Args);
		}
	}

	void UMenuItem::OnMouseLeave(FMouseEventArgs & Args)
	{
		UContentControl::OnMouseLeave(Args);
		if (!Args.Handled)
		{
			if (UMenuRef Menu = Owner.Lock())
				Menu->OnItemMouseLeave(*this, Args);
		}
	}

	void UMenuItem::OnMouseDown(FMouseButtonEventArgs & Args)
	{
		UContentControl::OnMouseDown(Args);
		if (!Args.Handled)
		{
			if (UMenuRef Menu = Owner.Lock())
				Menu->OnItemMouseDown(*this, Args);
		}
	}

	void UMenuItem::OnMouseUp(FMouseButtonEventArgs & Args)
	{
		UContentControl::OnMouseUp(Args);

		if (!Args.Handled)
		{
			FRoutedEventArgs ClickArgs { *this };
			Click(ClickArgs);

			if (UMenuRef Menu = Owner.Lock())
				Menu->OnItemMouseUp(*this, Args);
		}
	}

	void UMenuItem::OnIndicatingChanged(bool IndicatingBefore)
	{
		//if (Submenu.Valid())
		//{
		//	if (!IndicatingBefore)
		//		Window.ShowContextMenu(this, Submenu, EPlacementMode::Right);
		//	else
		//		Window.ShowContextMenu(this, noval, EPlacementMode::Right);
		//}
	}

	void UMenuSaperator::OnConstruct()
	{
		SelecteAble = false;
		UMenuItem::OnConstruct();
	}

	UMenu::UMenu()
	{
		
	}

	FStringV UMenu::Describe() const
	{
		return Format(u8"Indicating[{}], Hovering[{}]"V, IndicatingIndex, HoveringIndex);
	}

	void UMenu::OnConstruct()
	{
		UListControl::OnConstruct();
	}

	void UMenu::OnInitialize()
	{
		UListControl::OnInitialize();

		PresentItems();
	}

	void UMenu::OnFinalize()
	{
		UnpresentItems();
		UListControl::OnFinalize();
	}

	void UMenu::OnHidden()
	{
		IndicatingIndex = NullIndex;
		HoveringIndex = NullIndex;
		UListControl::OnHidden();
	}

	void UMenu::PreKeyDown(FKeyboardEventArgs & Args)
	{
		UListControl::PreKeyDown(Args);

		if (Args.Handled || Items.Size == 0)
			return;

		switch (Args.Key)
		{
		case EKeyCode::Up:
			IndicatingIndex = (IndicatingIndex == NullIndex) ? (Items.Size - 1) : (IndicatingIndex + Items.Size - 1) % Items.Size;
			Args.Handled = true;
			break;
		case EKeyCode::Down:
			IndicatingIndex = (IndicatingIndex == NullIndex) ? 0 : (IndicatingIndex + Items.Size + 1) % Items.Size;
			Args.Handled = true;
			break;
		case EKeyCode::Right:
			ShowSubmenu();
			Args.Handled = true;
			break;
		case EKeyCode::Left:
		case EKeyCode::Escape:
			Window.Close();
			Args.Handled = true;
			break;
		//case EKeyCode::Left:
		//	if (SubmenuShowing)
		//	{
		//		HideSubmenu();
		//		Args.Handled = true;
		//	}
		//	break;
		default:
			break;
		}
	}

	void UMenu::OnItemMouseEnter(UMenuItem & MenuItem, FMouseEventArgs & Args)
	{
		HoveringIndex = MenuItem.Index;
	}

	void UMenu::OnItemMouseLeave(UMenuItem & MenuItem, FMouseEventArgs & Args)
	{
		HoveringIndex = NullIndex;
	}

	void UMenu::OnItemMouseDown(UMenuItem & MenuItem, FMouseButtonEventArgs & Args)
	{
		if (SelectMethod == ESelectMethod::Press && MenuItem.SelecteAble)
		{
			AssertExpr(MenuItem.Index != NullIndex);

			FMenuItemSelectedEventArgs ItemSelectedEventArgs { *this, MenuItem };
			OnItemSelected(ItemSelectedEventArgs);
			Args.Handled = ItemSelectedEventArgs.Handled;
		}
	}

	void UMenu::OnItemMouseUp(UMenuItem & MenuItem, FMouseButtonEventArgs & Args)
	{
		Args.Handled = true;
		if (SelectMethod == ESelectMethod::Release && MenuItem.SelecteAble)
		{
			FMenuItemSelectedEventArgs ItemSelectedEventArgs { *this, MenuItem };
			OnItemSelected(ItemSelectedEventArgs);
			Args.Handled = ItemSelectedEventArgs.Handled;
		}
	}

	void UMenu::OnItemSelected(FMenuItemSelectedEventArgs & EventArgs)
	{
		ItemSelected(EventArgs);

		if (!EventArgs.Handled)
		{
			if (auto P = MenuParent.Lock())
				P->OnItemSelected(EventArgs);
		}

		if (!EventArgs.Handled)
		{
			Window.Close();
			EventArgs.Handled = true;
		}
	}

	void UMenu::PresentItems()
	{
		if (!ItemsPanel || ItemsPresented)
			return;

		ItemsPresented = true;
		for (uintx ItemIndex = 0; ItemIndex < Items.Size; ++ItemIndex)
		{
			UMenuItemRef & MenuItem = Items[ItemIndex];

			if (!MenuItem)
				continue;

			MenuItem->Owner = this;
			MenuItem->Index = ItemIndex;

			ItemsPanel->AddChild(MenuItem); 
		}
	}

	void UMenu::UnpresentItems()
	{
		if (!ItemsPresented)
			return;

		if (ItemsPanel)
			ItemsPanel->RemoveChildren();
		ItemsPresented = false;
	}

	void UMenu::ShowSubmenu()
	{
		if (IndicatingIndex != NullIndex)
		{
			UMenuItemRef & MenuItem = Items[IndicatingIndex];
			FVariant Submenu = MenuItem->Submenu;
			if (Submenu.IsA<UMenu>())
			{
				Submenu.Get<UMenu &>().MenuParent = this;
			}
			ShowContextMenu(MenuItem, MenuItem->Submenu, EPopupPosition::Right);
			SubmenuShowing = !!MenuItem->Submenu;
		}
	}

	void UMenu::HideSubmenu()
	{
		SubmenuShowing = false;
		ShowContextMenu(nullptr, noval, EPopupPosition::Right);
	}

	void UMenu::OnIndicatingIndexChanged(intx IndicatingIndexBefore)
	{
		if (IndicatingIndexBefore != NullIndex)
		{
			UMenuItemRef & MenuItem = Items[IndicatingIndexBefore];
			MenuItem->Indicating = false;
		}

		if (IndicatingIndex != NullIndex)
		{
			UMenuItemRef & MenuItem = Items[IndicatingIndex];
			MenuItem->Indicating = true;
			if (IndicatingIndex == HoveringIndex)
				ShowSubmenu();
		}
		else
		{
			HideSubmenu();
		}
	}

	void UMenu::OnHoveringIndexChanged(intx HoveringIndexBefore)
	{
		if (HoveringIndex != NullIndex)
		{
			IndicatingIndex = HoveringIndex;
		}
		else
		{
			if (!SubmenuShowing)
				IndicatingIndex = NullIndex;
		}
	}

	UMenuRef Menu(TView<FVariant> Items)
	{
		UMenuRef Menu = MakeRefer<UMenu>();
		TList<UMenuItemRef> MenuItems;
		for (const FVariant & Item: Items)
		{
			if (Item.IsA<UMenuItem>())
				MenuItems.Add(Item.Get<UMenuItemRef>());
			else
			{
				UMenuItemRef MenuItem = MakeRefer<UMenuItem>();
				MenuItem->Content = Item;
				MenuItems.Add(MenuItem);
			}
		}
		Menu->Items = MenuItems;
		return Menu;
	}
}
