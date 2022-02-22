#pragma once

#include "Themes/Default/DefaultTheme.h"
#include "ControlsEx/Navigator.h"

namespace Xin::UI::ControlsEx
{
	class UI_API FNavigatorHeaderItemTemplate : public TControlTemplate<UNavigatorHeaderItem>
	{
	public:
		FNavigatorHeaderItemTemplate() = default;
		UElementRef LoadTemplateT(UNavigatorHeaderItem & NavigatorHeaderItem) override;
	};

	class UI_API FNavigatorHeaderItemStyle : public TControlStyle<UNavigatorHeaderItem>
	{
	public:
		FNavigatorHeaderItemStyle() = default;
		void LoadStyleT(UNavigatorHeaderItem & NavigatorHeaderItem) override;
	};

	class UI_API FNavigatorHeaderTemplate : public TControlTemplate<UNavigatorHeader>
	{
	public:
		FNavigatorHeaderTemplate() = default;
		UElementRef LoadTemplateT(UNavigatorHeader & NavigatorHeader) override;
	};

	class UI_API FNavigatorHeaderStyle : public TControlStyle<UNavigatorHeader>
	{
	public:
		FNavigatorHeaderStyle() = default;
		void LoadStyleT(UNavigatorHeader & NavigatorHeader) override;
	};

	class UI_API FNavigatorTemplate : public TControlTemplate<UNavigator>
	{
	public:
		FNavigatorTemplate() = default;
		UElementRef LoadTemplateT(UNavigator & Navigator) override;
	};

	class UI_API FNavigatorStyle: public TControlStyle<UNavigator>
	{
	public:
		FNavigatorStyle() = default;
		void LoadStyleT(UNavigator & Navigator) override;
	};
}
