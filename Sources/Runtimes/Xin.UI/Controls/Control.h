#pragma once

#include "Xin.UI.Types.h"
#include "Element.h"
#include "Core/DataTemplate.h"
#include "Core/ControlTemplate.h"

namespace Xin::UI
{
	namespace ControlKinds
	{
		constexpr FName Label = u8"Label"N;
		constexpr FName Button = u8"Button"N;
		constexpr FName RadioButton = u8"RadioButton"N;
		constexpr FName CheckBox = u8"CheckBox"N;
		constexpr FName ListView = u8"ListView"N;
		constexpr FName TextBlock = u8"TextBlock"N;
		constexpr FName Window = u8"Window"N;
	}

	class UI_API UControl : public UElement
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }
		static constexpr FName PART_VerticalScrollBar = u8"PART_VerticalScrollBar"N;
		static constexpr FName PART_HorizontalScrollBar = u8"PART_HorizontalScrollBar"N;
		static constexpr FName PART_ConnerElement = u8"PART_ConnerElement"N;
		static constexpr FName PART_ContentPresenter = u8"PART_ContentPresenter"N;

	public:
		UControl();
		UControl(FName Name) : UElement(Name) {}
		~UControl() = default;

	public:
		void OnInitialize() override;
		void OnFinalize() override;
		FSize OnMeasure(FSize ConstrainedSize) override;
		void OnArrange() override;
		void OnResourcesChanged() override;

	public:
		virtual void OnTemplateChanged(IControlTemplateRef TemplateBefore);
		ProxyPropertyRW(IControlTemplateRef, Template);
		ProxyPropertyRW(UElementRef, TemplateElement);
		virtual void OnTemplated() {}

	public:
		[[Meta(AffectsRender, Hierarchical)]]
		DependencyPropertyRW(FColor, Foreground) = noval;

		[[Meta(AffectsRender)]]
		DependencyPropertyRW(IBrushRef, Background) = noval;

		DependencyPropertyRW(IBrushRef, BorderStroke) = noval;
		DependencyPropertyRW(FThickness, BorderThickness) = noval;
		DependencyPropertyRW(FThickness, Padding) = noval;
		DependencyPropertyRW(EElementAlignment, ContentHorizontalAlignment) = EElementAlignment::Stretch;
		DependencyPropertyRW(EElementAlignment, ContentVerticalAlignment) = EElementAlignment::Stretch;
	};
	using UControlRef = TReferPtr<UControl>;
}
