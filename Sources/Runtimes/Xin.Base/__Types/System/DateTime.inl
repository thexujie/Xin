// F512E8FBF4C7F402767D7FBD38A4C899 2024-12-25 11:51:57
// Generated code begin.

#pragma once
#include "PCH.h"
#include "System/DateTime.h"

namespace Xin
{
	class FDateTime_Type : public TClass<FDateTime>
	{
	public:
		// --- 0 Constructors
		static inline const TConstructor<FDateTime> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TConstructor<FDateTime, FDateTime const &> Constructor___FDateTime_const_lref { u8"!Constructor___FDateTime_const_lref"N, FMetadata::None };
		static inline const TConstructor<FDateTime, uint64> Constructor___uint64 { u8"!Constructor___uint64"N, FMetadata::None };
		static inline const TConstructor<FDateTime, uint32, uint32, uint32, uint32, uint32, uint32, uint32, uint32, uint32> Constructor___uint32__uint32__uint32__uint32__uint32__uint32__uint32__uint32__uint32 { u8"!Constructor___uint32__uint32__uint32__uint32__uint32__uint32__uint32__uint32__uint32"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
			Constructor___FDateTime_const_lref,
			Constructor___uint64,
			Constructor___uint32__uint32__uint32__uint32__uint32__uint32__uint32__uint32__uint32,
		};

		// --- 1 Methods
		struct Method___TView__char8_____Format__TView__char8______const
		{
			static inline const TMethod<FDateTime, FStringV(FStringV)> Method { u8"Format"N, &FDateTime::Format, FMetadata::None };
		};
		struct Method___uint64___GetNanoseconds____const
		{
			static inline const TMethod<FDateTime, uint64()> Method { u8"GetNanoseconds"N, &FDateTime::GetNanoseconds, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___TView__char8_____Format__TView__char8______const::Method,
			Method___uint64___GetNanoseconds____const::Method,
		};

		// --- 2 Fields
		struct Field___Year
		{
			static inline const TField Field { u8"Year"N, &FDateTime::Year, FMetadata::None };
		};
		struct Field___Month
		{
			static inline const TField Field { u8"Month"N, &FDateTime::Month, FMetadata::None };
		};
		struct Field___Day
		{
			static inline const TField Field { u8"Day"N, &FDateTime::Day, FMetadata::None };
		};
		struct Field___Hour
		{
			static inline const TField Field { u8"Hour"N, &FDateTime::Hour, FMetadata::None };
		};
		struct Field___Minute
		{
			static inline const TField Field { u8"Minute"N, &FDateTime::Minute, FMetadata::None };
		};
		struct Field___Second
		{
			static inline const TField Field { u8"Second"N, &FDateTime::Second, FMetadata::None };
		};
		struct Field___Millisecond
		{
			static inline const TField Field { u8"Millisecond"N, &FDateTime::Millisecond, FMetadata::None };
		};
		struct Field___Microsecond
		{
			static inline const TField Field { u8"Microsecond"N, &FDateTime::Microsecond, FMetadata::None };
		};
		struct Field___Nanosecond
		{
			static inline const TField Field { u8"Nanosecond"N, &FDateTime::Nanosecond, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FField> ReflectedFields[] =
		{
			Field___Year::Field,
			Field___Month::Field,
			Field___Day::Field,
			Field___Hour::Field,
			Field___Minute::Field,
			Field___Second::Field,
			Field___Millisecond::Field,
			Field___Microsecond::Field,
			Field___Nanosecond::Field,
		};

		// --- 3 Properties

		// --- 4 Base Types

		// --- 5 Constructor
		FDateTime_Type(FName Name = u8"FDateTime"N) : TClass(Name, FClass::None)
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Fields = ReflectedFields;
			FClass::Methods = ReflectedMethods;
		}
	};

	template<> struct XIN_EXPORT TStaticType<FDateTime>
	{
		static FType & Type()
		{
			static FDateTime_Type FDateTimeType { u8"FDateTime"N };
			return FDateTimeType;
		}
	};

	FType & FDateTime::StaticType()
	{
		return TStaticType<FDateTime>().Type();
	}

	static TTypeRegister<FDateTime> FDateTimeTypeRegister { TStaticType<FDateTime>().Type() };
}

// Generated code end.
