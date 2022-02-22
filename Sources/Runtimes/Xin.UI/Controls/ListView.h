#pragma once

#include "Xin.UI.Types.h"
#include "ListControl.h"
#include "ContentControl.h"
#include "Panel.h"
#include "SplitterPanel.h"

namespace Xin::UI
{
	class UListViewItem;
	using UListViewItemRef = TReferPtr<UListViewItem>;

	class UListView;
	using UListViewRef = TReferPtr<UListView>;

	class UI_API [[Meta]] FListViewColumn
	{
	public:
		FBindingPath BindingPath;

		ProxyPropertyRW(FVariant, Header);
		ProxyPropertyRW(FDimen, Width) = FDimen::Auto;
		ProxyPropertyRW(IControlTemplateRef, HeaderTemplate);
		ProxyPropertyRW(IDataTemplateRef, DataTemplate);
		ProxyPropertyRW(EElementAlignment, HorizontalAlignment);

		ProxyPropertyRW(FName, SortKey);
	};

	class UI_API UListViewHeader : public UContentControl
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

	public:
		UListViewHeader(UListView & ListView) : ListView(ListView) {}

	public:
		FSize OnMeasure(FSize ConstrainedSize) override;
		void OnArrange() override;

	public:
		UListView & ListView;

	public:
		[[Meta(AffectsArrange)]]
		DependencyPropertyRW(dimenx, ScrollOffset) = 0;
	};
	using UListViewHeaderRef = TReferPtr<UListViewHeader>;

	class UI_API UListViewColumnHeader: public UContentControl
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

	public:
		UListViewColumnHeader(UListView & ListView, const FListViewColumn & ListViewColumn);

	public:
		void OnConstruct() override;
		void UpdateState() override;
		void OnMouseDown(FMouseButtonEventArgs & Args) override;

	public:
		UListView & ListView;
		const FListViewColumn & ListViewColumn;

	public:
		DependencyPropertyRW(uintx, DisplayIndex) = NullIndex;

		[[Meta(AffectsState)]]
		ProxyPropertyRW(bool, Indicating) = false;

		[[Meta(AffectsState)]]
		ProxyPropertyRW(bool, Resizing) = false;

		[[Meta(AffectsState)]]
		ProxyPropertyRW(ESortOrder, SortOrder) = ESortOrder::None;
	};
	using UListViewColumnHeaderRef = TReferPtr<UListViewColumnHeader>;


	class UI_API UListViewItemCell : public UContentControl
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

	public:
		UListViewItemCell(UListViewItem & ListViewItem) : ListViewItem(ListViewItem) {}
		UElementRef LoadContent() override;

	public:
		UListViewItem & ListViewItem;
	};
	using UListViewCellRef = TReferPtr<UListViewItemCell>;

	class UI_API UListViewItem : public UContentControl
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

	public:
		UListViewItem(UListView & ListView) : ListView(ListView) {}

		void OnConstruct() override;
		void UpdateState() override;
		UElementRef LoadContent() override;

	public:
		void OnMouseDown(FMouseButtonEventArgs & Args) override;
		void OnMouseUp(FMouseButtonEventArgs & Args) override;

	public:
		UListView & ListView;
		uintx Index = NullIndex;

	public:
		UPanelRef ItemsPanel;
		TList<UListViewCellRef> Cells;

	public:
		[[Meta(AffectsState)]]
		DependencyPropertyRW(bool, Selected) = false;
	};

	class UI_API FListViewQueryItemContextMenuEventArgs : public FEventArgs
	{
	public:
		FListViewQueryItemContextMenuEventArgs(UListViewItem & ListViewItem);
		~FListViewQueryItemContextMenuEventArgs();

	public:
		UListViewItem & ListViewItem;

	public: // result
		UMenuRef Menu;
	};


	class UI_API IListViewer : public IDependency
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

	public:
		virtual UElementRef CreateViewer() = 0;
		virtual void OnAddItem(FVariant & Item) {}
		virtual void OnRemoveItem(FVariant & Item) {}

	public:
		virtual void OnSelectedItemChanged(UListViewItemRef SelectedItemBefore) {}
		ProxyPropertyRW(UListViewItemRef, SelectedItem) = nullptr;
	};
	using IListViewerRef = TReferPtr<IListViewer>;

	struct FListViewSortDescriptor
	{
		FName Key;
		TFunction<bool(const UListViewItemRef &, const UListViewItemRef &)> SortComparer;
	};

	class UI_API UListView : public UListControl, TIItemAddable<FVariant>
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

		static constexpr FName Name_ListViewHeader = u8"PART_ListViewHeader"N;

	public:
		UListView();
		~UListView() = default;

	public:
		void OnInitialize() override;
		void OnInitialized() override;
		void OnFinalize() override;

	public:
		void OnAddItem(FVariant & Item) override;
		void OnRemoveItem(FVariant & Item) override;

		TItemCollection<FVariant> Items { *this };
		TList<FListViewSortDescriptor> SortDescriptors;

	public:
		void PresentItems();

	public:
		void OnSortKeyChanged(FSortKey SortDescriptorBefore);
		ProxyPropertyRW(FSortKey, SortKey);
		void SortItems();

	public:
		TList<FListViewColumn> Columns;
		TList<UListViewColumnHeaderRef> ColumnHeaders;
		TList<UListViewItemRef> ListViewItems;

	public:
		void OnHeaderThumbDragDelta(uintx ColumnIndex, dimenx Delta);

	public:
		void OnIndicatingIndexChanged(FSplitterPanelIndicatingIndexChagnedEventArgs & EventArgs);
		void OnListPanelMeasured(FEventArgs & Args);

	public:
		void OnItemMouseDown(UListViewItem & ListViewItem, FMouseButtonEventArgs & Args);
		void OnItemMouseUp(UListViewItem & ListViewItem, FMouseButtonEventArgs & Args);
		void QueryItemContextMenu(FListViewQueryItemContextMenuEventArgs & Args);

	public:
		void OnSelectedItemChanged(UListViewItemRef SelectedItemBefore);
		DependencyPropertyRW(UListViewItemRef, SelectedItem) = nullptr;

	public:
		ProxyPropertyRW(IListViewerRef, View);
		DependencyPropertyRW(ESelectMethod, SelectMethod) = ESelectMethod::Press;
		DependencyPropertyRW(ESelectionMode, SelectionMode) = ESelectionMode::Single;

	public:
		AttachedPropertyRW(IControlTemplateRef, PanelTemplate);
		AttachedPropertyRW(IControlTemplateRef, ItemTemplate);

	public:
		TEvent<void(FListViewQueryItemContextMenuEventArgs & Args)> OnQueryItemContextMenu;
	};
}
