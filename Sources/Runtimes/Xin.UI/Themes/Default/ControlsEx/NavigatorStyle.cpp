#include "PCH.h"
#include "NavigatorStyle.h"

#include "Controls/Border.h"
#include "Controls/StackPanel.h"

namespace Xin::UI::ControlsEx
{
	UElementRef FNavigatorHeaderItemTemplate::LoadTemplateT(UNavigatorHeaderItem & NavigatorHeaderItem)
	{
		UOverlapPanelRef DockPanel = MakeRefer<UOverlapPanel>();
		UBorderRef ContentPresenter = MakeRefer<UBorder>();
		{
			ContentPresenter->Name = UNavigatorHeaderItem::PART_ContentPresenter;
			ContentPresenter->Padding = Binding(NavigatorHeaderItem, UControl::PaddingProperty());
		}
		UHLineRef IndicateLine = MakeRefer<UHLine>();
		{
			IndicateLine->StrokeThickness = 2.0f;
			IndicateLine->Stroke = MakeRefer<FSolidColorBrush>(Themes::Default::GDefaultTheme.Primary.NormalColor);
			IndicateLine->VerticalAlignment = EElementAlignment::Far;
			IndicateLine->Visibility = Binding(NavigatorHeaderItem, UNavigatorHeaderItem::IndicatingProperty(),
				[](bool Indicating)
				{
					return Indicating ? EVisibility::Visible : EVisibility::Hidden;
				});
		}
		DockPanel->Elements = { ContentPresenter, IndicateLine };
		return DockPanel;
	}

	void FNavigatorHeaderItemStyle::LoadStyleT(UNavigatorHeaderItem & NavigatorHeaderItem)
	{
		NavigatorHeaderItem.Padding = { 16, 10 };
	}

	UElementRef FNavigatorHeaderTemplate::LoadTemplateT(UNavigatorHeader & NavigatorHeader)
	{
		UBorderRef Border = MakeRefer<UBorder>();
		{
			UStackPanelRef ItemsPanel = MakeRefer<UStackPanel>(UListControl::Name_ItemsPanel);
			ItemsPanel->Orientation = EOrientation::Horizontal;
			Border->Child = ItemsPanel;
		}
		//Border->Fill = Brushs::Gray;
		Border->Stroke = MakeRefer<FSolidColorBrush>(Themes::Default::GDefaultTheme.Shape.NormalColor);
		Border->Thickness = { 0, 0, 0, 2 };
		return Border;
	}

	void FNavigatorHeaderStyle::LoadStyleT(UNavigatorHeader & NavigatorHeader)
	{
		//NavigatorHeader.Padding = { 0, 6 };
	}

	UElementRef FNavigatorTemplate::LoadTemplateT(UNavigator & Navigator)
	{
		UStackPanelRef MainPanel = MakeRefer<UStackPanel>();
		MainPanel->Orientation = EOrientation::Vertical;

		UNavigatorHeaderRef Header = MakeRefer<UNavigatorHeader>(Navigator);
		{
			Header->Name = UNavigator::Name_NavigatorHeader;
		}

		UBorderRef ItemPanel = MakeRefer<UBorder>();
		{
			ItemPanel->Name = UNavigator::Name_NavigatorPresenter;
			ItemPanel->Margin = 4;
			ItemPanel->Padding = 4;

			ItemPanel->Height = 100pct;
			//ElementsPresenter->Stroke = Brushs::Red;
			//ElementsPresenter->Thickness = 2;
		}

		MainPanel->Elements = { Header, ItemPanel };
		return MainPanel;
	}

	void FNavigatorStyle::LoadStyleT(UNavigator & Navigator)
	{
		
	}
}
