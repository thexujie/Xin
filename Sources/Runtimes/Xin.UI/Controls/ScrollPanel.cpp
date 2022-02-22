#include "PCH.h"
#include "ScrollPanel.h"

namespace Xin::UI
{
	void UScrollPanel::OnInitialize()
	{
		UElement::OnInitialize();
	}

	void UScrollPanel::OnInitialized()
	{
		HorizontalScrollBar = FindChildT<UScrollBar>(UControl::PART_HorizontalScrollBar);
		VerticalScrollBar = FindChildT<UScrollBar>(UControl::PART_VerticalScrollBar);
		ConnerElement = FindChildT<UElement>(UControl::PART_ConnerElement);

		if (ScrollInfo)
			ScrollInfo->ScrollChanged += { this, & UScrollPanel::ScrollArea_OnScrollChanged };
		if (HorizontalScrollBar)
		{
			HorizontalScrollBar->PageUp = { this, &UScrollPanel::HorizontalScrollBar_OnPageUp };
			HorizontalScrollBar->PageDown = { this, &UScrollPanel::HorizontalScrollBar_OnPageDown };
		}
		if (VerticalScrollBar)
		{
			VerticalScrollBar->PageUp = { this, &UScrollPanel::VerticalScrollBar_OnPageUp };
			VerticalScrollBar->PageDown = { this, &UScrollPanel::VerticalScrollBar_OnPageDown };
		}

		UpdateScrollBars();

		UElement::OnInitialized();
	}

	FSize UScrollPanel::OnMeasure(FSize ConstrainedSize)
	{
		MeasureChild(Child, ConstrainedSize);
		MeasureChild(HorizontalScrollBar, ConstrainedSize);
		MeasureChild(VerticalScrollBar, ConstrainedSize);
		MeasureChild(ConnerElement, ConstrainedSize);
		return FSize::Zero;
	}

	void UScrollPanel::OnArrange()
	{
		FSize ElementSize = FinalSize;
		if (HorizontalScrollBar)
		{
			//HorizontalScrollBar->Measure(FinalSize);

			HorizontalScrollBar->Arrange({
					0,
					FinalSize.Height - HorizontalScrollBar->DesiredSize.Height,
					HorizontalScrollBar->DesiredSize.Width,
					HorizontalScrollBar->DesiredSize.Height
				});
			if (HorizontalScrollBar->Visibility != EVisibility::Collapsed)
				ElementSize.Height -= HorizontalScrollBar->FinalSize.Height;
		}

		if (VerticalScrollBar)
		{
			//VerticalScrollBar->Measure(FinalSize);

			VerticalScrollBar->Arrange({ 
				FinalSize.Width - VerticalScrollBar->DesiredSize.Width,
				0,
				VerticalScrollBar->DesiredSize.Width,
				ElementSize.Height,
			});
			if (VerticalScrollBar->Visibility != EVisibility::Collapsed)
				ElementSize.Width -= VerticalScrollBar->FinalSize.Width;
		}

		if (ConnerElement &&
			HorizontalScrollBar && HorizontalScrollBar->Visibility != EVisibility::Collapsed &&
			VerticalScrollBar && VerticalScrollBar->Visibility != EVisibility::Collapsed)
		{
			//ConnerElement->Measure({ VerticalScrollBar->FinalSize.Width, HorizontalScrollBar->FinalSize.Height });

			ConnerElement->Arrange( {
				ElementSize.Width,
				ElementSize.Height,
				VerticalScrollBar->FinalSize.Width,
				HorizontalScrollBar->FinalSize.Height,
			});
		}

		if (UElementRef ChildElement = Child)
			ChildElement->Arrange({ FPoint::Zero, ElementSize });
	}

	void UScrollPanel::OnArranged()
	{
		UpdateScrollInfo();
		UpdateScrollBars();
		UElement::OnArranged();
	}

	void UScrollPanel::ScrollArea_OnScrollChanged(FEventArgs & EventArgs)
	{
		UpdateScrollInfo();
		UpdateScrollBars();
	}

	void UScrollPanel::VerticalScrollBar_OnPageUp(FEventArgs & EventArgs)
	{
		if (ScrollInfo)
			ScrollInfo->PageUp();
	}

	void UScrollPanel::VerticalScrollBar_OnPageDown(FEventArgs & EventArgs)
	{
		if (ScrollInfo)
			ScrollInfo->PageDown();
	}

	void UScrollPanel::HorizontalScrollBar_OnPageUp(FEventArgs & EventArgs)
	{
		if (ScrollInfo)
			ScrollInfo->PageLeft();
	}

	void UScrollPanel::HorizontalScrollBar_OnPageDown(FEventArgs & EventArgs)
	{
		if (ScrollInfo)
			ScrollInfo->PageRight();
	}

	void UScrollPanel::UpdateScrollInfo()
	{
		if (ScrollInfo)
		{
			ViewportSize = ScrollInfo->ViewportSize;
			ScrollExtent = ScrollInfo->ExtentSize;
			ScrollOffset = ScrollInfo->ScrollOffset;
		}
	}

	void UScrollPanel::UpdateScrollBars()
	{
		if (HorizontalScrollBar)
		{
			if (HorizontalScrollBarVisibility.HasValue)
				HorizontalScrollBar->Visibility = HorizontalScrollBarVisibility;
			else
				HorizontalScrollBar->Visibility = (ScrollInfo && ScrollInfo->ExtentSize.Width > 0) ? 
				EVisibility::Visible : EVisibility::Collapsed;
		}

		if (VerticalScrollBar)
		{
			if (VerticalScrollBarVisibility.HasValue)
				VerticalScrollBar->Visibility = VerticalScrollBarVisibility;
			else
				VerticalScrollBar->Visibility = (ScrollInfo && ScrollInfo->ExtentSize.Height > 0) ? 
				EVisibility::Visible : EVisibility::Collapsed;
		}

		if (ConnerElement)
		{
			ConnerElement->Visibility = (HorizontalScrollBar && HorizontalScrollBar->Visibility != EVisibility::Collapsed &&
				VerticalScrollBar && VerticalScrollBar->Visibility != EVisibility::Collapsed) ?
				EVisibility::Visible :EVisibility::Collapsed;
		}
	}

	void UScrollPanel::OnScrollOffsetChanged(FVec2 ValueBefore)
	{
		if (ScrollInfo)
			ScrollInfo->Scroll(ScrollOffset);
	}

	void UScrollPanel::OnChildChanged(UElementRef ChildBefore)
	{
		if (ChildBefore)
			OnRemoveChild(ChildBefore);

		if (UElementRef ChildElement = Child)
			OnAddChild(ChildElement);
	}

	void UScrollPanel::OnHorizontalScrollBarChanged(UScrollBarRef HorizontalScrollBarBefore)
	{
		if (HorizontalScrollBarBefore)
			OnRemoveChild(HorizontalScrollBarBefore);

		if (UElementRef ChildElement = HorizontalScrollBar)
			OnAddChild(ChildElement);
	}

	void UScrollPanel::OnVerticalScrollBarChanged(UScrollBarRef VerticalScrollBarBefore)
	{
		if (VerticalScrollBarBefore)
			OnRemoveChild(VerticalScrollBarBefore);

		if (UElementRef Element = VerticalScrollBar)
			OnAddChild(Element);
	}

	void UScrollPanel::OnConnerElementChanged(UElementRef ConnerElementBefore)
	{
		if (ConnerElementBefore)
			OnRemoveChild(ConnerElementBefore);

		if (UElementRef Element = ConnerElement)
			OnAddChild(Element);
	}
}
