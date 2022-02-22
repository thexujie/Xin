// 7C0DE3EB804844FBB5D2CFF81CF42AFD 2025-03-25 15:49:24
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Controls/Menu.h"

namespace Xin
{
	class UMenuItem_Type : public TClass<UI::UMenuItem>
	{
	public:
		using UMenuItem = UI::UMenuItem;

		// --- 0 Constructors
		static inline const TConstructor<UMenuItem> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<UMenuItem, FType &()> Method { u8"Type"N, &UMenuItem::Type, FMetadata::None };
		};
		struct Method___void___OnConstruct
		{
			static inline const TMethod<UMenuItem, void()> Method { u8"OnConstruct"N, &UMenuItem::OnConstruct, FMetadata::None };
		};
		struct Method___void___UpdateState
		{
			static inline const TMethod<UMenuItem, void()> Method { u8"UpdateState"N, &UMenuItem::UpdateState, FMetadata::None };
		};
		struct Method___void___OnMouseEnter__FMouseEventArgs_lref
		{
			static inline const TMethod<UMenuItem, void(UI::FMouseEventArgs &)> Method { u8"OnMouseEnter"N, &UMenuItem::OnMouseEnter, FMetadata::None };
		};
		struct Method___void___OnMouseLeave__FMouseEventArgs_lref
		{
			static inline const TMethod<UMenuItem, void(UI::FMouseEventArgs &)> Method { u8"OnMouseLeave"N, &UMenuItem::OnMouseLeave, FMetadata::None };
		};
		struct Method___void___OnMouseDown__FMouseButtonEventArgs_lref
		{
			static inline const TMethod<UMenuItem, void(UI::FMouseButtonEventArgs &)> Method { u8"OnMouseDown"N, &UMenuItem::OnMouseDown, FMetadata::None };
		};
		struct Method___void___OnMouseUp__FMouseButtonEventArgs_lref
		{
			static inline const TMethod<UMenuItem, void(UI::FMouseButtonEventArgs &)> Method { u8"OnMouseUp"N, &UMenuItem::OnMouseUp, FMetadata::None };
		};
		struct Method___void___OnIndicatingChanged__bool
		{
			static inline const TMethod<UMenuItem, void(bool)> Method { u8"OnIndicatingChanged"N, &UMenuItem::OnIndicatingChanged, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___void___OnConstruct::Method,
			Method___void___UpdateState::Method,
			Method___void___OnMouseEnter__FMouseEventArgs_lref::Method,
			Method___void___OnMouseLeave__FMouseEventArgs_lref::Method,
			Method___void___OnMouseDown__FMouseButtonEventArgs_lref::Method,
			Method___void___OnMouseUp__FMouseButtonEventArgs_lref::Method,
			Method___void___OnIndicatingChanged__bool::Method,
		};

		// --- 2 Fields
		struct Field___Owner
		{
			static inline const TField Field { u8"Owner"N, &UMenuItem::Owner, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FField> ReflectedFields[] =
		{
			Field___Owner::Field,
		};

		// --- 3 Properties
		struct Property___Index
		{
			static inline const TProxyProperty Property { u8"Index"N, &UMenuItem::__Index, &UMenuItem::__GetIndex, &UMenuItem::__SetIndex, UMenuItem::FPropertyMetadata::None };
		};
		struct Property___SelecteAble
		{
			static inline const TProxyProperty Property { u8"SelecteAble"N, &UMenuItem::__SelecteAble, &UMenuItem::__GetSelecteAble, &UMenuItem::__SetSelecteAble, UMenuItem::FPropertyMetadata::None };
		};
		struct Property___Submenu
		{
			struct FPropertyMetadata : UMenuItem::FPropertyMetadata
			{
				FPropertyMetadata()
				{
					AffectsState = true;
				}
			};
			static inline const FPropertyMetadata Metadata;
			static inline const TDependencyProperty Property { u8"Submenu"N, &UMenuItem::__Submenu, &UMenuItem::__GetSubmenu, &UMenuItem::__SetSubmenuValue, UMenuItem::__Submenu__, Metadata };
		};
		struct Property___Selected
		{
			struct FPropertyMetadata : UMenuItem::FPropertyMetadata
			{
				FPropertyMetadata()
				{
					AffectsState = true;
				}
			};
			static inline const FPropertyMetadata Metadata;
			static inline const TDependencyProperty Property { u8"Selected"N, &UMenuItem::__Selected, &UMenuItem::__GetSelected, &UMenuItem::__SetSelectedValue, UMenuItem::__Selected__, Metadata };
		};
		struct Property___Indicating
		{
			struct FPropertyMetadata : UMenuItem::FPropertyMetadata
			{
				FPropertyMetadata()
				{
					AffectsState = true;
				}
			};
			static inline const FPropertyMetadata Metadata;
			static inline const TDependencyProperty Property { u8"Indicating"N, &UMenuItem::__Indicating, &UMenuItem::__GetIndicating, &UMenuItem::__SetIndicatingValue, UMenuItem::__Indicating__, Metadata };
		};
		static inline const TReferenceWrapper<const FProperty> ReflectedProperties[] =
		{
			Property___Index::Property,
			Property___SelecteAble::Property,
			Property___Submenu::Property,
			Property___Selected::Property,
			Property___Indicating::Property,
		};

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<UI::UContentControl>(),
		};

		// --- 5 Constructor
		UMenuItem_Type(FName Name = u8"UMenuItem"N) : TClass(Name, Classof<UI::UContentControl>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Fields = ReflectedFields;
			FClass::Methods = ReflectedMethods;
			FClass::Properties = ReflectedProperties;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	const TProperty<intx> & UI::UMenuItem::IndexProperty() { return UMenuItem_Type::Property___Index::Property; }

	const TProperty<bool> & UI::UMenuItem::SelecteAbleProperty() { return UMenuItem_Type::Property___SelecteAble::Property; }

	const TProperty<FVariant> & UI::UMenuItem::SubmenuProperty() { return UMenuItem_Type::Property___Submenu::Property; }

	const TProperty<bool> & UI::UMenuItem::SelectedProperty() { return UMenuItem_Type::Property___Selected::Property; }

	const TProperty<bool> & UI::UMenuItem::IndicatingProperty() { return UMenuItem_Type::Property___Indicating::Property; }

	template<> struct XIN_EXPORT TStaticType<UI::UMenuItem>
	{
		static inline UMenuItem_Type UMenuItemType { u8"UMenuItem"N };

		static FType & Type()
		{
			return UMenuItemType;
		}
	};

	FType & UI::UMenuItem::StaticType()
	{
		return TStaticType<UI::UMenuItem>().Type();
	}

	static TTypeRegister<UI::UMenuItem> UMenuItemTypeRegister { TStaticType<UI::UMenuItem>().Type() };

	class UMenuSaperator_Type : public TClass<UI::UMenuSaperator>
	{
	public:
		using UMenuSaperator = UI::UMenuSaperator;

		// --- 0 Constructors
		static inline const TConstructor<UMenuSaperator> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<UMenuSaperator, FType &()> Method { u8"Type"N, &UMenuSaperator::Type, FMetadata::None };
		};
		struct Method___void___OnConstruct
		{
			static inline const TMethod<UMenuSaperator, void()> Method { u8"OnConstruct"N, &UMenuSaperator::OnConstruct, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___void___OnConstruct::Method,
		};

		// --- 2 Fields

		// --- 3 Properties

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<UI::UMenuItem>(),
		};

		// --- 5 Constructor
		UMenuSaperator_Type(FName Name = u8"UMenuSaperator"N) : TClass(Name, Classof<UI::UMenuItem>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Methods = ReflectedMethods;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	template<> struct XIN_EXPORT TStaticType<UI::UMenuSaperator>
	{
		static inline UMenuSaperator_Type UMenuSaperatorType { u8"UMenuSaperator"N };

		static FType & Type()
		{
			return UMenuSaperatorType;
		}
	};

	FType & UI::UMenuSaperator::StaticType()
	{
		return TStaticType<UI::UMenuSaperator>().Type();
	}

	static TTypeRegister<UI::UMenuSaperator> UMenuSaperatorTypeRegister { TStaticType<UI::UMenuSaperator>().Type() };

	class UMenu_Type : public TClass<UI::UMenu>
	{
	public:
		using UMenu = UI::UMenu;

		// --- 0 Constructors
		static inline const TConstructor<UMenu> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<UMenu, FType &()> Method { u8"Type"N, &UMenu::Type, FMetadata::None };
		};
		struct Method___TView__char8_____Describe____const
		{
			static inline const TMethod<UMenu, FStringV()> Method { u8"Describe"N, &UMenu::Describe, FMetadata::None };
		};
		struct Method___void___OnConstruct
		{
			static inline const TMethod<UMenu, void()> Method { u8"OnConstruct"N, &UMenu::OnConstruct, FMetadata::None };
		};
		struct Method___void___OnInitialize
		{
			static inline const TMethod<UMenu, void()> Method { u8"OnInitialize"N, &UMenu::OnInitialize, FMetadata::None };
		};
		struct Method___void___OnFinalize
		{
			static inline const TMethod<UMenu, void()> Method { u8"OnFinalize"N, &UMenu::OnFinalize, FMetadata::None };
		};
		struct Method___void___OnHidden
		{
			static inline const TMethod<UMenu, void()> Method { u8"OnHidden"N, &UMenu::OnHidden, FMetadata::None };
		};
		struct Method___void___OnKeyDown__FKeyboardEventArgs_lref
		{
			static inline const TMethod<UMenu, void(UI::FKeyboardEventArgs &)> Method { u8"OnKeyDown"N, &UMenu::OnKeyDown, FMetadata::None };
		};
		struct Method___void___OnItemMouseEnter__UMenuItem_lref__FMouseEventArgs_lref
		{
			static inline const TMethod<UMenu, void(UI::UMenuItem &, UI::FMouseEventArgs &)> Method { u8"OnItemMouseEnter"N, &UMenu::OnItemMouseEnter, FMetadata::None };
		};
		struct Method___void___OnItemMouseLeave__UMenuItem_lref__FMouseEventArgs_lref
		{
			static inline const TMethod<UMenu, void(UI::UMenuItem &, UI::FMouseEventArgs &)> Method { u8"OnItemMouseLeave"N, &UMenu::OnItemMouseLeave, FMetadata::None };
		};
		struct Method___void___OnItemMouseDown__UMenuItem_lref__FMouseButtonEventArgs_lref
		{
			static inline const TMethod<UMenu, void(UI::UMenuItem &, UI::FMouseButtonEventArgs &)> Method { u8"OnItemMouseDown"N, &UMenu::OnItemMouseDown, FMetadata::None };
		};
		struct Method___void___OnItemMouseUp__UMenuItem_lref__FMouseButtonEventArgs_lref
		{
			static inline const TMethod<UMenu, void(UI::UMenuItem &, UI::FMouseButtonEventArgs &)> Method { u8"OnItemMouseUp"N, &UMenu::OnItemMouseUp, FMetadata::None };
		};
		struct Method___void___OnItemSelected__FMenuItemSelectedEventArgs_lref
		{
			static inline const TMethod<UMenu, void(UI::FMenuItemSelectedEventArgs &)> Method { u8"OnItemSelected"N, &UMenu::OnItemSelected, FMetadata::None };
		};
		struct Method___void___ShowSubmenu
		{
			static inline const TMethod<UMenu, void()> Method { u8"ShowSubmenu"N, &UMenu::ShowSubmenu, FMetadata::None };
		};
		struct Method___void___HideSubmenu
		{
			static inline const TMethod<UMenu, void()> Method { u8"HideSubmenu"N, &UMenu::HideSubmenu, FMetadata::None };
		};
		struct Method___void___OnIndicatingIndexChanged__intx
		{
			static inline const TMethod<UMenu, void(intx)> Method { u8"OnIndicatingIndexChanged"N, &UMenu::OnIndicatingIndexChanged, FMetadata::None };
		};
		struct Method___void___OnHoveringIndexChanged__intx
		{
			static inline const TMethod<UMenu, void(intx)> Method { u8"OnHoveringIndexChanged"N, &UMenu::OnHoveringIndexChanged, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___TView__char8_____Describe____const::Method,
			Method___void___OnConstruct::Method,
			Method___void___OnInitialize::Method,
			Method___void___OnFinalize::Method,
			Method___void___OnHidden::Method,
			Method___void___OnKeyDown__FKeyboardEventArgs_lref::Method,
			Method___void___OnItemMouseEnter__UMenuItem_lref__FMouseEventArgs_lref::Method,
			Method___void___OnItemMouseLeave__UMenuItem_lref__FMouseEventArgs_lref::Method,
			Method___void___OnItemMouseDown__UMenuItem_lref__FMouseButtonEventArgs_lref::Method,
			Method___void___OnItemMouseUp__UMenuItem_lref__FMouseButtonEventArgs_lref::Method,
			Method___void___OnItemSelected__FMenuItemSelectedEventArgs_lref::Method,
			Method___void___ShowSubmenu::Method,
			Method___void___HideSubmenu::Method,
			Method___void___OnIndicatingIndexChanged__intx::Method,
			Method___void___OnHoveringIndexChanged__intx::Method,
		};

		// --- 2 Fields
		struct Field___ItemsPresented
		{
			static inline const TField Field { u8"ItemsPresented"N, &UMenu::ItemsPresented, FMetadata::None };
		};
		struct Field___ItemTemplate
		{
			static inline const TField Field { u8"ItemTemplate"N, &UMenu::ItemTemplate, FMetadata::None };
		};
		struct Field___MenuParent
		{
			static inline const TField Field { u8"MenuParent"N, &UMenu::MenuParent, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FField> ReflectedFields[] =
		{
			Field___ItemsPresented::Field,
			Field___ItemTemplate::Field,
			Field___MenuParent::Field,
		};

		// --- 3 Properties
		struct Property___SelectMethod
		{
			static inline const TProxyProperty Property { u8"SelectMethod"N, &UMenu::__SelectMethod, &UMenu::__GetSelectMethod, &UMenu::__SetSelectMethod, UMenu::FPropertyMetadata::None };
		};
		struct Property___IndicatingIndex
		{
			static inline const TProxyProperty Property { u8"IndicatingIndex"N, &UMenu::__IndicatingIndex, &UMenu::__GetIndicatingIndex, &UMenu::__SetIndicatingIndex, UMenu::FPropertyMetadata::None };
		};
		struct Property___HoveringIndex
		{
			static inline const TProxyProperty Property { u8"HoveringIndex"N, &UMenu::__HoveringIndex, &UMenu::__GetHoveringIndex, &UMenu::__SetHoveringIndex, UMenu::FPropertyMetadata::None };
		};
		static inline const TReferenceWrapper<const FProperty> ReflectedProperties[] =
		{
			Property___SelectMethod::Property,
			Property___IndicatingIndex::Property,
			Property___HoveringIndex::Property,
		};

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<UI::UListControl>(),
		};

		// --- 5 Constructor
		UMenu_Type(FName Name = u8"UMenu"N) : TClass(Name, Classof<UI::UListControl>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Fields = ReflectedFields;
			FClass::Methods = ReflectedMethods;
			FClass::Properties = ReflectedProperties;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	const TProperty<UI::ESelectMethod> & UI::UMenu::SelectMethodProperty() { return UMenu_Type::Property___SelectMethod::Property; }

	const TProperty<intx> & UI::UMenu::IndicatingIndexProperty() { return UMenu_Type::Property___IndicatingIndex::Property; }

	const TProperty<intx> & UI::UMenu::HoveringIndexProperty() { return UMenu_Type::Property___HoveringIndex::Property; }

	template<> struct XIN_EXPORT TStaticType<UI::UMenu>
	{
		static inline UMenu_Type UMenuType { u8"UMenu"N };

		static FType & Type()
		{
			return UMenuType;
		}
	};

	FType & UI::UMenu::StaticType()
	{
		return TStaticType<UI::UMenu>().Type();
	}

	static TTypeRegister<UI::UMenu> UMenuTypeRegister { TStaticType<UI::UMenu>().Type() };
}

// Generated code end.
