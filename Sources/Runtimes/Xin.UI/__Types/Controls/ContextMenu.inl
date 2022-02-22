// 9B754D7C4BC3D9DA7206666464F593C9 2025-03-31 14:40:00
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Controls/ContextMenu.h"

namespace Xin
{
	class UContextMenu_Type : public TClass<UI::UContextMenu>
	{
	public:
		using UContextMenu = UI::UContextMenu;

		// --- 0 Constructors
		static inline const TConstructor<UContextMenu> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<UContextMenu, FType &()> Method { u8"Type"N, &UContextMenu::Type, FMetadata::None };
		};
		struct Method___void___OnConstruct
		{
			static inline const TMethod<UContextMenu, void()> Method { u8"OnConstruct"N, &UContextMenu::OnConstruct, FMetadata::None };
		};
		struct Method___void___OnInitialize
		{
			static inline const TMethod<UContextMenu, void()> Method { u8"OnInitialize"N, &UContextMenu::OnInitialize, FMetadata::None };
		};
		struct Method___void___OnFinalize
		{
			static inline const TMethod<UContextMenu, void()> Method { u8"OnFinalize"N, &UContextMenu::OnFinalize, FMetadata::None };
		};
		struct Method___void___OnHidden
		{
			static inline const TMethod<UContextMenu, void()> Method { u8"OnHidden"N, &UContextMenu::OnHidden, FMetadata::None };
		};
		struct Method___void___ShowMenu__UElement_ptr__FVariant_const_lref__EPopupPosition__TFunction__void________
		{
			static inline const TMethod<UContextMenu, void(UI::UElement *, FVariant const &, EPopupPosition, TFunction<void()>)> Method { u8"ShowMenu"N, &UContextMenu::ShowMenu, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___void___OnConstruct::Method,
			Method___void___OnInitialize::Method,
			Method___void___OnFinalize::Method,
			Method___void___OnHidden::Method,
			Method___void___ShowMenu__UElement_ptr__FVariant_const_lref__EPopupPosition__TFunction__void________::Method,
		};

		// --- 2 Fields

		// --- 3 Properties

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<UI::UPopup>(),
		};

		// --- 5 Constructor
		UContextMenu_Type(FName Name = u8"UContextMenu"N) : TClass(Name, Classof<UI::UPopup>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Methods = ReflectedMethods;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	template<> struct XIN_EXPORT TStaticType<UI::UContextMenu>
	{
		static inline UContextMenu_Type UContextMenuType { u8"UContextMenu"N };

		static FType & Type()
		{
			return UContextMenuType;
		}
	};

	FType & UI::UContextMenu::StaticType()
	{
		return TStaticType<UI::UContextMenu>().Type();
	}

	static TTypeRegister<UI::UContextMenu> UContextMenuTypeRegister { TStaticType<UI::UContextMenu>().Type() };
}

// Generated code end.
