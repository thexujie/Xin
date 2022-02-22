#pragma once

#include "Media.Types.h"
#include "Animation/Animatable.h"
#include "Color.h"

namespace Xin
{
	class CORE_API IBrush : public IAnimatable
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

	public:
		IBrush() = default;
		IBrush(NullptrT) {}

	public:
		bool IsValid() const { return !!this && this != &None; }
		operator bool() const { return IsValid(); }
		
	public:
		CORE_VAR static IBrush None;
	};
	using IBrushRef = TReferPtr<IBrush>;

	class CORE_API FSolidColorBrush : public IBrush
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

	public:
		FSolidColorBrush() = default;
		FSolidColorBrush(FColor Color) : __Color(Color) {}

	public:
		DependencyPropertyRW(FColor, Color) = Colors::None;
	};
	using FSolidColorBrushRef = TReferPtr<FSolidColorBrush>;

	namespace Brushs
	{
#define COLORS_ITEM(Name, Value) inline IBrushRef Name = MakeRefer<FSolidColorBrush>(EColor::Name);
#include "Colors.inl"
#undef COLORS_ITEM
	}
}
