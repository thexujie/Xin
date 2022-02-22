#pragma once

#include "Xin.UI.Types.h"
#include "Controls/Element.h"

namespace Xin::UI
{
	class UI_API UShape : public UElement
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

	public:
		UShape() = default;
		UShape(FName Name) : UElement(Name) {}
		~UShape() = default;

	public:
		void OnConstruct() override;

	public:
		[[Meta(AffectsRender)]]
		DependencyPropertyRW(IBrushRef, Fill) = Brushs::None;

		[[Meta(AffectsRender)]]
		DependencyPropertyRW(IBrushRef, Stroke) = Brushs::None;

		[[Meta(AffectsRender)]]
		DependencyPropertyRW(FStrokeStyle, StrokeStyle) = FStrokeStyle { };

		[[Meta(AffectsRender)]]
		DependencyPropertyRW(float32, StrokeThickness) = 1.0f;
	};
	using UShapeRef = TReferPtr<UShape>;
}
