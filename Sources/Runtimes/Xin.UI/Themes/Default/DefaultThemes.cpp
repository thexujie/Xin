#include "PCH.h"
#include "DefaultThemes.h"

#include "ControlsEx/NavigatorStyle.h"

namespace Xin::UI::Themes::Default
{
	TReferPtr<FResourceTable> LoadDefaultThremeResourceTable()
	{
		TReferPtr<FResourceTable> ResourceTable = MakeRefer<FResourceTable>();

		ResourceTable->Resources.Add(MakeRefer<FDefaultInt64DataTemplate>());
		ResourceTable->Resources.Add(MakeRefer<FDefaultUInt32DataTemplate>());
		ResourceTable->Resources.Add(MakeRefer<FDefaultUInt64DataTemplate>());
		ResourceTable->Resources.Add(MakeRefer<FDefaultStringDataTemplate>());
		ResourceTable->Resources.Add(MakeRefer<FDefaultTextDataTemplate>());
		ResourceTable->Resources.Add(MakeRefer<FDefaultPixmapTemplate>());
		ResourceTable->Resources.Add(MakeRefer<FDefaultBitmapTemplate>());

		ResourceTable->Resources.Add(MakeRefer<FButtonTemplate>());
		ResourceTable->Resources.Add(MakeRefer<FButtonStyle>());
		ResourceTable->Resources.Add(MakeRefer<FRadioButtonTemplate>());
		ResourceTable->Resources.Add(MakeRefer<FCheckBoxTemplate>());

		ResourceTable->Resources.Add(MakeRefer<FScrollBarTemplate>());

		ResourceTable->Resources.Add(MakeRefer<FScrollViewerTemplate>());

		ResourceTable->Resources.Add(MakeRefer<FListControlTemplate>());
		ResourceTable->Resources.Add(MakeRefer<FListControlStyle>());

		ResourceTable->Resources.Add(MakeRefer<FListBoxItemTemplate>());
		ResourceTable->Resources.Add(MakeRefer<FListBoxItemStyle>());
		ResourceTable->Resources.Add(MakeRefer<FListBoxTemplate>());
		ResourceTable->Resources.Add(MakeRefer<FListBoxStyle>());

		ResourceTable->Resources.Add(MakeRefer<FListViewItemTemplate>());
		ResourceTable->Resources.Add(MakeRefer<FListViewItemStyle>());
		ResourceTable->Resources.Add(MakeRefer<FListViewTemplate>());
		ResourceTable->Resources.Add(MakeRefer<FListViewStyle>());

		ResourceTable->Resources.Add(MakeRefer<FListViewHeaderTemplate>());

		ResourceTable->Resources.Add(MakeRefer<FListViewColumnHeaderTemplate>());
		ResourceTable->Resources.Add(MakeRefer<FListViewColumnHeaderStyle>());

		ResourceTable->Resources.Add(MakeRefer<FListViewItemCellTemplate>());
		ResourceTable->Resources.Add(MakeRefer<FListViewItemCellStyle>());

		ResourceTable->Resources.Add(MakeRefer<FMenuItemTemplate>());
		ResourceTable->Resources.Add(MakeRefer<FMenuItemStyle>());
		ResourceTable->Resources.Add(MakeRefer<FMenuSaperatorTemplate>());
		ResourceTable->Resources.Add(MakeRefer<FMenuSaperatorStyle>());
		ResourceTable->Resources.Add(MakeRefer<FMenuTemplate>());
		ResourceTable->Resources.Add(MakeRefer<FMenuStyle>());

		ResourceTable->Resources.Add(MakeRefer<FDefaultWindowTemplate>());
		ResourceTable->Resources.Add(MakeRefer<FContextMenuTemplate>());
		ResourceTable->Resources.Add(MakeRefer<FTooltipTemplate>());

		ResourceTable->Resources.Add(MakeRefer<FChromeStyle>());
		ResourceTable->Resources.Add(MakeRefer<FChromeTemplate>());
		ResourceTable->Resources.Add(MakeRefer<FChromeButtonStyle>());
		ResourceTable->Resources.Add(MakeRefer<FChromeButtonTemplate>());
		ResourceTable->Resources.Add(MakeRefer<FChromeTitleStyle>());
		ResourceTable->Resources.Add(MakeRefer<FChromeTitleTemplate>());

		ResourceTable->Resources.Add(MakeRefer<ControlsEx::FNavigatorTemplate>());
		ResourceTable->Resources.Add(MakeRefer<ControlsEx::FNavigatorStyle>());
		ResourceTable->Resources.Add(MakeRefer<ControlsEx::FNavigatorHeaderTemplate>());
		ResourceTable->Resources.Add(MakeRefer<ControlsEx::FNavigatorHeaderStyle>());
		ResourceTable->Resources.Add(MakeRefer<ControlsEx::FNavigatorHeaderItemTemplate>());
		ResourceTable->Resources.Add(MakeRefer<ControlsEx::FNavigatorHeaderItemStyle>());

		return ResourceTable;
	}
}

namespace Xin::UI
{
	TReferPtr<FResourceTable> GDefaultResourceList;

	TReferPtr<FResourceTable> FResourceTable::Default()
	{
		if (!GDefaultResourceList)
			GDefaultResourceList = Themes::Default::LoadDefaultThremeResourceTable();
		return GDefaultResourceList;
	}
}