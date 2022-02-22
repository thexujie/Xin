// 289C472E61ECD65287A64F6E23F3EA6D 2025-03-06 15:48:11
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Animations/Storyboard.h"

namespace Xin
{
	class FStoryboard_Type : public TClass<UI::FStoryboard>
	{
	public:
		using FStoryboard = UI::FStoryboard;

		// --- 0 Constructors
		static inline const TConstructor<FStoryboard, IDependency &> Constructor___IDependency_lref { u8"!Constructor___IDependency_lref"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor___IDependency_lref,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<FStoryboard, FType &()> Method { u8"Type"N, &FStoryboard::Type, FMetadata::None };
		};
		struct Method___void___Play
		{
			static inline const TMethod<FStoryboard, void()> Method { u8"Play"N, &FStoryboard::Play, FMetadata::None };
		};
		struct Method___void___Stop
		{
			static inline const TMethod<FStoryboard, void()> Method { u8"Stop"N, &FStoryboard::Stop, FMetadata::None };
		};
		struct Method___void___OnTimer
		{
			static inline const TMethod<FStoryboard, void()> Method { u8"OnTimer"N, &FStoryboard::OnTimer, FMetadata::None };
		};
		struct Method___void___ApplyTime__IDependency_lref__FTimelineTime_const_lref
		{
			static inline const TMethod<FStoryboard, void(IDependency &, UI::FTimelineTime const &)> Method { u8"ApplyTime"N, &FStoryboard::ApplyTime, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___void___Play::Method,
			Method___void___Stop::Method,
			Method___void___OnTimer::Method,
			Method___void___ApplyTime__IDependency_lref__FTimelineTime_const_lref::Method,
		};

		// --- 2 Fields
		struct Field___Timelines
		{
			static inline const TField Field { u8"Timelines"N, &FStoryboard::Timelines, FMetadata::None };
		};
		struct Field___StartTime
		{
			static inline const TField Field { u8"StartTime"N, &FStoryboard::StartTime, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FField> ReflectedFields[] =
		{
			Field___Timelines::Field,
			Field___StartTime::Field,
		};

		// --- 3 Properties

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<UI::FTimeline>(),
		};

		// --- 5 Constructor
		FStoryboard_Type(FName Name = u8"FStoryboard"N) : TClass(Name, Classof<UI::FTimeline>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Fields = ReflectedFields;
			FClass::Methods = ReflectedMethods;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	template<> struct XIN_EXPORT TStaticType<UI::FStoryboard>
	{
		static inline FStoryboard_Type FStoryboardType { u8"FStoryboard"N };

		static FType & Type()
		{
			return FStoryboardType;
		}
	};

	FType & UI::FStoryboard::StaticType()
	{
		return TStaticType<UI::FStoryboard>().Type();
	}

	static TTypeRegister<UI::FStoryboard> FStoryboardTypeRegister { TStaticType<UI::FStoryboard>().Type() };
}

// Generated code end.
