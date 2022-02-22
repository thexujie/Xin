// B252AD60B8E3F9B623DAF84B870F7B90 2025-03-24 18:07:20
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Core/ScrollInfo.h"

namespace Xin
{
	class IScrollInfo_Type : public TClass<UI::IScrollInfo>
	{
	public:
		using IScrollInfo = UI::IScrollInfo;

		// --- 0 Constructors
		static inline const TConstructor<IScrollInfo> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
		};

		// --- 1 Methods
		struct Method___void___SetViewport__TVec2__float64____TVec2__float64__
		{
			static inline const TMethod<IScrollInfo, void(FSize, FSize)> Method { u8"SetViewport"N, &IScrollInfo::SetViewport, FMetadata::None };
		};
		struct Method___void___Scroll__TVec2__float64__
		{
			static inline const TMethod<IScrollInfo, void(FVec2)> Method { u8"Scroll"N, &IScrollInfo::Scroll, FMetadata::None };
		};
		struct Method___void___LineUp
		{
			static inline const TMethod<IScrollInfo, void()> Method { u8"LineUp"N, &IScrollInfo::LineUp, FMetadata::None };
		};
		struct Method___void___LineDown
		{
			static inline const TMethod<IScrollInfo, void()> Method { u8"LineDown"N, &IScrollInfo::LineDown, FMetadata::None };
		};
		struct Method___void___PageUp
		{
			static inline const TMethod<IScrollInfo, void()> Method { u8"PageUp"N, &IScrollInfo::PageUp, FMetadata::None };
		};
		struct Method___void___PageDown
		{
			static inline const TMethod<IScrollInfo, void()> Method { u8"PageDown"N, &IScrollInfo::PageDown, FMetadata::None };
		};
		struct Method___void___PageLeft
		{
			static inline const TMethod<IScrollInfo, void()> Method { u8"PageLeft"N, &IScrollInfo::PageLeft, FMetadata::None };
		};
		struct Method___void___PageRight
		{
			static inline const TMethod<IScrollInfo, void()> Method { u8"PageRight"N, &IScrollInfo::PageRight, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___void___SetViewport__TVec2__float64____TVec2__float64__::Method,
			Method___void___Scroll__TVec2__float64__::Method,
			Method___void___LineUp::Method,
			Method___void___LineDown::Method,
			Method___void___PageUp::Method,
			Method___void___PageDown::Method,
			Method___void___PageLeft::Method,
			Method___void___PageRight::Method,
		};

		// --- 2 Fields
		struct Field___ViewportSize
		{
			static inline const TField Field { u8"ViewportSize"N, &IScrollInfo::ViewportSize, FMetadata::None };
		};
		struct Field___ExtentSize
		{
			static inline const TField Field { u8"ExtentSize"N, &IScrollInfo::ExtentSize, FMetadata::None };
		};
		struct Field___ScrollOffset
		{
			static inline const TField Field { u8"ScrollOffset"N, &IScrollInfo::ScrollOffset, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FField> ReflectedFields[] =
		{
			Field___ViewportSize::Field,
			Field___ExtentSize::Field,
			Field___ScrollOffset::Field,
		};

		// --- 3 Properties

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<IObject>(),
		};

		// --- 5 Constructor
		IScrollInfo_Type(FName Name = u8"IScrollInfo"N) : TClass(Name, Classof<IObject>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Fields = ReflectedFields;
			FClass::Methods = ReflectedMethods;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	template<> struct XIN_EXPORT TStaticType<UI::IScrollInfo>
	{
		static inline IScrollInfo_Type IScrollInfoType { u8"IScrollInfo"N };

		static FType & Type()
		{
			return IScrollInfoType;
		}
	};

	template<> XIN_EXPORT inline FType & Typeof<UI::IScrollInfo>()
	{
		return TStaticType<UI::IScrollInfo>().Type();
	}

	static TTypeRegister<UI::IScrollInfo> IScrollInfoTypeRegister { TStaticType<UI::IScrollInfo>().Type() };
}

// Generated code end.
