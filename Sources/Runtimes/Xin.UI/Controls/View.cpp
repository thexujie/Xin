#include "PCH.h"
#include "View.h"
#include "RadioButton.h"

namespace Xin::UI
{
	UView::UView()
	{
	}

	UView::~UView()
	{
		
	}

	void UView::OnConstruct()
	{
		UContentControl::OnConstruct();
	}

	void UView::OnInitialize()
	{
		UContentControl::OnInitialize();
	}

	UElement * UView::HitElement(FPoint Point) const
	{
		if (auto Captured = CapturedElement.Lock())
		{
			if (auto CapturedChild = Captured->HitElement(Captured->WindowToLocal(Point)))
				return CapturedChild;
			return Captured;
		}
		return UContentControl::HitElement(Point);
	}

	void UView::SetHoveredElement(UElement * Element, FPoint Point)
	{
		if (HoveredElement == Element)
			return;

		{
			//AssertExpresion(!Element || !Element->IsMouseOver);

			UElement * BranchElement = Element;
			while (BranchElement)
			{
				if (BranchElement->IsMouseOver)
					break;
				BranchElement = BranchElement->Parent.Lock();
			}

			UElement * LeavingElement = HoveredElement.Lock();
			while (LeavingElement && LeavingElement != BranchElement)
			{
				FMouseEventArgs MouseLeaveArgs { *this };
				MouseLeaveArgs.Position = LeavingElement->WindowToLocal(Point);
				MouseLeaveArgs.Element = HoveredElement.Lock();
				LeavingElement->PreMouseLeave(MouseLeaveArgs);

				LeavingElement = LeavingElement->Parent.Lock();
			}

			HoveredElement = Element;

			TList<UElement *> EnteringElements;
			{
				UElement * EnteringElement = HoveredElement.Lock();
				while (EnteringElement && EnteringElement != BranchElement)
				{
					EnteringElements.Add(EnteringElement);
					EnteringElement = EnteringElement->Parent.Lock();
				}
			}

			for (uintx Index = 0; Index < EnteringElements.Size; ++Index)
			{
				UElement * EnteringElement = EnteringElements[EnteringElements.Size - Index - 1];

				FMouseEventArgs MouseEnterArgs { *this };
				MouseEnterArgs.Position = EnteringElement->WindowToLocal(Point);
				MouseEnterArgs.Element = HoveredElement.Lock();
				EnteringElement->PreMouseEnter(MouseEnterArgs);
			}
		}

		ShowToolTip(Element, ToolTip);

		ECursor Cursor = Element ? Element->QueryCursor(Element->WindowToLocal(Point)) : ECursor::Arrow;
		UpdateCursor(Cursor, false);

		ElementHovered(Element);
	}

	void UView::SetCapturedElement(UElement * Element)
	{
		CapturedElement = Element;
		UpdateCapture(!!CapturedElement);
		ElementCaptured(Element);
	}

	static void ValidLayoutInitialize(UElement & RootElement)
	{
		TList<UElement *> ElementStack;
		auto ValidInitialize = [&ElementStack](this auto && Self, UElement & Element) -> void
			{
				ElementStack.PushBack(&Element);
				if (Element.Visibility != EVisibility::Collapsed)
				{
					EnsureExpr(Element.Initialized);
					for (UElementRef & Child : Element.Children)
						Self(*Child);
				}
				ElementStack.PopBack();
			};
		ValidInitialize(RootElement);
	}

	static void ValidLayoutMeasure(UElement & RootElement)
	{
		TList<UElement *> ElementStack;
		auto ValidMeasure = [&ElementStack](this auto && Self, UElement & Element) -> void
			{
				ElementStack.PushBack(&Element);
				if (Element.Visibility != EVisibility::Collapsed)
				{
					EnsureExpr(Element.MeasureDone);
					for (UElementRef & Child : Element.Children)
						Self(*Child);
				}
				ElementStack.PopBack();
			};
		ValidMeasure(RootElement);
	}

	static void ValidLayoutMeasureArrange(UElement & RootElement)
	{
		TList<UElement *> ElementStack;
		auto ValidArrange = [&ElementStack](this auto && Self, UElement & Element) -> void
			{
				ElementStack.PushBack(&Element);
				if (Element.Visibility != EVisibility::Collapsed)
				{
					EnsureForaml(Element.MeasureDone, u8"{}"V, Element.Name);
					EnsureForaml(Element.ArrangeDone, u8"{}"V, Element.Name);
					for (UElementRef & Child : Element.Children)
						Self(*Child);
				}
				ElementStack.PopBack();
			};
		ValidArrange(RootElement);
	}

	void UView::DispatchLayout()
	{
		if (LayoutDispatching)
			return;

		LayoutDispatching = true;
		//if (FDispatcher::MainThread().ThreadId == 0)
		//	FSystem::MessageBox(u8"Caption"V, Format(u8"UWindow::DispatchLayout {}, {}."V, FDispatcher::MainThread().ThreadHandle, voidp(&FDispatcher::MainThread()) ));

		FDispatcher::Current().BeginInvoke({ this, [this]
			{
				//static bool ShownMSB = false;
				//if (!ShownMSB)
				//	FSystem::MessageBox(u8"Caption"V, u8"FDispatcher::MainThread().BeginInvoke"V);
				//ShownMSB = true;

				AssertExpr(LayoutDispatching);
				if (Shown)
				{
					uint32 LayoutDepth = 0;
					while ((!MeasureDone || !ArrangeDone) && LayoutDepth++ < 512)
					{
						ValidLayoutInitialize(*this);
						Measure(AvailableSize);
						ValidLayoutMeasure(*this);
						Arrange(ArrangeRect);
					}
					ValidLayoutMeasureArrange(*this);

					//FConsole::WriteLine(u8"Layout depth: {}"V, LayoutDepth);
					if (IsMouseOver)
					{
						FPoint MousePosition = GlobalToLocal(Desktop.GetCursorPosition());
						UElement * Element = HitElement(MousePosition);
						SetHoveredElement(Element, MousePosition);
					}
				}
				else
				{
					if (IsMouseOver)
						SetHoveredElement(nullptr, FPoint::Infinity);
				}

				LayoutDispatching = false;
			} });
	}

	void UView::RegisterRadioButton(URadioButton * RadioButton)
	{
		AssertExpr(not RadioButtons.Contains(RadioButton));
		RadioButtons.Add(RadioButton);

		if (RadioButton->GroupName)
		{
			uintx GroupIndex = NullIndex;
			for (uintx Index = 0; Index < RadioButtonGroups.Size; ++Index)
			{
				if (RadioButtonGroups[Index].First == RadioButton->GroupName)
				{
					GroupIndex = Index;
					break;
				}
			}

			if (GroupIndex == NullIndex)
				RadioButtonGroups.Add({ RadioButton->GroupName, { RadioButton } });
			else
			{
				TPair<FName, TList<URadioButtonRef>> & RadioButtonGroup = RadioButtonGroups[GroupIndex];
				RadioButtonGroup.Second.Add(RadioButton);

				if (RadioButton->Checked == true)
				{
					for (URadioButtonRef & BrotherRadioButton : RadioButtonGroup.Second)
					{
						if (RadioButton != BrotherRadioButton.Get())
							BrotherRadioButton->Checked = false;
					}
				}
				else { }
			}
		}
	}

	void UView::UnregisterRadioButton(URadioButton * RadioButton)
	{
		RadioButtons.Remove(RadioButton);
	}

	void UView::CheckRadioButton(URadioButton * RadioButton)
	{
		//AssertExpr(RadioButtons.Contains(RadioButton));
		if (RadioButton->GroupName)
		{
			for (TPair<FName, TList<URadioButtonRef>> & RadioButtonGroup : RadioButtonGroups)
			{
				if (RadioButtonGroup.First == RadioButton->GroupName)
				{
					for (URadioButtonRef & BrotherRadioButton : RadioButtonGroup.Second)
					{
						if (RadioButton != BrotherRadioButton.Get())
							BrotherRadioButton->Checked = false;
					}
				}
			}
		}
	}
}
