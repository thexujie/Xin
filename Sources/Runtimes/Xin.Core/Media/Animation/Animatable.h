#pragma once

#include "Core/Freezable.h"

namespace Xin
{
	template<typename T>
	concept AnimatableValueC = requires(const T & From, const T & To, float32 Inter)
	{
		DefaultConstructibleC<T>;
		{ Lerp(From, To, Inter) }-> ConvertibleToC<T>;
	};

	template<typename T>
	inline constexpr bool IsAnimatableValueV = false;
	template<AnimatableValueC T>
	inline constexpr bool IsAnimatableValueV<T> = true;
	static_assert(IsAnimatableValueV<float32>);
	static_assert(IsAnimatableValueV<Vec2I>);
	static_assert(IsAnimatableValueV<Vec2U>);
	static_assert(IsAnimatableValueV<Vec2F>);
	static_assert(IsAnimatableValueV<Vec2D>);
	static_assert(IsAnimatableValueV<Vec4I>);
	static_assert(IsAnimatableValueV<Vec4U>);
	static_assert(IsAnimatableValueV<Vec4F>);
	static_assert(IsAnimatableValueV<Vec4D>);

	class CORE_API IAnimatable : public IFreezable
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

	public:
		IAnimatable() = default;
		~IAnimatable() = default;

	public:

	};
	using IAnimatableRef = TReferPtr<IAnimatable>;
}
