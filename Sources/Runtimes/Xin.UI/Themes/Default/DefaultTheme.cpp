#include "PCH.h"
#include "DefaultTheme.h"

namespace Xin::UI::Themes::Default
{
	UI_API FDefaultTheme GDefaultTheme;

	UElementRef FScrollViewerTemplate::LoadTemplateT(UScrollViewer & ScrollViewer)
	{
		UScrollPanelRef ScrollPanel = MakeRefer<UScrollPanel>();

		UScrollAreaRef ScrollArea = MakeRefer<UScrollArea>(UControl::PART_ContentPresenter);

		UScrollBarRef VerticalScrollBar = MakeRefer<UScrollBar>(UControl::PART_VerticalScrollBar);
		{
			VerticalScrollBar->Size = FDimen2 { 20, FDimen::Percent100 };
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
		}
		URectangleRef ConnerRectangle = MakeRefer<URectangle>(UControl::PART_ConnerElement);
		{
			ConnerRectangle->Fill = MakeRefer<FSolidColorBrush>(0xFFE8E8E8);
		}

		{
			ScrollPanel->HorizontalScrollBar = HorizontalScrollBar;
			ScrollPanel->VerticalScrollBar = VerticalScrollBar;
			ScrollPanel->ConnerElement = ConnerRectangle;

			ScrollPanel->ScrollInfo = ScrollArea->ScrollInfo;
			ScrollPanel->Child = ScrollArea;
		}
		return ScrollPanel;
	}

	void FScrollViewerStyle::LoadStyleT(UScrollViewer & ScrollViewer)
	{

	}

	UElementRef FListControlTemplate::LoadTemplateT(UListControl & ListControl)
	{
		UBorderRef Border = MakeRefer<UBorder>();
		//Border->Margin = 30;
		//Border->Padding = { 2.0, 2.0 };
		Border->Fill = Binding(&ListControl, UControl::BackgroundProperty());
		Border->Thickness = Binding(&ListControl, UControl::BorderThicknessProperty());
		Border->Stroke = Binding(&ListControl, UControl::BorderStrokeProperty());
		return Border;
	}

	void FListControlStyle::LoadStyleT(UListControl & ListControl)
	{
		ListControl.BorderThickness = 1;
		ListControl.BorderStroke = MakeRefer<FSolidColorBrush>(GDefaultTheme.BorderColor);
	}
}
