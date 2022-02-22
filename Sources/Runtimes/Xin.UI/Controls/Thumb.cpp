#include "PCH.h"
#include "Thumb.h"

namespace Xin::UI
{
	UThumb::UThumb()
	{
	}

	void UThumb::OnConstruct()
	{
		{
			const FProperty & Property = BackgroundProperty();
			static FPropertyMetadata BackgroundPropertyMetadata = StaticCast<FPropertyMetadata>(BackgroundProperty().Metadata);
			std::once_flag OnceFlag;
			std::call_once(OnceFlag, []() {BackgroundPropertyMetadata.AffectsRender = true; });
			OverridenMetadatas.Add({ Property , BackgroundPropertyMetadata });
		}
		UControl::OnConstruct();
	}

	void UThumb::OnPaint(IPainter & Painter)
	{
		if (Background)
			Painter.DrawRect({ FPoint::Zero, FinalSize }, Background.Ref(), IPen::None);
	}

	void UThumb::OnMouseEnter(FMouseEventArgs & Args)
	{
		UControl::OnMouseEnter(Args);
	}

	void UThumb::OnMouseLeave(FMouseEventArgs & Args)
	{
		UControl::OnMouseLeave(Args);
	}

	void UThumb::OnMouseDown(FMouseButtonEventArgs & Args)
	{
		if (!MouseCaptured && Args.Button == EMouseButton::Left)
		{
			AcquireMouseCapture();
			DragLastPoint = DragStartPoint = LocalToGlobal(Args.Position);

			FDragStartedEventArgs DragStartedEventArgs { DragStartPoint };
			DragStarted(DragStartedEventArgs);
		}
		UControl::OnMouseDown(Args);
	}

	void UThumb::OnMouseUp(FMouseButtonEventArgs & Args)
	{
		if (MouseCaptured && Args.Button == EMouseButton::Left)
		{
			ReleaseMouseCapture();
			FPoint DragEndPoint = LocalToGlobal(Args.Position);

			FDragCompletedEventArgs DragStartedEventArgs;
			DragCompleted(DragStartedEventArgs);
		}
		UControl::OnMouseDown(Args);
	}

	void UThumb::OnMouseMove(FMouseEventArgs & Args)
	{
		if (MouseCaptured)
		{
			FPoint CursorPoint = LocalToGlobal(Args.Position);
			FVec2 Delta = CursorPoint - DragLastPoint;
			//FConsole::WriteLine(u8"DragDelta {}"V, Delta);
			DragLastPoint = CursorPoint;

			FDragDeltaEventArgs DragDeltaEventArgs { Delta, CursorPoint, DragStartPoint };
			DragDelta(DragDeltaEventArgs);
		}
		UControl::OnMouseMove(Args);
	}

	void UThumb::OnMouseCapturedChanged(bool ValueBefore)
	{
		UControl::OnMouseCapturedChanged(ValueBefore);
	}
}
