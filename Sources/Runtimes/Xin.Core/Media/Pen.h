#pragma once

#include "Media.Types.h"
#include "Brush.h"

namespace Xin
{
	enum class ELineJoin
	{
		Miter = 0,
		Bevel,
		Round,
	};

	enum class EStrokePattern
	{
		Solid = 0,
		Dotted,
		Dashed,
		Double,
		Groove,
		Ridge,
		Inset,
		Outset,
		Inherit,
	};

	struct FStrokeStyle
	{
		bool operator ==(const FStrokeStyle &) const noexcept = default;
		auto operator <=>(const FStrokeStyle &) const noexcept = default;

		EStrokePattern Pattern = EStrokePattern::Solid;
		ELineJoin LineJoin = ELineJoin::Miter;

		float32 DashOffset = 0.0f;
		float32 MiterValue = 0.0f;
	};

	class CORE_API IPen : public IAnimatable
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

	public:
		IPen() = default;
		IPen(const IBrush & Brush, float32 Thickness = 1.0f, FStrokeStyle StrokeStyle = { }) : Brush(Brush), StrokeStyle(StrokeStyle), __Thickness(Thickness) {}

		bool IsValid() const { return !!this && this != &None; }
		operator bool() const { return IsValid(); }

	public:
		const IBrush & Brush = IBrush::None;
		FStrokeStyle StrokeStyle { };

	public:
		DependencyPropertyRW(float32, Thickness) = noval;

	public:
		CORE_VAR static IPen None;
	};
	using IPenRef = TReferPtr<IPen>;

	namespace Pens
	{
#define COLORS_ITEM(Name, Value) inline IPenRef Name = MakeRefer<IPen>(Brushs::Name.Ref());
#include "Colors.inl"
#undef COLORS_ITEM
	}
}
