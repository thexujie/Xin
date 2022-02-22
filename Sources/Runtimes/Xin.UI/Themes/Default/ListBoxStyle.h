#pragma once

#include "DefaultTheme.h"

namespace Xin::UI::Themes::Default
{
	class UI_API FListBoxItemTemplate : public TControlTemplate<UListBoxItem>
	{
	public:
		FListBoxItemTemplate() = default;
		UElementRef LoadTemplateT(UListBoxItem & ListBoxItem) override;
	};

	class UI_API FListBoxItemStyle : public TControlStyle<UListBoxItem>
	{
	public:
		FListBoxItemStyle() = default;
		void LoadStyleT(UListBoxItem & ListBoxItem) override;
	};

	class UI_API FListBoxTemplate : public TControlTemplate<UListBox>
	{
	public:
		FListBoxTemplate() = default;
		UElementRef LoadTemplateT(UListBox & ListBox) override;
	};

	class UI_API FListBoxStyle : public TControlStyle<UListBox>
	{
	public:
		FListBoxStyle() = default;
		void LoadStyleT(UListBox & ListBox) override;
	};
}

