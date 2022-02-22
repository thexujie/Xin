#include "PCH.h"
#include "ScrollArea.h"

namespace Xin::UI
{
	void UScrollArea::OnConstruct()
	{
		ScrollInfo->ScrollChanged += { this, & UScrollArea::OnScrollChanged };
	}

	void UScrollArea::OnInitialize()
	{
		UPresenter::OnInitialize();
	}

	FSize UScrollArea::OnMeasure(FSize ConstrainedSize)
	{
		return UPresenter::OnMeasure(ConstrainedSize);
	}

	void UScrollArea::OnArrange()
	{
		if (Child)
			Child->Arrange({ -ScrollInfo->ScrollOffset, Max(Child->DesiredSize, FinalSize) });
	}

	void UScrollArea::OnArranged()
	{
		UPresenter::OnArranged();

		FDispatcher::Current().BeginInvoke({ this, [this]()
			{
				if (UElementRef ChildElement = Child; ChildElement && ScrollInfo)
					ScrollInfo->SetViewport(FinalSize, Max(ChildElement->FinalRect.Size - FinalSize, FSize::Zero));
			}
		});
	}

	void UScrollArea::OnMouseWhell(FMouseWhellEventArgs & Args)
	{
		if (ScrollInfo)
		{
			dimenx ScrollOffsetY = Clamp(ScrollInfo->ScrollOffset.Y + Args.Whell.Y * 30.0, 0.0, ScrollInfo->ExtentSize.Y);
			if (ScrollOffsetY != ScrollInfo->ScrollOffset.Y)
			{
				ScrollInfo->Scroll(FVec2 { ScrollInfo->ScrollOffset.X, ScrollOffsetY });
				Args.Handled = true;
			}
		}
	}

	void UScrollArea::OnScrollChanged(FEventArgs & EventArgs)
	{
		InvalidateArrange();
	}
}

