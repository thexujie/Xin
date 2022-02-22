#pragma once

#include "DefaultTheme.h"

namespace Xin::UI::Themes::Default
{
	class UI_API FButtonTemplate : public TControlTemplate<UButton>
	{
	public:
		FButtonTemplate() = default;
		UElementRef LoadTemplateT(UButton & Button) override;
	};

	class UI_API FButtonStyle : public TControlStyle<UButton>
	{
	public:
		FButtonStyle() = default;
		void LoadStyleT(UButton & Button) override;
	};

	class UI_API FRadioButtonTemplate : public TControlTemplate<URadioButton>
	{
	public:
		FRadioButtonTemplate() = default;

		UElementRef LoadTemplateT(URadioButton & RadioButton) override;
	};

	class UI_API FCheckBoxTemplate : public TControlTemplate<UCheckBox>
	{
	public:
		FCheckBoxTemplate() = default;
		UElementRef LoadTemplateT(UCheckBox & CheckBox) override;
	};

}

