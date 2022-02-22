// 45FEB612ACD3B4472B81D3685189940C 2024-12-25 11:51:17
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Basic/Thickness.h"

namespace Xin
{
	template<typename T>
	class TThickness_Type : public TClass<TThickness<T>>
	{
	public:
		using TThickness = TThickness<T>;

		// --- 0 Constructors
		static inline const TConstructor<TThickness> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TConstructor<TThickness, TThickness const &> Constructor___TThickness_const_lref { u8"!Constructor___TThickness_const_lref"N, FMetadata::None };
		static inline const TConstructor<TThickness, T, T> Constructor___T__T { u8"!Constructor___T__T"N, FMetadata::None };
		static inline const TConstructor<TThickness, T, T, T, T> Constructor___T__T__T__T { u8"!Constructor___T__T__T__T"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
			Constructor___TThickness_const_lref,
			Constructor___T__T,
			Constructor___T__T__T__T,
		};

		// --- 1 Methods
		struct Method___void___Clear
		{
			static inline const TMethod<TThickness, void()> Method { u8"Clear"N, &TThickness::Clear, FMetadata::None };
		};
		struct Method___bool___Uniform____const
		{
			static inline const TMethod<TThickness, bool()> Method { u8"Uniform"N, &TThickness::Uniform, FMetadata::None };
		};
		struct Method___bool___AnyZero____const
		{
			static inline const TMethod<TThickness, bool()> Method { u8"AnyZero"N, &TThickness::AnyZero, FMetadata::None };
		};
		struct Method___bool___AllZero____const
		{
			static inline const TMethod<TThickness, bool()> Method { u8"AllZero"N, &TThickness::AllZero, FMetadata::None };
		};
		struct Method___bool___EmptyRect____const
		{
			static inline const TMethod<TThickness, bool()> Method { u8"EmptyRect"N, &TThickness::EmptyRect, FMetadata::None };
		};
		struct Method___TVec2__T___const_lref___GetLeftTop____const
		{
			static inline const TMethod<TThickness, TVec2<T> const &()> Method { u8"GetLeftTop"N, &TThickness::GetLeftTop, FMetadata::None };
		};
		struct Method___void___SetLeftTop__TVec2__T___const_lref
		{
			static inline const TMethod<TThickness, void(TVec2<T> const &)> Method { u8"SetLeftTop"N, &TThickness::SetLeftTop, FMetadata::None };
		};
		struct Method___TVec2__T___const_lref___GetRightBottom____const
		{
			static inline const TMethod<TThickness, TVec2<T> const &()> Method { u8"GetRightBottom"N, &TThickness::GetRightBottom, FMetadata::None };
		};
		struct Method___void___SetRightBottom__TVec2__T___const_lref
		{
			static inline const TMethod<TThickness, void(TVec2<T> const &)> Method { u8"SetRightBottom"N, &TThickness::SetRightBottom, FMetadata::None };
		};
		struct Method___T___GetWidth____const
		{
			static inline const TMethod<TThickness, T()> Method { u8"GetWidth"N, &TThickness::GetWidth, FMetadata::None };
		};
		struct Method___T___GetHeight____const
		{
			static inline const TMethod<TThickness, T()> Method { u8"GetHeight"N, &TThickness::GetHeight, FMetadata::None };
		};
		struct Method___TVec2__T_____GetSize____const
		{
			static inline const TMethod<TThickness, TVec2<T>()> Method { u8"GetSize"N, &TThickness::GetSize, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___void___Clear::Method,
			Method___bool___Uniform____const::Method,
			Method___bool___AnyZero____const::Method,
			Method___bool___AllZero____const::Method,
			Method___bool___EmptyRect____const::Method,
			Method___TVec2__T___const_lref___GetLeftTop____const::Method,
			Method___void___SetLeftTop__TVec2__T___const_lref::Method,
			Method___TVec2__T___const_lref___GetRightBottom____const::Method,
			Method___void___SetRightBottom__TVec2__T___const_lref::Method,
			Method___T___GetWidth____const::Method,
			Method___T___GetHeight____const::Method,
			Method___TVec2__T_____GetSize____const::Method,
		};

		// --- 2 Fields
		struct Field___Left
		{
			static inline const TField Field { u8"Left"N, &TThickness::Left, FMetadata::None };
		};
		struct Field___Top
		{
			static inline const TField Field { u8"Top"N, &TThickness::Top, FMetadata::None };
		};
		struct Field___Right
		{
			static inline const TField Field { u8"Right"N, &TThickness::Right, FMetadata::None };
		};
		struct Field___Bottom
		{
			static inline const TField Field { u8"Bottom"N, &TThickness::Bottom, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FField> ReflectedFields[] =
		{
			Field___Left::Field,
			Field___Top::Field,
			Field___Right::Field,
			Field___Bottom::Field,
		};

		// --- 3 Properties
		struct Property___LeftTop
		{
			static inline const TFunctorProperty Property { u8"LeftTop"N, &TThickness::GetLeftTop, &TThickness::SetLeftTop, FMetadata::None };
		};
		struct Property___RightBottom
		{
			static inline const TFunctorProperty Property { u8"RightBottom"N, &TThickness::GetRightBottom, &TThickness::SetRightBottom, FMetadata::None };
		};
		struct Property___Width
		{
			static inline const TFunctorProperty Property { u8"Width"N, &TThickness::GetWidth, nullptr, FMetadata::None };
		};
		struct Property___Height
		{
			static inline const TFunctorProperty Property { u8"Height"N, &TThickness::GetHeight, nullptr, FMetadata::None };
		};
		struct Property___Size
		{
			static inline const TFunctorProperty Property { u8"Size"N, &TThickness::GetSize, nullptr, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FProperty> ReflectedProperties[] =
		{
			Property___LeftTop::Property,
			Property___RightBottom::Property,
			Property___Width::Property,
			Property___Height::Property,
			Property___Size::Property,
		};

		// --- 4 Base Types

		// --- 5 Constructor
		TThickness_Type(FName Name = u8"TThickness"N) : TClass<TThickness>(Name, FClass::None)
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Fields = ReflectedFields;
			FClass::Methods = ReflectedMethods;
			FClass::Properties = ReflectedProperties;
		}
	};

	template<typename T>
	struct TStaticType<TThickness<T>>
	{
		static FType & Type()
		{
			static TThickness_Type<T> TThicknessType { u8"TThickness"N };
			return TThicknessType;
		}
	};
}

// Generated code end.
