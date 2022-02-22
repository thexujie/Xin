#include "PCH.h"
#include "ListBox.h"

#include "StackPanel.h"

namespace Xin::UI
{
	void UListBoxItem::OnConstruct()
	{
		UContentControl::OnConstruct();
	}

	void UListBoxItem::UpdateState()
	{
		if (Selected)
			SetState(u8"Selected"N);
		else if (IsMouseOver)
			SetState(u8"MouseOver"N);
		else
			SetState(u8"Normal"N);
	}

	void UListBoxItem::OnMouseDown(FMouseButtonEventArgs & Args)
	{
		UContentControl::OnMouseDown(Args);
		if (!Args.Handled)
		{
			if (UListBoxRef ListBox = Owner.Lock())
				ListBox->OnItemMouseDown(*this, Args);
		}
	}

	void UListBoxItem::OnMouseUp(FMouseButtonEventArgs & Args)
	{
		UContentControl::OnMouseUp(Args);
		if (!Args.Handled)
		{
			if (UListBoxRef ListBox = Owner.Lock())
				ListBox->OnItemMouseUp(*this, Args);
		}
	}

	UListBox::UListBox()
	{
		
	}

	void UListBox::OnConstruct()
	{
		UListControl::OnConstruct();
	}

	void UListBox::OnInitialize()
	{
		UListControl::OnInitialize();
	}

	void UListBox::OnInitialized()
	{
		UListControl::OnInitialized();
		PresentItems();
	}

	void UListBox::OnFinalize()
	{
		UnpresentItems();
		UListControl::OnFinalize();
	}

	void UListBox::OnItemMouseDown(UListBoxItem & ListBoxItem, FMouseButtonEventArgs & Args)
	{
		if (SelectMethod == ESelectMethod::Press)
		{
			AssertExpr(ListBoxItem.Index != NullIndex);
			SelectedIndex = ListBoxItem.Index;
			Args.Handled = true;
		}
	}

	void UListBox::OnItemMouseUp(UListBoxItem & ListBoxItem, FMouseButtonEventArgs & Args)
	{
		if (SelectMethod == ESelectMethod::Release)
		{
			AssertExpr(ListBoxItem.Index != NullIndex);
			SelectedIndex = ListBoxItem.Index;
			Args.Handled = true;
		}
	}

	void UListBox::OnSelectedIndexChanged(uintx SelectedIndexBefore)
	{
		if (SelectedIndexBefore != NullIndex)
		{
			UListBoxItemRef SelecctedItemBefore = Items[SelectedIndexBefore];
			SelecctedItemBefore->Selected = false;
		}

		if (SelectedIndex != NullIndex)
		{
			UListBoxItemRef SelecctedItemCurrent = Items[SelectedIndex];
			SelecctedItemCurrent->Selected = true;
			//FConsole::WriteLine(u8"Selected item: {}"V, SelecctedItemCurrent->Item);
		}
	}

	void UListBox::PresentItems()
	{
		if (!ItemsPanel || ItemsPresented)
			return;

		ItemsPresented = true;

		for (uintx ItemIndex = 0; ItemIndex < Items.Size; ++ItemIndex)
		{
			UListBoxItemRef & ListBoxItem = Items[ItemIndex];

			if (!ListBoxItem)
				continue;

			ListBoxItem->Owner = this;
			ListBoxItem->Index = ItemIndex;
			ItemsPanel->AddChild(ListBoxItem);
		}
	}

	void UListBox::UnpresentItems()
	{
		if (!ItemsPresented || !ItemsPanel)
			return;

		ItemsPresented = false;
		ItemsPanel->RemoveChildren();
	}
}
