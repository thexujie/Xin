// 02C30A579D7F6E5B17754D1E093946AA 2025-03-24 18:06:24
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Controls/ListView.h"

namespace Xin
{
	class FListViewColumn_Type : public TClass<UI::FListViewColumn>
	{
	public:
		using FListViewColumn = UI::FListViewColumn;

		// --- 0 Constructors
		static inline const TConstructor<FListViewColumn, UI::FListViewColumn const &> Constructor___FListViewColumn_const_lref { u8"!Constructor___FListViewColumn_const_lref"N, FMetadata::None };
		static inline const TConstructor<FListViewColumn, UI::FListViewColumn &&> Constructor___FListViewColumn_ref { u8"!Constructor___FListViewColumn_ref"N, FMetadata::None };
		static inline const TConstructor<FListViewColumn> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor___FListViewColumn_const_lref,
			Constructor___FListViewColumn_ref,
			Constructor,
		};

		// --- 1 Methods

		// --- 2 Fields

		// --- 3 Properties
		struct Property___Header
		{
			static inline const TProxyProperty Property { u8"Header"N, &FListViewColumn::__Header, &FListViewColumn::__GetHeader, &FListViewColumn::__SetHeader, FMetadata::None };
		};
		struct Property___Width
		{
			static inline const TProxyProperty Property { u8"Width"N, &FListViewColumn::__Width, &FListViewColumn::__GetWidth, &FListViewColumn::__SetWidth, FMetadata::None };
		};
		struct Property___HeaderTemplate
		{
			static inline const TProxyProperty Property { u8"HeaderTemplate"N, &FListViewColumn::__HeaderTemplate, &FListViewColumn::__GetHeaderTemplate, &FListViewColumn::__SetHeaderTemplate, FMetadata::None };
		};
		struct Property___DataTemplate
		{
			static inline const TProxyProperty Property { u8"DataTemplate"N, &FListViewColumn::__DataTemplate, &FListViewColumn::__GetDataTemplate, &FListViewColumn::__SetDataTemplate, FMetadata::None };
		};
		struct Property___HorizontalAlignment
		{
			static inline const TProxyProperty Property { u8"HorizontalAlignment"N, &FListViewColumn::__HorizontalAlignment, &FListViewColumn::__GetHorizontalAlignment, &FListViewColumn::__SetHorizontalAlignment, FMetadata::None };
		};
		struct Property___SortKey
		{
			static inline const TProxyProperty Property { u8"SortKey"N, &FListViewColumn::__SortKey, &FListViewColumn::__GetSortKey, &FListViewColumn::__SetSortKey, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FProperty> ReflectedProperties[] =
		{
			Property___Header::Property,
			Property___Width::Property,
			Property___HeaderTemplate::Property,
			Property___DataTemplate::Property,
			Property___HorizontalAlignment::Property,
			Property___SortKey::Property,
		};

		// --- 4 Base Types

		// --- 5 Constructor
		FListViewColumn_Type(FName Name = u8"FListViewColumn"N) : TClass(Name, FClass::None)
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Properties = ReflectedProperties;
		}
	};

	const TProperty<FVariant> & UI::FListViewColumn::HeaderProperty() { return FListViewColumn_Type::Property___Header::Property; }

	const TProperty<UI::FDimen> & UI::FListViewColumn::WidthProperty() { return FListViewColumn_Type::Property___Width::Property; }

	const TProperty<UI::IControlTemplateRef> & UI::FListViewColumn::HeaderTemplateProperty() { return FListViewColumn_Type::Property___HeaderTemplate::Property; }

	const TProperty<UI::IDataTemplateRef> & UI::FListViewColumn::DataTemplateProperty() { return FListViewColumn_Type::Property___DataTemplate::Property; }

	const TProperty<UI::EElementAlignment> & UI::FListViewColumn::HorizontalAlignmentProperty() { return FListViewColumn_Type::Property___HorizontalAlignment::Property; }

	const TProperty<FName> & UI::FListViewColumn::SortKeyProperty() { return FListViewColumn_Type::Property___SortKey::Property; }

	template<> struct XIN_EXPORT TStaticType<UI::FListViewColumn>
	{
		static inline FListViewColumn_Type FListViewColumnType { u8"FListViewColumn"N };

		static FType & Type()
		{
			return FListViewColumnType;
		}
	};

	template<> XIN_EXPORT inline FType & Typeof<UI::FListViewColumn>()
	{
		return TStaticType<UI::FListViewColumn>().Type();
	}

	static TTypeRegister<UI::FListViewColumn> FListViewColumnTypeRegister { TStaticType<UI::FListViewColumn>().Type() };

	class UListViewHeader_Type : public TClass<UI::UListViewHeader>
	{
	public:
		using UListViewHeader = UI::UListViewHeader;

		// --- 0 Constructors
		static inline const TConstructor<UListViewHeader, UI::UListView &> Constructor___UListView_lref { u8"!Constructor___UListView_lref"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor___UListView_lref,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<UListViewHeader, FType &()> Method { u8"Type"N, &UListViewHeader::Type, FMetadata::None };
		};
		struct Method___TVec2__float64_____OnMeasure__TVec2__float64__
		{
			static inline const TMethod<UListViewHeader, FSize(FSize)> Method { u8"OnMeasure"N, &UListViewHeader::OnMeasure, FMetadata::None };
		};
		struct Method___void___OnArrange
		{
			static inline const TMethod<UListViewHeader, void()> Method { u8"OnArrange"N, &UListViewHeader::OnArrange, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___TVec2__float64_____OnMeasure__TVec2__float64__::Method,
			Method___void___OnArrange::Method,
		};

		// --- 2 Fields

		// --- 3 Properties
		struct Property___ScrollOffset
		{
			struct FPropertyMetadata : UListViewHeader::FPropertyMetadata
			{
				FPropertyMetadata()
				{
					AffectsArrange = true;
				}
			};
			static inline const FPropertyMetadata Metadata;
			static inline const TDependencyProperty Property { u8"ScrollOffset"N, &UListViewHeader::__ScrollOffset, &UListViewHeader::__GetScrollOffset, &UListViewHeader::__SetScrollOffsetValue, UListViewHeader::__ScrollOffset__, Metadata };
		};
		static inline const TReferenceWrapper<const FProperty> ReflectedProperties[] =
		{
			Property___ScrollOffset::Property,
		};

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<UI::UContentControl>(),
		};

		// --- 5 Constructor
		UListViewHeader_Type(FName Name = u8"UListViewHeader"N) : TClass(Name, Classof<UI::UContentControl>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Methods = ReflectedMethods;
			FClass::Properties = ReflectedProperties;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	const TProperty<dimenx> & UI::UListViewHeader::ScrollOffsetProperty() { return UListViewHeader_Type::Property___ScrollOffset::Property; }

	template<> struct XIN_EXPORT TStaticType<UI::UListViewHeader>
	{
		static inline UListViewHeader_Type UListViewHeaderType { u8"UListViewHeader"N };

		static FType & Type()
		{
			return UListViewHeaderType;
		}
	};

	FType & UI::UListViewHeader::StaticType()
	{
		return TStaticType<UI::UListViewHeader>().Type();
	}

	static TTypeRegister<UI::UListViewHeader> UListViewHeaderTypeRegister { TStaticType<UI::UListViewHeader>().Type() };

	class UListViewColumnHeader_Type : public TClass<UI::UListViewColumnHeader>
	{
	public:
		using UListViewColumnHeader = UI::UListViewColumnHeader;

		// --- 0 Constructors
		static inline const TConstructor<UListViewColumnHeader, UI::UListView &, UI::FListViewColumn const &> Constructor___UListView_lref__FListViewColumn_const_lref { u8"!Constructor___UListView_lref__FListViewColumn_const_lref"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor___UListView_lref__FListViewColumn_const_lref,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<UListViewColumnHeader, FType &()> Method { u8"Type"N, &UListViewColumnHeader::Type, FMetadata::None };
		};
		struct Method___void___OnConstruct
		{
			static inline const TMethod<UListViewColumnHeader, void()> Method { u8"OnConstruct"N, &UListViewColumnHeader::OnConstruct, FMetadata::None };
		};
		struct Method___void___UpdateState
		{
			static inline const TMethod<UListViewColumnHeader, void()> Method { u8"UpdateState"N, &UListViewColumnHeader::UpdateState, FMetadata::None };
		};
		struct Method___void___OnMouseDown__FMouseButtonEventArgs_lref
		{
			static inline const TMethod<UListViewColumnHeader, void(UI::FMouseButtonEventArgs &)> Method { u8"OnMouseDown"N, &UListViewColumnHeader::OnMouseDown, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___void___OnConstruct::Method,
			Method___void___UpdateState::Method,
			Method___void___OnMouseDown__FMouseButtonEventArgs_lref::Method,
		};

		// --- 2 Fields

		// --- 3 Properties
		struct Property___DisplayIndex
		{
			static inline const TDependencyProperty Property { u8"DisplayIndex"N, &UListViewColumnHeader::__DisplayIndex, &UListViewColumnHeader::__GetDisplayIndex, &UListViewColumnHeader::__SetDisplayIndexValue, UListViewColumnHeader::__DisplayIndex__, UListViewColumnHeader::FPropertyMetadata::None };
		};
		struct Property___Indicating
		{
			struct FPropertyMetadata : UListViewColumnHeader::FPropertyMetadata
			{
				FPropertyMetadata()
				{
					AffectsState = true;
				}
			};
			static inline const FPropertyMetadata Metadata;
			static inline const TProxyProperty Property { u8"Indicating"N, &UListViewColumnHeader::__Indicating, &UListViewColumnHeader::__GetIndicating, &UListViewColumnHeader::__SetIndicating, Metadata };
		};
		struct Property___Resizing
		{
			struct FPropertyMetadata : UListViewColumnHeader::FPropertyMetadata
			{
				FPropertyMetadata()
				{
					AffectsState = true;
				}
			};
			static inline const FPropertyMetadata Metadata;
			static inline const TProxyProperty Property { u8"Resizing"N, &UListViewColumnHeader::__Resizing, &UListViewColumnHeader::__GetResizing, &UListViewColumnHeader::__SetResizing, Metadata };
		};
		struct Property___SortOrder
		{
			struct FPropertyMetadata : UListViewColumnHeader::FPropertyMetadata
			{
				FPropertyMetadata()
				{
					AffectsState = true;
				}
			};
			static inline const FPropertyMetadata Metadata;
			static inline const TProxyProperty Property { u8"SortOrder"N, &UListViewColumnHeader::__SortOrder, &UListViewColumnHeader::__GetSortOrder, &UListViewColumnHeader::__SetSortOrder, Metadata };
		};
		static inline const TReferenceWrapper<const FProperty> ReflectedProperties[] =
		{
			Property___DisplayIndex::Property,
			Property___Indicating::Property,
			Property___Resizing::Property,
			Property___SortOrder::Property,
		};

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<UI::UContentControl>(),
		};

		// --- 5 Constructor
		UListViewColumnHeader_Type(FName Name = u8"UListViewColumnHeader"N) : TClass(Name, Classof<UI::UContentControl>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Methods = ReflectedMethods;
			FClass::Properties = ReflectedProperties;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	const TProperty<uintx> & UI::UListViewColumnHeader::DisplayIndexProperty() { return UListViewColumnHeader_Type::Property___DisplayIndex::Property; }

	const TProperty<bool> & UI::UListViewColumnHeader::IndicatingProperty() { return UListViewColumnHeader_Type::Property___Indicating::Property; }

	const TProperty<bool> & UI::UListViewColumnHeader::ResizingProperty() { return UListViewColumnHeader_Type::Property___Resizing::Property; }

	const TProperty<UI::ESortOrder> & UI::UListViewColumnHeader::SortOrderProperty() { return UListViewColumnHeader_Type::Property___SortOrder::Property; }

	template<> struct XIN_EXPORT TStaticType<UI::UListViewColumnHeader>
	{
		static inline UListViewColumnHeader_Type UListViewColumnHeaderType { u8"UListViewColumnHeader"N };

		static FType & Type()
		{
			return UListViewColumnHeaderType;
		}
	};

	FType & UI::UListViewColumnHeader::StaticType()
	{
		return TStaticType<UI::UListViewColumnHeader>().Type();
	}

	static TTypeRegister<UI::UListViewColumnHeader> UListViewColumnHeaderTypeRegister { TStaticType<UI::UListViewColumnHeader>().Type() };

	class UListViewItemCell_Type : public TClass<UI::UListViewItemCell>
	{
	public:
		using UListViewItemCell = UI::UListViewItemCell;

		// --- 0 Constructors
		static inline const TConstructor<UListViewItemCell, UI::UListViewItem &> Constructor___UListViewItem_lref { u8"!Constructor___UListViewItem_lref"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor___UListViewItem_lref,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<UListViewItemCell, FType &()> Method { u8"Type"N, &UListViewItemCell::Type, FMetadata::None };
		};
		struct Method___TReferPtr__UElement_____LoadContent
		{
			static inline const TMethod<UListViewItemCell, UI::UElementRef()> Method { u8"LoadContent"N, &UListViewItemCell::LoadContent, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___TReferPtr__UElement_____LoadContent::Method,
		};

		// --- 2 Fields

		// --- 3 Properties

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<UI::UContentControl>(),
		};

		// --- 5 Constructor
		UListViewItemCell_Type(FName Name = u8"UListViewItemCell"N) : TClass(Name, Classof<UI::UContentControl>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Methods = ReflectedMethods;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	template<> struct XIN_EXPORT TStaticType<UI::UListViewItemCell>
	{
		static inline UListViewItemCell_Type UListViewItemCellType { u8"UListViewItemCell"N };

		static FType & Type()
		{
			return UListViewItemCellType;
		}
	};

	FType & UI::UListViewItemCell::StaticType()
	{
		return TStaticType<UI::UListViewItemCell>().Type();
	}

	static TTypeRegister<UI::UListViewItemCell> UListViewItemCellTypeRegister { TStaticType<UI::UListViewItemCell>().Type() };

	class UListViewItem_Type : public TClass<UI::UListViewItem>
	{
	public:
		using UListViewItem = UI::UListViewItem;

		// --- 0 Constructors
		static inline const TConstructor<UListViewItem, UI::UListView &> Constructor___UListView_lref { u8"!Constructor___UListView_lref"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor___UListView_lref,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<UListViewItem, FType &()> Method { u8"Type"N, &UListViewItem::Type, FMetadata::None };
		};
		struct Method___void___OnConstruct
		{
			static inline const TMethod<UListViewItem, void()> Method { u8"OnConstruct"N, &UListViewItem::OnConstruct, FMetadata::None };
		};
		struct Method___void___UpdateState
		{
			static inline const TMethod<UListViewItem, void()> Method { u8"UpdateState"N, &UListViewItem::UpdateState, FMetadata::None };
		};
		struct Method___TReferPtr__UElement_____LoadContent
		{
			static inline const TMethod<UListViewItem, UI::UElementRef()> Method { u8"LoadContent"N, &UListViewItem::LoadContent, FMetadata::None };
		};
		struct Method___void___OnMouseDown__FMouseButtonEventArgs_lref
		{
			static inline const TMethod<UListViewItem, void(UI::FMouseButtonEventArgs &)> Method { u8"OnMouseDown"N, &UListViewItem::OnMouseDown, FMetadata::None };
		};
		struct Method___void___OnMouseUp__FMouseButtonEventArgs_lref
		{
			static inline const TMethod<UListViewItem, void(UI::FMouseButtonEventArgs &)> Method { u8"OnMouseUp"N, &UListViewItem::OnMouseUp, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___void___OnConstruct::Method,
			Method___void___UpdateState::Method,
			Method___TReferPtr__UElement_____LoadContent::Method,
			Method___void___OnMouseDown__FMouseButtonEventArgs_lref::Method,
			Method___void___OnMouseUp__FMouseButtonEventArgs_lref::Method,
		};

		// --- 2 Fields
		struct Field___Index
		{
			static inline const TField Field { u8"Index"N, &UListViewItem::Index, FMetadata::None };
		};
		struct Field___ItemsPanel
		{
			static inline const TField Field { u8"ItemsPanel"N, &UListViewItem::ItemsPanel, FMetadata::None };
		};
		struct Field___Cells
		{
			static inline const TField Field { u8"Cells"N, &UListViewItem::Cells, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FField> ReflectedFields[] =
		{
			Field___Index::Field,
			Field___ItemsPanel::Field,
			Field___Cells::Field,
		};

		// --- 3 Properties
		struct Property___Selected
		{
			struct FPropertyMetadata : UListViewItem::FPropertyMetadata
			{
				FPropertyMetadata()
				{
					AffectsState = true;
				}
			};
			static inline const FPropertyMetadata Metadata;
			static inline const TDependencyProperty Property { u8"Selected"N, &UListViewItem::__Selected, &UListViewItem::__GetSelected, &UListViewItem::__SetSelectedValue, UListViewItem::__Selected__, Metadata };
		};
		static inline const TReferenceWrapper<const FProperty> ReflectedProperties[] =
		{
			Property___Selected::Property,
		};

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<UI::UContentControl>(),
		};

		// --- 5 Constructor
		UListViewItem_Type(FName Name = u8"UListViewItem"N) : TClass(Name, Classof<UI::UContentControl>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Fields = ReflectedFields;
			FClass::Methods = ReflectedMethods;
			FClass::Properties = ReflectedProperties;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	const TProperty<bool> & UI::UListViewItem::SelectedProperty() { return UListViewItem_Type::Property___Selected::Property; }

	template<> struct XIN_EXPORT TStaticType<UI::UListViewItem>
	{
		static inline UListViewItem_Type UListViewItemType { u8"UListViewItem"N };

		static FType & Type()
		{
			return UListViewItemType;
		}
	};

	FType & UI::UListViewItem::StaticType()
	{
		return TStaticType<UI::UListViewItem>().Type();
	}

	static TTypeRegister<UI::UListViewItem> UListViewItemTypeRegister { TStaticType<UI::UListViewItem>().Type() };

	class IListViewer_Type : public TClass<UI::IListViewer>
	{
	public:
		using IListViewer = UI::IListViewer;

		// --- 0 Constructors

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<IListViewer, FType &()> Method { u8"Type"N, &IListViewer::Type, FMetadata::None };
		};
		struct Method___TReferPtr__UElement_____CreateViewer
		{
			static inline const TMethod<IListViewer, UI::UElementRef()> Method { u8"CreateViewer"N, &IListViewer::CreateViewer, FMetadata::None };
		};
		struct Method___void___OnAddItem__FVariant_lref
		{
			static inline const TMethod<IListViewer, void(FVariant &)> Method { u8"OnAddItem"N, &IListViewer::OnAddItem, FMetadata::None };
		};
		struct Method___void___OnRemoveItem__FVariant_lref
		{
			static inline const TMethod<IListViewer, void(FVariant &)> Method { u8"OnRemoveItem"N, &IListViewer::OnRemoveItem, FMetadata::None };
		};
		struct Method___void___OnSelectedItemChanged__TReferPtr__UListViewItem__
		{
			static inline const TMethod<IListViewer, void(UI::UListViewItemRef)> Method { u8"OnSelectedItemChanged"N, &IListViewer::OnSelectedItemChanged, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___TReferPtr__UElement_____CreateViewer::Method,
			Method___void___OnAddItem__FVariant_lref::Method,
			Method___void___OnRemoveItem__FVariant_lref::Method,
			Method___void___OnSelectedItemChanged__TReferPtr__UListViewItem__::Method,
		};

		// --- 2 Fields

		// --- 3 Properties
		struct Property___SelectedItem
		{
			static inline const TProxyProperty Property { u8"SelectedItem"N, &IListViewer::__SelectedItem, &IListViewer::__GetSelectedItem, &IListViewer::__SetSelectedItem, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FProperty> ReflectedProperties[] =
		{
			Property___SelectedItem::Property,
		};

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<IDependency>(),
		};

		// --- 5 Constructor
		IListViewer_Type(FName Name = u8"IListViewer"N) : TClass(Name, Classof<IDependency>())
		{
			FClass::Methods = ReflectedMethods;
			FClass::Properties = ReflectedProperties;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	const TProperty<UI::UListViewItemRef> & UI::IListViewer::SelectedItemProperty() { return IListViewer_Type::Property___SelectedItem::Property; }

	template<> struct XIN_EXPORT TStaticType<UI::IListViewer>
	{
		static inline IListViewer_Type IListViewerType { u8"IListViewer"N };

		static FType & Type()
		{
			return IListViewerType;
		}
	};

	FType & UI::IListViewer::StaticType()
	{
		return TStaticType<UI::IListViewer>().Type();
	}

	static TTypeRegister<UI::IListViewer> IListViewerTypeRegister { TStaticType<UI::IListViewer>().Type() };

	class UListView_Type : public TClass<UI::UListView>
	{
	public:
		using UListView = UI::UListView;

		// --- 0 Constructors
		static inline const TConstructor<UListView> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<UListView, FType &()> Method { u8"Type"N, &UListView::Type, FMetadata::None };
		};
		struct Method___void___OnInitialize
		{
			static inline const TMethod<UListView, void()> Method { u8"OnInitialize"N, &UListView::OnInitialize, FMetadata::None };
		};
		struct Method___void___OnInitialized
		{
			static inline const TMethod<UListView, void()> Method { u8"OnInitialized"N, &UListView::OnInitialized, FMetadata::None };
		};
		struct Method___void___OnFinalize
		{
			static inline const TMethod<UListView, void()> Method { u8"OnFinalize"N, &UListView::OnFinalize, FMetadata::None };
		};
		struct Method___void___OnAddItem__FVariant_lref
		{
			static inline const TMethod<UListView, void(FVariant &)> Method { u8"OnAddItem"N, &UListView::OnAddItem, FMetadata::None };
		};
		struct Method___void___OnRemoveItem__FVariant_lref
		{
			static inline const TMethod<UListView, void(FVariant &)> Method { u8"OnRemoveItem"N, &UListView::OnRemoveItem, FMetadata::None };
		};
		struct Method___void___PresentItems
		{
			static inline const TMethod<UListView, void()> Method { u8"PresentItems"N, &UListView::PresentItems, FMetadata::None };
		};
		struct Method___void___OnSortKeyChanged__FSortKey
		{
			static inline const TMethod<UListView, void(UI::FSortKey)> Method { u8"OnSortKeyChanged"N, &UListView::OnSortKeyChanged, FMetadata::None };
		};
		struct Method___void___SortItems
		{
			static inline const TMethod<UListView, void()> Method { u8"SortItems"N, &UListView::SortItems, FMetadata::None };
		};
		struct Method___void___OnHeaderThumbDragDelta__uintx__float64
		{
			static inline const TMethod<UListView, void(uintx, dimenx)> Method { u8"OnHeaderThumbDragDelta"N, &UListView::OnHeaderThumbDragDelta, FMetadata::None };
		};
		struct Method___void___OnIndicatingIndexChanged__FSplitterPanelIndicatingIndexChagnedEventArgs_lref
		{
			static inline const TMethod<UListView, void(UI::FSplitterPanelIndicatingIndexChagnedEventArgs &)> Method { u8"OnIndicatingIndexChanged"N, &UListView::OnIndicatingIndexChanged, FMetadata::None };
		};
		struct Method___void___OnListPanelMeasured__FEventArgs_lref
		{
			static inline const TMethod<UListView, void(FEventArgs &)> Method { u8"OnListPanelMeasured"N, &UListView::OnListPanelMeasured, FMetadata::None };
		};
		struct Method___void___OnItemMouseDown__UListViewItem_lref__FMouseButtonEventArgs_lref
		{
			static inline const TMethod<UListView, void(UI::UListViewItem &, UI::FMouseButtonEventArgs &)> Method { u8"OnItemMouseDown"N, &UListView::OnItemMouseDown, FMetadata::None };
		};
		struct Method___void___OnItemMouseUp__UListViewItem_lref__FMouseButtonEventArgs_lref
		{
			static inline const TMethod<UListView, void(UI::UListViewItem &, UI::FMouseButtonEventArgs &)> Method { u8"OnItemMouseUp"N, &UListView::OnItemMouseUp, FMetadata::None };
		};
		struct Method___void___QueryItemContextMenu__FListViewQueryItemContextMenuEventArgs_lref
		{
			static inline const TMethod<UListView, void(UI::FListViewQueryItemContextMenuEventArgs &)> Method { u8"QueryItemContextMenu"N, &UListView::QueryItemContextMenu, FMetadata::None };
		};
		struct Method___void___OnSelectedItemChanged__TReferPtr__UListViewItem__
		{
			static inline const TMethod<UListView, void(UI::UListViewItemRef)> Method { u8"OnSelectedItemChanged"N, &UListView::OnSelectedItemChanged, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___void___OnInitialize::Method,
			Method___void___OnInitialized::Method,
			Method___void___OnFinalize::Method,
			Method___void___OnAddItem__FVariant_lref::Method,
			Method___void___OnRemoveItem__FVariant_lref::Method,
			Method___void___PresentItems::Method,
			Method___void___OnSortKeyChanged__FSortKey::Method,
			Method___void___SortItems::Method,
			Method___void___OnHeaderThumbDragDelta__uintx__float64::Method,
			Method___void___OnIndicatingIndexChanged__FSplitterPanelIndicatingIndexChagnedEventArgs_lref::Method,
			Method___void___OnListPanelMeasured__FEventArgs_lref::Method,
			Method___void___OnItemMouseDown__UListViewItem_lref__FMouseButtonEventArgs_lref::Method,
			Method___void___OnItemMouseUp__UListViewItem_lref__FMouseButtonEventArgs_lref::Method,
			Method___void___QueryItemContextMenu__FListViewQueryItemContextMenuEventArgs_lref::Method,
			Method___void___OnSelectedItemChanged__TReferPtr__UListViewItem__::Method,
		};

		// --- 2 Fields
		struct Field___SortDescriptors
		{
			static inline const TField Field { u8"SortDescriptors"N, &UListView::SortDescriptors, FMetadata::None };
		};
		struct Field___Columns
		{
			static inline const TField Field { u8"Columns"N, &UListView::Columns, FMetadata::None };
		};
		struct Field___ColumnHeaders
		{
			static inline const TField Field { u8"ColumnHeaders"N, &UListView::ColumnHeaders, FMetadata::None };
		};
		struct Field___ListViewItems
		{
			static inline const TField Field { u8"ListViewItems"N, &UListView::ListViewItems, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FField> ReflectedFields[] =
		{
			Field___SortDescriptors::Field,
			Field___Columns::Field,
			Field___ColumnHeaders::Field,
			Field___ListViewItems::Field,
		};

		// --- 3 Properties
		struct Property___SortKey
		{
			static inline const TProxyProperty Property { u8"SortKey"N, &UListView::__SortKey, &UListView::__GetSortKey, &UListView::__SetSortKey, UListView::FPropertyMetadata::None };
		};
		struct Property___SelectedItem
		{
			static inline const TDependencyProperty Property { u8"SelectedItem"N, &UListView::__SelectedItem, &UListView::__GetSelectedItem, &UListView::__SetSelectedItemValue, UListView::__SelectedItem__, UListView::FPropertyMetadata::None };
		};
		struct Property___View
		{
			static inline const TProxyProperty Property { u8"View"N, &UListView::__View, &UListView::__GetView, &UListView::__SetView, UListView::FPropertyMetadata::None };
		};
		struct Property___SelectMethod
		{
			static inline const TDependencyProperty Property { u8"SelectMethod"N, &UListView::__SelectMethod, &UListView::__GetSelectMethod, &UListView::__SetSelectMethodValue, UListView::__SelectMethod__, UListView::FPropertyMetadata::None };
		};
		struct Property___SelectionMode
		{
			static inline const TDependencyProperty Property { u8"SelectionMode"N, &UListView::__SelectionMode, &UListView::__GetSelectionMode, &UListView::__SetSelectionModeValue, UListView::__SelectionMode__, UListView::FPropertyMetadata::None };
		};
		struct Property___PanelTemplate
		{
			static inline const TAttachedProperty<UListView, UI::IControlTemplateRef> Property { u8"PanelTemplate"N, UListView::__PanelTemplate__, UListView::FPropertyMetadata::None };
		};
		struct Property___ItemTemplate
		{
			static inline const TAttachedProperty<UListView, UI::IControlTemplateRef> Property { u8"ItemTemplate"N, UListView::__ItemTemplate__, UListView::FPropertyMetadata::None };
		};
		static inline const TReferenceWrapper<const FProperty> ReflectedProperties[] =
		{
			Property___SortKey::Property,
			Property___SelectedItem::Property,
			Property___View::Property,
			Property___SelectMethod::Property,
			Property___SelectionMode::Property,
			Property___PanelTemplate::Property,
			Property___ItemTemplate::Property,
		};

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<UI::UListControl>(),
		};

		// --- 5 Constructor
		UListView_Type(FName Name = u8"UListView"N) : TClass(Name, Classof<UI::UListControl>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Fields = ReflectedFields;
			FClass::Methods = ReflectedMethods;
			FClass::Properties = ReflectedProperties;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	const TProperty<UI::FSortKey> & UI::UListView::SortKeyProperty() { return UListView_Type::Property___SortKey::Property; }

	const TProperty<UI::UListViewItemRef> & UI::UListView::SelectedItemProperty() { return UListView_Type::Property___SelectedItem::Property; }

	const TProperty<UI::IListViewerRef> & UI::UListView::ViewProperty() { return UListView_Type::Property___View::Property; }

	const TProperty<UI::ESelectMethod> & UI::UListView::SelectMethodProperty() { return UListView_Type::Property___SelectMethod::Property; }

	const TProperty<UI::ESelectionMode> & UI::UListView::SelectionModeProperty() { return UListView_Type::Property___SelectionMode::Property; }

	const TProperty<UI::IControlTemplateRef> & UI::UListView::PanelTemplateProperty() { return UListView_Type::Property___PanelTemplate::Property; }

	const TProperty<UI::IControlTemplateRef> & UI::UListView::ItemTemplateProperty() { return UListView_Type::Property___ItemTemplate::Property; }

	template<> struct XIN_EXPORT TStaticType<UI::UListView>
	{
		static inline UListView_Type UListViewType { u8"UListView"N };

		static FType & Type()
		{
			return UListViewType;
		}
	};

	FType & UI::UListView::StaticType()
	{
		return TStaticType<UI::UListView>().Type();
	}

	static TTypeRegister<UI::UListView> UListViewTypeRegister { TStaticType<UI::UListView>().Type() };
}

// Generated code end.
