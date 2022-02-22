#include "PCH.h"
#include "ChromeStyle.h"

namespace Xin::UI::Themes::Default
{
	void FChromeStyle::LoadStyleT(UChrome & Chrome)
	{
		Chrome.Padding = 2;
		Chrome.BorderThickness = 2;
		Chrome.BorderStroke = Brushs::BlueViolet;
		Chrome.Background = MakeRefer<FSolidColorBrush>(0x80F5F5F5);
	}

	UElementRef FChromeTemplate::LoadTemplateT(UChrome & Chrome)
	{
		//FColor CC { Colors::Red - Colors::Black };

		UChromeBorderRef ChromeBorder = MakeRefer<UChromeBorder>(Chrome);

		//Border->Margin = 30;
		// We use apply padding for ContentBorder, not ChromeBorder
		//ChromeBorder->Padding = Binding(&Chrome, UChrome::PaddingProperty());
		ChromeBorder->Fill = Binding(&Chrome, UChrome::BackgroundProperty());
		ChromeBorder->Thickness = Binding(&Chrome, UChrome::BorderThicknessProperty());
		ChromeBorder->Stroke = Binding(&Chrome, UChrome::BorderStrokeProperty());
		ChromeBorder->ChromeThickness = 8;

		UStackPanelRef StackPanel = MakeRefer<UStackPanel>();
		{
			StackPanel->Orientation = EOrientation::Vertical;
			UChromeTitleRef ChromeTitle = MakeRefer<UChromeTitle>(Chrome);
			ChromeTitle->Title = Binding(&Chrome, UChrome::TitleProperty());
			ChromeTitle->Cursor = ECursor::SizeAll;

			UBorderRef ContentBorder = MakeRefer<UBorder>(UContentControl::PART_ContentPresenter);
			ContentBorder->Size = { FDimen::Percent100, FDimen::Percent100 };
			ContentBorder->Padding = Binding(&Chrome, UChrome::PaddingProperty());

			StackPanel->Elements = { ChromeTitle, ContentBorder };
		}

		ChromeBorder->Child = StackPanel;
		return ChromeBorder;
	}


	void FChromeButtonStyle::LoadStyleT(UChromeButton & ChromeButton)
	{
		ChromeButton.Padding = 8;
		//ChromeButton.PathStroke = Brushs::Black;
		//ChromeButton.PathStrokeThickness = 2.0f;
		ChromeButton.PathFill = Brushs::Black;
	}

	UElementRef FChromeButtonTemplate::LoadTemplateT(UChromeButton & ChromeButton)
	{
		FSolidColorBrushRef BackFillBrush = MakeRefer<FSolidColorBrush>(Colors::None);
		UBorderRef Border = MakeRefer<UBorder>();
		//Border->Margin = 30;
		Border->Fill = BackFillBrush;
		Border->Padding = Binding(&ChromeButton, UChromeButton::PaddingProperty());

		FVisualStateGroupRef StateGroup_Back_Common = MakeRefer<FControlStateGroup>(u8"CommonStates"N);
		{
			{
				auto BackgroundAnimation = MakeRefer<TDoubleAnimation<FColor>>(FSolidColorBrush::ColorProperty(), noval, Colors::None);
				BackgroundAnimation->Duration = 200;
				TReferPtr<FStoryboard> Storyboard_Normal = MakeRefer<FStoryboard>(BackFillBrush.Ref());
				Storyboard_Normal->Timelines.AddRange({ BackgroundAnimation });
				StateGroup_Back_Common->AddVisualState(u8"Normal"N, { Storyboard_Normal });
			}
			{
				auto BackgroundAnimation = MakeRefer<TDoubleAnimation<FColor>>(FSolidColorBrush::ColorProperty(), noval, GDefaultTheme.Button.HoverColor);
				BackgroundAnimation->Duration = 200;
				TReferPtr<FStoryboard> Storyboard_MouseOver = MakeRefer<FStoryboard>(BackFillBrush.Ref());
				Storyboard_MouseOver->Timelines.AddRange({ BackgroundAnimation });
				StateGroup_Back_Common->AddVisualState(u8"MouseOver"N, { Storyboard_MouseOver });
			}
			{
				auto BackgroundAnimation = MakeRefer<TDoubleAnimation<FColor>>(FSolidColorBrush::ColorProperty(), noval, GDefaultTheme.Button.PressedColor);
				BackgroundAnimation->Duration = 200;
				TReferPtr<FStoryboard> Storyboard_Pressed = MakeRefer<FStoryboard>(BackFillBrush.Ref());
				Storyboard_Pressed->Timelines.AddRange({ BackgroundAnimation });
				StateGroup_Back_Common->AddVisualState(u8"Pressed"N, { Storyboard_Pressed });
			}
		}
		ChromeButton.StateManager.StateGroups.Add(StateGroup_Back_Common);

		UPathRef Path = MakeRefer<UPath>();

		Path->Command = Binding(&ChromeButton, UChromeButton::PathCommandProperty());
		Path->Fill = Binding(&ChromeButton, UChromeButton::PathFillProperty());
		Path->Stroke = Binding(&ChromeButton, UChromeButton::PathStrokeProperty());
		Path->StrokeThickness = Binding(&ChromeButton, UChromeButton::PathStrokeThicknessProperty());
		Path->Stretch = EStretch::Uniform;

		Border->Child = Path;
		return Border;
	}

	void FChromeTitleStyle::LoadStyleT(UChromeTitle & ChromeTitle)
	{
		ChromeTitle.BorderThickness = { 0, 0, 0, 1 };
		ChromeTitle.BorderStroke = Brushs::Black;
		ChromeTitle.Background = Brushs::LightSteelBlue;
		ChromeTitle.Padding = { 4.0, 2.0 };
	}

	UElementRef FChromeTitleTemplate::LoadTemplateT(UChromeTitle & ChromeTitle)
	{
		//FColor CC { Colors::Red - Colors::Black };

		UBorderRef Border = MakeRefer<UBorder>();
		//Border->Margin = 30;
		Border->Padding = Binding(&ChromeTitle, UChromeTitle::PaddingProperty());
		Border->Fill = Binding(&ChromeTitle, UChromeTitle::BackgroundProperty());
		Border->Thickness = Binding(&ChromeTitle, UChromeTitle::BorderThicknessProperty());
		Border->Stroke = Binding(&ChromeTitle, UChromeTitle::BorderStrokeProperty());
		//Border->Stroke = MakeRefer<FSolidColorBrush>(GDefaultTheme.BorderColor);

		UTextBlockRef TextBlock = MakeRefer<UTextBlock>();
		TextBlock->Text = Binding(&ChromeTitle, UChromeTitle::TitleProperty());
		TextBlock->HorizontalAlignment = EElementAlignment::Center;
		TextBlock->VerticalAlignment = EElementAlignment::Center;
		TextBlock->FontWeight = EFontWeight::Bold;

		UOverlapPanelRef OverlapPanel = MakeRefer<UOverlapPanel>();
		{
			static FStringV IconClose = u8"M517.38624 570.32704l-91.66848 91.66848-48.08704-48.08704 91.66848-91.66848-98.75456-98.73408 48.08704-48.08704 98.75456 98.73408 98.73408-98.73408 48.08704 48.08704-98.73408 98.73408 91.648 91.66848-48.08704 48.08704-91.648-91.66848z m417.3824-126.464c4.8128 25.3952 7.31136 51.6096 7.31136 78.37696C942.08 754.11456 754.11456 942.08 522.24 942.08S102.4 754.11456 102.4 522.24 290.36544 102.4 522.24 102.4c66.31424 0 129.024 15.36 184.79104 42.74176l-44.8512 73.87136C616.6528 196.64896 574.27968 186.368 522.24 186.368c-185.48736 0-335.872 150.38464-335.872 335.872 0 185.48736 150.38464 335.872 335.872 335.872 185.48736 0 335.872-150.38464 335.872-335.872 0-21.2992-1.96608-42.27072-5.8368-62.79168l82.49344-15.5648z m-120.97536-61.56288a81.63328 81.63328 0 1 1 0-163.28704 81.63328 81.63328 0 0 1 0 163.28704z"V;
			UChromeButtonRef CloseButton = MakeRefer<UChromeButton>(ChromeTitle.Chrome, u8"Close"N);
			CloseButton->HorizontalAlignment = EElementAlignment::Far;
			CloseButton->Size = { 40, 40 };
			CloseButton->Command = EChromeCommand::Close;
			CloseButton->PathCommand = IconClose;

			OverlapPanel->Elements = { TextBlock, CloseButton };
		}
		Border->Child = OverlapPanel;

		return Border;
	}
}
