#include "PCH.h"
#include "ButtonStyle.h"

namespace Xin::UI::Themes::Default
{
	UElementRef FButtonTemplate::LoadTemplateT(UButton & Button)
	{
		//FColor CC { Colors::Red - Colors::Black };

		UBorderRef Border = MakeRefer<UBorder>(UContentControl::PART_ContentPresenter);
		//Border->Margin = 30;
		Border->Padding = { 10.0, 4.0 };
		Border->Fill = Binding(&Button, UButton::BackgroundProperty());
		Border->Thickness = Binding(&Button, UButton::BorderThicknessProperty());
		Border->Stroke = MakeRefer<FSolidColorBrush>(GDefaultTheme.BorderColor);
		return Border;
	}

	void FButtonStyle::LoadStyleT(UButton & Button)
	{
		FSolidColorBrushRef BackgroundBrush = MakeRefer<FSolidColorBrush>(GDefaultTheme.Button.NormalColor);
		Button.BorderThickness = 2;
		Button.Background = BackgroundBrush;
		Button.Margin = 4;

		TReferPtr<FBeginStoryboardAction> BorderThicknessStoryboard_True;
		{
			TReferPtr<FStoryboard> Storyboard = MakeRefer<FStoryboard>(Button);
			auto BorderThicknessAnimation = MakeRefer<TDoubleAnimation<FThickness>>(UButton::BorderThicknessProperty(), noval, 10.0);
			Storyboard->Timelines.AddRange({ /*BackgroundAnimation,*/ BorderThicknessAnimation });
			BorderThicknessStoryboard_True = MakeRefer<FBeginStoryboardAction>(Storyboard);
		}
		TReferPtr<FBeginStoryboardAction> BorderThicknessStoryboard_False;
		{
			TReferPtr<FStoryboard> Storyboard = MakeRefer<FStoryboard>(Button);
			auto BorderThicknessAnimation = MakeRefer<TDoubleAnimation<FThickness>>(UButton::BorderThicknessProperty(), noval, 2.0);
			Storyboard->Timelines.AddRange({ /*BackgroundAnimation,*/ BorderThicknessAnimation });
			BorderThicknessStoryboard_False = MakeRefer<FBeginStoryboardAction>(Storyboard);
		}
		//Button.Triggers.Add(MakeRefer<FTrigger>(UButton::IsMouseOverProperty(),
		//	TView<ITriggerActionRef> { BorderThicknessStoryboard_True }, TView<ITriggerActionRef> { BorderThicknessStoryboard_False }));

		// Visual States
		FVisualStateGroupRef StateGroup_Common = MakeRefer<FControlStateGroup>(u8"CommonStates"N);
		{
			TReferPtr<FStoryboard> Storyboard_Normal = MakeRefer<FStoryboard>(BackgroundBrush.Ref());
			Storyboard_Normal->Duration = 200;
			auto BackgroundAnimation = MakeRefer<TDoubleAnimation<FColor>>(FSolidColorBrush::ColorProperty(), noval, GDefaultTheme.Button.NormalColor);
			BackgroundAnimation->Duration = 200;
			Storyboard_Normal->Timelines.AddRange({ BackgroundAnimation });
			StateGroup_Common->AddVisualState(u8"Normal"N, { Storyboard_Normal });
		}
		{
			TReferPtr<FStoryboard> Storyboard_MouseOver = MakeRefer<FStoryboard>(BackgroundBrush.Ref());
			Storyboard_MouseOver->Duration = 500;
			auto BackgroundAnimation = MakeRefer<TDoubleAnimation<FColor>>(FSolidColorBrush::ColorProperty(), noval, GDefaultTheme.Button.HoverColor);
			BackgroundAnimation->Duration = 500;
			Storyboard_MouseOver->Timelines.AddRange({ BackgroundAnimation });
			StateGroup_Common->AddVisualState(u8"MouseOver"N, { Storyboard_MouseOver });
		}
		{
			TReferPtr<FStoryboard> Storyboard_Pressed = MakeRefer<FStoryboard>(BackgroundBrush.Ref());
			Storyboard_Pressed->Duration = 100;
			auto BackgroundAnimation = MakeRefer<TOneFrameAnimation<FColor>>(FSolidColorBrush::ColorProperty(), GDefaultTheme.Button.PressedColor);
			BackgroundAnimation->Duration = 100;
			Storyboard_Pressed->Timelines.AddRange({ BackgroundAnimation });
			StateGroup_Common->AddVisualState(u8"Pressed"N, { Storyboard_Pressed });
		}
		Button.StateManager.StateGroups.Add(StateGroup_Common);
	}

	UElementRef FRadioButtonTemplate::LoadTemplateT(URadioButton & RadioButton)
	{
		//Button.GroundingStroke = Brushs::Red;

		UStackPanelRef StackPanel = MakeRefer<UStackPanel>();
		StackPanel->Orientation = EOrientation::Horizontal;

		UOverlapPanelRef MarkPanel = MakeRefer<UOverlapPanel>();
		{
			MarkPanel->VerticalAlignment = EElementAlignment::Center;
			MarkPanel->Size = { 34, 34 };
		}

		UEllipseRef Back = MakeRefer<UEllipse>(u8"Back"N);
		FSolidColorBrushRef BackFillBrush = MakeRefer<FSolidColorBrush>(GDefaultTheme.Plane.NormalColor);
		FSolidColorBrushRef BackStrokeBrush = MakeRefer<FSolidColorBrush>(GDefaultTheme.BorderColor);
		{
			Back->Visibility = Binding(RadioButton, UToggleButton::CheckedProperty(),
				[](const TOptional<bool> & Checked) { return Checked == true ? EVisibility::Collapsed : EVisibility::Visible; });
			Back->HorizontalAlignment = EElementAlignment::Center;
			Back->VerticalAlignment = EElementAlignment::Center;
			Back->Size = { FDimen::Percent80, FDimen::Percent80 };
			Back->Fill = BackFillBrush;
			Back->Stroke = BackStrokeBrush;
			Back->StrokeThickness = 2.0f;
		}

		UEllipseRef Mark = MakeRefer<UEllipse>(u8"Mark"N);
		FSolidColorBrushRef MarkFillBrush = MakeRefer<FSolidColorBrush>(GDefaultTheme.Plane.NormalColor);
		FSolidColorBrushRef MarkStrokeBrush = MakeRefer<FSolidColorBrush>(GDefaultTheme.Primary.NormalColor);
		{
			Mark->Visibility = Binding(RadioButton, UToggleButton::CheckedProperty(),
				[](const TOptional<bool> & Checked) { return Checked == true ? EVisibility::Visible : EVisibility::Collapsed; });

			Mark->HorizontalAlignment = EElementAlignment::Center;
			Mark->VerticalAlignment = EElementAlignment::Center;
			Mark->Size = { FDimen::Percent80, FDimen::Percent80 };
			Mark->Fill = MarkFillBrush;
			Mark->Stroke = MarkStrokeBrush;
			Mark->StrokeThickness = 6.0f;
		}

		FVisualStateGroupRef StateGroup_Back_Common = MakeRefer<FControlStateGroup>(u8"CommonStates"N);
		{
			auto BackgroundAnimation = MakeRefer<TDoubleAnimation<FColor>>(FSolidColorBrush::ColorProperty(), noval, GDefaultTheme.Plane.NormalColor);
			BackgroundAnimation->Duration = 200;
			TReferPtr<FStoryboard> Storyboard_Normal = MakeRefer<FStoryboard>(BackFillBrush.Ref());
			Storyboard_Normal->Timelines.AddRange({ BackgroundAnimation });
			StateGroup_Back_Common->AddVisualState(u8"Normal"N, { Storyboard_Normal });
		}
		{
			auto BackgroundAnimation = MakeRefer<TDoubleAnimation<FColor>>(FSolidColorBrush::ColorProperty(), noval, GDefaultTheme.Plane.HoverColor);
			BackgroundAnimation->Duration = 200;
			TReferPtr<FStoryboard> Storyboard_MouseOver = MakeRefer<FStoryboard>(BackFillBrush.Ref());
			Storyboard_MouseOver->Timelines.AddRange({ BackgroundAnimation });
			StateGroup_Back_Common->AddVisualState(u8"MouseOver"N, { Storyboard_MouseOver });
		}
		{
			auto BackgroundAnimation = MakeRefer<TDoubleAnimation<FColor>>(FSolidColorBrush::ColorProperty(), noval, GDefaultTheme.Plane.PressedColor);
			BackgroundAnimation->Duration = 200;
			TReferPtr<FStoryboard> Storyboard_Pressed = MakeRefer<FStoryboard>(BackFillBrush.Ref());
			Storyboard_Pressed->Timelines.AddRange({ BackgroundAnimation });
			StateGroup_Back_Common->AddVisualState(u8"Pressed"N, { Storyboard_Pressed });
		}

		FVisualStateGroupRef StateGroup_MarkEllipse_Common = MakeRefer<FControlStateGroup>(u8"CommonStates"N);
		{
			TReferPtr<FStoryboard> Storyboard_StrokeBrush = MakeRefer<FStoryboard>(MarkStrokeBrush.Ref());
			{
				auto BackgroundAnimation = MakeRefer<TDoubleAnimation<FColor>>(FSolidColorBrush::ColorProperty(), noval, GDefaultTheme.Primary.NormalColor);
				BackgroundAnimation->Duration = 200;
				Storyboard_StrokeBrush->Timelines.AddRange({ BackgroundAnimation });
			}

			FStoryboardRef Storyboard_StrokeThickness = MakeRefer<FStoryboard>(Mark.Ref());
			{
				auto StrokeThicknessAnimation = MakeRefer<TDoubleAnimation<float32>>(UShape::StrokeThicknessProperty(), noval, 6.0f);
				StrokeThicknessAnimation->Duration = 200;
				Storyboard_StrokeThickness->Timelines.AddRange({ StrokeThicknessAnimation });
			}
			FStoryboardRef Storyboard_Fill = MakeRefer<FStoryboard>(MarkFillBrush.Ref());
			{
				auto StrokeThicknessAnimation = MakeRefer<TDoubleAnimation<FColor>>(FSolidColorBrush::ColorProperty(), noval, GDefaultTheme.Plane.NormalColor);
				StrokeThicknessAnimation->Duration = 200;
				Storyboard_Fill->Timelines.AddRange({ StrokeThicknessAnimation });
			}
			StateGroup_MarkEllipse_Common->AddVisualState(u8"Normal"N, { Storyboard_StrokeBrush, Storyboard_StrokeThickness, /*Storyboard_Fill*/ });
		}
		{
			TReferPtr<FStoryboard> Storyboard_StrokeBrush = MakeRefer<FStoryboard>(MarkStrokeBrush.Ref());
			{
				auto BackgroundAnimation = MakeRefer<TDoubleAnimation<FColor>>(FSolidColorBrush::ColorProperty(), noval, GDefaultTheme.Primary.NormalColor);
				BackgroundAnimation->Duration = 200;
				Storyboard_StrokeBrush->Timelines.AddRange({ BackgroundAnimation });
			}

			FStoryboardRef Storyboard_StrokeThickness = MakeRefer<FStoryboard>(Mark.Ref());
			{
				auto StrokeThicknessAnimation = MakeRefer<TDoubleAnimation<float32>>(UShape::StrokeThicknessProperty(), noval, 5.0f);
				StrokeThicknessAnimation->Duration = 200;
				Storyboard_StrokeThickness->Timelines.AddRange({ StrokeThicknessAnimation });
			}
			FStoryboardRef Storyboard_Fill = MakeRefer<FStoryboard>(MarkFillBrush.Ref());
			{
				FVariant ColorV = nullptr;
				//static_assert(ConvertibleToC<nullptr_t, FColor>);
				auto StrokeThicknessAnimation = MakeRefer<TDoubleAnimation<FColor>>(FSolidColorBrush::ColorProperty(), noval, GDefaultTheme.Plane.HoverColor);
				StrokeThicknessAnimation->Duration = 200;
				Storyboard_Fill->Timelines.AddRange({ StrokeThicknessAnimation });
			}

			StateGroup_MarkEllipse_Common->AddVisualState(u8"MouseOver"N, { Storyboard_StrokeBrush, Storyboard_StrokeThickness, /*Storyboard_Fill*/ });
		}
		{
			auto BackgroundAnimation = MakeRefer<TDoubleAnimation<FColor>>(FSolidColorBrush::ColorProperty(), noval, GDefaultTheme.Button.DisableColor);
			BackgroundAnimation->Duration = 200;
			TReferPtr<FStoryboard> Storyboard_Brush_Disabled = MakeRefer<FStoryboard>(MarkStrokeBrush.Ref());
			Storyboard_Brush_Disabled->Timelines.AddRange({ BackgroundAnimation });
			StateGroup_MarkEllipse_Common->AddVisualState(u8"Disabled"N, { Storyboard_Brush_Disabled });
		}

		RadioButton.StateManager.StateGroups.AddRange({ StateGroup_Back_Common, StateGroup_MarkEllipse_Common });

		UBorderRef Border = MakeRefer<UBorder>(UContentControl::PART_ContentPresenter);
		//Border->Grounding = Brushs::DeepPink;
		{
			Border->Margin = { 0, 0, 0, 0 };
			Border->Padding = { 4.0, 2.0 };
			Border->Thickness = 0;

			//Border->Background = Binding(&Button, UButton::BackgroundProperty());
			//Border->BorderThickness = Binding(&Button, UButton::BorderThicknessProperty());
		}

		MarkPanel->Elements = { Back, Mark };
		StackPanel->Elements = { MarkPanel, Border };
		return StackPanel;
	}

	UElementRef FCheckBoxTemplate::LoadTemplateT(UCheckBox & CheckBox)
	{
		//CheckBox.GroundingFill = Brushs::Red;
		UStackPanelRef StackPanel = MakeRefer<UStackPanel>();
		StackPanel->Orientation = EOrientation::Horizontal;

		UOverlapPanelRef MarkPanel = MakeRefer<UOverlapPanel>();
		{
			MarkPanel->VerticalAlignment = EElementAlignment::Center;
			MarkPanel->Size = { 34, 34 };

			URectangleRef UncheckedBack = MakeRefer<URectangle>(u8"UncheckedBack"N);
			{
				FSolidColorBrushRef BackFillBrush = MakeRefer<FSolidColorBrush>(GDefaultTheme.Plane.NormalColor);
				FSolidColorBrushRef BackStrokeBrush = MakeRefer<FSolidColorBrush>(GDefaultTheme.BorderColor);
				FVisualStateGroupRef StateGroup_Back_Common = MakeRefer<FControlStateGroup>(u8"CommonStates"N);

				{
					UncheckedBack->Visibility = Binding(CheckBox, UCheckBox::CheckedProperty(),
						[](const TOptional<bool> & Checked) { return Checked == false ? EVisibility::Visible : EVisibility::Collapsed; });

					UncheckedBack->HorizontalAlignment = EElementAlignment::Center;
					UncheckedBack->VerticalAlignment = EElementAlignment::Center;
					UncheckedBack->Size = { FDimen::Percent80, FDimen::Percent80 };
					UncheckedBack->Fill = BackFillBrush;
					UncheckedBack->Stroke = BackStrokeBrush;
					UncheckedBack->StrokeThickness = 2.0f;

					{
						auto BackgroundAnimation = MakeRefer<TDoubleAnimation<FColor>>(FSolidColorBrush::ColorProperty(), noval, GDefaultTheme.Plane.NormalColor);
						BackgroundAnimation->Duration = 200;
						TReferPtr<FStoryboard> Storyboard_Normal = MakeRefer<FStoryboard>(BackFillBrush.Ref());
						Storyboard_Normal->Timelines.AddRange({ BackgroundAnimation });
						StateGroup_Back_Common->AddVisualState(u8"Normal"N, { Storyboard_Normal });
					}
					{
						auto BackgroundAnimation = MakeRefer<TDoubleAnimation<FColor>>(FSolidColorBrush::ColorProperty(), noval, GDefaultTheme.Plane.HoverColor);
						BackgroundAnimation->Duration = 200;
						TReferPtr<FStoryboard> Storyboard_MouseOver = MakeRefer<FStoryboard>(BackFillBrush.Ref());
						Storyboard_MouseOver->Timelines.AddRange({ BackgroundAnimation });
						StateGroup_Back_Common->AddVisualState(u8"MouseOver"N, { Storyboard_MouseOver });
					}
					{
						auto BackgroundAnimation = MakeRefer<TDoubleAnimation<FColor>>(FSolidColorBrush::ColorProperty(), noval, GDefaultTheme.Plane.PressedColor);
						BackgroundAnimation->Duration = 200;
						TReferPtr<FStoryboard> Storyboard_Pressed = MakeRefer<FStoryboard>(BackFillBrush.Ref());
						Storyboard_Pressed->Timelines.AddRange({ BackgroundAnimation });
						StateGroup_Back_Common->AddVisualState(u8"Pressed"N, { Storyboard_Pressed });
					}

				}
				CheckBox.StateManager.StateGroups.Add(StateGroup_Back_Common);
			}

			URectangleRef CheckedBack = MakeRefer<URectangle>(u8"CheckedBack"N);
			{
				FSolidColorBrushRef MarkBackFillBrush = MakeRefer<FSolidColorBrush>(GDefaultTheme.Primary.NormalColor);
				FSolidColorBrushRef MarkBackStrokeBrush = MakeRefer<FSolidColorBrush>(GDefaultTheme.BorderColor);
				FVisualStateGroupRef StateGroup_MarkBack_Common = MakeRefer<FControlStateGroup>(u8"CommonStates"N);

				{
					CheckedBack->Visibility = Binding(CheckBox, UCheckBox::CheckedProperty(),
						[](const TOptional<bool> & Checked) { return Checked != false ? EVisibility::Visible : EVisibility::Collapsed; });
					CheckedBack->HorizontalAlignment = EElementAlignment::Center;
					CheckedBack->VerticalAlignment = EElementAlignment::Center;
					CheckedBack->Size = { FDimen::Percent80, FDimen::Percent80 };
					CheckedBack->Fill = MarkBackFillBrush;
					CheckedBack->Stroke = MarkBackStrokeBrush;
					CheckedBack->StrokeThickness = 1.0f;

					{
						TReferPtr<FStoryboard> Storyboard_MarkBackFill = MakeRefer<FStoryboard>(MarkBackFillBrush.Ref());
						{
							auto BackgroundAnimation = MakeRefer<TDoubleAnimation<FColor>>(FSolidColorBrush::ColorProperty(), noval, GDefaultTheme.Primary.NormalColor);
							BackgroundAnimation->Duration = 200;
							Storyboard_MarkBackFill->Timelines.AddRange({ BackgroundAnimation });
						}
						TReferPtr<FStoryboard> Storyboard_MarkBackStrokeThickness = MakeRefer<FStoryboard>(CheckedBack.Ref());
						{
							auto Animation = MakeRefer<TDoubleAnimation<float32>>(UShape::StrokeThicknessProperty(), noval, 0.0f);
							Animation->Duration = 200;
							Storyboard_MarkBackStrokeThickness->Timelines.AddRange({ Animation });
						}
						StateGroup_MarkBack_Common->AddVisualState(u8"Normal"N, { Storyboard_MarkBackFill, Storyboard_MarkBackStrokeThickness });
					}
					{
						TReferPtr<FStoryboard> Storyboard_MarkBackFill = MakeRefer<FStoryboard>(MarkBackFillBrush.Ref());
						{
							auto Animation = MakeRefer<TDoubleAnimation<FColor>>(FSolidColorBrush::ColorProperty(), noval, GDefaultTheme.Primary.HoverColor);
							Animation->Duration = 200;
							Storyboard_MarkBackFill->Timelines.AddRange({ Animation });
						}
						TReferPtr<FStoryboard> Storyboard_MarkBackStrokeThickness = MakeRefer<FStoryboard>(CheckedBack.Ref());
						{
							auto Animation = MakeRefer<TDoubleAnimation<float32>>(UShape::StrokeThicknessProperty(), noval, 1.0f);
							Animation->Duration = 200;
							Storyboard_MarkBackStrokeThickness->Timelines.AddRange({ Animation });
						}
						StateGroup_MarkBack_Common->AddVisualState(u8"MouseOver"N, { Storyboard_MarkBackFill, Storyboard_MarkBackStrokeThickness });
					}
					{
						TReferPtr<FStoryboard> Storyboard_MarkBackFill = MakeRefer<FStoryboard>(MarkBackFillBrush.Ref());
						{
							auto Animation = MakeRefer<TDoubleAnimation<FColor>>(FSolidColorBrush::ColorProperty(), noval, GDefaultTheme.Plane.DisableColor);
							Animation->Duration = 200;
							Storyboard_MarkBackFill->Timelines.AddRange({ Animation });
						}
						TReferPtr<FStoryboard> Storyboard_MarkBackStrokeThickness = MakeRefer<FStoryboard>(CheckedBack.Ref());
						{
							auto Animation = MakeRefer<TDoubleAnimation<float32>>(UShape::StrokeThicknessProperty(), noval, 1.0f);
							Animation->Duration = 200;
							Storyboard_MarkBackStrokeThickness->Timelines.AddRange({ Animation });
						}
						StateGroup_MarkBack_Common->AddVisualState(u8"Disabled"N, { Storyboard_MarkBackFill, Storyboard_MarkBackStrokeThickness });
					}
				}
				CheckBox.StateManager.StateGroups.Add(StateGroup_MarkBack_Common);
			}

			UPolylineRef CheckedMark = MakeRefer<UPolyline>(u8"CheckedMark"N);
			{
				FSolidColorBrushRef MarkFillBrush = MakeRefer<FSolidColorBrush>(GDefaultTheme.Primary.NormalColor);
				FSolidColorBrushRef MarkStrokeBrush = MakeRefer<FSolidColorBrush>(GDefaultTheme.Plane.NormalColor);

				CheckedMark->Points = { { 0, 12 }, { 8, 18 }, { 20, 3 } };
				CheckedMark->Stretch = EStretch::UniformFill;

				CheckedMark->Visibility = Binding(CheckBox, UCheckBox::CheckedProperty(),
					[](const TOptional<bool> & Checked) { return Checked == true ? EVisibility::Visible : EVisibility::Collapsed; });

				CheckedMark->HorizontalAlignment = EElementAlignment::Center;
				CheckedMark->VerticalAlignment = EElementAlignment::Center;
				CheckedMark->Size = { FDimen::Percent50, FDimen::Percent50 };
				CheckedMark->Fill = MarkFillBrush;
				CheckedMark->Stroke = MarkStrokeBrush;
				CheckedMark->StrokeThickness = 4.0f;
			}

			URectangleRef IndeterminateMark = MakeRefer<URectangle>(u8"IndeterminateMark"N);
			{
				FSolidColorBrushRef IndeterminateFillBrush = MakeRefer<FSolidColorBrush>(GDefaultTheme.Plane.NormalColor);
				IndeterminateMark->HorizontalAlignment = EElementAlignment::Center;
				IndeterminateMark->VerticalAlignment = EElementAlignment::Center;
				IndeterminateMark->Size = { FDimen::Percent60, 4.0f };
				IndeterminateMark->Fill = IndeterminateFillBrush;
				IndeterminateMark->Visibility = Binding(CheckBox, UCheckBox::CheckedProperty(),
					[](const TOptional<bool> & Checked) { return !Checked.HasValue ? EVisibility::Visible : EVisibility::Collapsed; });
			}

			MarkPanel->Elements = { UncheckedBack, CheckedBack, CheckedMark, IndeterminateMark };
			//RadioPanel->Grounding = Brushs::Blue;
		}

		UBorderRef Border = MakeRefer<UBorder>(UContentControl::PART_ContentPresenter);
		//Border->Grounding = Brushs::DeepPink;
		{
			Border->Margin = { 0, 0, 0, 0 };
			Border->Padding = { 4.0, 2.0 };
			Border->Thickness = 0;

			//Border->Background = Binding(&Button, UButton::BackgroundProperty());
			//Border->BorderThickness = Binding(&Button, UButton::BorderThicknessProperty());
		}

		StackPanel->Elements = { MarkPanel, Border };
		return StackPanel;
	}
}
