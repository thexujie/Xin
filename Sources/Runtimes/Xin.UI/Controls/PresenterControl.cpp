#include "PCH.h"
#include "PresenterControl.h"

namespace Xin::UI
{
	UPresenterControl::UPresenterControl()
	{

	}

	void UPresenterControl::OnChildMeasured(UElement * Element)
	{
		UControl::OnChildMeasured(Element);
	}

	void UPresenterControl::OnInitialize() 
	{
		UControl::OnInitialize();

		if (UElementRef Element = TemplateElement)
		{
			ContentPresenter = Element->FindChildT<UPresenter>(PART_ContentPresenter);
			if (ContentPresenter)
				ContentPresenter->Child = __Child;
		}
	}

	void UPresenterControl::OnFinalize()
	{
		if (ContentPresenter)
			ContentPresenter->Child = nullptr;

		UControl::OnFinalize();
	}

	void UPresenterControl::OnInitialized()
	{
		UControl::OnInitialized();
	}

	FSize UPresenterControl::OnMeasure(FSize ConstrainedSize)
	{
		return UControl::OnMeasure(ConstrainedSize);

		if (UElementRef Element = Child)
			return Element->Measure(ConstrainedSize) + Padding.Size;
		else
			return FSize::Zero;
	}

	void UPresenterControl::OnArrange()
	{
		UControl::OnArrange();
		//ArrangeChild(Child, { Padding.LeftTop, FinalSize - Padding.Size });
	}

	void UPresenterControl::PreKeyDown(FKeyboardEventArgs & Args)
	{
		UElement::PreKeyDown(Args);

		if (!Args.Handled && Child)
			Child->PreKeyDown(Args);
	}

	void UPresenterControl::PreKeyUp(FKeyboardEventArgs & Args)
	{
		UElement::PreKeyUp(Args);

		if (!Args.Handled && Child)
			Child->PreKeyUp(Args);
	}

	void UPresenterControl::OnChildChanged(UElementRef ChildBefore)
	{
		if (!ContentPresenter)
			return;

		if (ChildBefore)
			ContentPresenter->OnRemoveChild(ChildBefore);

		if (UElementRef Element = Child)
			ContentPresenter->OnAddChild(Element);
	}
}
