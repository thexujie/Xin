#include "PCH.h"
#include "ListBoxStyle.h"

namespace Xin::UI::Themes::Default
{
	UElementRef FListBoxItemTemplate::LoadTemplateT(UListBoxItem & ListBoxItem)
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
				BackgroundAnimation->Duration = 200;
				TReferPtr<FStoryboard> Storyboard_Normal = MakeRefer<FStoryboard>(BackFillBrush.Ref());
				Storyboard_Normal->Timelines.AddRange({ BackgroundAnimation });
				StateGroup_Back_Common->AddVisualState(u8"Normal"N, { Storyboard_Normal });
			}
			{
				auto BackgroundAnimation = MakeRefer<TDoubleAnimation<FColor>>(FSolidColorBrush::ColorProperty(), noval, GDefaultTheme.Item.HoverColor);
				BackgroundAnimation->Duration = 200;
				TReferPtr<FStoryboard> Storyboard_MouseOver = MakeRefer<FStoryboard>(BackFillBrush.Ref());
				Storyboard_MouseOver->Timelines.AddRange({ BackgroundAnimation });
				StateGroup_Back_Common->AddVisualState(u8"MouseOver"N, { Storyboard_MouseOver });
			}
			{
				auto BackgroundAnimation = MakeRefer<TDoubleAnimation<FColor>>(FSolidColorBrush::ColorProperty(), noval, GDefaultTheme.Item.SelectedColor);
				BackgroundAnimation->Duration = 200;
				TReferPtr<FStoryboard> Storyboard_Pressed = MakeRefer<FStoryboard>(BackFillBrush.Ref());
				Storyboard_Pressed->Timelines.AddRange({ BackgroundAnimation });
				StateGroup_Back_Common->AddVisualState(u8"Selected"N, { Storyboard_Pressed });
			}

		}
		ListBoxItem.StateManager.StateGroups.Add(StateGroup_Back_Common);
		return Border;
	}

	void FListBoxItemStyle::LoadStyleT(UListBoxItem & ListBoxItem)
	{
		//ListBoxItem.GroundingFill = Brushs::AliceBlue;
	}

	UElementRef FListBoxTemplate::LoadTemplateT(UListBox & ListBox)
	{
		class FDefaultListBoxItemTemplate : public IControlTemplate
		{
		public:
			FDefaultListBoxItemTemplate() = default;
			UElementRef LoadTemplate(UControl & Control) override
			{
				AssertExpr(Control.IsA<UListBox>());
				UListBoxItemRef ListBoxItem = MakeRefer<UListBoxItem>();
				//ListBoxItem->HorizontalAlignment = EElementAlignment::Stretch;
				//ListBoxItem->Size = { FDimen::Percent100, FDimen::Auto };
				ListBoxItem->Owner = StaticCast<UListBox>(&Control);
				return ListBoxItem;
			}
		};
		static TReferPtr<FDefaultListBoxItemTemplate> DefaultListBoxItemTemplate = MakeRefer<FDefaultListBoxItemTemplate>();
		ListBox.ItemTemplate = DefaultListBoxItemTemplate;

		UBorderRef Border = MakeRefer<UBorder>();
		//Border->Margin = 30;
		//Border->Padding = { 2.0, 2.0 };
		Border->Fill = Binding(&ListBox, UControl::BackgroundProperty());
		Border->Thickness = Binding(&ListBox, UControl::BorderThicknessProperty());
		Border->Stroke = Binding(&ListBox, UControl::BorderStrokeProperty());

		UScrollViewerRef ScrollViewer = MakeRefer<UScrollViewer>();
		{
			UStackPanelRef ItemsPanel = MakeRefer<UStackPanel>(UListControl::Name_ItemsPanel);
			ItemsPanel->Orientation = EOrientation::Vertical;
			ScrollViewer->Child = ItemsPanel;
		}
		Border->Child = ScrollViewer;
		return Border;
	}

	void FListBoxStyle::LoadStyleT(UListBox & ListBox)
	{
		ListBox.BorderThickness = 2;
		ListBox.BorderStroke = MakeRefer<FSolidColorBrush>(GDefaultTheme.BorderColor);
	}
}
