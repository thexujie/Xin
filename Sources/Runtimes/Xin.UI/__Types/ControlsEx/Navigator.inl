// AB7DE5A3395647ED302BCE900C49F0FC 2025-03-24 18:07:09
// Generated code begin.

#pragma once
#include "PCH.h"
#include "ControlsEx/Navigator.h"

namespace Xin
{
	class FNavigatorItem_Type : public TClass<UI::ControlsEx::FNavigatorItem>
	{
	public:
		using FNavigatorItem = UI::ControlsEx::FNavigatorItem;

		// --- 0 Constructors
		static inline const TConstructor<FNavigatorItem> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TConstructor<FNavigatorItem, FName, FVariant, UI::UElementRef> Constructor___FName__FVariant__TReferPtr__UElement__ { u8"!Constructor___FName__FVariant__TReferPtr__UElement__"N, FMetadata::None };
		static inline const TConstructor<FNavigatorItem, UI::ControlsEx::FNavigatorItem const &> Constructor___FNavigatorItem_const_lref { u8"!Constructor___FNavigatorItem_const_lref"N, FMetadata::None };
		static inline const TConstructor<FNavigatorItem, UI::ControlsEx::FNavigatorItem &&> Constructor___FNavigatorItem_ref { u8"!Constructor___FNavigatorItem_ref"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
			Constructor___FName__FVariant__TReferPtr__UElement__,
			Constructor___FNavigatorItem_const_lref,
			Constructor___FNavigatorItem_ref,
		};

		// --- 1 Methods

		// --- 2 Fields
		struct Field___Element
		{
			static inline const TField Field { u8"Element"N, &FNavigatorItem::Element, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FField> ReflectedFields[] =
		{
			Field___Element::Field,
		};

		// --- 3 Properties
		struct Property___Name
		{
			static inline const TProxyProperty Property { u8"Name"N, &FNavigatorItem::__Name, &FNavigatorItem::__GetName, &FNavigatorItem::__SetName, FMetadata::None };
		};
		struct Property___Header
		{
			static inline const TProxyProperty Property { u8"Header"N, &FNavigatorItem::__Header, &FNavigatorItem::__GetHeader, &FNavigatorItem::__SetHeader, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FProperty> ReflectedProperties[] =
		{
			Property___Name::Property,
			Property___Header::Property,
		};

		// --- 4 Base Types

		// --- 5 Constructor
		FNavigatorItem_Type(FName Name = u8"FNavigatorItem"N) : TClass(Name, FClass::None)
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Fields = ReflectedFields;
			FClass::Properties = ReflectedProperties;
		}
	};

	const TProperty<FName> & UI::ControlsEx::FNavigatorItem::NameProperty() { return FNavigatorItem_Type::Property___Name::Property; }

	const TProperty<FVariant> & UI::ControlsEx::FNavigatorItem::HeaderProperty() { return FNavigatorItem_Type::Property___Header::Property; }

	template<> struct XIN_EXPORT TStaticType<UI::ControlsEx::FNavigatorItem>
	{
		static inline FNavigatorItem_Type FNavigatorItemType { u8"FNavigatorItem"N };

		static FType & Type()
		{
			return FNavigatorItemType;
		}
	};

	template<> XIN_EXPORT inline FType & Typeof<UI::ControlsEx::FNavigatorItem>()
	{
		return TStaticType<UI::ControlsEx::FNavigatorItem>().Type();
	}

	static TTypeRegister<UI::ControlsEx::FNavigatorItem> FNavigatorItemTypeRegister { TStaticType<UI::ControlsEx::FNavigatorItem>().Type() };

	class UNavigatorHeaderItem_Type : public TClass<UI::ControlsEx::UNavigatorHeaderItem>
	{
	public:
		using UNavigatorHeaderItem = UI::ControlsEx::UNavigatorHeaderItem;

		// --- 0 Constructors
		static inline const TConstructor<UNavigatorHeaderItem, UI::ControlsEx::UNavigator &> Constructor___UNavigator_lref { u8"!Constructor___UNavigator_lref"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor___UNavigator_lref,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<UNavigatorHeaderItem, FType &()> Method { u8"Type"N, &UNavigatorHeaderItem::Type, FMetadata::None };
		};
		struct Method___void___OnConstruct
		{
			static inline const TMethod<UNavigatorHeaderItem, void()> Method { u8"OnConstruct"N, &UNavigatorHeaderItem::OnConstruct, FMetadata::None };
		};
		struct Method___void___UpdateState
		{
			static inline const TMethod<UNavigatorHeaderItem, void()> Method { u8"UpdateState"N, &UNavigatorHeaderItem::UpdateState, FMetadata::None };
		};
		struct Method___void___OnMouseDown__FMouseButtonEventArgs_lref
		{
			static inline const TMethod<UNavigatorHeaderItem, void(UI::FMouseButtonEventArgs &)> Method { u8"OnMouseDown"N, &UNavigatorHeaderItem::OnMouseDown, FMetadata::None };
		};
		struct Method___void___OnIndicatingChanged__bool
		{
			static inline const TMethod<UNavigatorHeaderItem, void(bool)> Method { u8"OnIndicatingChanged"N, &UNavigatorHeaderItem::OnIndicatingChanged, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___void___OnConstruct::Method,
			Method___void___UpdateState::Method,
			Method___void___OnMouseDown__FMouseButtonEventArgs_lref::Method,
			Method___void___OnIndicatingChanged__bool::Method,
		};

		// --- 2 Fields

		// --- 3 Properties
		struct Property___DisplayIndex
		{
			static inline const TDependencyProperty Property { u8"DisplayIndex"N, &UNavigatorHeaderItem::__DisplayIndex, &UNavigatorHeaderItem::__GetDisplayIndex, &UNavigatorHeaderItem::__SetDisplayIndexValue, UNavigatorHeaderItem::__DisplayIndex__, UNavigatorHeaderItem::FPropertyMetadata::None };
		};
		struct Property___ItemName
		{
			static inline const TProxyProperty Property { u8"ItemName"N, &UNavigatorHeaderItem::__ItemName, &UNavigatorHeaderItem::__GetItemName, &UNavigatorHeaderItem::__SetItemName, UNavigatorHeaderItem::FPropertyMetadata::None };
		};
		struct Property___Indicating
		{
			struct FPropertyMetadata : UNavigatorHeaderItem::FPropertyMetadata
			{
				FPropertyMetadata()
				{
					AffectsState = true;
				}
			};
			static inline const FPropertyMetadata Metadata;
			static inline const TProxyProperty Property { u8"Indicating"N, &UNavigatorHeaderItem::__Indicating, &UNavigatorHeaderItem::__GetIndicating, &UNavigatorHeaderItem::__SetIndicating, Metadata };
		};
		static inline const TReferenceWrapper<const FProperty> ReflectedProperties[] =
		{
			Property___DisplayIndex::Property,
			Property___ItemName::Property,
			Property___Indicating::Property,
		};

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<UI::UContentControl>(),
		};

		// --- 5 Constructor
		UNavigatorHeaderItem_Type(FName Name = u8"UNavigatorHeaderItem"N) : TClass(Name, Classof<UI::UContentControl>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Methods = ReflectedMethods;
			FClass::Properties = ReflectedProperties;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	const TProperty<uintx> & UI::ControlsEx::UNavigatorHeaderItem::DisplayIndexProperty() { return UNavigatorHeaderItem_Type::Property___DisplayIndex::Property; }

	const TProperty<FName> & UI::ControlsEx::UNavigatorHeaderItem::ItemNameProperty() { return UNavigatorHeaderItem_Type::Property___ItemName::Property; }

	const TProperty<bool> & UI::ControlsEx::UNavigatorHeaderItem::IndicatingProperty() { return UNavigatorHeaderItem_Type::Property___Indicating::Property; }

	template<> struct XIN_EXPORT TStaticType<UI::ControlsEx::UNavigatorHeaderItem>
	{
		static inline UNavigatorHeaderItem_Type UNavigatorHeaderItemType { u8"UNavigatorHeaderItem"N };

		static FType & Type()
		{
			return UNavigatorHeaderItemType;
		}
	};

	FType & UI::ControlsEx::UNavigatorHeaderItem::StaticType()
	{
		return TStaticType<UI::ControlsEx::UNavigatorHeaderItem>().Type();
	}

	static TTypeRegister<UI::ControlsEx::UNavigatorHeaderItem> UNavigatorHeaderItemTypeRegister { TStaticType<UI::ControlsEx::UNavigatorHeaderItem>().Type() };

	class UNavigatorHeader_Type : public TClass<UI::ControlsEx::UNavigatorHeader>
	{
	public:
		using UNavigatorHeader = UI::ControlsEx::UNavigatorHeader;

		// --- 0 Constructors
		static inline const TConstructor<UNavigatorHeader, UI::ControlsEx::UNavigator &> Constructor___UNavigator_lref { u8"!Constructor___UNavigator_lref"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor___UNavigator_lref,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<UNavigatorHeader, FType &()> Method { u8"Type"N, &UNavigatorHeader::Type, FMetadata::None };
		};
		struct Method___void___OnInitialized
		{
			static inline const TMethod<UNavigatorHeader, void()> Method { u8"OnInitialized"N, &UNavigatorHeader::OnInitialized, FMetadata::None };
		};
		struct Method___void___OnFinalize
		{
			static inline const TMethod<UNavigatorHeader, void()> Method { u8"OnFinalize"N, &UNavigatorHeader::OnFinalize, FMetadata::None };
		};
		struct Method___void___OnArrange
		{
			static inline const TMethod<UNavigatorHeader, void()> Method { u8"OnArrange"N, &UNavigatorHeader::OnArrange, FMetadata::None };
		};
		struct Method___void___UpdateItems
		{
			static inline const TMethod<UNavigatorHeader, void()> Method { u8"UpdateItems"N, &UNavigatorHeader::UpdateItems, FMetadata::None };
		};
		struct Method___void___IndicateItem__FName
		{
			static inline const TMethod<UNavigatorHeader, void(FName)> Method { u8"IndicateItem"N, &UNavigatorHeader::IndicateItem, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___void___OnInitialized::Method,
			Method___void___OnFinalize::Method,
			Method___void___OnArrange::Method,
			Method___void___UpdateItems::Method,
			Method___void___IndicateItem__FName::Method,
		};

		// --- 2 Fields
		struct Field___HeaderItems
		{
			static inline const TField Field { u8"HeaderItems"N, &UNavigatorHeader::HeaderItems, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FField> ReflectedFields[] =
		{
			Field___HeaderItems::Field,
		};

		// --- 3 Properties
		struct Property___ScrollOffset
		{
			struct FPropertyMetadata : UNavigatorHeader::FPropertyMetadata
			{
				FPropertyMetadata()
				{
					AffectsArrange = true;
				}
			};
			static inline const FPropertyMetadata Metadata;
			static inline const TDependencyProperty Property { u8"ScrollOffset"N, &UNavigatorHeader::__ScrollOffset, &UNavigatorHeader::__GetScrollOffset, &UNavigatorHeader::__SetScrollOffsetValue, UNavigatorHeader::__ScrollOffset__, Metadata };
		};
		static inline const TReferenceWrapper<const FProperty> ReflectedProperties[] =
		{
			Property___ScrollOffset::Property,
		};

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<UI::UListControl>(),
		};

		// --- 5 Constructor
		UNavigatorHeader_Type(FName Name = u8"UNavigatorHeader"N) : TClass(Name, Classof<UI::UListControl>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Fields = ReflectedFields;
			FClass::Methods = ReflectedMethods;
			FClass::Properties = ReflectedProperties;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	const TProperty<dimenx> & UI::ControlsEx::UNavigatorHeader::ScrollOffsetProperty() { return UNavigatorHeader_Type::Property___ScrollOffset::Property; }

	template<> struct XIN_EXPORT TStaticType<UI::ControlsEx::UNavigatorHeader>
	{
		static inline UNavigatorHeader_Type UNavigatorHeaderType { u8"UNavigatorHeader"N };

		static FType & Type()
		{
			return UNavigatorHeaderType;
		}
	};

	FType & UI::ControlsEx::UNavigatorHeader::StaticType()
	{
		return TStaticType<UI::ControlsEx::UNavigatorHeader>().Type();
	}

	static TTypeRegister<UI::ControlsEx::UNavigatorHeader> UNavigatorHeaderTypeRegister { TStaticType<UI::ControlsEx::UNavigatorHeader>().Type() };

	class UNavigator_Type : public TClass<UI::ControlsEx::UNavigator>
	{
	public:
		using UNavigator = UI::ControlsEx::UNavigator;

		// --- 0 Constructors
		static inline const TConstructor<UNavigator> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<UNavigator, FType &()> Method { u8"Type"N, &UNavigator::Type, FMetadata::None };
		};
		struct Method___void___OnConstruct
		{
			static inline const TMethod<UNavigator, void()> Method { u8"OnConstruct"N, &UNavigator::OnConstruct, FMetadata::None };
		};
		struct Method___void___OnInitialize
		{
			static inline const TMethod<UNavigator, void()> Method { u8"OnInitialize"N, &UNavigator::OnInitialize, FMetadata::None };
		};
		struct Method___TVec2__float64_____OnMeasure__TVec2__float64__
		{
			static inline const TMethod<UNavigator, FSize(FSize)> Method { u8"OnMeasure"N, &UNavigator::OnMeasure, FMetadata::None };
		};
		struct Method___void___OnArrange
		{
			static inline const TMethod<UNavigator, void()> Method { u8"OnArrange"N, &UNavigator::OnArrange, FMetadata::None };
		};
		struct Method___void___Navigate__FName
		{
			static inline const TMethod<UNavigator, void(FName)> Method { u8"Navigate"N, &UNavigator::Navigate, FMetadata::None };
		};
		struct Method___void___OnItemsChanged__TList__FNavigatorItem___lref
		{
			static inline const TMethod<UNavigator, void(TList<UI::ControlsEx::FNavigatorItem> &)> Method { u8"OnItemsChanged"N, &UNavigator::OnItemsChanged, FMetadata::None };
		};
		struct Method___void___OnIndicatingIndexChanged__uintx
		{
			static inline const TMethod<UNavigator, void(uintx)> Method { u8"OnIndicatingIndexChanged"N, &UNavigator::OnIndicatingIndexChanged, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___void___OnConstruct::Method,
			Method___void___OnInitialize::Method,
			Method___TVec2__float64_____OnMeasure__TVec2__float64__::Method,
			Method___void___OnArrange::Method,
			Method___void___Navigate__FName::Method,
			Method___void___OnItemsChanged__TList__FNavigatorItem___lref::Method,
			Method___void___OnIndicatingIndexChanged__uintx::Method,
		};

		// --- 2 Fields
		struct Field___NavigatorHeader
		{
			static inline const TField Field { u8"NavigatorHeader"N, &UNavigator::NavigatorHeader, FMetadata::None };
		};
		struct Field___NavigatorPresenter
		{
			static inline const TField Field { u8"NavigatorPresenter"N, &UNavigator::NavigatorPresenter, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FField> ReflectedFields[] =
		{
			Field___NavigatorHeader::Field,
			Field___NavigatorPresenter::Field,
		};

		// --- 3 Properties
		struct Property___IndicatingIndex
		{
			static inline const TProxyProperty Property { u8"IndicatingIndex"N, &UNavigator::__IndicatingIndex, &UNavigator::__GetIndicatingIndex, &UNavigator::__SetIndicatingIndex, UNavigator::FPropertyMetadata::None };
		};
		static inline const TReferenceWrapper<const FProperty> ReflectedProperties[] =
		{
			Property___IndicatingIndex::Property,
		};

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<UI::UControl>(),
		};

		// --- 5 Constructor
		UNavigator_Type(FName Name = u8"UNavigator"N) : TClass(Name, Classof<UI::UControl>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Fields = ReflectedFields;
			FClass::Methods = ReflectedMethods;
			FClass::Properties = ReflectedProperties;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	const TProperty<uintx> & UI::ControlsEx::UNavigator::IndicatingIndexProperty() { return UNavigator_Type::Property___IndicatingIndex::Property; }

	template<> struct XIN_EXPORT TStaticType<UI::ControlsEx::UNavigator>
	{
		static inline UNavigator_Type UNavigatorType { u8"UNavigator"N };

		static FType & Type()
		{
			return UNavigatorType;
		}
	};

	FType & UI::ControlsEx::UNavigator::StaticType()
	{
		return TStaticType<UI::ControlsEx::UNavigator>().Type();
	}

	static TTypeRegister<UI::ControlsEx::UNavigator> UNavigatorTypeRegister { TStaticType<UI::ControlsEx::UNavigator>().Type() };
}

// Generated code end.
