// 398C1BD8806A146CCBA0DC5FF5E3C709 2025-03-24 17:05:11
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Controls/ListControl.h"

namespace Xin
{
	class ESelectMethod_Type : public TEnum<UI::ESelectMethod>
	{
	public:
		// --- 1 Items
		struct Item___Press
		{
			static inline const TEnumItem Item { u8"Press"N, UI::ESelectMethod::Press, FMetadata::None };
		};
		struct Item___Release
		{
			static inline const TEnumItem Item { u8"Release"N, UI::ESelectMethod::Release, FMetadata::None };
		};
		struct Item___Click
		{
			static inline const TEnumItem Item { u8"Click"N, UI::ESelectMethod::Click, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FEnumItem> ReflectedItems[] =
		{
			Item___Press::Item,
			Item___Release::Item,
			Item___Click::Item,
		};

		// --- 2 Constructor
		ESelectMethod_Type(FName Name = u8"ESelectMethod"N) : TEnum(Name, FMetadata::None)
		{
			FEnum::Items = ReflectedItems;
		}
	};

	template<> struct XIN_EXPORT TStaticType<UI::ESelectMethod>
	{
		static inline ESelectMethod_Type ESelectMethodType { u8"ESelectMethod"N };

		static FType & Type()
		{
			return ESelectMethodType;
		}
	};

	template<> XIN_EXPORT inline FType & Typeof<UI::ESelectMethod>()
	{
		return TStaticType<UI::ESelectMethod>().Type();
	}

	static TTypeRegister<UI::ESelectMethod> ESelectMethodTypeRegister { TStaticType<UI::ESelectMethod>().Type() };

	class ESelectionMode_Type : public TEnum<UI::ESelectionMode>
	{
	public:
		// --- 1 Items
		struct Item___Single
		{
			static inline const TEnumItem Item { u8"Single"N, UI::ESelectionMode::Single, FMetadata::None };
		};
		struct Item___Multiple
		{
			static inline const TEnumItem Item { u8"Multiple"N, UI::ESelectionMode::Multiple, FMetadata::None };
		};
		struct Item___Extended
		{
			static inline const TEnumItem Item { u8"Extended"N, UI::ESelectionMode::Extended, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FEnumItem> ReflectedItems[] =
		{
			Item___Single::Item,
			Item___Multiple::Item,
			Item___Extended::Item,
		};

		// --- 2 Constructor
		ESelectionMode_Type(FName Name = u8"ESelectionMode"N) : TEnum(Name, FMetadata::None)
		{
			FEnum::Items = ReflectedItems;
		}
	};

	template<> struct XIN_EXPORT TStaticType<UI::ESelectionMode>
	{
		static inline ESelectionMode_Type ESelectionModeType { u8"ESelectionMode"N };

		static FType & Type()
		{
			return ESelectionModeType;
		}
	};

	template<> XIN_EXPORT inline FType & Typeof<UI::ESelectionMode>()
	{
		return TStaticType<UI::ESelectionMode>().Type();
	}

	static TTypeRegister<UI::ESelectionMode> ESelectionModeTypeRegister { TStaticType<UI::ESelectionMode>().Type() };

	class UListControl_Type : public TClass<UI::UListControl>
	{
	public:
		using UListControl = UI::UListControl;

		// --- 0 Constructors
		static inline const TConstructor<UListControl> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TConstructor<UListControl, FName> Constructor___FName { u8"!Constructor___FName"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
			Constructor___FName,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<UListControl, FType &()> Method { u8"Type"N, &UListControl::Type, FMetadata::None };
		};
		struct Method___void___OnTemplated
		{
			static inline const TMethod<UListControl, void()> Method { u8"OnTemplated"N, &UListControl::OnTemplated, FMetadata::None };
		};
		struct Method___void___OnInitialize
		{
			static inline const TMethod<UListControl, void()> Method { u8"OnInitialize"N, &UListControl::OnInitialize, FMetadata::None };
		};
		struct Method___void___OnInitialized
		{
			static inline const TMethod<UListControl, void()> Method { u8"OnInitialized"N, &UListControl::OnInitialized, FMetadata::None };
		};
		struct Method___void___OnFinalize
		{
			static inline const TMethod<UListControl, void()> Method { u8"OnFinalize"N, &UListControl::OnFinalize, FMetadata::None };
		};
		struct Method___void___OnPaint__IPainter_lref
		{
			static inline const TMethod<UListControl, void(IPainter &)> Method { u8"OnPaint"N, &UListControl::OnPaint, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___void___OnTemplated::Method,
			Method___void___OnInitialize::Method,
			Method___void___OnInitialized::Method,
			Method___void___OnFinalize::Method,
			Method___void___OnPaint__IPainter_lref::Method,
		};

		// --- 2 Fields
		struct Field___ItemsPanel
		{
			static inline const TField Field { u8"ItemsPanel"N, &UListControl::ItemsPanel, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FField> ReflectedFields[] =
		{
			Field___ItemsPanel::Field,
		};

		// --- 3 Properties

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<UI::UControl>(),
		};

		// --- 5 Constructor
		UListControl_Type(FName Name = u8"UListControl"N) : TClass(Name, Classof<UI::UControl>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Fields = ReflectedFields;
			FClass::Methods = ReflectedMethods;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	template<> struct XIN_EXPORT TStaticType<UI::UListControl>
	{
		static inline UListControl_Type UListControlType { u8"UListControl"N };

		static FType & Type()
		{
			return UListControlType;
		}
	};

	FType & UI::UListControl::StaticType()
	{
		return TStaticType<UI::UListControl>().Type();
	}

	static TTypeRegister<UI::UListControl> UListControlTypeRegister { TStaticType<UI::UListControl>().Type() };
}

// Generated code end.
