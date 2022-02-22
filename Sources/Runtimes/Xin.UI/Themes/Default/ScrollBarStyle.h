#pragma once

#include "DefaultTheme.h"

namespace Xin::UI::Themes::Default
{
	class UI_API FScrollBarThumbControlTemplate : public TControlTemplate<UThumb>
	{
	public:
		FScrollBarThumbControlTemplate() = default;
		UElementRef LoadTemplateT(UThumb & Thumb) override;
	};

	class UI_API FScrollBarTemplate : public TControlTemplate<UScrollBar>
	{
	public:
		FScrollBarTemplate() = default;
		UElementRef LoadTemplateT(UScrollBar & ScrollBar) override;
	};

	class UI_API FScrollBarStyle : public TControlStyle<UScrollBar>
	{
	public:
		FScrollBarStyle() = default;
		void LoadStyleT(UScrollBar & ScrollBar) override;
	};
}

