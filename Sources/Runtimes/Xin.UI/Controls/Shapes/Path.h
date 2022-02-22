#pragma once

#include "Xin.UI.Types.h"
#include "Shape.h"

namespace Xin::UI
{
	class UI_API UPath : public UShape
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

	public:
		UPath() = default;
		~UPath() = default;

	public:
		void OnInitialize() override;
		void OnFinalize() override;
		FSize OnMeasure(FSize ConstrainedSize) override;
		void OnArranged() override;
		void OnPaint(IPainter & Painter) override;

	private:
		void OnPathTransformChanged(const FTransform3x2F & ValueBefore);

	public:
		[[Meta(AffectsMeasure, AffectsRender)]]
		DependencyPropertyRW(FString, Command);

		[[Meta(AffectsMeasure, AffectsRender)]]
		DependencyPropertyRW(FTransform3x2F, PathTransform);

		[[Meta(AffectsRender)]]
		DependencyPropertyRW(EStretch, Stretch) = EStretch::None;

	private:
		IPathRef Path = nullptr;

		Vec2F StretchOffset = Vec2F::Zero;
		Vec2F StretchScale = Vec2F::One;
		IPathRef OriginPath = nullptr;
	};
	using UPathRef = TReferPtr<UPath>;
}
