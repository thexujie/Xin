#include "PCH.h"
#include "Presenter.h"

namespace Xin::UI
{
	UPresenter::UPresenter()
	{

	}

	UPresenter::~UPresenter()
	{
	}

	void UPresenter::OnInitialize()
	{
		UElement::OnInitialize();
	}

	void UPresenter::OnFinalize()
	{
		UElement::OnFinalize();
	}

	void UPresenter::OnChildMeasured(UElement * Element)
	{
		UElement::OnChildMeasured(Element);
	}

	FSize UPresenter::OnMeasure(FSize ConstrainedSize)
	{
		if (UElementRef Element = Child)
			return Element->Measure(ConstrainedSize) + Padding.Size;
		else
			return FSize::Zero;
	}

	void UPresenter::OnArrange()
	{
		ArrangeChild(*Child, { Padding.LeftTop, FinalSize - Padding.Size });
	}

	void UPresenter::PreKeyDown(FKeyboardEventArgs & Args)
	{
		UElement::PreKeyDown(Args);

		if (!Args.Handled && Child)
			Child->PreKeyDown(Args);
	}

	void UPresenter::PreKeyUp(FKeyboardEventArgs & Args)
	{
		UElement::OnKeyDown(Args);

		if (!Args.Handled && Child)
			Child->PreKeyUp(Args);
	}

	void UPresenter::OnChildChanged(UElementRef ChildBefore)
	{
		if (ChildBefore)
			OnRemoveChild(ChildBefore);

		if (UElementRef Element = Child; Element)
			OnAddChild(Element);
	}
}
