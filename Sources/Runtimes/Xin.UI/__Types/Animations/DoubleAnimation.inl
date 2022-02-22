// E26CED48379B01F6F31E8544A2FED9C2 2025-03-24 17:56:33
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Animations/DoubleAnimation.h"

namespace Xin
{
	template<AnimatableValueC T>
	class TDoubleAnimation_Type : public TClass<UI::TDoubleAnimation<T>>
	{
	public:
		using TDoubleAnimation = UI::TDoubleAnimation<T>;

		// --- 0 Constructors
		static inline const TConstructor<TDoubleAnimation, TProperty<T> const &, TOptional<T>, TOptional<T>> Constructor___TProperty__T___const_lref__TOptional__T____TOptional__T__ { u8"!Constructor___TProperty__T___const_lref__TOptional__T____TOptional__T__"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor___TProperty__T___const_lref__TOptional__T____TOptional__T__,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<TDoubleAnimation, FType &()> Method { u8"Type"N, &TDoubleAnimation::Type, FMetadata::None };
		};
		struct Method___void___Begin__IDependency_lref
		{
			static inline const TMethod<TDoubleAnimation, void(IDependency &)> Method { u8"Begin"N, &TDoubleAnimation::Begin, FMetadata::None };
		};
		struct Method___void___End__IDependency_lref
		{
			static inline const TMethod<TDoubleAnimation, void(IDependency &)> Method { u8"End"N, &TDoubleAnimation::End, FMetadata::None };
		};
		struct Method___void___ApplyTime__IDependency_lref__FTimelineTime_const_lref
		{
			static inline const TMethod<TDoubleAnimation, void(IDependency &, UI::FTimelineTime const &)> Method { u8"ApplyTime"N, &TDoubleAnimation::ApplyTime, FMetadata::None };
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
		struct Property___From
		{
			static inline const TDependencyProperty Property { u8"From"N, &TDoubleAnimation::__From, &TDoubleAnimation::__GetFrom, &TDoubleAnimation::__SetFromValue, TDoubleAnimation::__From__, FMetadata::None };
		};
		struct Property___To
		{
			static inline const TDependencyProperty Property { u8"To"N, &TDoubleAnimation::__To, &TDoubleAnimation::__GetTo, &TDoubleAnimation::__SetToValue, TDoubleAnimation::__To__, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FProperty> ReflectedProperties[] =
		{
			Property___From::Property,
			Property___To::Property,
		};

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<UI::FTimeline>(),
		};

		// --- 5 Constructor
		TDoubleAnimation_Type(FName Name = u8"TDoubleAnimation"N) : TClass<TDoubleAnimation>(Name, Classof<UI::FTimeline>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Methods = ReflectedMethods;
			FClass::Properties = ReflectedProperties;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	template<AnimatableValueC T>
	inline const TProperty<T> & UI::TDoubleAnimation<T>::FromProperty() { return TDoubleAnimation_Type<T>::Property___From::Property; }

	template<AnimatableValueC T>
	inline const TProperty<T> & UI::TDoubleAnimation<T>::ToProperty() { return TDoubleAnimation_Type<T>::Property___To::Property; }

	template<AnimatableValueC T>
	struct TStaticType<UI::TDoubleAnimation<T>>
	{
		static FType & Type()
		{
			static TDoubleAnimation_Type<T> TDoubleAnimationType { u8"TDoubleAnimation"N };
			return TDoubleAnimationType;
		}
	};

	template<AnimatableValueC T>
	FType & UI::TDoubleAnimation<T>::StaticType()
	{
		return TStaticType<UI::TDoubleAnimation<T>>().Type();
	}
}

// Generated code end.
