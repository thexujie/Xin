#include "PCH.h"

#include "00.Test.UI.Types.h"

UElementRef CreateElement_ScrollViewer()
{
	UPanelRef MainPanel;
	UPanelRef SecondPanel;
	UButtonRef ButtonA;
	auto OnButtonClick = [&](const FRoutedEventArgs & Args)
		{
			AssertExpr(Args.Sender.IsA<UButtonControl>());
			UButtonControl & Button = StaticCast<UButtonControl>(Args.Sender);
			static_assert(IsStaticPointerConvertibleV<const UButtonControl &, const IObject &>);
			FConsole::WriteLine(u8"Click Button {}, {}"V, Args.Sender.Name, StaticCast<UButtonControl>(Args.Sender).Content);
			//Button.Storyboards[0]->Begin();
		};

	UScrollViewerRef Element = ScrollViewer(Image(u8"../../Data/AryaStark.jpg"V,
				{
					{ UElement::HorizontalAlignmentProperty(), EElementAlignment::Center },
					{ UImage::StretchProperty(), EStretch::None }
				}),
			{ { UElement::SizeProperty(), FDimen2 { FDimen::Percent75, 512 } } }
		);

	return Element;
}
