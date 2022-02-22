// 9D87587DA27B1678258CC27FB7A1EFEC 2025-03-24 18:06:49
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Controls/ToolTip.h"

namespace Xin
{
	class UToolTip_Type : public TClass<UI::UToolTip>
	{
	public:
		using UToolTip = UI::UToolTip;

		// --- 0 Constructors
		static inline const TConstructor<UToolTip> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<UToolTip, FType &()> Method { u8"Type"N, &UToolTip::Type, FMetadata::None };
		};
		struct Method___void___OnConstruct
		{
			static inline const TMethod<UToolTip, void()> Method { u8"OnConstruct"N, &UToolTip::OnConstruct, FMetadata::None };
		};
		struct Method___void___OnInitialize
		{
			static inline const TMethod<UToolTip, void()> Method { u8"OnInitialize"N, &UToolTip::OnInitialize, FMetadata::None };
		};
		struct Method___void___OnFinalize
		{
			static inline const TMethod<UToolTip, void()> Method { u8"OnFinalize"N, &UToolTip::OnFinalize, FMetadata::None };
		};
		struct Method___void___OnHidden
		{
			static inline const TMethod<UToolTip, void()> Method { u8"OnHidden"N, &UToolTip::OnHidden, FMetadata::None };
		};
		struct Method___void___OnTrackElementChanged__TEntryPtr__UElement__
		{
			static inline const TMethod<UToolTip, void(UI::UElementObj)> Method { u8"OnTrackElementChanged"N, &UToolTip::OnTrackElementChanged, FMetadata::None };
		};
		struct Method___void___ShowElementToolTip__UElement_ptr
		{
			static inline const TMethod<UToolTip, void(UI::UElement *)> Method { u8"ShowElementToolTip"N, &UToolTip::ShowElementToolTip, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___void___OnConstruct::Method,
			Method___void___OnInitialize::Method,
			Method___void___OnFinalize::Method,
			Method___void___OnHidden::Method,
			Method___void___OnTrackElementChanged__TEntryPtr__UElement__::Method,
			Method___void___ShowElementToolTip__UElement_ptr::Method,
		};

		// --- 2 Fields

		// --- 3 Properties

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<UI::UPopup>(),
		};

		// --- 5 Constructor
		UToolTip_Type(FName Name = u8"UToolTip"N) : TClass(Name, Classof<UI::UPopup>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Methods = ReflectedMethods;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	template<> struct XIN_EXPORT TStaticType<UI::UToolTip>
	{
		static inline UToolTip_Type UToolTipType { u8"UToolTip"N };

		static FType & Type()
		{
			return UToolTipType;
		}
	};

	FType & UI::UToolTip::StaticType()
	{
		return TStaticType<UI::UToolTip>().Type();
	}

	static TTypeRegister<UI::UToolTip> UToolTipTypeRegister { TStaticType<UI::UToolTip>().Type() };
}

// Generated code end.
