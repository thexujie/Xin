// 6DDD2C2F20558E0B48B3741D04FEE78C 2025-03-24 18:05:56
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Animations/OneFrameAnimation.h"

namespace Xin
{
	template<typename T>
	class TOneFrameAnimation_Type : public TClass<UI::TOneFrameAnimation<T>>
	{
	public:
		using TOneFrameAnimation = UI::TOneFrameAnimation<T>;

		// --- 0 Constructors
		static inline const TConstructor<TOneFrameAnimation, TProperty<T> const &, TOptional<T>> Constructor___TProperty__T___const_lref__TOptional__T__ { u8"!Constructor___TProperty__T___const_lref__TOptional__T__"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor___TProperty__T___const_lref__TOptional__T__,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<TOneFrameAnimation, FType &()> Method { u8"Type"N, &TOneFrameAnimation::Type, FMetadata::None };
		};
		struct Method___void___Begin__IDependency_lref
		{
			static inline const TMethod<TOneFrameAnimation, void(IDependency &)> Method { u8"Begin"N, &TOneFrameAnimation::Begin, FMetadata::None };
		};
		struct Method___void___End__IDependency_lref
		{
			static inline const TMethod<TOneFrameAnimation, void(IDependency &)> Method { u8"End"N, &TOneFrameAnimation::End, FMetadata::None };
		};
		struct Method___void___ApplyTime__IDependency_lref__FTimelineTime_const_lref
		{
			static inline const TMethod<TOneFrameAnimation, void(IDependency &, UI::FTimelineTime const &)> Method { u8"ApplyTime"N, &TOneFrameAnimation::ApplyTime, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___void___Begin__IDependency_lref::Method,
			Method___void___End__IDependency_lref::Method,
			Method___void___ApplyTime__IDependency_lref__FTimelineTime_const_lref::Method,
		};

		// --- 2 Fields

		// --- 3 Properties
		struct Property___To
		{
			static inline const TDependencyProperty Property { u8"To"N, &TOneFrameAnimation::__To, &TOneFrameAnimation::__GetTo, &TOneFrameAnimation::__SetToValue, TOneFrameAnimation::__To__, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FProperty> ReflectedProperties[] =
		{
			Property___To::Property,
		};

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<UI::FTimeline>(),
		};

		// --- 5 Constructor
		TOneFrameAnimation_Type(FName Name = u8"TOneFrameAnimation"N) : TClass<TOneFrameAnimation>(Name, Classof<UI::FTimeline>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Methods = ReflectedMethods;
			FClass::Properties = ReflectedProperties;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	template<typename T>
	inline const TProperty<T> & UI::TOneFrameAnimation<T>::ToProperty() { return TOneFrameAnimation_Type<T>::Property___To::Property; }

	template<typename T>
	struct TStaticType<UI::TOneFrameAnimation<T>>
	{
		static FType & Type()
		{
			static TOneFrameAnimation_Type<T> TOneFrameAnimationType { u8"TOneFrameAnimation"N };
			return TOneFrameAnimationType;
		}
	};

	template<typename T>
	FType & UI::TOneFrameAnimation<T>::StaticType()
	{
		return TStaticType<UI::TOneFrameAnimation<T>>().Type();
	}
}

// Generated code end.
