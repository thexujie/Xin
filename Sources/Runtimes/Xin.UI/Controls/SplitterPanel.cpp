#include "PCH.h"
#include "SplitterPanel.h"

namespace Xin::UI
{
	void USplitterPanel::OnConstruct()
	{
		UStackPanel::OnConstruct();
		SplitterSize = 20.0f;
	}

	void USplitterPanel::OnInitialize()
	{
		UStackPanel::OnInitialize();
		HitInside = EHitResult::Inside;
	}

	FSize USplitterPanel::OnMeasure(FSize ConstrainedSize)
	{
		return UStackPanel::OnMeasure(ConstrainedSize);
	}

	void USplitterPanel::OnArrange()
	{
		UStackPanel::OnArrange();
	}

	UElement * USplitterPanel::HitElement(FPoint Point) const
	{
		if (HitTest(Point) < EHitResult::Inside)
			return nullptr;

		uintx ItemIndex = FindResizingItem(Point);
		if (ItemIndex == NullIndex)
			return UStackPanel::HitElement(Point);
		else
			return ConstCast(this);
	}

	ECursor USplitterPanel::QueryCursor(FPoint Point) const
	{
		uintx ItemIndex = FindResizingItem(Point);
		if (ItemIndex == NullIndex)
			return UStackPanel::QueryCursor(Point);
		return Orientation == EOrientation::Horizontal ? ECursor::SizeHorizontal : ECursor::SizeVertical;
	}

	void USplitterPanel::OnMouseDown(FMouseButtonEventArgs & Args)
	{
		if (!MouseCaptured && Args.Button == EMouseButton::Left)
		{
			ResizingIndex = FindResizingItem(Args.Position);
			if (ResizingIndex != NullIndex)
			{
				AcquireMouseCapture();
				DragStartPoint = LocalToGlobal(Args.Position);

				UElement & Element = Elements[ResizingIndex].Ref();
				DragStartSize = Element.FinalSize;
			}
		}
		UStackPanel::OnMouseMove(Args);
	}

	void USplitterPanel::OnMouseUp(FMouseButtonEventArgs & Args)
	{
		if (MouseCaptured && Args.Button == EMouseButton::Left)
		{
			ReleaseMouseCapture();
			DragStartPoint = FPoint::NaN;
			DragStartSize = FSize::NaN;
			ResizingIndex = NullIndex;
		}
		UStackPanel::OnMouseUp(Args);
	}

	void USplitterPanel::OnMouseMove(FMouseEventArgs & Args)
	{
		if (MouseCaptured && ResizingIndex != NullIndex)
		{
			FPoint CursorPoint = LocalToGlobal(Args.Position);
			FVec2 Delta = CursorPoint - DragStartPoint;

			UElement & Element = Elements[ResizingIndex].Ref();
			Element.SuspendLayout();
			if (Orientation == EOrientation::Horizontal)
				Element.Width = DragStartSize.Width + Delta.Width;
			else
				Element.Height = DragStartSize.Height + Delta.Height;
			Element.ResumeLayout();
			//LogWarning(u8"USplitterPanel::OnMouseMove {}"V, FDateTime::SteadyMilliseconds());
		}
		else
		{
			IndicatingIndex = FindResizingItem(Args.Position);
		}
		UStackPanel::OnMouseMove(Args);
	}

	void USplitterPanel::OnMouseLeave(FMouseEventArgs & Args)
	{
		IndicatingIndex = NullIndex;
		UStackPanel::OnMouseLeave(Args);
	}

	uintx USplitterPanel::FindResizingItem(FPoint Position) const
	{
		const dimenx HalfSplitterSize = SplitterSize * 0.5dim;
		if (Orientation == EOrientation::Horizontal)
		{
			for (uintx Index = 0; Index < Elements.Size; ++Index)
			{
				const UElement & Element = Elements[Index].Ref();
				if (Index > 0)
				{
					if (Element.FinalRect.X <= Position.X && Position.X < Element.FinalRect.X + HalfSplitterSize)
					{
						if (Elements[Index - 1].Ref()[ResizeEnabledProperty()].Get(true))
							return Index - 1;
					}
				}

				if (Element.FinalRect.Right - HalfSplitterSize <= Position.X && Position.X < Element.FinalRect.Right + HalfSplitterSize)
				{
					if (Element[ResizeEnabledProperty()].Get(true))
						return Index;
					else
						return NullIndex;
				}
			}
		}
		else
		{
			for (uintx Index = 1; Index < Elements.Size; ++Index)
			{
				const UElement & Element = Elements[Index].Ref();
				if (Element.FinalRect.Bottom - HalfSplitterSize <= Position.Y &&
					Position.Y < Element.FinalRect.Bottom + HalfSplitterSize)
				{
					if (Element[ResizeEnabledProperty()].Get(true))
						return Index;
					else
						return NullIndex;
				}
			}
		}
		return NullIndex;
	}

	void USplitterPanel::OnIndicatingIndexChanged(uintx IndicatingIndexBefore)
	{
		FSplitterPanelIndicatingIndexChagnedEventArgs EventArgs { IndicatingIndex, IndicatingIndexBefore };
		IndicatingIndexChanged(EventArgs);
	}

	void USplitterPanel::OnResigingIndexChanged(uintx ResigingIndexBefore)
	{
		FSplitterPanelResizingIndexChagnedEventArgs EventArgs { ResizingIndex, ResigingIndexBefore };
		ResigingIndexChanged(EventArgs);
	}
}
