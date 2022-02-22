#include "PCH.h"
#include "ListViewStyle.h"

namespace Xin::UI::Themes::Default
{
	UElementRef FListViewHeaderTemplate::LoadTemplateT(UListViewHeader & ListViewHeader)
	{
		UOverlapPanelRef DockPanel = MakeRefer<UOverlapPanel>();
		UBorderRef ContentPresenter = MakeRefer<UBorder>();
		{
			ContentPresenter->Name = UContentControl::PART_ContentPresenter;
			//ContentPresenter->Size = { FDimen::Auto, FDimen::Auto };
		}
		UHLineRef Line = MakeRefer<UHLine>();
		{
			Line->Stroke = MakeRefer<FSolidColorBrush>(GDefaultTheme.BorderColor);
			Line->StrokeThickness = 2.0f;
			Line->VerticalAlignment = EElementAlignment::Far;
			Line->ZIndex = IntXMax;
		}
		//DockPanel->Size = { FDimen::Auto, FDimen::Auto };
		DockPanel->Elements = { ContentPresenter, Line };
		return DockPanel;
	}

	UElementRef FListViewColumnHeaderTemplate::LoadTemplateT(UListViewColumnHeader & ListViewColumnHeader)
	{
		FSolidColorBrushRef BackgroundBrush = MakeRefer<FSolidColorBrush>(GDefaultTheme.Button.NormalColor);
		{
			ListViewColumnHeader.Background = BackgroundBrush;
		}
		//ListViewColumnHeader.Background = Brushs::Beige;
		UBorderRef Border = MakeRefer<UBorder>(UContentControl::PART_ContentPresenter);
		{
			//Border->Width = 100pct;
			//Border->Margin = { 5, 3, 5, 3 };
			Border->Padding = 5;
			Border->Padding = { 8, 5, 8, 5 };
			//Border->Padding = { 2.0, 2.0 };
			Border->Fill = Binding(&ListViewColumnHeader, UControl::BackgroundProperty());
			Border->Thickness = Binding(&ListViewColumnHeader, UControl::BorderThicknessProperty());
			Border->Stroke = Binding(&ListViewColumnHeader, UControl::BorderStrokeProperty());
		}


		FVisualStateGroupRef StateGroup_Common = MakeRefer<FControlStateGroup>(u8"CommonStates"N);
		{
			TReferPtr<FStoryboard> Storyboard_Normal = MakeRefer<FStoryboard>(BackgroundBrush.Ref());
			auto BackgroundAnimation = MakeRefer<TDoubleAnimation<FColor>>(FSolidColorBrush::ColorProperty(), noval, GDefaultTheme.Button.NormalColor);
			BackgroundAnimation->Duration = 100;
			Storyboard_Normal->Timelines.AddRange({ BackgroundAnimation });
			StateGroup_Common->AddVisualState(u8"Normal"N, { Storyboard_Normal });
		}
		{
			TReferPtr<FStoryboard> Storyboard_MouseOver = MakeRefer<FStoryboard>(BackgroundBrush.Ref());
			auto BackgroundAnimation = MakeRefer<TDoubleAnimation<FColor>>(FSolidColorBrush::ColorProperty(), noval, GDefaultTheme.Button.HoverColor);
			BackgroundAnimation->Duration = 100;
			Storyboard_MouseOver->Timelines.AddRange({ BackgroundAnimation });
			StateGroup_Common->AddVisualState(u8"MouseOver"N, { Storyboard_MouseOver });
		}
		{
			TReferPtr<FStoryboard> Storyboard_Resizing = MakeRefer<FStoryboard>(BackgroundBrush.Ref());
			auto BackgroundAnimation = MakeRefer<TOneFrameAnimation<FColor>>(FSolidColorBrush::ColorProperty(), GDefaultTheme.Button.PressedColor);
			BackgroundAnimation->Duration = 100;
			Storyboard_Resizing->Timelines.AddRange({ BackgroundAnimation });
			StateGroup_Common->AddVisualState(u8"Indicating"N, { Storyboard_Resizing });
		}
		ListViewColumnHeader.StateManager.StateGroups.Add(StateGroup_Common);
		//UVLineRef Line = MakeRefer<UVLine>();
		//{
		//	Line->Stroke = MakeRefer<FSolidColorBrush>(GDefaultTheme.BorderColor);
		//	Line->StrokeThickness = 1.0f;
		//}

		//UStackPanelRef StackPanel = MakeRefer<UStackPanel>();
		//StackPanel->Elements = { Border, Line };
		return Border;
	}

	void FListViewColumnHeaderStyle::LoadStyleT(UListViewColumnHeader & ListViewColumnHeader)
	{
		ListViewColumnHeader.Foreground = GDefaultTheme.Title.NormalColor;
		ListViewColumnHeader.BorderThickness = { 0, 0, 1, 0 };
		ListViewColumnHeader.BorderStroke = MakeRefer<FSolidColorBrush>(GDefaultTheme.Shape.NormalColor);
	}

	UElementRef FListViewItemCellTemplate::LoadTemplateT(UListViewItemCell & ListViewItemCell)
	{
		UBorderRef Border = MakeRefer<UBorder>(UContentControl::PART_ContentPresenter);
		//Border->Margin = 3;
		Border->Padding = Binding(&ListViewItemCell, UControl::PaddingProperty());
		Border->Fill = Binding(&ListViewItemCell, UControl::BackgroundProperty());
		Border->Thickness = Binding(&ListViewItemCell, UControl::BorderThicknessProperty());
		Border->Stroke = Binding(&ListViewItemCell, UControl::BorderStrokeProperty());
		return Border;
	}

	void FListViewItemCellStyle::LoadStyleT(UListViewItemCell & ListViewItemCell)
	{
		ListViewItemCell.BorderThickness = { 0, 0, 1, 0 };
		ListViewItemCell.Padding = { 10, 4 };
		//ListViewItemCell.Padding = { 0, 50, 0, 50 };
		//ListViewItemCell.BorderThickness = { 0, 0, 0, 1 };
		ListViewItemCell.BorderStroke = MakeRefer<FSolidColorBrush>(GDefaultTheme.BorderColor);
		ListViewItemCell.ContentVerticalAlignment = EElementAlignment::Center;
	}

	UElementRef FListViewItemTemplate::LoadTemplateT(UListViewItem & ListViewItem)
	{
		UBorderRef Border = MakeRefer<UBorder>(UContentControl::PART_ContentPresenter);
		//Border->Margin = 30;
		//Border->Padding = { 2.0, 2.0 };

		FSolidColorBrushRef BackFillBrush = MakeRefer<FSolidColorBrush>(Colors::None);
		FVisualStateGroupRef StateGroup_Back_Common = MakeRefer<FControlStateGroup>(u8"CommonStates"N);

		{
			Border->Fill = BackFillBrush;
			{
				auto BackgroundAnimation = MakeRefer<TDoubleAnimation<FColor>>(FSolidColorBrush::ColorProperty(), noval, /*GDefaultTheme.Item.NormalColor*/Colors::None);
				BackgroundAnimation->Duration = 10;

				//auto BackgroundAnimation = MakeRefer<TOneFrameAnimation<FColor>>(FSolidColorBrush::ColorProperty(), Colors::None);

				TReferPtr<FStoryboard> Storyboard_Normal = MakeRefer<FStoryboard>(BackFillBrush.Ref());
				Storyboard_Normal->Timelines.AddRange({ BackgroundAnimation });
				StateGroup_Back_Common->AddVisualState(u8"Normal"N, { Storyboard_Normal });
			}
			{
				auto BackgroundAnimation = MakeRefer<TDoubleAnimation<FColor>>(FSolidColorBrush::ColorProperty(), noval, GDefaultTheme.Item.HoverColor);
				BackgroundAnimation->Duration = 200;

				//auto BackgroundAnimation = MakeRefer<TOneFrameAnimation<FColor>>(FSolidColorBrush::ColorProperty(), GDefaultTheme.Item.HoverColor);

				TReferPtr<FStoryboard> Storyboard_MouseOver = MakeRefer<FStoryboard>(BackFillBrush.Ref());
				Storyboard_MouseOver->Timelines.AddRange({ BackgroundAnimation });
				StateGroup_Back_Common->AddVisualState(u8"MouseOver"N, { Storyboard_MouseOver });
			}
			{
				auto BackgroundAnimation = MakeRefer<TDoubleAnimation<FColor>>(FSolidColorBrush::ColorProperty(), noval, GDefaultTheme.Item.SelectedColor);
				BackgroundAnimation->Duration = 200;

				//auto BackgroundAnimation = MakeRefer<TOneFrameAnimation<FColor>>(FSolidColorBrush::ColorProperty(), GDefaultTheme.Item.SelectedColor);
				TReferPtr<FStoryboard> Storyboard_Pressed = MakeRefer<FStoryboard>(BackFillBrush.Ref());
				Storyboard_Pressed->Timelines.AddRange({ BackgroundAnimation });
				StateGroup_Back_Common->AddVisualState(u8"Selected"N, { Storyboard_Pressed });
			}
		}
		ListViewItem.StateManager.StateGroups.Add(StateGroup_Back_Common);
		return Border;
	}

	void FListViewItemStyle::LoadStyleT(UListViewItem & ListViewItem)
	{
		ListViewItem.HorizontalAlignment = EElementAlignment::Near;
		//ListBoxItem.GroundingFill = Brushs::AliceBlue;
	}

	UElementRef FListViewTemplate::LoadTemplateT(UListView & ListView)
	{
		UBorderRef Border = MakeRefer<UBorder>();
		//Border->Margin = 30;
		//Border->Padding = { 2.0, 2.0 };
		Border->Fill = Binding(&ListView, UControl::BackgroundProperty());
		Border->Thickness = Binding(&ListView, UControl::BorderThicknessProperty());
		Border->Stroke = Binding(&ListView, UControl::BorderStrokeProperty());

		UScrollPanelRef ScrollPanel = MakeRefer<UScrollPanel>();

		//MainPanel = MakeRefer<UStackPanel>();
		//MainPanel->Orientation = EOrientation::Vertical;
		//MainPanel->Size = { FDimen::Percent100, FDimen::Percent100 };

		UListViewHeaderRef Header = MakeRefer<UListViewHeader>(ListView);
		Header->Name = UListView::Name_ListViewHeader;

		Header->ScrollOffset = Binding(ScrollPanel.Get(), UScrollPanel::ScrollOffsetProperty(), u8"X"N);
		USplitterPanelRef HeaderSplitter = MakeRefer<USplitterPanel>();
		{
			HeaderSplitter->Orientation = EOrientation::Horizontal;
			for (uintx ColumnIndex = 0; ColumnIndex < ListView.Columns.Size; ++ColumnIndex)
			{
				const FListViewColumn & ListViewColumn = ListView.Columns[ColumnIndex];
				UListViewColumnHeaderRef ListViewColumnHeader = MakeRefer<UListViewColumnHeader>(ListView, ListViewColumn);
				ListViewColumnHeader->DisplayIndex = ColumnIndex;
				ListViewColumnHeader->Content = ListViewColumn.Header;
				ListView.ColumnHeaders.Add(ListViewColumnHeader);
			}

			HeaderSplitter->Elements = ListView.ColumnHeaders;
			HeaderSplitter->IndicatingIndexChanged += { &ListView, &UListView::OnIndicatingIndexChanged };
		}

		//class FDefaultListViewItemTemplate : public IControlTemplate
		//{
		//public:
		//	FDefaultListViewItemTemplate() = default;
		//	UElementRef LoadTemplate(UControl & Control) override
		//	{
		//		AssertExpresion(Control.IsA<UListView>());
		//		UListViewItemRef ListViewItem = MakeRefer<UListViewItem>();
		//		ListViewItem->Owner = StaticCast<UListView>(&Control);
		//		return ListViewItem;
		//	}
		//};
		//static TReferPtr<FDefaultListViewItemTemplate> DefaultListBoxItemTemplate = MakeRefer<FDefaultListViewItemTemplate>();
		//if (!ItemTemplate)
		//	ItemTemplate = DefaultListBoxItemTemplate;

		UStackPanelRef ItemsPanel = MakeRefer<UStackPanel>(UListControl::Name_ItemsPanel);
		ItemsPanel->Orientation = EOrientation::Vertical;

		UScrollAreaRef ScrollArea = MakeRefer<UScrollArea>();
		ScrollArea->Width = 100pct;
		ScrollArea->Height = 100pct;

		ScrollArea->Child = ItemsPanel;

		Header->Content = HeaderSplitter;

		UStackPanelRef MainPanel = MakeRefer<UStackPanel>(u8"MainPanel"N);
		MainPanel->Orientation = EOrientation::Vertical;
		MainPanel->Width = 100pct;
		MainPanel->HorizontalAlignment = EElementAlignment::Near;

		MainPanel->Elements = { Header, ScrollArea };

		{
			ScrollPanel->Child = MainPanel;
			UScrollBarRef VerticalScrollBar = MakeRefer<UScrollBar>(UControl::PART_VerticalScrollBar);
			{
				VerticalScrollBar->Size = FDimen2 { 20, 100pct };
				VerticalScrollBar->Orientation = EOrientation::Vertical;

				VerticalScrollBar->ViewportSize = Binding(ScrollPanel, UScrollPanel::ViewportSizeProperty(), u8"Y"N);
				VerticalScrollBar->Maximum = Binding(ScrollPanel, UScrollPanel::ScrollExtentProperty(), u8"Y"N);
				VerticalScrollBar->Value = Binding(ScrollPanel, UScrollPanel::ScrollOffsetProperty(), u8"Y"N, EBindingMode::TwoWay);
			}
			UScrollBarRef HorizontalScrollBar = MakeRefer<UScrollBar>(UControl::PART_HorizontalScrollBar);
			{
				HorizontalScrollBar->Size = FDimen2 { FDimen::Percent100, 20 };
				HorizontalScrollBar->Orientation = EOrientation::Horizontal;

				HorizontalScrollBar->ViewportSize = Binding(ScrollPanel, UScrollPanel::ViewportSizeProperty(), u8"X"N);
				HorizontalScrollBar->Maximum = Binding(ScrollPanel, UScrollPanel::ScrollExtentProperty(), u8"X"N);
				HorizontalScrollBar->Value = Binding(ScrollPanel, UScrollPanel::ScrollOffsetProperty(), u8"X"N, EBindingMode::TwoWay);

				//HorizontalScrollBar->Value = Binding(ScrollPanel, { UScrollPanel::ScrollOffsetProperty(), u8"X"N }, EBindingMode::TwoWay);
			}

			URectangleRef ConnerRectangle = MakeRefer<URectangle>(UControl::PART_ConnerElement);
			{
				ConnerRectangle->Fill = MakeRefer<FSolidColorBrush>(0xFFE8E8E8);
			}

			ScrollPanel->HorizontalScrollBar = HorizontalScrollBar;
			ScrollPanel->VerticalScrollBar = VerticalScrollBar;
			ScrollPanel->ConnerElement = ConnerRectangle;

			ScrollPanel->ScrollInfo = ScrollArea->ScrollInfo;
		}
		//MainScroll->VerticalScrollBarVisibility = EVisibility::Hidden;

		ItemsPanel->Measured += { &ListView, &UListView::OnListPanelMeasured };

		Border->Child = ScrollPanel;
		return Border;
	}

	void FListViewStyle::LoadStyleT(UListView & ListView)
	{
		ListView.BorderThickness = 2;
		ListView.BorderStroke = MakeRefer<FSolidColorBrush>(GDefaultTheme.BorderColor);
	}
}
