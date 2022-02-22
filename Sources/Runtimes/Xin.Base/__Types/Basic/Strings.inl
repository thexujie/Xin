// 42AAC695DD817BE4B91554B69DFCE159 2025-02-14 16:16:06
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Basic/Strings.h"

namespace Xin
{
	template<typename T, typename AllocatorT>
	class TString_Type : public TClass<TString<T, AllocatorT>>
	{
	public:
		using TString = TString<T, AllocatorT>;

		// --- 0 Constructors
		static inline const TConstructor<TString> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TConstructor<TString, uintx> Constructor___uintx { u8"!Constructor___uintx"N, FMetadata::None };
		static inline const TConstructor<TString, TString &&> Constructor___TString_ref { u8"!Constructor___TString_ref"N, FMetadata::None };
		static inline const TConstructor<TString, TString const &> Constructor___TString_const_lref { u8"!Constructor___TString_const_lref"N, FMetadata::None };
		static inline const TConstructor<TString, typename TString::ViewType> Constructor___TView__T__ { u8"!Constructor___TView__T__"N, FMetadata::None };
		static inline const TConstructor<TString, T const *, uintx> Constructor___T_const_ptr__uintx { u8"!Constructor___T_const_ptr__uintx"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
			Constructor___uintx,
			Constructor___TString_ref,
			Constructor___TString_const_lref,
			Constructor___TView__T__,
			Constructor___T_const_ptr__uintx,
		};

		// --- 1 Methods
		struct Method___uintx___GetSize____const
		{
			static inline const TMethod<TString, uintx()> Method { u8"GetSize"N, &TString::GetSize, FMetadata::None };
		};
		struct Method___uintx___GetCapacity____const
		{
			static inline const TMethod<TString, uintx()> Method { u8"GetCapacity"N, &TString::GetCapacity, FMetadata::None };
		};
		struct Method___uintx___GetCapacityWithoutNull____const
		{
			static inline const TMethod<TString, uintx()> Method { u8"GetCapacityWithoutNull"N, &TString::GetCapacityWithoutNull, FMetadata::None };
		};
		struct Method___T_ptr___GetData____const
		{
			static inline const TMethod<TString, T *()> Method { u8"GetData"N, &TString::GetData, FMetadata::None };
		};
		struct Method___void___Append__T
		{
			static inline const TMethod<TString, void(T)> Method { u8"Append"N, &TString::Append, FMetadata::None };
		};
		struct Method___void___Append__T_const_ptr__uintx
		{
			static inline const TMethod<TString, void(T const *, uintx)> Method { u8"Append"N, &TString::Append, FMetadata::None };
		};
		struct Method___void___Append__TView__T__
		{
			static inline const TMethod<TString, void(typename TString::ViewType)> Method { u8"Append"N, &TString::Append, FMetadata::None };
		};
		struct Method___void___Insert__uintx__TView__T__
		{
			static inline const TMethod<TString, void(uintx, typename TString::ViewType)> Method { u8"Insert"N, &TString::Insert, FMetadata::None };
		};
		struct Method___void___Erase__uintx__uintx
		{
			static inline const TMethod<TString, void(uintx, uintx)> Method { u8"Erase"N, &TString::Erase, FMetadata::None };
		};
		struct Method___bool___Empty____const
		{
			static inline const TMethod<TString, bool()> Method { u8"Empty"N, &TString::Empty, FMetadata::None };
		};
		struct Method___bool___Valid____const
		{
			static inline const TMethod<TString, bool()> Method { u8"Valid"N, &TString::Valid, FMetadata::None };
		};
		struct Method___bool___ValidIndex__uintx____const
		{
			static inline const TMethod<TString, bool(uintx)> Method { u8"ValidIndex"N, &TString::ValidIndex, FMetadata::None };
		};
		struct Method___TView__T_____View__uintx__uintx____const
		{
			static inline const TMethod<TString, typename TString::ViewType(uintx, uintx)> Method { u8"View"N, &TString::View, FMetadata::None };
		};
		struct Method___TSlice__T_____Slice__uintx__uintx____const
		{
			static inline const TMethod<TString, typename TString::SliceType(uintx, uintx)> Method { u8"Slice"N, &TString::Slice, FMetadata::None };
		};
		struct Method___void___AddN__uintx
		{
			static inline const TMethod<TString, void(uintx)> Method { u8"AddN"N, &TString::AddN, FMetadata::None };
		};
		struct Method___void___AddN__uintx__T_const_lref
		{
			static inline const TMethod<TString, void(uintx, T const &)> Method { u8"AddN"N, &TString::AddN, FMetadata::None };
		};
		struct Method___void___Resize__uintx__uintx
		{
			static inline const TMethod<TString, void(uintx, uintx)> Method { u8"Resize"N, &TString::Resize, FMetadata::None };
		};
		struct Method___void___Reserve__uintx
		{
			static inline const TMethod<TString, void(uintx)> Method { u8"Reserve"N, &TString::Reserve, FMetadata::None };
		};
		struct Method___void___Clear
		{
			static inline const TMethod<TString, void()> Method { u8"Clear"N, &TString::Clear, FMetadata::None };
		};
		struct Method___void___Reset
		{
			static inline const TMethod<TString, void()> Method { u8"Reset"N, &TString::Reset, FMetadata::None };
		};
		struct Method___TString_lref___Remove__TView__T__
		{
			static inline const TMethod<TString, TString &(typename TString::ViewType)> Method { u8"Remove"N, &TString::Remove, FMetadata::None };
		};
		struct Method___TString_lref___Replace__TView__T____TView__T__
		{
			static inline const TMethod<TString, TString &(typename TString::ViewType, typename TString::ViewType)> Method { u8"Replace"N, &TString::Replace, FMetadata::None };
		};
		struct Method___TString_lref___ReplaceAt__uintx__uintx__TView__T__
		{
			static inline const TMethod<TString, TString &(uintx, uintx, typename TString::ViewType)> Method { u8"ReplaceAt"N, &TString::ReplaceAt, FMetadata::None };
		};
		struct Method___TString_lref___SetAt__uintx__TView__T__
		{
			static inline const TMethod<TString, TString &(uintx, typename TString::ViewType)> Method { u8"SetAt"N, &TString::SetAt, FMetadata::None };
		};
		struct Method___TString_lref___RemoveAt__uintx__uintx
		{
			static inline const TMethod<TString, TString &(uintx, uintx)> Method { u8"RemoveAt"N, &TString::RemoveAt, FMetadata::None };
		};
		struct Method___TString_lref___TrimStart__TView__T__
		{
			static inline const TMethod<TString, TString &(TView<T>)> Method { u8"TrimStart"N, &TString::TrimStart, FMetadata::None };
		};
		struct Method___TString_lref___TrimEnd__TView__T__
		{
			static inline const TMethod<TString, TString &(TView<T>)> Method { u8"TrimEnd"N, &TString::TrimEnd, FMetadata::None };
		};
		struct Method___TString_lref___Trim__TView__T__
		{
			static inline const TMethod<TString, TString &(TView<T>)> Method { u8"Trim"N, &TString::Trim, FMetadata::None };
		};
		struct Method___TString_lref___ToUpper
		{
			static inline const TMethod<TString, TString &()> Method { u8"ToUpper"N, &TString::ToUpper, FMetadata::None };
		};
		struct Method___TString_lref___ToLower
		{
			static inline const TMethod<TString, TString &()> Method { u8"ToLower"N, &TString::ToLower, FMetadata::None };
		};
		struct Method___uintx___FindFirst__T_const_lref____const
		{
			static inline const TMethod<TString, uintx(T const &)> Method { u8"FindFirst"N, &TString::FindFirst, FMetadata::None };
		};
		struct Method___uintx___FindFirst__TView__T______const
		{
			static inline const TMethod<TString, uintx(typename TString::ViewType)> Method { u8"FindFirst"N, &TString::FindFirst, FMetadata::None };
		};
		struct Method___uintx___FindFirstOf__TView__T______const
		{
			static inline const TMethod<TString, uintx(typename TString::ViewType)> Method { u8"FindFirstOf"N, &TString::FindFirstOf, FMetadata::None };
		};
		struct Method___uintx___FindLast__T_const_lref____const
		{
			static inline const TMethod<TString, uintx(T const &)> Method { u8"FindLast"N, &TString::FindLast, FMetadata::None };
		};
		struct Method___uintx___FindLast__TView__T______const
		{
			static inline const TMethod<TString, uintx(typename TString::ViewType)> Method { u8"FindLast"N, &TString::FindLast, FMetadata::None };
		};
		struct Method___uintx___FindLastOf__TView__T______const
		{
			static inline const TMethod<TString, uintx(typename TString::ViewType)> Method { u8"FindLastOf"N, &TString::FindLastOf, FMetadata::None };
		};
		struct Method___bool___Contains__TView__T______const
		{
			static inline const TMethod<TString, bool(typename TString::ViewType)> Method { u8"Contains"N, &TString::Contains, FMetadata::None };
		};
		struct Method___bool___StartsWith__TView__T______const
		{
			static inline const TMethod<TString, bool(typename TString::ViewType)> Method { u8"StartsWith"N, &TString::StartsWith, FMetadata::None };
		};
		struct Method___bool___EndsWith__TView__T______const
		{
			static inline const TMethod<TString, bool(typename TString::ViewType)> Method { u8"EndsWith"N, &TString::EndsWith, FMetadata::None };
		};
		struct Method___void___FinishNull
		{
			static inline const TMethod<TString, void()> Method { u8"FinishNull"N, &TString::FinishNull, FMetadata::None };
		};
		struct Method___TView__T_____ToString____const
		{
			static inline const TMethod<TString, typename TString::ViewType()> Method { u8"ToString"N, &TString::ToString, FMetadata::None };
		};
		struct Method___void___SetSize__uintx
		{
			static inline const TMethod<TString, void(uintx)> Method { u8"SetSize"N, &TString::SetSize, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___uintx___GetSize____const::Method,
			Method___uintx___GetCapacity____const::Method,
			Method___uintx___GetCapacityWithoutNull____const::Method,
			Method___T_ptr___GetData____const::Method,
			Method___void___Append__T::Method,
			Method___void___Append__T_const_ptr__uintx::Method,
			Method___void___Append__TView__T__::Method,
			Method___void___Insert__uintx__TView__T__::Method,
			Method___void___Erase__uintx__uintx::Method,
			Method___bool___Empty____const::Method,
			Method___bool___Valid____const::Method,
			Method___bool___ValidIndex__uintx____const::Method,
			Method___TView__T_____View__uintx__uintx____const::Method,
			Method___TSlice__T_____Slice__uintx__uintx____const::Method,
			Method___void___AddN__uintx::Method,
			Method___void___AddN__uintx__T_const_lref::Method,
			Method___void___Resize__uintx__uintx::Method,
			Method___void___Reserve__uintx::Method,
			Method___void___Clear::Method,
			Method___void___Reset::Method,
			Method___TString_lref___Remove__TView__T__::Method,
			Method___TString_lref___Replace__TView__T____TView__T__::Method,
			Method___TString_lref___ReplaceAt__uintx__uintx__TView__T__::Method,
			Method___TString_lref___SetAt__uintx__TView__T__::Method,
			Method___TString_lref___RemoveAt__uintx__uintx::Method,
			Method___TString_lref___TrimStart__TView__T__::Method,
			Method___TString_lref___TrimEnd__TView__T__::Method,
			Method___TString_lref___Trim__TView__T__::Method,
			Method___TString_lref___ToUpper::Method,
			Method___TString_lref___ToLower::Method,
			Method___uintx___FindFirst__T_const_lref____const::Method,
			Method___uintx___FindFirst__TView__T______const::Method,
			Method___uintx___FindFirstOf__TView__T______const::Method,
			Method___uintx___FindLast__T_const_lref____const::Method,
			Method___uintx___FindLast__TView__T______const::Method,
			Method___uintx___FindLastOf__TView__T______const::Method,
			Method___bool___Contains__TView__T______const::Method,
			Method___bool___StartsWith__TView__T______const::Method,
			Method___bool___EndsWith__TView__T______const::Method,
			Method___void___FinishNull::Method,
			Method___TView__T_____ToString____const::Method,
			Method___void___SetSize__uintx::Method,
		};

		// --- 2 Fields
		struct Field___Length
		{
			static inline const TField Field { u8"Length"N, &TString::Length, FMetadata::None };
		};
		struct Field___Capacity
		{
			static inline const TField Field { u8"Capacity"N, &TString::Capacity, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FField> ReflectedFields[] =
		{
			Field___Length::Field,
			Field___Capacity::Field,
		};

		// --- 3 Properties

		// --- 4 Base Types

		// --- 5 Constructor
		TString_Type(FName Name = u8"TString"N) : TClass<TString>(Name, FClass::None)
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Fields = ReflectedFields;
			FClass::Methods = ReflectedMethods;
		}
	};

	template<typename T, typename AllocatorT>
	struct TStaticType<TString<T, AllocatorT>>
	{
		static FType & Type()
		{
			static TString_Type<T, AllocatorT> TStringType { u8"TString"N };
			return TStringType;
		}
	};
}

// Generated code end.
