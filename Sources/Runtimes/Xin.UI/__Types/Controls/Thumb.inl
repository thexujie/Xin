// 7DE2C8EC1EF871E94EF266571149AD4A 2025-03-06 15:49:03
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Controls/Thumb.h"

namespace Xin
{
	class UThumb_Type : public TClass<UI::UThumb>
	{
	public:
		using UThumb = UI::UThumb;

		// --- 0 Constructors
		static inline const TConstructor<UThumb> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TConstructor<UThumb, FName> Constructor___FName { u8"!Constructor___FName"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
			Constructor___FName,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<UThumb, FType &()> Method { u8"Type"N, &UThumb::Type, FMetadata::None };
		};
		struct Method___void___OnConstruct
		{
			static inline const TMethod<UThumb, void()> Method { u8"OnConstruct"N, &UThumb::OnConstruct, FMetadata::None };
		};
		struct Method___void___OnPaint__IPainter_lref
		{
			static inline const TMethod<UThumb, void(IPainter &)> Method { u8"OnPaint"N, &UThumb::OnPaint, FMetadata::None };
		};
		struct Method___void___OnMouseEnter__FMouseEventArgs_lref
		{
			static inline const TMethod<UThumb, void(UI::FMouseEventArgs &)> Method { u8"OnMouseEnter"N, &UThumb::OnMouseEnter, FMetadata::None };
		};
		struct Method___void___OnMouseLeave__FMouseEventArgs_lref
		{
			static inline const TMethod<UThumb, void(UI::FMouseEventArgs &)> Method { u8"OnMouseLeave"N, &UThumb::OnMouseLeave, FMetadata::None };
		};
		struct Method___void___OnMouseDown__FMouseButtonEventArgs_lref
		{
			static inline const TMethod<UThumb, void(UI::FMouseButtonEventArgs &)> Method { u8"OnMouseDown"N, &UThumb::OnMouseDown, FMetadata::None };
		};
		struct Method___void___OnMouseUp__FMouseButtonEventArgs_lref
		{
			static inline const TMethod<UThumb, void(UI::FMouseButtonEventArgs &)> Method { u8"OnMouseUp"N, &UThumb::OnMouseUp, FMetadata::None };
		};
		struct Method___void___OnMouseMove__FMouseEventArgs_lref
		{
			static inline const TMethod<UThumb, void(UI::FMouseEventArgs &)> Method { u8"OnMouseMove"N, &UThumb::OnMouseMove, FMetadata::None };
		};
		struct Method___void___OnMouseCapturedChanged__bool
		{
			static inline const TMethod<UThumb, void(bool)> Method { u8"OnMouseCapturedChanged"N, &UThumb::OnMouseCapturedChanged, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___void___OnConstruct::Method,
			Method___void___OnPaint__IPainter_lref::Method,
			Method___void___OnMouseEnter__FMouseEventArgs_lref::Method,
			Method___void___OnMouseLeave__FMouseEventArgs_lref::Method,
			Method___void___OnMouseDown__FMouseButtonEventArgs_lref::Method,
			Method___void___OnMouseUp__FMouseButtonEventArgs_lref::Method,
			Method___void___OnMouseMove__FMouseEventArgs_lref::Method,
			Method___void___OnMouseCapturedChanged__bool::Method,
		};

		// --- 2 Fields

		// --- 3 Properties

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<UI::UControl>(),
		};

		// --- 5 Constructor
		UThumb_Type(FName Name = u8"UThumb"N) : TClass(Name, Classof<UI::UControl>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Methods = ReflectedMethods;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	template<> struct XIN_EXPORT TStaticType<UI::UThumb>
	{
		static inline UThumb_Type UThumbType { u8"UThumb"N };

		static FType & Type()
		{
			return UThumbType;
		}
	};

	FType & UI::UThumb::StaticType()
	{
		return TStaticType<UI::UThumb>().Type();
	}

	static TTypeRegister<UI::UThumb> UThumbTypeRegister { TStaticType<UI::UThumb>().Type() };
}

// Generated code end.
