#include "PCH.h"
#include "ListView.h"

#include "Window.h"
#include "Menu.h"
#include "StackPanel.h"

namespace Xin::UI
{
	FSize UListViewHeader::OnMeasure(FSize ConstrainedSize)
	{
		return UContentControl::OnMeasure(ConstrainedSize);
	}

	void UListViewHeader::OnArrange()
	{
		if (UElementRef Element = TemplateElement)
			Element->Arrange({ { -ScrollOffset, 0 }, Element->DesiredSize });
	}

	UListViewColumnHeader::UListViewColumnHeader(UListView & ListView, const FListViewColumn & ListViewColumn) : ListView(ListView), ListViewColumn(ListViewColumn)
	{
	}

	void UListViewColumnHeader::OnConstruct()
	{
		UContentControl::OnConstruct();
	}

	void UListViewColumnHeader::UpdateState()
	{
		if (Indicating)
			SetState(u8"Indicating"N);
		else if (Resizing)
			SetState(u8"Resizing"N);
		else 
			UContentControl::UpdateState();
	}

	void UListViewColumnHeader::OnMouseDown(FMouseButtonEventArgs & Args)
	{
		UContentControl::OnMouseDown(Args);

		if (Args.Button == EMouseButton::Left)
		{
			SortOrder = (SortOrder == ESortOrder::Ascend) ? ESortOrder::Descend : ESortOrder::Ascend;
			ListView.SortKey = { ListViewColumn.SortKey, SortOrder };
		}
	}

	UElementRef UListViewItemCell::LoadContent()
	{
		return UContentControl::LoadContent();
	}

	void UListViewItem::OnConstruct()
	{
		UContentControl::OnConstruct();
	}

	void UListViewItem::UpdateState()
	{
		if (Selected)
			SetState(u8"Selected"N);
		else if (IsMouseOver)
			SetState(u8"MouseOver"N);
		else
			SetState(u8"Normal"N);
	}

	UElementRef UListViewItem::LoadContent()
	{
		if (!__Content)
			return nullptr;

		if (!ItemsPanel)
		{
			UStackPanelRef StackPanel = MakeRefer<UStackPanel>();
			StackPanel->Orientation = EOrientation::Horizontal;
			ItemsPanel = StackPanel;
		}

		TList<UElementRef> Elements;
		for (uintx ColumnIndex = 0; ColumnIndex < ListView.Columns.Size; ++ColumnIndex)
		{
			const FListViewColumn & ListViewColumn = ListView.Columns[ColumnIndex];

			UListViewCellRef ListViewCell = MakeRefer<UListViewItemCell>(*this);

			ListViewCell->ContentHorizontalAlignment = ListViewColumn.HorizontalAlignment;
			ListViewCell->ContentTemplate = ListViewColumn.DataTemplate;
			ListViewCell->Content = Binding(Content.Get<IDependency *>(), ListViewColumn.BindingPath, EBindingMode::OneWay);
			ListViewCell->Size = Binding(ListView.ColumnHeaders[ColumnIndex].Ref(), UElement::FinalRectProperty(),
				[Cell = ListViewCell.Get()](const FRect & FinalRect)
				{
					return FDimen2 { FinalRect.Width, Cell->Height };
				});

			Cells.Add(ListViewCell);
		}
		ItemsPanel->Elements = Cells;
		return ItemsPanel;
	}

	void UListViewItem::OnMouseDown(FMouseButtonEventArgs & Args)
	{
		UContentControl::OnMouseDown(Args);
		if (!Args.Handled)
			ListView.OnItemMouseDown(*this, Args);
	}

	void UListViewItem::OnMouseUp(FMouseButtonEventArgs & Args)
	{
		UContentControl::OnMouseUp(Args);
		if (!Args.Handled)
			ListView.OnItemMouseUp(*this, Args);
	}


	FListViewQueryItemContextMenuEventArgs::FListViewQueryItemContextMenuEventArgs(UListViewItem & ListViewItem) : ListViewItem(ListViewItem) { }

	FListViewQueryItemContextMenuEventArgs::~FListViewQueryItemContextMenuEventArgs() { }

	UListView::UListView() { }

	void UListView::OnInitialize()
	{
		UListControl::OnInitialize();
	}

	void UListView::OnInitialized()
	{
		UListControl::OnInitialized();

		PresentItems();
	}

	void UListView::OnFinalize()
	{
		ColumnHeaders.Clear();
		UListControl::OnFinalize();
	}

	void UListView::OnAddItem(FVariant & Item)
	{
		if (!ItemsPanel)
			return;

		UListViewItemRef ListViewItem = MakeRefer<UListViewItem>(*this);
		ListViewItem->Index = ListViewItems.Size;
		ListViewItem->Content = Item;

		ListViewItems.Add(ListViewItem);
		ItemsPanel->AddChild(ListViewItem);
	}

	void UListView::OnRemoveItem(FVariant & Item)
	{
		if (!ItemsPanel)
			return;

		for (uintx ItemIndex = 0; ItemIndex < ListViewItems.Size; ++ItemIndex)
		{
			UListViewItemRef ListViewItem = ListViewItems[ItemIndex];
			if (ListViewItem->Content == Item)
			{
				if (SelectedItem == ListViewItem)
					SelectedItem = nullptr;

				AssertExpr(ListViewItem->Parent.Lock().Get() == ItemsPanel.Get());
				AssertExpr(ItemsPanel->Elements.Contains(ListViewItem));
				ItemsPanel->RemoveChild(ListViewItem);
				AssertExpr(!ListViewItem->Parent.Lock());
				AssertExpr(!ItemsPanel->Elements.Contains(ListViewItem));
				ListViewItems.EraseAt(ItemIndex);
				break;
			}
		}

		for (uintx ItemIndex = 0; ItemIndex < ListViewItems.Size; ++ItemIndex)
		{
			UListViewItemRef ListViewItem = ListViewItems[ItemIndex];
			ListViewItems[ItemIndex]->Index = ItemIndex;
		}
	}

	void UListView::PresentItems()
	{
		if (!ItemsPanel)
			return;

		for (uintx ItemIndex = 0; ItemIndex < Items.Size; ++ItemIndex)
		{
			FVariant & VariantItem = Items[ItemIndex];

			if (!VariantItem)
				continue;

			UListViewItemRef ListViewItem = MakeRefer<UListViewItem>(*this);
			ListViewItem->Index = ItemIndex;
			ListViewItem->Content = VariantItem;

			ListViewItems.Add(ListViewItem);
			ItemsPanel->AddChild(ListViewItem);
		}
	}

	void UListView::OnSortKeyChanged(FSortKey SortDescriptorBefore)
	{
		if (ItemsPanel && SortKey)
		{
			for (const FListViewSortDescriptor & SortDescriptor : SortDescriptors)
			{
				if (SortDescriptor.Key == SortKey.Name)
				{
					ItemsPanel->SortComparer = { this, [SortOrder = SortKey.Order, SortComparer = SortDescriptor.SortComparer](const UElementRef & A, const UElementRef & B)
						{
							bool Result = SortComparer(StaticCast<UListViewItem>(A), StaticCast<UListViewItem>(B));
							return SortOrder == ESortOrder::Descend ? !Result : Result;
						}
					};
					break;
				}
			}
		}
		else
			ItemsPanel->SortComparer = nullptr;
	}

	void UListView::SortItems()
	{
		if (ItemsPanel)
			ItemsPanel->SortElements();
	}

	void UListView::OnItemMouseDown(UListViewItem & ListViewItem, FMouseButtonEventArgs & Args)
	{
		if (Args.Button == EMouseButton::Left)
		{
			if (SelectMethod == ESelectMethod::Press)
			{
				AssertExpr(ListViewItem.Index != NullIndex);
				SelectedItem = &ListViewItem;
				Args.Handled = true;
			}
		}
	}

	void UListView::OnItemMouseUp(UListViewItem & ListViewItem, FMouseButtonEventArgs & Args)
	{
		if (Args.Button == EMouseButton::Left)
		{
			if (SelectMethod == ESelectMethod::Release)
			{
				AssertExpr(ListViewItem.Index != NullIndex);
				SelectedItem = &ListViewItem;
				Args.Handled = true;
			}
		}

		if (Args.Button == EMouseButton::Right)
		{
			SelectedItem = &ListViewItem;

			FListViewQueryItemContextMenuEventArgs EventArgs { ListViewItem };
			QueryItemContextMenu(EventArgs);
			if (EventArgs.Menu)
			{
				ShowContextMenu(&ListViewItem, EventArgs.Menu, EPopupPosition::Mouse);
				Args.Handled = true;
			}
		}
	}

	void UListView::QueryItemContextMenu(FListViewQueryItemContextMenuEventArgs & Args)
	{
		if (OnQueryItemContextMenu.Binded())
			OnQueryItemContextMenu(Args);
	}

	void UListView::OnSelectedItemChanged(UListViewItemRef SelectedItemBefore)
	{
		if (SelectedItemBefore)
			SelectedItemBefore->Selected = false;

		if (SelectedItem)
			SelectedItem->Selected = true;
	}


	void UListView::OnHeaderThumbDragDelta(uintx ColumnIndex, dimenx Delta)
	{
		ColumnHeaders[ColumnIndex]->Width = ColumnHeaders[ColumnIndex]->FinalSize.Width + Delta;
	}

	void UListView::OnIndicatingIndexChanged(FSplitterPanelIndicatingIndexChagnedEventArgs & EventArgs)
	{
		if (EventArgs.IndicatingIndexBefore != NullIndex)
			ColumnHeaders[EventArgs.IndicatingIndexBefore]->Indicating = false;
		if (EventArgs.IndicatingIndex != NullIndex)
			ColumnHeaders[EventArgs.IndicatingIndex]->Indicating = true;
	}

	void UListView::OnListPanelMeasured(FEventArgs & Args)
	{
		for (uintx ColumnIndex = 0; ColumnIndex < Columns.Size; ++ColumnIndex)
		{
			const FListViewColumn & GridViewColumn = Columns[ColumnIndex];
			UListViewColumnHeaderRef & ColumnHeader = ColumnHeaders[ColumnIndex];
			if (not GridViewColumn.Width.IsAuto() || not ColumnHeader->Width.IsAuto())
				break;

			if (ListViewItems.Size)
			{
				dimenx ColumnWidth = 0.0dim;
				for (uintx ItemIndex = 0; ItemIndex < ListViewItems.Size; ++ItemIndex)
				{
					UListViewItemRef & ListViewItem = ListViewItems[ItemIndex];
					UListViewCellRef Cell = ListViewItem->Cells[ColumnIndex];
					ColumnWidth = Max(ColumnWidth, Cell->DesiredSize.Width);
				}
				//ColumnHeader->Width = ColumnWidth;
			}
		}
	}
}
