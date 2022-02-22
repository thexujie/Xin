#pragma once

#include "Xin.UI.Types.h"
#include "Controls/Control.h"
#include "Controls/ListControl.h"

namespace Xin::UI::ControlsEx
{
	class UNavigator;

	struct [[Meta]] FNavigatorItem
	{
		ProxyPropertyRW(FName, Name);

		ProxyPropertyRW(FVariant, Header) = noval;

		UElementRef Element;

		FNavigatorItem() = default;
		FNavigatorItem(FName Name, FVariant Header, UElementRef Element) : __Name(Name), __Header(Header), Element(Element) {  }
	};


	class UI_API UNavigatorHeaderItem : public UContentControl
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

	public:
		UNavigatorHeaderItem(UNavigator & Navigator);

	public:
		void OnConstruct() override;
		void UpdateState() override;

	public:
		void OnMouseDown(FMouseButtonEventArgs & Args) override;

	public:
		DependencyPropertyRW(uintx, DisplayIndex) = NullIndex;

		ProxyPropertyRW(FName, ItemName) = FName::None;

		void OnIndicatingChanged(bool IndicatingBefore);
		[[Meta(AffectsState)]]
		ProxyPropertyRW(bool, Indicating) = false;

	public:
		UNavigator & Navigator;
	};
	using UNavigatorHeaderItemRef = TReferPtr<UNavigatorHeaderItem>;


	class UI_API UNavigatorHeader : public UListControl, TIItemAddable<UNavigatorHeaderItemRef>
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

	public:
		UNavigatorHeader(UNavigator & Navigator);

	public:
		void OnInitialized() override;
		void OnFinalize() override;
		void OnArrange() override;

	public:
		void UpdateItems();
		void IndicateItem(FName ItemName);

	private:
		void PresentItems();

	public:
		UNavigator & Navigator;

	public:
		TList<UNavigatorHeaderItemRef> HeaderItems;

	public:
		[[Meta(AffectsArrange)]]
		DependencyPropertyRW(dimenx, ScrollOffset) = 0;
	};
	using UNavigatorHeaderRef = TReferPtr<UNavigatorHeader>;

	class UI_API UNavigator : public UControl, TIItemAddable<FNavigatorItem>
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }
		static constexpr FName Name_NavigatorHeader = u8"PART_NavigatorHeader"N;
		static constexpr FName Name_NavigatorPresenter = u8"PART_NavigatorPresenter"N;


	public:
		UNavigator();
		~UNavigator() = default;

	public:
		void OnConstruct() override;
		void OnInitialize() override;

		FSize OnMeasure(FSize ConstrainedSize) override;
		void OnArrange() override;

	public:
		void Navigate(FName ItemName);

	public:
		void OnItemsChanged(TList<FNavigatorItem> & List) override;
		TItemCollection<FNavigatorItem> NavigatorItems { *this };

	public:
		UNavigatorHeaderRef NavigatorHeader;
		UPresenterRef NavigatorPresenter;

	public:
		void OnIndicatingIndexChanged(uintx IndicatingIndexBefore);
		ProxyPropertyRW(uintx, IndicatingIndex) = uintx(NullIndex);
	};
	using UNavigatorRef = TReferPtr<UNavigator>;

	inline UNavigatorRef Navigator(TView<TPair<const FProperty &, FVariant>> Properties = { })
	{
		TReferPtr<UNavigator> Navigator = MakeRefer<UNavigator>();
		for (auto & Pair : Properties)
			Navigator->SetPropertyValue(Pair.First, Pair.Second);
		return Navigator;
	}
}
