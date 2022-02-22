#include "PCH.h"
#include "ScrollBarStyle.h"
#include "Core/Setter.h"

namespace Xin::UI::Themes::Default
{
	UElementRef FScrollBarThumbControlTemplate::LoadTemplateT(UThumb & Thumb)
	{
		Thumb.Background = MakeRefer<FSolidColorBrush>(GDefaultTheme.Bar.NormalColor);
		Thumb.Triggers.Add(MakeRefer<FTrigger>(UElement::IsMouseOverProperty(), 
			TView<ITriggerActionRef>{ MakeRefer<FSetterAction>(Thumb.Background, FSolidColorBrush::ColorProperty(), GDefaultTheme.Bar.HoverColor) },
			TView<ITriggerActionRef>{ MakeRefer<FSetterAction>(Thumb.Background, FSolidColorBrush::ColorProperty(), GDefaultTheme.Bar.NormalColor) }
		));
		return nullptr;
		//URectangleRef Rectangle = MakeRefer<URectangle>();
		//Rectangle->Fill = Binding(&Thumb, UThumb::BackgroundProperty());
		//return Rectangle;
	}

	UElementRef FScrollBarTemplate::LoadTemplateT(UScrollBar & ScrollBar)
	{
		ScrollBar.ResourceTable.Resources.Add(MakeRefer<FScrollBarThumbControlTemplate>());

		UTrackRef Track = MakeRefer<UTrack>();
		{
			Track->Name = UScrollBar::Name_Track;

			Track->Thumb = MakeRefer<UThumb>(UTrack::Name_Thumb);
			Track->IncreaseButton = MakeRefer<URepeatButton>(UTrack::Name_Increase);
			Track->DecreaseButton = MakeRefer<URepeatButton>(UTrack::Name_Decrease);

			Track->IncreaseButton->Command = UScrollBar::PageUpCommand;
			Track->DecreaseButton->Command = UScrollBar::PageDownCommand;

			Track->Size = { FDimen::Percent100, FDimen::Percent100 };
			Track->Orientation = Binding(&ScrollBar, UScrollBar::OrientationProperty());
			Track->ViewportSize = Binding(ScrollBar, UScrollBar::ViewportSizeProperty());
			Track->Maximum = Binding(ScrollBar, UScrollBar::MaximumProperty());
			Track->Minimum = Binding(ScrollBar, UScrollBar::MinimumProperty());
			Track->Value = Binding(ScrollBar, UScrollBar::ValueProperty(), EBindingMode::TwoWay);
			//Viewbox->Child = Track;
			//return Viewbox;
			Track->Background = MakeRefer<FSolidColorBrush>(0xFFE8E8E8);
		}

		return Track;
	}

	void FScrollBarStyle::LoadStyleT(UScrollBar & ScrollBar)
	{
	}
}
