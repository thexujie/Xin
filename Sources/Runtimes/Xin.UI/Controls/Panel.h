#pragma once

#include "Xin.UI.Types.h"
#include "Element.h"

namespace Xin::UI
{
	class UI_API UPanel : public UElement, TIItemAddable<UElementRef>
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

	public:
		UPanel() = default;
		UPanel(FName Name) : UElement(Name) {}
		~UPanel() = default;

	public:
		void OnConstruct() override;
		void OnInitialize() override;
		void OnFinalize() override;
		FSize OnMeasure(FSize ConstrainedSize) override;
		void OnArrange() override;
		void OnPaint(IPainter & Painter) override;

		void AddChild(UElement * Child);
		void RemoveChild(UElement * Child);
		void RemoveChildren();

		void OnSizeChanged(const FDimen2 & SizeBefore) override;

	public:
		void OnAddItem(TReferPtr<UElement> & Element) override;
		void OnRemoveItem(TReferPtr<UElement> & Element) override;
		TItemCollection<UElementRef> Elements { *this };

		void OnSortComparerChanged(TFunction<bool(const UElementRef &, const UElementRef &)> SortComparerBefore);
		ProxyPropertyRW(TFunction<bool(const UElementRef &, const UElementRef &)>, SortComparer) = nullptr;
		void SortElements();
	};
	using UPanelRef = TReferPtr<UPanel>;
}
