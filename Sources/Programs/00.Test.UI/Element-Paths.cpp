#include "PCH.h"

#include "00.Test.UI.Types.h"
#include "Element-Paths.h"

UElementRef CreateElement_Paths()
{
	constexpr FStringV TestIconClose =
		u8"M517.38624 570.32704l-91.66848 91.66848-48.08704-48.08704 91.66848-91.66848-98.75456-98.73408 48.08704-48.08704 98.75456 98.73408 98.73408-98.73408 48.08704 48.08704-98.73408 98.73408 91.648 91.66848-48.08704 48.08704-91.648-91.66848z"

		u8"m417.3824-126.464c4.8128 25.3952 7.31136 51.6096 7.31136 78.37696"
		u8"C942.08 754.11456 754.11456 942.08 522.24 942.08"
		u8"S102.4 754.11456 102.4 522.24 290.36544 102.4 522.24 102.4"
		u8"c66.31424 0 129.024 15.36 184.79104 42.74176l-44.8512 73.87136C616.6528 196.64896 574.27968 186.368 522.24 186.368"
		u8"c-185.48736 0-335.872 150.38464-335.872 335.872 0 185.48736 150.38464 335.872 335.872 335.872 185.48736 0 335.872-150.38464 335.872-335.872 0-21.2992-1.96608-42.27072-5.8368-62.79168"
		u8"l82.49344-15.5648"
		u8"z"

		u8"m-120.97536-61.56288a81.63328 81.63328 0 1 1 0-163.28704 81.63328 81.63328 0 0 1 0 163.28704z"V
		;


	UPanelRef MainPanel;
	UScrollViewerRef Element = ScrollViewer(StackPanel(EOrientation::Horizontal,
		{
			// TestIconClose
			Path(TestIconClose, { 0.5f, {} },
				{
					{ UElement::HorizontalAlignmentProperty(), EElementAlignment::Center },
					{ UPath::FillProperty(), Brushs::AntiqueWhite },
					{ UPath::StrokeProperty(), Brushs::CadetBlue },
					{ UPath::StrokeThicknessProperty(), 4 },
				}),

			Image(u8"../../Data/AryaStark.jpg"V,
				{
					{ UElement::HorizontalAlignmentProperty(), EElementAlignment::Center },
					{ UImage::StretchProperty(), EStretch::None }
				})
		}
	));

	return Element;
}
