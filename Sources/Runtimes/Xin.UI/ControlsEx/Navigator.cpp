#include "PCH.h"
#include "Navigator.h"

namespace Xin::UI::ControlsEx
{
	UNavigatorHeader::UNavigatorHeader(UNavigator & Navigator) : Navigator(Navigator)
	{
		
	}

	void UNavigatorHeader::OnInitialized()
	{
		UListControl::OnInitialized();

		PresentItems();
	}

	void UNavigatorHeader::OnFinalize()
	{
		UListControl::OnFinalize();
	}

	void UNavigatorHeader::OnArrange()
	{
		UListControl::OnArrange();
	}

	void UNavigatorHeader::UpdateItems()
	{
		if (!ItemsPanel)
			return;

		ItemsPanel->Elements.ClearItems();
		PresentItems();
	}

	void UNavigatorHeader::IndicateItem(FName ItemName)
	{
		for (UNavigatorHeaderItemRef & HeaderItem : HeaderItems)
		{
			if (HeaderItem->ItemName != ItemName)
				HeaderItem->Indicating = false;
		}

		for (UNavigatorHeaderItemRef & HeaderItem : HeaderItems)
		{
			if (HeaderItem->ItemName == ItemName)
			{
				HeaderItem->Indicating = true;
				break;
			}
		}
	}

	void UNavigatorHeader::PresentItems()
	{
		if (!ItemsPanel)
			return;

		for (uintx ItemIndex = 0; ItemIndex < Navigator.NavigatorItems.Size; ++ItemIndex)
		{
			FNavigatorItem & Item = Navigator.NavigatorItems[ItemIndex];

			UNavigatorHeaderItemRef HeaderItem = MakeRefer<UNavigatorHeaderItem>(Navigator);
			HeaderItem->DisplayIndex = ItemIndex;
			HeaderItem->ItemName = Item.Name;
			HeaderItem->Content = Item.Header;
			HeaderItem->Indicating = Navigator.IndicatingIndex == ItemIndex;
			ItemsPanel->AddChild(HeaderItem);
			HeaderItems.Add(HeaderItem);
		}
	}

	UNavigatorHeaderItem::UNavigatorHeaderItem(UNavigator & Navigator) :Navigator(Navigator) {}

	void UNavigatorHeaderItem::OnConstruct()
	{
		UContentControl::OnConstruct();
	}

	void UNavigatorHeaderItem::UpdateState()
	{
		UContentControl::UpdateState();
	}

	void UNavigatorHeaderItem::OnMouseDown(FMouseButtonEventArgs & Args)
	{
		if (Args.Button == EMouseButton::Left)
		{
			Navigator.Navigate(ItemName);
			Args.Handled = true;
		}
		//UContentControl::OnMouseDown(Args);
	}

	void UNavigatorHeaderItem::OnIndicatingChanged(bool IndicatingBefore)
	{
	}

	UNavigator::UNavigator()
	{
	}

	void UNavigator::OnConstruct()
	{
		UControl::OnConstruct();
	}

	void UNavigator::OnInitialize()
	{
		UControl::OnInitialize();
		if (UElementRef Element = TemplateElement)
		{
			NavigatorHeader = Element->FindChildT<UNavigatorHeader>(Name_NavigatorHeader);
			NavigatorPresenter = Element->FindChildT<UPresenter>(Name_NavigatorPresenter);
		}

		if (IndicatingIndex != NullIndex)
			Navigate(NavigatorItems[IndicatingIndex].Name);
	}

	FSize UNavigator::OnMeasure(FSize ConstrainedSize)
	{
		return UControl::OnMeasure(ConstrainedSize);
	}

	void UNavigator::OnArrange()
	{
		UControl::OnArrange();
	}

	void UNavigator::Navigate(FName ItemName)
	{
		if (NavigatorHeader)
			NavigatorHeader->IndicateItem(ItemName);

		for (FNavigatorItem & NavigatorItem : NavigatorItems)
		{
			if (NavigatorItem.Name == ItemName)
			{
				if (NavigatorPresenter)
					NavigatorPresenter->Child = NavigatorItem.Element;
				break;
			}
		}
	}

	void UNavigator::OnItemsChanged(TList<FNavigatorItem> & List)
	{
		if (NavigatorHeader)
			NavigatorHeader->UpdateItems();
	}

	void UNavigator::OnIndicatingIndexChanged(uintx IndicatingIndexBefore)
	{
		Navigate(NavigatorItems[IndicatingIndex].Name);
	}
}
