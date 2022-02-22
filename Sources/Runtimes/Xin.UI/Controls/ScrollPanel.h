#pragma once

#include "Xin.UI.Types.h"
#include "Element.h"
#include "ScrollArea.h"
#include "ScrollBar.h"
#include "Shapes/Rectangle.h"

namespace Xin::UI
{
	class UI_API UScrollPanel : public UElement
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

	public:
		UScrollPanel() = default;
		~UScrollPanel() = default;

	public:
		void OnInitialize() override;
		void OnInitialized() override;
		FSize OnMeasure(FSize ConstrainedSize) override;
		void OnArrange() override;
		void OnArranged() override;

	private:
		void ScrollArea_OnScrollChanged(FEventArgs & EventArgs);
		void VerticalScrollBar_OnPageUp(FEventArgs & EventArgs);
		void VerticalScrollBar_OnPageDown(FEventArgs & EventArgs);
		void HorizontalScrollBar_OnPageUp(FEventArgs & EventArgs);
		void HorizontalScrollBar_OnPageDown(FEventArgs & EventArgs);

	public:
		void UpdateScrollInfo();
		void UpdateScrollBars();
		bool IsHorizontalScrollBarVisible() const noexcept { return HorizontalScrollBar && HorizontalScrollBar->Visibility == EVisibility::Visible; }
		bool IsVerticalScrollBarVisible() const noexcept { return VerticalScrollBar && VerticalScrollBar->Visibility == EVisibility::Visible; }

	public:
		void OnChildChanged(UElementRef ChildBefore);
		ProxyPropertyRW(UElementRef, Child);

		DependencyPropertyRW(FSize, ViewportSize) = FSize::Zero;
		DependencyPropertyRW(FVec2, ScrollExtent) = FVec2::Zero;

		void OnScrollOffsetChanged(FVec2 ValueBefore);
		DependencyPropertyRW(FVec2, ScrollOffset) = FVec2::Zero;

		ProxyPropertyRW(TOptional<EVisibility>, HorizontalScrollBarVisibility) = noval;
		ProxyPropertyRW(TOptional<EVisibility>, VerticalScrollBarVisibility) = noval;

	public:
		IScrollInfoRef ScrollInfo;

		void OnHorizontalScrollBarChanged(UScrollBarRef HorizontalScrollBarBefore);
		ProxyPropertyRW(UScrollBarRef, HorizontalScrollBar);

		void OnVerticalScrollBarChanged(UScrollBarRef VerticalScrollBarBefore);
		ProxyPropertyRW(UScrollBarRef, VerticalScrollBar);

		void OnConnerElementChanged(UElementRef ConnerElementBefore);
		ProxyPropertyRW(UElementRef, ConnerElement);
	};
	using UScrollPanelRef = TReferPtr<UScrollPanel>;
}
