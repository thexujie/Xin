#pragma once

#include "Xin.UI.Types.h"
#include "Shape.h"

namespace Xin::UI
{
	class UI_API ULine : public UShape
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

	public:
		ULine() = default;
		ULine(FName Name) : UShape(Name) {}
		~ULine() = default;

	public:
		FSize OnMeasure(FSize ConstrainedSize) override;
		void OnPaint(IPainter & Painter) override;

	public:
		DependencyPropertyRW(FPoint, From) = FPoint::Zero;
		DependencyPropertyRW(FPoint, To) = FPoint::XAxis;

		[[Meta(AffectsMeasure, AffectsRender)]]
		DependencyPropertyRW(ECoordinateMode, CoordinateMode) = ECoordinateMode::Relative;
	};
	using ULineRef = TReferPtr<ULine>;

	class UI_API UHLine : public UShape
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

	public:
		UHLine() = default;
		UHLine(FName Name) : UShape(Name) {}
		~UHLine() = default;

	public:
		FSize OnMeasure(FSize ConstrainedSize) override;
		void OnPaint(IPainter & Painter) override;
	};
	using UHLineRef = TReferPtr<UHLine>;

	class UI_API UVLine : public UShape
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

	public:
		UVLine() = default;
		UVLine(FName Name) : UShape(Name) {}
		~UVLine() = default;

	public:
		FSize OnMeasure(FSize ConstrainedSize) override;
		void OnPaint(IPainter & Painter) override;
	};
	using UVLineRef = TReferPtr<UVLine>;
}
