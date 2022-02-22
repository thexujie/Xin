// FE13DD0E852218C764AAAAE7524D787F 2025-03-24 18:06:21
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Controls/ListBox.h"

namespace Xin
{
	class UListBoxItem_Type : public TClass<UI::UListBoxItem>
	{
	public:
		using UListBoxItem = UI::UListBoxItem;

		// --- 0 Constructors
		static inline const TConstructor<UListBoxItem> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<UListBoxItem, FType &()> Method { u8"Type"N, &UListBoxItem::Type, FMetadata::None };
		};
		struct Method___void___OnConstruct
		{
			static inline const TMethod<UListBoxItem, void()> Method { u8"OnConstruct"N, &UListBoxItem::OnConstruct, FMetadata::None };
		};
		struct Method___void___UpdateState
		{
			static inline const TMethod<UListBoxItem, void()> Method { u8"UpdateState"N, &UListBoxItem::UpdateState, FMetadata::None };
		};
		struct Method___void___OnMouseDown__FMouseButtonEventArgs_lref
		{
			static inline const TMethod<UListBoxItem, void(UI::FMouseButtonEventArgs &)> Method { u8"OnMouseDown"N, &UListBoxItem::OnMouseDown, FMetadata::None };
		};
		struct Method___void___OnMouseUp__FMouseButtonEventArgs_lref
		{
			static inline const TMethod<UListBoxItem, void(UI::FMouseButtonEventArgs &)> Method { u8"OnMouseUp"N, &UListBoxItem::OnMouseUp, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___void___OnConstruct::Method,
			Method___void___UpdateState::Method,
			Method___void___OnMouseDown__FMouseButtonEventArgs_lref::Method,
			Method___void___OnMouseUp__FMouseButtonEventArgs_lref::Method,
		};

		// --- 2 Fields
		struct Field___Owner
		{
			static inline const TField Field { u8"Owner"N, &UListBoxItem::Owner, FMetadata::None };
		};
		struct Field___Index
		{
			static inline const TField Field { u8"Index"N, &UListBoxItem::Index, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FField> ReflectedFields[] =
		{
			Field___Owner::Field,
			Field___Index::Field,
		};

		// --- 3 Properties
		struct Property___Selected
		{
			struct FPropertyMetadata : UListBoxItem::FPropertyMetadata
			{
				FPropertyMetadata()
				{
					AffectsState = true;
				}
			};
			static inline const FPropertyMetadata Metadata;
			static inline const TDependencyProperty Property { u8"Selected"N, &UListBoxItem::__Selected, &UListBoxItem::__GetSelected, &UListBoxItem::__SetSelectedValue, UListBoxItem::__Selected__, Metadata };
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
		UListBoxItem_Type(FName Name = u8"UListBoxItem"N) : TClass(Name, Classof<UI::UContentControl>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Fields = ReflectedFields;
			FClass::Methods = ReflectedMethods;
			FClass::Properties = ReflectedProperties;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	const TProperty<bool> & UI::UListBoxItem::SelectedProperty() { return UListBoxItem_Type::Property___Selected::Property; }

	template<> struct XIN_EXPORT TStaticType<UI::UListBoxItem>
	{
		static inline UListBoxItem_Type UListBoxItemType { u8"UListBoxItem"N };

		static FType & Type()
		{
			return UListBoxItemType;
		}
	};

	FType & UI::UListBoxItem::StaticType()
	{
		return TStaticType<UI::UListBoxItem>().Type();
	}

	static TTypeRegister<UI::UListBoxItem> UListBoxItemTypeRegister { TStaticType<UI::UListBoxItem>().Type() };

	class UListBox_Type : public TClass<UI::UListBox>
	{
	public:
		using UListBox = UI::UListBox;

		// --- 0 Constructors
		static inline const TConstructor<UListBox> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<UListBox, FType &()> Method { u8"Type"N, &UListBox::Type, FMetadata::None };
		};
		struct Method___void___OnConstruct
		{
			static inline const TMethod<UListBox, void()> Method { u8"OnConstruct"N, &UListBox::OnConstruct, FMetadata::None };
		};
		struct Method___void___OnInitialize
		{
			static inline const TMethod<UListBox, void()> Method { u8"OnInitialize"N, &UListBox::OnInitialize, FMetadata::None };
		};
		struct Method___void___OnInitialized
		{
			static inline const TMethod<UListBox, void()> Method { u8"OnInitialized"N, &UListBox::OnInitialized, FMetadata::None };
		};
		struct Method___void___OnFinalize
		{
			static inline const TMethod<UListBox, void()> Method { u8"OnFinalize"N, &UListBox::OnFinalize, FMetadata::None };
		};
		struct Method___void___OnItemMouseDown__UListBoxItem_lref__FMouseButtonEventArgs_lref
		{
			static inline const TMethod<UListBox, void(UI::UListBoxItem &, UI::FMouseButtonEventArgs &)> Method { u8"OnItemMouseDown"N, &UListBox::OnItemMouseDown, FMetadata::None };
		};
		struct Method___void___OnItemMouseUp__UListBoxItem_lref__FMouseButtonEventArgs_lref
		{
			static inline const TMethod<UListBox, void(UI::UListBoxItem &, UI::FMouseButtonEventArgs &)> Method { u8"OnItemMouseUp"N, &UListBox::OnItemMouseUp, FMetadata::None };
		};
		struct Method___void___OnSelectedIndexChanged__uintx
		{
			static inline const TMethod<UListBox, void(uintx)> Method { u8"OnSelectedIndexChanged"N, &UListBox::OnSelectedIndexChanged, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___void___OnConstruct::Method,
			Method___void___OnInitialize::Method,
			Method___void___OnInitialized::Method,
			Method___void___OnFinalize::Method,
			Method___void___OnItemMouseDown__UListBoxItem_lref__FMouseButtonEventArgs_lref::Method,
			Method___void___OnItemMouseUp__UListBoxItem_lref__FMouseButtonEventArgs_lref::Method,
			Method___void___OnSelectedIndexChanged__uintx::Method,
		};

		// --- 2 Fields
		struct Field___ItemsPresented
		{
			static inline const TField Field { u8"ItemsPresented"N, &UListBox::ItemsPresented, FMetadata::None };
		};
		struct Field___ItemTemplate
		{
			static inline const TField Field { u8"ItemTemplate"N, &UListBox::ItemTemplate, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FField> ReflectedFields[] =
		{
			Field___ItemsPresented::Field,
			Field___ItemTemplate::Field,
		};

		// --- 3 Properties
		struct Property___SelectedIndices
		{
			static inline const TDependencyProperty Property { u8"SelectedIndices"N, &UListBox::__SelectedIndices, &UListBox::__GetSelectedIndices, &UListBox::__SetSelectedIndicesValue, UListBox::__SelectedIndices__, UListBox::FPropertyMetadata::None };
		};
		struct Property___SelectedIndex
		{
			static inline const TDependencyProperty Property { u8"SelectedIndex"N, &UListBox::__SelectedIndex, &UListBox::__GetSelectedIndex, &UListBox::__SetSelectedIndexValue, UListBox::__SelectedIndex__, UListBox::FPropertyMetadata::None };
		};
		struct Property___SelectMethod
		{
			static inline const TDependencyProperty Property { u8"SelectMethod"N, &UListBox::__SelectMethod, &UListBox::__GetSelectMethod, &UListBox::__SetSelectMethodValue, UListBox::__SelectMethod__, UListBox::FPropertyMetadata::None };
		};
		struct Property___SelectionMode
		{
			static inline const TDependencyProperty Property { u8"SelectionMode"N, &UListBox::__SelectionMode, &UListBox::__GetSelectionMode, &UListBox::__SetSelectionModeValue, UListBox::__SelectionMode__, UListBox::FPropertyMetadata::None };
		};
		static inline const TReferenceWrapper<const FProperty> ReflectedProperties[] =
		{
			Property___SelectedIndices::Property,
			Property___SelectedIndex::Property,
			Property___SelectMethod::Property,
			Property___SelectionMode::Property,
		};

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<UI::UListControl>(),
		};

		// --- 5 Constructor
		UListBox_Type(FName Name = u8"UListBox"N) : TClass(Name, Classof<UI::UListControl>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Fields = ReflectedFields;
			FClass::Methods = ReflectedMethods;
			FClass::Properties = ReflectedProperties;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	const TProperty<TList<uintx>> & UI::UListBox::SelectedIndicesProperty() { return UListBox_Type::Property___SelectedIndices::Property; }

	const TProperty<uintx> & UI::UListBox::SelectedIndexProperty() { return UListBox_Type::Property___SelectedIndex::Property; }

	const TProperty<UI::ESelectMethod> & UI::UListBox::SelectMethodProperty() { return UListBox_Type::Property___SelectMethod::Property; }

	const TProperty<UI::ESelectionMode> & UI::UListBox::SelectionModeProperty() { return UListBox_Type::Property___SelectionMode::Property; }

	template<> struct XIN_EXPORT TStaticType<UI::UListBox>
	{
		static inline UListBox_Type UListBoxType { u8"UListBox"N };

		static FType & Type()
		{
			return UListBoxType;
		}
	};

	FType & UI::UListBox::StaticType()
	{
		return TStaticType<UI::UListBox>().Type();
	}

	static TTypeRegister<UI::UListBox> UListBoxTypeRegister { TStaticType<UI::UListBox>().Type() };
}

// Generated code end.
