#pragma once

#include "DefaultTheme.h"

namespace Xin::UI::Themes::Default
{
	class UI_API FChromeStyle : public TControlStyle<UChrome>
	{
	public:
		FChromeStyle() = default;
		void LoadStyleT(UChrome & Chrome) override;
	};

	class UI_API FChromeTemplate : public TControlTemplate<UChrome>
	{
	public:
		FChromeTemplate() = default;
		UElementRef LoadTemplateT(UChrome & Chrome) override;
	};

	class UI_API FChromeButtonStyle : public TControlStyle<UChromeButton>
	{
	public:
		FChromeButtonStyle() = default;
		void LoadStyleT(UChromeButton & ChromeButton) override;
	};

	class UI_API FChromeButtonTemplate : public TControlTemplate<UChromeButton>
	{
	public:
		FChromeButtonTemplate() = default;
		UElementRef LoadTemplateT(UChromeButton & ChromeButton) override;
	};

	class UI_API FChromeTitleStyle : public TControlStyle<UChromeTitle>
	{
	public:
		FChromeTitleStyle() = default;
		void LoadStyleT(UChromeTitle & ChromeTitle) override;
	};

	class UI_API FChromeTitleTemplate : public TControlTemplate<UChromeTitle>
	{
	public:
		FChromeTitleTemplate() = default;
		UElementRef LoadTemplateT(UChromeTitle & ChromeTitle) override;
	};
}

