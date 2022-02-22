#include "PCH.h"
#include "Track.h"

namespace Xin::UI
{
	UTrack::UTrack()
	{
	}

	void UTrack::OnInitialize()
	{
		UElement::OnInitialize();

		Thumb->DragStarted += { this, & UTrack::OnThumbDragStarted };
		Thumb->DragDelta += { this, &UTrack::OnThumbDragDelta };
	}

	void UTrack::OnFinalize()
	{
		UElement::OnFinalize();
	}

	FSize UTrack::OnMeasure(FSize ConstrainedSize)
	{
		MeasureChild(Thumb, ConstrainedSize);
		MeasureChild(IncreaseButton, ConstrainedSize);
		MeasureChild(DecreaseButton, ConstrainedSize);

		return UElement::OnMeasure(ConstrainedSize);
	}

	void UTrack::OnArrange()
	{
		ArrangeParts();
	}

	void UTrack::OnPaint(IPainter & Painter)
	{
		if (Background)
			Painter.DrawRect({ { }, FinalSize }, Background.Ref(), IPen::None);
	}

	void UTrack::OnMouseDown(FMouseButtonEventArgs & Args)
	{
		UElement::OnMouseDown(Args);
	}

	void UTrack::OnMouseUp(FMouseButtonEventArgs & Args)
	{
		UElement::OnMouseDown(Args);
	}

	void UTrack::OnMouseEnter(FMouseEventArgs & Args)
	{
		UElement::OnMouseEnter(Args);
	}

	void UTrack::OnMouseLeave(FMouseEventArgs & Args)
	{
		UElement::OnMouseLeave(Args);
	}

	void UTrack::ArrangeParts()
	{
		if (not IsNaN(ViewportSize))
		{
			dimenx ValueRange = Max(Maximum - Minimum, 0);
			dimenx ValueOffset = Min(ValueRange, Value - Minimum);

			dimenx TrackLength = Orientation == EOrientation::Horizontal ? FinalSize.Width : FinalSize.Height;
			constexpr dimenx ButtonLengthMin = 24.0f;

			dimenx ThumbLength = (ViewportSize / (ValueRange + ViewportSize)) * TrackLength;
			ThumbLength = Clamp(ThumbLength, /*DimenT(0)*/ButtonLengthMin * 0.5, TrackLength);

			if (ThumbLength < TrackLength && LessThanOrClose(0dim, ValueRange))
			{
				dimenx RemainingLength = TrackLength - ThumbLength;
				dimenx IncreaseLength = Clamp(RemainingLength * ValueOffset / ValueRange, 0dim, RemainingLength);
				dimenx DecreaseLength = Clamp(RemainingLength - IncreaseLength, 0dim, RemainingLength);

				DimenPerPixel = ValueRange / RemainingLength;
				if (Orientation == EOrientation::Vertical)
				{
					// Vertical Normal   :    |Inc Button |
					//                        |Thumb      |
					//                        |Dec Button | 
					// Vertical Reversed :    |Dec Button |
					//                        |Thumb      |
					//                        |Inc Button |
					IncreaseButton->Arrange({ FPoint::Zero, { FinalSize.Width, IncreaseLength } });
					Thumb->Arrange({ { 0dim, IncreaseLength }, { FinalSize.Width, ThumbLength } });
					DecreaseButton->Arrange({ { 0dim, IncreaseLength + ThumbLength }, { FinalSize.Width, DecreaseLength } });
				}
				else
				{
					IncreaseButton->Arrange({ FPoint::Zero, { IncreaseLength, FinalSize.Height } });
					Thumb->Arrange({ { IncreaseLength, 0dim }, { ThumbLength, FinalSize.Height } });
					DecreaseButton->Arrange({ { IncreaseLength + ThumbLength, 0dim }, { DecreaseLength, FinalSize.Height } });
				}

				IncreaseButton->Visibility = EVisibility::Visible;
				Thumb->Visibility = EVisibility::Visible;
				DecreaseButton->Visibility = EVisibility::Visible;
			}
			else
			{
				IncreaseButton->Visibility = EVisibility::Collapsed;
				Thumb->Visibility = EVisibility::Collapsed;
				DecreaseButton->Visibility = EVisibility::Collapsed;
			}
		}
	}

	void UTrack::OnThumbDragStarted(FDragStartedEventArgs & Args)
	{
		DragStartPosition = Args.Position;
		DragStartValue = Value;
	}

	void UTrack::OnThumbDragDelta(FDragDeltaEventArgs & Args)
	{
		if (DragStartPosition.AnyNaN() || LessThanOrCloseZero(DimenPerPixel))
			return;

		FVec2 DragDelta = Args.Position - DragStartPosition;
		dimenx CurrentValue = DragStartValue + (Orientation == EOrientation::Horizontal ? DragDelta.X : DragDelta.Y) * DimenPerPixel;
		Value = Clamp(CurrentValue, Minimum, Maximum);
		ArrangeParts();
	}

	void UTrack::OnThumbChanged(UThumbRef ThumbBefore)
	{
		if (ThumbBefore)
			OnRemoveChild(ThumbBefore);

		if (UElementRef Element = Thumb)
			OnAddChild(Element);
	}

	void UTrack::OnIncreaseButtonChanged(URepeatButtonRef IncreaseButtonBefore)
	{
		if (IncreaseButtonBefore)
			OnRemoveChild(IncreaseButtonBefore);

		if (UElementRef Element = IncreaseButton)
			OnAddChild(Element);
	}

	void UTrack::OnDecreaseButtonChanged(URepeatButtonRef DecreaseButtonBefore)
	{
		if (DecreaseButtonBefore)
			OnRemoveChild(DecreaseButtonBefore);

		if (UElementRef Element = DecreaseButton)
			OnAddChild(Element);
	}
}
