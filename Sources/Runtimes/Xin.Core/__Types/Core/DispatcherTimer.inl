// 9A884AFEB7BCA2A7DAF470CD7A7D6DAA 2025-03-06 15:45:39
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Core/DispatcherTimer.h"

namespace Xin
{
	class FDispatcherTimer_Type : public TClass<FDispatcherTimer>
	{
	public:
		// --- 0 Constructors
		static inline const TConstructor<FDispatcherTimer> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TConstructor<FDispatcherTimer, uint32> Constructor___uint32 { u8"!Constructor___uint32"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
			Constructor___uint32,
		};

		// --- 1 Methods
		struct Method___uint32___Start
		{
			static inline const TMethod<FDispatcherTimer, uint32()> Method { u8"Start"N, &FDispatcherTimer::Start, FMetadata::None };
		};
		struct Method___uint32___Stop
		{
			static inline const TMethod<FDispatcherTimer, uint32()> Method { u8"Stop"N, &FDispatcherTimer::Stop, FMetadata::None };
		};
		struct Method___bool___IsTicking____const
		{
			static inline const TMethod<FDispatcherTimer, bool()> Method { u8"IsTicking"N, &FDispatcherTimer::IsTicking, FMetadata::None };
		};
		struct Method___void___OnIntervalChanged__uint32
		{
			static inline const TMethod<FDispatcherTimer, void(uint32)> Method { u8"OnIntervalChanged"N, &FDispatcherTimer::OnIntervalChanged, FMetadata::None };
		};
		struct Method___void___OnTimer
		{
			static inline const TMethod<FDispatcherTimer, void()> Method { u8"OnTimer"N, &FDispatcherTimer::OnTimer, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___uint32___Start::Method,
			Method___uint32___Stop::Method,
			Method___bool___IsTicking____const::Method,
			Method___void___OnIntervalChanged__uint32::Method,
			Method___void___OnTimer::Method,
		};

		// --- 2 Fields
		struct Field___StartTime
		{
			static inline const TField Field { u8"StartTime"N, &FDispatcherTimer::StartTime, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FField> ReflectedFields[] =
		{
			Field___StartTime::Field,
		};

		// --- 3 Properties
		struct Property___Ticking
		{
			static inline const TFunctorProperty Property { u8"Ticking"N, &FDispatcherTimer::IsTicking, nullptr, FMetadata::None };
		};
		struct Property___Interval
		{
			static inline const TProxyProperty Property { u8"Interval"N, &FDispatcherTimer::__Interval, &FDispatcherTimer::__GetInterval, &FDispatcherTimer::__SetInterval, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FProperty> ReflectedProperties[] =
		{
			Property___Ticking::Property,
			Property___Interval::Property,
		};

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<IObject>(),
		};

		// --- 5 Constructor
		FDispatcherTimer_Type(FName Name = u8"FDispatcherTimer"N) : TClass(Name, Classof<IObject>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Fields = ReflectedFields;
			FClass::Methods = ReflectedMethods;
			FClass::Properties = ReflectedProperties;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	const TProperty<uint32> & FDispatcherTimer::IntervalProperty() { return FDispatcherTimer_Type::Property___Interval::Property; }

	template<> struct XIN_EXPORT TStaticType<FDispatcherTimer>
	{
		static inline FDispatcherTimer_Type FDispatcherTimerType { u8"FDispatcherTimer"N };

		static FType & Type()
		{
			return FDispatcherTimerType;
		}
	};

	template<> XIN_EXPORT inline FType & Typeof<FDispatcherTimer>()
	{
		return TStaticType<FDispatcherTimer>().Type();
	}

	static TTypeRegister<FDispatcherTimer> FDispatcherTimerTypeRegister { TStaticType<FDispatcherTimer>().Type() };
}

// Generated code end.
