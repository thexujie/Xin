#include "PCH.h"
#include "OverlapPanel.h"

namespace Xin::UI
{
	void UOverlapPanel::OnInitialize()
	{
		UPanel::OnInitialize();
	}

	FSize UOverlapPanel::OnMeasure(FSize ConstrainedSize)
	{
		FSize MeasuredSize = FSize::Zero;
		for (UElementRef & Element : Elements)
		{
			Element->Measure(ConstrainedSize);
			MeasuredSize = Max(MeasuredSize, StaticCast<UElement>(Element)->DesiredSize);
		}
		return MeasuredSize;
	}

	void UOverlapPanel::OnArrange()
	{
		for (UElementRef Element : Elements)
		{
			EElementAlignment ChildHorizontalAlignment = Element->GetValueT(UElement::HorizontalAlignmentProperty(), ElementHorizontalAlignment);
			FPoint ArrangePosition = FPoint::Zero;
			FSize ArrangeSize = Element->DesiredSize;
			switch (ChildHorizontalAlignment)
			{
			case EElementAlignment::Center:
				ArrangePosition.X = (FinalSize.Width - ArrangeSize.Width) * 0.5;
				break;
			case EElementAlignment::Far:
				ArrangePosition.X = FinalSize.Width - ArrangeSize.Width;
				break;
			case EElementAlignment::Stretch:
				ArrangeSize.Width = Max(FinalSize.Width, Element->DesiredSize.Width);
				break;
			default:
				break;
			}
			EElementAlignment ChildVerticallAlignment = Element->GetValueT(UElement::VerticalAlignmentProperty(), ElementVerticalAlignment);
			switch (ChildVerticallAlignment)
			{
			case EElementAlignment::Center:
				ArrangePosition.Y = (FinalSize.Height - ArrangeSize.Height) * 0.5;
				break;
			case EElementAlignment::Far:
				ArrangePosition.Y = FinalSize.Height - ArrangeSize.Height;
				break;
			case EElementAlignment::Stretch:
				ArrangeSize.Height = Max(FinalSize.Height, Element->DesiredSize.Height);
				break;
			default:
				break;
			}
			Element->Arrange({ ArrangePosition, ArrangeSize });
		}
	}
}
