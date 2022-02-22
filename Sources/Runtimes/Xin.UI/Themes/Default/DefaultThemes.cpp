#include "PCH.h"
#include "DefaultThemes.h"

#include "ControlsEx/NavigatorStyle.h"

namespace Xin::UI::Themes::Default
{
	UI_API TList<IResourceRef> LoadDefaultThemesResources()
	{
		TList<IResourceRef> Resources;

		Resources.Add(MakeRefer<FDefaultInt64DataTemplate>());
		Resources.Add(MakeRefer<FDefaultUInt32DataTemplate>());
		Resources.Add(MakeRefer<FDefaultUInt64DataTemplate>());
		Resources.Add(MakeRefer<FDefaultStringDataTemplate>());
		Resources.Add(MakeRefer<FDefaultTextDataTemplate>());
		Resources.Add(MakeRefer<FDefaultPixmapTemplate>());
		Resources.Add(MakeRefer<FDefaultBitmapTemplate>());

		Resources.Add(MakeRefer<FButtonTemplate>());
		Resources.Add(MakeRefer<FButtonStyle>());
		Resources.Add(MakeRefer<FRadioButtonTemplate>());
		Resources.Add(MakeRefer<FCheckBoxTemplate>());

		Resources.Add(MakeRefer<FScrollBarTemplate>());

		Resources.Add(MakeRefer<FScrollViewerTemplate>());

		Resources.Add(MakeRefer<FListControlTemplate>());
		Resources.Add(MakeRefer<FListControlStyle>());

		Resources.Add(MakeRefer<FListBoxItemTemplate>());
		Resources.Add(MakeRefer<FListBoxItemStyle>());
		Resources.Add(MakeRefer<FListBoxTemplate>());
		Resources.Add(MakeRefer<FListBoxStyle>());

		Resources.Add(MakeRefer<FListViewItemTemplate>());
		Resources.Add(MakeRefer<FListViewItemStyle>());
		Resources.Add(MakeRefer<FListViewTemplate>());
		Resources.Add(MakeRefer<FListViewStyle>());

		Resources.Add(MakeRefer<FListViewHeaderTemplate>());

		Resources.Add(MakeRefer<FListViewColumnHeaderTemplate>());
		Resources.Add(MakeRefer<FListViewColumnHeaderStyle>());

		Resources.Add(MakeRefer<FListViewItemCellTemplate>());
		Resources.Add(MakeRefer<FListViewItemCellStyle>());

		Resources.Add(MakeRefer<FMenuItemTemplate>());
		Resources.Add(MakeRefer<FMenuItemStyle>());
		Resources.Add(MakeRefer<FMenuSaperatorTemplate>());
		Resources.Add(MakeRefer<FMenuSaperatorStyle>());
		Resources.Add(MakeRefer<FMenuTemplate>());
		Resources.Add(MakeRefer<FMenuStyle>());

		Resources.Add(MakeRefer<FDefaultWindowTemplate>());
		Resources.Add(MakeRefer<FContextMenuTemplate>());
		Resources.Add(MakeRefer<FTooltipTemplate>());

		Resources.Add(MakeRefer<FChromeStyle>());
		Resources.Add(MakeRefer<FChromeTemplate>());
		Resources.Add(MakeRefer<FChromeButtonStyle>());
		Resources.Add(MakeRefer<FChromeButtonTemplate>());
		Resources.Add(MakeRefer<FChromeTitleStyle>());
		Resources.Add(MakeRefer<FChromeTitleTemplate>());

		Resources.Add(MakeRefer<ControlsEx::FNavigatorTemplate>());
		Resources.Add(MakeRefer<ControlsEx::FNavigatorStyle>());
		Resources.Add(MakeRefer<ControlsEx::FNavigatorHeaderTemplate>());
		Resources.Add(MakeRefer<ControlsEx::FNavigatorHeaderStyle>());
		Resources.Add(MakeRefer<ControlsEx::FNavigatorHeaderItemTemplate>());
		Resources.Add(MakeRefer<ControlsEx::FNavigatorHeaderItemStyle>());

		return Resources;
	}
}
