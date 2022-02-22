#include "PCH.h"
#include "MenuStyle.h"

namespace Xin::UI::Themes::Default
{
	UElementRef FMenuItemTemplate::LoadTemplateT(UMenuItem & MenuItem)
	{
		UBorderRef Border = MakeRefer<UBorder>();
		//Border->Margin = 30;
		Border->Padding = { 8.0dim, 4.0dim, 8dim, 4dim};

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
				TReferPtr<FStoryboard> Storyboard_Indicating = MakeRefer<FStoryboard>(BackFillBrush.Ref());
				Storyboard_Indicating->Timelines.AddRange({ BackgroundAnimation });
				StateGroup_Back_Common->AddVisualState(u8"Indicating"N, { Storyboard_Indicating });
			}
			{
				auto BackgroundAnimation = MakeRefer<TDoubleAnimation<FColor>>(FSolidColorBrush::ColorProperty(), noval, GDefaultTheme.Item.SelectedColor);
				BackgroundAnimation->Duration = 200;
				TReferPtr<FStoryboard> Storyboard_Pressed = MakeRefer<FStoryboard>(BackFillBrush.Ref());
				Storyboard_Pressed->Timelines.AddRange({ BackgroundAnimation });
				StateGroup_Back_Common->AddVisualState(u8"Selected"N, { Storyboard_Pressed });
			}

		}
		MenuItem.StateManager.StateGroups.Add(StateGroup_Back_Common);


		UStackPanelRef StackPanel = MakeRefer<UStackPanel>();
		{
			StackPanel->Orientation = EOrientation::Horizontal;
			StackPanel->HorizontalAlignment = EElementAlignment::Stretch;

			UBorderRef ContentBorder = MakeRefer<UBorder>(UContentControl::PART_ContentPresenter);
			//ContentBorder->Width = 100pct;

			USeparatorRef Separator = MakeRefer< USeparator>();
			Separator->Width = 100pct;
			Separator->MinWidth = 16;

			UPolylineRef Polyline = MakeRefer<UPolyline>();
			Polyline->Size = { 16, 16};
			Polyline->Points = { { 4, 1 }, { 11, 7 }, { 4, 13 } };
			Polyline->Stretch = EStretch::UniformFill;

			Polyline->Visibility = Binding(MenuItem, UMenuItem::SubmenuProperty(),
				[](const FVariant & Submenu)
				{
					return Submenu.Valid() ? EVisibility::Visible : EVisibility::Hidden;
				});

			Polyline->HorizontalAlignment = EElementAlignment::Center;
			Polyline->VerticalAlignment = EElementAlignment::Center;
			Polyline->Stroke = Brushs::Black;
			Polyline->StrokeThickness = 3.0f;
			Polyline->StrokeStyle = FStrokeStyle { .LineJoin = ELineJoin::Miter };

			StackPanel->Elements = { ContentBorder, Separator, Polyline };

		}
		Border->Child = StackPanel;
		return Border;
	}

	void FMenuItemStyle::LoadStyleT(UMenuItem & MenuItem)
	{
		//MenuItem.GroundingFill = Brushs::AliceBlue;
	}

	UElementRef FMenuSaperatorTemplate::LoadTemplateT(UMenuSaperator & MenuSaperator)
	{
		UHLineRef HLine = MakeRefer<UHLine>();
		HLine->Stroke = MakeRefer<FSolidColorBrush>(GDefaultTheme.Shape.NormalColor);
		HLine->StrokeThickness = 1.0f;
		return HLine;
	}

	void FMenuSaperatorStyle::LoadStyleT(UMenuSaperator & MenuSaperator)
	{
		MenuSaperator.Margin = { 0dim, 8.0dim };
	}

	UElementRef FMenuTemplate::LoadTemplateT(UMenu & Menu)
	{
		UBorderRef Border = MakeRefer<UBorder>();
		{
			//Border->Margin = 30;
			//Border->Padding = { 2.0, 2.0 };
			Border->Padding = 3;
			Border->Fill = Binding(&Menu, UControl::BackgroundProperty());
			Border->Thickness = Binding(&Menu, UControl::BorderThicknessProperty());
			Border->Stroke = Binding(&Menu, UControl::BorderStrokeProperty());

			UStackPanelRef ItemsPanel = MakeRefer<UStackPanel>(UListControl::Name_ItemsPanel);
			ItemsPanel->Orientation = EOrientation::Vertical;
			Border->Child = ItemsPanel;
		}
		return Border;
	}

	void FMenuStyle::LoadStyleT(UMenu & Menu)
	{
		Menu.Background = MakeRefer<FSolidColorBrush>(GDefaultTheme.Menu.BackgroundColor);
		Menu.BorderThickness = 2;
		Menu.BorderStroke = MakeRefer<FSolidColorBrush>(GDefaultTheme.Frame.ActiveColor);
	}

	UElementRef FContextMenuTemplate::LoadTemplateT(UContextMenu & Window)
	{
		//if (Window.WindowStyle == EWindowStyle::Frameless)
		//{
		//	Window.BorderThickness = 0;
		//	Window.BorderStroke = MakeRefer<FSolidColorBrush>(GDefaultTheme.Frame.ActiveColor);

		//	UBorderRef Border = MakeRefer<UBorder>(UContentControl::PART_ContentPresenter);
		//	//Border->Margin = 30;
		//	Border->Thickness = Binding(&Window, UControl::BorderThicknessProperty());
		//	Border->Stroke = Binding(&Window, UControl::BorderStrokeProperty());
		//	return Border;
		//}
		return MakeRefer<UBorder>(UContentControl::PART_ContentPresenter);
	}
}
