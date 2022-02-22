#include "PCH.h"
#include "ChromePanel.h"
#include "Window.h"

namespace Xin::UI
{
	void UChromePanel::OnConstruct()
	{
		UPanel::OnConstruct();
		SortComparer = [](const UElementRef & ElementA, const UElementRef & ElementB)
			{
				return StaticCast<UChrome>(ElementA)->ChromeDepth < StaticCast<UChrome>(ElementB)->ChromeDepth;
			};
		HitInside = EHitResult::Ignore;
	}

	void UChromePanel::Initialize()
	{
		for (UElementRef Element : Chromes)
			Elements.AddItem(Element);

		UPanel::Initialize();
	}

	void UChromePanel::OnInitialize()
	{
		UPanel::OnInitialize();
	}

	FSize UChromePanel::OnMeasure(FSize ConstrainedSize)
	{
		FSize ExpectedSize = FSize::Zero;

		for (UElementRef Element : Chromes)
		{
			FSize ChildSize = Element->Measure(ConstrainedSize);
			ExpectedSize = Max(ExpectedSize, ChildSize);
		}
		return ExpectedSize;
	}

	void UChromePanel::OnArrange()
	{
		SortElements();

		for (UChromeRef Chrome : Chromes)
		{
			Chrome->Arrange({ Chrome->Location , Chrome->DesiredSize });
		}
	}

	UElement * UChromePanel::HitElement(FPoint Point) const
	{
		//UChromeRef Chrome = AdjustingChrome.Lock();
		//if (Chrome)
		//	return Chrome;

		return UPanel::HitElement(Point);
	}

	ECursor UChromePanel::QueryCursor(FPoint Point) const
	{
		return UPanel::QueryCursor(Point);
	}

	EHitResult UChromePanel::HitTest(FPoint Point) const
	{
		return UPanel::HitTest(Point);
	}


	void UChromePanel::OnMouseDown(FMouseButtonEventArgs & Args) 
	{
		return UPanel::OnMouseDown(Args);
	}

	void UChromePanel::OnMouseUp(FMouseButtonEventArgs & Args) 
	{
		EndAdjustChrome();
		return UPanel::OnMouseUp(Args);
	}

	void UChromePanel::OnMouseMove(FMouseEventArgs & Args) 
	{
		UChromeRef Chrome = AdjustingChrome.Lock();
		if (MouseCaptured && Chrome && !PreviousCursorPosition.AnyNaN())
		{
			FPoint CursorPosition = LocalToGlobal(Args.Position);
			FVec2 CursorDelta = CursorPosition - PreviousCursorPosition;

			// Moving
			if (ResizingChromeComponent == EHitResult::None)
			{

				Chrome->Location = PreviousChromeLocation + CursorDelta;
				//LogWarning(u8"USplitterPanel::OnMouseMove {}"V, FDateTime::SteadyMilliseconds());
			}
			// Resizing
			else
			{
				FPoint Location = PreviousChromeLocation;
				FSize Size = PreviousChromeSize;
				switch (ResizingChromeComponent)
				{
				case EHitResult::Left:
				case EHitResult::TopLeft:
				case EHitResult::BottomLeft:
					Location.X += CursorDelta.X;
					Size.Width -= CursorDelta.X;
					break;
				case EHitResult::Right:
				case EHitResult::TopRight:
				case EHitResult::BottomRight:
					Size.Width += CursorDelta.X;
					break;
				default:
					break;
				}

				switch (ResizingChromeComponent)
				{
				case EHitResult::Top:
				case EHitResult::TopLeft:
				case EHitResult::TopRight:
					Location.Y += CursorDelta.Y;
					Size.Height -= CursorDelta.Y;
					break;
				case EHitResult::Bottom:
				case EHitResult::BottomLeft:
				case EHitResult::BottomRight:
					Size.Height += CursorDelta.Y;
					break;
				default:
					break;
				}

				Size = Max(Size, MinSize);
				if (Size != Chrome->Size)
					Chrome->SetEvaluatedValue<FDimen2>(UElement::SizeProperty(), Size);
				Chrome->Location = Location;
			}
		}
		else
		{
			AdjustingChrome = nullptr;
		}
		return UPanel::OnMouseMove(Args);
	}

	void UChromePanel::OnMouseLeave(FMouseEventArgs & Args)
	{
		EndAdjustChrome();
		return UPanel::OnMouseLeave(Args);
	}

	UChromeRef UChromePanel::FindChrome(FPoint Point) const
	{
		for (const UChromeRef & Chrome : Chromes)
		{
			if (Chrome->HitTest(Chrome->ParentToLocal(Point)) >= EHitResult::Inside)
				return Chrome;
		}

		return nullptr;
	}

	void UChromePanel::ShowChromes()
	{
		for (UChromeRef & Chrome : Chromes)
			Chrome->Visibility = EVisibility::Visible;
	}

	void UChromePanel::BeginDragChrome(UChrome & Chrome)
	{
		BringToTop(Chrome);

		AcquireMouseCapture();
		PreviousCursorPosition = Window.Desktop.GetCursorPosition();
		PreviousChromeLocation = Chrome.Location;
		AdjustingChrome = &Chrome;
	}

	void UChromePanel::EndDragChrome()
	{
		if (!MouseCaptured || ResizingChromeComponent != EHitResult::None)
			return;

		EndAdjustChrome();
	}

	void UChromePanel::BeginResizeChrome(UChrome & Chrome, EHitResult ResizingComponnet)
	{
		BringToTop(Chrome);

		AcquireMouseCapture();
		AdjustingChrome = &Chrome;

		PreviousCursorPosition = Window.Desktop.GetCursorPosition();
		PreviousChromeLocation = Chrome.Location;
		PreviousChromeSize = Chrome.FinalSize;
		ResizingChromeComponent = ResizingComponnet;
	}

	void UChromePanel::EndResizeChrome()
	{
		if (!MouseCaptured || ResizingChromeComponent == EHitResult::None)
			return;

		EndAdjustChrome();
	}

	void UChromePanel::EndAdjustChrome()
	{
		if (!MouseCaptured)
			return;

		ReleaseMouseCapture();
		AdjustingChrome = nullptr;
		PreviousCursorPosition = FPoint::NaN;
		PreviousChromeLocation = FPoint::NaN;
		PreviousChromeSize = FPoint::NaN;
		ResizingChromeComponent = EHitResult::None;
	}

	void UChromePanel::CloseChrome(UChrome & Chrome)
	{
		Chrome.Visibility = EVisibility::Hidden;
	}

	void UChromePanel::BringToTop(UChrome & Chrome)
	{
		if (Chrome.ChromeDepth + 1 == Chromes.Size)
			return;

		Chrome.ChromeDepth = IntXMax;

		auto ChromeSortComparer = [](const UChromeRef & ChromeA, const UChromeRef & ChromeB)
			{
				return ChromeA->ChromeDepth < ChromeB->ChromeDepth;
			};
		std::sort(Chromes.Begin(), Chromes.End(), ChromeSortComparer);

		for (intx ChromeDepth = 0; ChromeDepth < Chromes.Size; ++ChromeDepth)
		{
			Chromes[ChromeDepth]->ChromeDepth = ChromeDepth;
			Chromes[ChromeDepth]->InnerZIndex = ChromeDepth;
		}

		SortChildrenZ();
	}
}
