#pragma once

#include "DefaultTheme.h"

namespace Xin::UI::Themes::Default
{
	class UI_API FMenuItemTemplate : public TControlTemplate<UMenuItem>
	{
	public:
		FMenuItemTemplate() = default;
		UElementRef LoadTemplateT(UMenuItem & MenuItem) override;
	};

	class UI_API FMenuItemStyle : public TControlStyle<UMenuItem>
	{
	public:
		FMenuItemStyle() = default;
		void LoadStyleT(UMenuItem & MenuItem) override;
	};

	class UI_API FMenuSaperatorTemplate : public TControlTemplate<UMenuSaperator>
	{
	public:
		FMenuSaperatorTemplate() = default;
		UElementRef LoadTemplateT(UMenuSaperator & MenuSaperator) override;
	};

	class UI_API FMenuSaperatorStyle : public TControlStyle<UMenuSaperator>
	{
	public:
		FMenuSaperatorStyle() = default;
		void LoadStyleT(UMenuSaperator & MenuSaperator) override;
	};

	class UI_API FMenuTemplate : public TControlTemplate<UMenu>
	{
	public:
		FMenuTemplate() = default;
		UElementRef LoadTemplateT(UMenu & Menu) override;
	};

	class UI_API FMenuStyle : public TControlStyle<UMenu>
	{
	public:
		FMenuStyle() = default;
		void LoadStyleT(UMenu & Menu) override;
	};
	class UI_API FContextMenuTemplate : public TControlTemplate<UContextMenu>
	{
	public:
		FContextMenuTemplate() = default;
		UElementRef LoadTemplateT(UContextMenu & Window) override;
	};
}

