#pragma once

#include "Core/Object.h"

namespace Xin::UI
{
	template<AnimatableValueC T>
	class TDoubleAnimation final : public FTimeline
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

	public:
		TDoubleAnimation(const TProperty<T> & Property, TOptional<T> From, TOptional<T> To) : Property(Property), __From(From), __To(To) {}
		~TDoubleAnimation() = default;

		void Begin(IDependency & Object) override
		{
			//FConsole::WriteLineF(u8"TDoubleAnimation {} -> {}"V, __From, __To);
			ValueFrom = __From.HasValue ? __From.Value : Object.GetValueT<T>(Property);
			ValueTo = __To.HasValue ? __To.Value : Object.GetBaseValueT<T>(Property);
			if (__To.HasValue)
				Object.SetEvaluatedValue<T>(Property, ValueTo);
			else
				Object.UnsetEvaluatedValue<T>(Property);
			FTimeline::Begin(Object);
		}

		void End(IDependency & Object) override
		{
			Object.UnsetAnimatedValue<T>(Property);
			FTimeline::End(Object);
		}

		void ApplyTime(IDependency & Object, const FTimelineTime & Time) override
		{
			if (Time.Duration < Start || Duration == 0)
				return;

			float32 Inter = Saturate((float32)(Time.Duration - Start) / Duration);
			T Value = Lerp(ValueFrom, ValueTo, Inter);
			Object.SetAnimatedValue<T>(Property, Value);
		}

	public:
		const FProperty & Property;

	private:
		T ValueFrom {};
		T ValueTo {};
		bool Reversing = false;

	public:
		DependencyPropertyRW(T, From);
		DependencyPropertyRW(T, To);
	};
}

#if !XIN_REFLECT
#include "__Types/Animations/DoubleAnimation.inl"
#endif
