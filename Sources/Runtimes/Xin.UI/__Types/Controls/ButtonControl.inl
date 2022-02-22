// ADCF15A20BF0336436380472A51BAAD7 2025-03-07 20:04:07
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Controls/ButtonControl.h"

namespace Xin
{
	class UButtonControl_Type : public TClass<UI::UButtonControl>
	{
	public:
		using UButtonControl = UI::UButtonControl;

		// --- 0 Constructors
		static inline const TConstructor<UButtonControl> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TConstructor<UButtonControl, FName> Constructor___FName { u8"!Constructor___FName"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
			Constructor___FName,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<UButtonControl, FType &()> Method { u8"Type"N, &UButtonControl::Type, FMetadata::None };
		};
		struct Method___void___OnConstruct
		{
			static inline const TMethod<UButtonControl, void()> Method { u8"OnConstruct"N, &UButtonControl::OnConstruct, FMetadata::None };
		};
		struct Method___void___OnPaint__IPainter_lref
		{
			static inline const TMethod<UButtonControl, void(IPainter &)> Method { u8"OnPaint"N, &UButtonControl::OnPaint, FMetadata::None };
		};
		struct Method___void___UpdateState
		{
			static inline const TMethod<UButtonControl, void()> Method { u8"UpdateState"N, &UButtonControl::UpdateState, FMetadata::None };
		};
		struct Method___void___OnMouseDown__FMouseButtonEventArgs_lref
		{
			static inline const TMethod<UButtonControl, void(UI::FMouseButtonEventArgs &)> Method { u8"OnMouseDown"N, &UButtonControl::OnMouseDown, FMetadata::None };
		};
		struct Method___void___OnMouseUp__FMouseButtonEventArgs_lref
		{
			static inline const TMethod<UButtonControl, void(UI::FMouseButtonEventArgs &)> Method { u8"OnMouseUp"N, &UButtonControl::OnMouseUp, FMetadata::None };
		};
		struct Method___void___OnMouseEnter__FMouseEventArgs_lref
		{
			static inline const TMethod<UButtonControl, void(UI::FMouseEventArgs &)> Method { u8"OnMouseEnter"N, &UButtonControl::OnMouseEnter, FMetadata::None };
		};
		struct Method___void___OnMouseLeave__FMouseEventArgs_lref
		{
			static inline const TMethod<UButtonControl, void(UI::FMouseEventArgs &)> Method { u8"OnMouseLeave"N, &UButtonControl::OnMouseLeave, FMetadata::None };
		};
		struct Method___void___OnClick
		{
			static inline const TMethod<UButtonControl, void()> Method { u8"OnClick"N, &UButtonControl::OnClick, FMetadata::None };
		};
		struct Method___void___OnClick__FRoutedEventArgs_lref
		{
			static inline const TMethod<UButtonControl, void(UI::FRoutedEventArgs &)> Method { u8"OnClick"N, &UButtonControl::OnClick, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___void___OnConstruct::Method,
			Method___void___OnPaint__IPainter_lref::Method,
			Method___void___UpdateState::Method,
			Method___void___OnMouseDown__FMouseButtonEventArgs_lref::Method,
			Method___void___OnMouseUp__FMouseButtonEventArgs_lref::Method,
			Method___void___OnMouseEnter__FMouseEventArgs_lref::Method,
			Method___void___OnMouseLeave__FMouseEventArgs_lref::Method,
			Method___void___OnClick::Method,
			Method___void___OnClick__FRoutedEventArgs_lref::Method,
		};

		// --- 2 Fields
		struct Field___ClickMode
		{
			static inline const TField Field { u8"ClickMode"N, &UButtonControl::ClickMode, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FField> ReflectedFields[] =
		{
			Field___ClickMode::Field,
		};

		// --- 3 Properties
		struct Property___IsPressed
		{
			struct FPropertyMetadata : UButtonControl::FPropertyMetadata
			{
				FPropertyMetadata()
				{
					AffectsState = true;
				}
			};
			static inline const FPropertyMetadata Metadata;
			static inline const TDependencyProperty Property { u8"IsPressed"N, &UButtonControl::__IsPressed, &UButtonControl::__GetIsPressed, &UButtonControl::__SetIsPressedValue, UButtonControl::__IsPressed__, Metadata };
		};
		struct Property___Command
		{
			static inline const TDependencyProperty Property { u8"Command"N, &UButtonControl::__Command, &UButtonControl::__GetCommand, &UButtonControl::__SetCommandValue, UButtonControl::__Command__, UButtonControl::FPropertyMetadata::None };
		};
		static inline const TReferenceWrapper<const FProperty> ReflectedProperties[] =
		{
			Property___IsPressed::Property,
			Property___Command::Property,
		};

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<UI::UContentControl>(),
		};

		// --- 5 Constructor
		UButtonControl_Type(FName Name = u8"UButtonControl"N) : TClass(Name, Classof<UI::UContentControl>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Fields = ReflectedFields;
			FClass::Methods = ReflectedMethods;
			FClass::Properties = ReflectedProperties;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	const TProperty<bool> & UI::UButtonControl::IsPressedProperty() { return UButtonControl_Type::Property___IsPressed::Property; }

	const TProperty<UI::ICommandRef> & UI::UButtonControl::CommandProperty() { return UButtonControl_Type::Property___Command::Property; }

	template<> struct XIN_EXPORT TStaticType<UI::UButtonControl>
	{
		static inline UButtonControl_Type UButtonControlType { u8"UButtonControl"N };

		static FType & Type()
		{
			return UButtonControlType;
		}
	};

	FType & UI::UButtonControl::StaticType()
	{
		return TStaticType<UI::UButtonControl>().Type();
	}

	static TTypeRegister<UI::UButtonControl> UButtonControlTypeRegister { TStaticType<UI::UButtonControl>().Type() };
}

// Generated code end.
