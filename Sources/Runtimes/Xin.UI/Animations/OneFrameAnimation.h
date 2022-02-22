#pragma once

#include "Core/Object.h"

namespace Xin::UI
{
	template<typename T>
	class TOneFrameAnimation final : public FTimeline
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

	public:
		TOneFrameAnimation(const TProperty<T> & Property, TOptional<T> To) : Property(Property), __To(To) {}
		~TOneFrameAnimation() = default;

		void Begin(IDependency & Object) override
		{
			ValueTo = __To.HasValue ? __To.Value : Object.GetBaseValueT<T>(Property);
			if (__To.HasValue)
				Object.SetEvaluatedValue<T>(Property, ValueTo);
			else
				Object.UnsetEvaluatedValue<T>(Property);
			FTimeline::Begin(Object);
		}

		void End(IDependency & Object) override
		{
			if (!__To.Valid())
				Object.UnsetAnimatedValue<T>(Property);
			FTimeline::End(Object);
		}

		void ApplyTime(IDependency & Object, const FTimelineTime & Time) override
		{
			//if (Time.Duration < Start || Duration == 0)
			//	return;

			//Object.SetAnimatedValue<T>(Property, ValueTo);
		}

	public:
		const FProperty & Property;

	private:
		T ValueTo { };

	public:
		DependencyPropertyRW(T, To);
	};
}

#if !XIN_REFLECT
#include "__Types/Animations/OneFrameAnimation.inl"
#endif
