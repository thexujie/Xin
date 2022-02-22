#include "PCH.h"
#include "Chrome.h"
#include "ChromePanel.h"

namespace Xin::UI
{
	static EHitResult HitTestBorder(FPoint Point, FSize FinalSize, FThickness FrameThickness, EHitResult HitInside)
	{
		EHitResult HitTestValues[9] = 
		{
			EHitResult::TopLeft, EHitResult::Top, EHitResult::TopRight,
			EHitResult::Left, HitInside, EHitResult::Right,
			EHitResult::BottomLeft, EHitResult::Bottom, EHitResult::BottomRight
		};

		uintx TestColIndex;
		if (Point.X < 0)
			TestColIndex = NullIndex;
		else if (Point.X < FrameThickness.Left)
			TestColIndex = 0;
		else if (Point.X < FinalSize.Width - FrameThickness.Right)
			TestColIndex = 1;
		else if (Point.X < FinalSize.Width)
			TestColIndex = 2;
		else
			TestColIndex = NullIndex;

		uintx TestRowIndex;
		if (Point.Y < 0)
			TestRowIndex = NullIndex;
		else if (Point.Y < FrameThickness.Top)
			TestRowIndex = 0;
		else if (Point.Y < FinalSize.Height - FrameThickness.Bottom)
			TestRowIndex = 1;
		else if (Point.Y < FinalSize.Height)
			TestRowIndex = 2;
		else
			TestRowIndex = NullIndex;

		if (TestRowIndex == NullIndex || TestColIndex == NullIndex)
			return EHitResult::None;

		return HitTestValues[TestRowIndex * 3 + TestColIndex];
	}

	void UChromeButton::OnMouseDown(FMouseButtonEventArgs & Args)
	{
		UButtonControl::OnMouseDown(Args);

		if (!Args.Handled && Command != EChromeCommand::None)
		{
			Args.Handled = true;
		}
	}

	void UChromeButton::OnClick(FRoutedEventArgs & Args)
	{
		UButtonControl::OnClick(Args);

		if (!Args.Handled && Command != EChromeCommand::None)
		{
			switch (Command)
			{
			case EChromeCommand::Close:
				Chrome.ChromePanel.CloseChrome(Chrome);
				Args.Handled = true;
				break;
			default: 
				break;
			}
		}
	}

	void UChromeBorder::OnConstruct()
	{
		UBorder::OnConstruct();
	}

	void UChromeBorder::OnMouseDown(FMouseButtonEventArgs & Args)
	{
		UBorder::OnMouseDown(Args);

		if (!Args.Handled)
		{
			EHitResult ResizingComponnet = HitTestBorder(Args.Position, FinalSize, ChromeThickness, HitInside);
			if (EHitResult::Left <= ResizingComponnet && ResizingComponnet <= EHitResult::BottomRight)
			{
				Chrome.ChromePanel.BeginResizeChrome(Chrome, ResizingComponnet);
				Args.Handled = true;
			}
		}
		
	}

	EHitResult UChromeBorder::HitTest(FPoint Point) const
	{
		EHitResult HitResult = HitTestBorder(Point, FinalSize, ChromeThickness, HitInside);
		return HitResult >= EHitResult::Left ? EHitResult::Inside : HitInside;
	}

	ECursor UChromeBorder::QueryCursor(FPoint Point) const
	{
		EHitResult HitResult = HitTestBorder(Point, FinalSize, ChromeThickness, HitInside);
		switch (HitResult)
		{
		default: 
			return UBorder::QueryCursor(Point);

		case EHitResult::Left:
		case EHitResult::Right:
			return ECursor::SizeHorizontal;

		case EHitResult::Top:
		case EHitResult::Bottom:
			return ECursor::SizeVertical;

		case EHitResult::TopLeft:
		case EHitResult::BottomRight:
			return ECursor::SizeNWSE;
			break;
		case EHitResult::TopRight:
		case EHitResult::BottomLeft:
			return ECursor::SizeNESW;
		}
	}

	void UChromeTitle::OnConstruct()
	{
		//ChromeComponent = EChromeComponent::Title;
		UControl::OnConstruct();
	}

	void UChromeTitle::OnMouseDown(FMouseButtonEventArgs & Args)
	{
		UControl::OnMouseDown(Args);

		if (!Args.Handled)
		{
			Chrome.ChromePanel.BeginDragChrome(Chrome);
			Args.Handled = true;
		}
	}

	void UChrome::OnConstruct()
	{
		UContentControl::OnConstruct();
		MinSize = { 64, 64 };
	}

	void UChrome::OnInitialize()
	{
		UContentControl::OnInitialize();
	}

	FSize UChrome::OnMeasure(FSize ConstrainedSize)
	{
		return UContentControl::OnMeasure(ConstrainedSize);
	}

	void UChrome::OnArrange()
	{
		UContentControl::OnArrange();
	}

	void UChrome::OnMouseDown(FMouseButtonEventArgs & Args)
	{
		UContentControl::OnMouseDown(Args);
		ChromePanel.BringToTop(*this);
	}

	void UChrome::OnLocationChanged(FPoint LocationBefore)
	{
		ChromePanel.InvalidateArrange();
	}
}
