#include "PCH.h"
#include "Panel.h"

namespace Xin::UI
{
	void UPanel::OnConstruct()
	{
		UElement::OnConstruct();
		HitInside = EHitResult::Ignore;
	}

	void UPanel::OnInitialize()
	{
		SortElements();
		UElement::OnInitialize();
	}

	void UPanel::OnFinalize()
	{
		UElement::OnFinalize();
	}

	FSize UPanel::OnMeasure(FSize ConstrainedSize)
	{
		throw EError::NotSupported;
	}

	void UPanel::OnArrange()
	{
		throw EError::NotSupported;
	}

	void UPanel::OnPaint(IPainter & Painter)
	{
	}

	void UPanel::AddChild(UElement * Child)
	{
		AssertExpr(!Elements.Contains(Child));
		Elements.AddItem(Child);
	}

	void UPanel::RemoveChild(UElement * Child)
	{
		AssertExpr(Elements.Contains(Child));
		Elements.RemoveItem(Child);
	}

	void UPanel::RemoveChildren()
	{
		Elements.ClearItems();
	}

	void UPanel::OnSizeChanged(const FDimen2 & SizeBefore)
	{
		UElement::OnSizeChanged(SizeBefore);
	}

	void UPanel::OnAddItem(TReferPtr<UElement> & Element)
	{
		AssertExpr(Element);

		TReferPtr<UElement> Item = Element;
		SortElements();

		OnAddChild(Item);
		InvalidateArrange();
	}

	void UPanel::OnRemoveItem(TReferPtr<UElement> & Element)
	{
		OnRemoveChild(Element);
		InvalidateArrange();
	}

	void UPanel::OnSortComparerChanged(TFunction<bool(const UElementRef &, const UElementRef &)> SortComparerBefore)
	{
		SortElements();
		InvalidateArrange();
	}

	void UPanel::SortElements()
	{
		if (auto Comparer = SortComparer)
		{
			std::sort(Elements.Begin(), Elements.End(), Comparer);

			for (intx ElementZIndex = 0; ElementZIndex < Elements.Size; ++ElementZIndex)
				Elements[ElementZIndex]->InnerZIndex = ElementZIndex;
			SortChildrenZ();
		}


		//else
		//	std::sort(Elements.Begin(), Elements.End(), [&](const Xin::UI::UElementRef & A, const Xin::UI::UElementRef & B)
		//	{
		//		return (A->ZIndex < B->ZIndex) ? true : (A.Pointer < B.Pointer);
		//	});
	}

	//void UPanel::OnItemsChanged(TItemCollection<UElementRef> & Items)
	//{
	//	for (UElementRef LogicalChild : ElementsBefore)
	//	{
	//		RemoveLogicalChild(LogicalChild);
	//		RemoveVisualChild(LogicalChild);
	//	}

	//	for (UElementRef Element : Elements)
	//	{
	//		AddLogicalChild(Element);
	//		AddVisualChild(Element);
	//	}
	//}
}
