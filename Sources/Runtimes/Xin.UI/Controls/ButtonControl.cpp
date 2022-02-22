#include "PCH.h"
#include "ButtonControl.h"

namespace Xin::UI
{
	UButtonControl::UButtonControl()
	{
	}

	void UButtonControl::OnConstruct()
	{
		UContentControl::OnConstruct();
		Cursor = ECursor::Arrow;
	}

	void UButtonControl::OnPaint(IPainter & Painter)
	{
		//Painter.FillRect({ FPoint::Zero, ArrangeRect.Size }, Colors::Red);
		//FString Text = Content.Get<FString>();
	}

	void UButtonControl::UpdateState()
	{
		if (IsPressed)
			SetState(u8"Pressed"N);
		else if (IsMouseOver)
			SetState(u8"MouseOver"N);
		else
			SetState(u8"Normal"N);
	}

	void UButtonControl::OnMouseDown(FMouseButtonEventArgs & Args)
	{
		IsPressed = true;
		UContentControl::OnMouseDown(Args);
		if (ClickMode == EClickMode::Press)
			OnClick();
	}

	void UButtonControl::OnMouseUp(FMouseButtonEventArgs & Args)
	{
		IsPressed = false;
		UContentControl::OnMouseDown(Args);
		if (ClickMode == EClickMode::Release)
			OnClick();
	}

	void UButtonControl::OnMouseEnter(FMouseEventArgs & Args)
	{
		UContentControl::OnMouseEnter(Args);
	}

	void UButtonControl::OnMouseLeave(FMouseEventArgs & Args)
	{
		IsPressed = false;
		UContentControl::OnMouseLeave(Args);
	}

	void UButtonControl::OnClick(FRoutedEventArgs & Args)
	{
		Click.Excute(Args);
		if (!Args.Handled && Command)
			OnCommand(Command.Ref(), Args);
	}

	//void UButtonBase::SetIsPressed(bool Value)
	//{
	//	if (IsPressed_ != Value)
	//	{
	//		IsPressed_ = Value;
	//		FBoolEventArgs BoolEventArgs { IsPressed_ };
	//		IsPressedChanged(BoolEventArgs);

	//		if ((!IsPressed_ && ClickMode == EClickMode::Release) || (IsPressed_ && ClickMode == EClickMode::Press))
	//		{
	//			FRoutedEventArgs RoutedEventArgs { *this };
	//			Click.Excute(RoutedEventArgs);
	//		}
	//		else {}
	//	}
	//}
}
