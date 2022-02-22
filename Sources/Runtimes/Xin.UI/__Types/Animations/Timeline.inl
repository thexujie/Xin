// DAA4E9862C8E34AA0D3B5DDABB73E26D 2025-03-06 15:48:12
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Animations/Timeline.h"

namespace Xin
{
	class FTimeline_Type : public TClass<UI::FTimeline>
	{
	public:
		using FTimeline = UI::FTimeline;

		// --- 0 Constructors

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<FTimeline, FType &()> Method { u8"Type"N, &FTimeline::Type, FMetadata::None };
		};
		struct Method___void___Activate__IDependency_lref
		{
			static inline const TMethod<FTimeline, void(IDependency &)> Method { u8"Activate"N, &FTimeline::Activate, FMetadata::None };
		};
		struct Method___void___Begin__IDependency_lref
		{
			static inline const TMethod<FTimeline, void(IDependency &)> Method { u8"Begin"N, &FTimeline::Begin, FMetadata::None };
		};
		struct Method___void___End__IDependency_lref
		{
			static inline const TMethod<FTimeline, void(IDependency &)> Method { u8"End"N, &FTimeline::End, FMetadata::None };
		};
		struct Method___void___Inactivate__IDependency_lref
		{
			static inline const TMethod<FTimeline, void(IDependency &)> Method { u8"Inactivate"N, &FTimeline::Inactivate, FMetadata::None };
		};
		struct Method___void___ApplyTime__IDependency_lref__FTimelineTime_const_lref
		{
			static inline const TMethod<FTimeline, void(IDependency &, UI::FTimelineTime const &)> Method { u8"ApplyTime"N, &FTimeline::ApplyTime, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___void___Activate__IDependency_lref::Method,
			Method___void___Begin__IDependency_lref::Method,
			Method___void___End__IDependency_lref::Method,
			Method___void___Inactivate__IDependency_lref::Method,
			Method___void___ApplyTime__IDependency_lref__FTimelineTime_const_lref::Method,
		};

		// --- 2 Fields
		struct Field___Activated
		{
			static inline const TField Field { u8"Activated"N, &FTimeline::Activated, FMetadata::None };
		};
		struct Field___Begun
		{
			static inline const TField Field { u8"Begun"N, &FTimeline::Begun, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FField> ReflectedFields[] =
		{
			Field___Activated::Field,
			Field___Begun::Field,
		};

		// --- 3 Properties
		struct Property___Start
		{
			static inline const TProxyProperty Property { u8"Start"N, &FTimeline::__Start, &FTimeline::__GetStart, &FTimeline::__SetStart, FMetadata::None };
		};
		struct Property___Duration
		{
			static inline const TProxyProperty Property { u8"Duration"N, &FTimeline::__Duration, &FTimeline::__GetDuration, &FTimeline::__SetDuration, FMetadata::None };
		};
		struct Property___AutoReverse
		{
			static inline const TProxyProperty Property { u8"AutoReverse"N, &FTimeline::__AutoReverse, &FTimeline::__GetAutoReverse, &FTimeline::__SetAutoReverse, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FProperty> ReflectedProperties[] =
		{
			Property___Start::Property,
			Property___Duration::Property,
			Property___AutoReverse::Property,
		};

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<IAnimatable>(),
		};

		// --- 5 Constructor
		FTimeline_Type(FName Name = u8"FTimeline"N) : TClass(Name, Classof<IAnimatable>())
		{
			FClass::Fields = ReflectedFields;
			FClass::Methods = ReflectedMethods;
			FClass::Properties = ReflectedProperties;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	const TProperty<uint32> & UI::FTimeline::StartProperty() { return FTimeline_Type::Property___Start::Property; }

	const TProperty<uint32> & UI::FTimeline::DurationProperty() { return FTimeline_Type::Property___Duration::Property; }

	const TProperty<bool> & UI::FTimeline::AutoReverseProperty() { return FTimeline_Type::Property___AutoReverse::Property; }

	template<> struct XIN_EXPORT TStaticType<UI::FTimeline>
	{
		static inline FTimeline_Type FTimelineType { u8"FTimeline"N };

		static FType & Type()
		{
			return FTimelineType;
		}
	};

	FType & UI::FTimeline::StaticType()
	{
		return TStaticType<UI::FTimeline>().Type();
	}

	static TTypeRegister<UI::FTimeline> FTimelineTypeRegister { TStaticType<UI::FTimeline>().Type() };
}

// Generated code end.
