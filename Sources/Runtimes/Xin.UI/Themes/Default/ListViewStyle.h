#pragma once

#include "DefaultTheme.h"

namespace Xin::UI::Themes::Default
{
	class UI_API FListViewHeaderTemplate : public TControlTemplate<UListViewHeader>
	{
	public:
		FListViewHeaderTemplate() = default;
		UElementRef LoadTemplateT(UListViewHeader & ListViewHeader) override;
	};

	class UI_API FListViewColumnHeaderTemplate : public TControlTemplate<UListViewColumnHeader>
	{
	public:
		FListViewColumnHeaderTemplate() = default;
		UElementRef LoadTemplateT(UListViewColumnHeader & ListViewColumnHeader) override;
	};

	class UI_API FListViewColumnHeaderStyle : public TControlStyle<UListViewColumnHeader>
	{
	public:
		FListViewColumnHeaderStyle() = default;
		void LoadStyleT(UListViewColumnHeader & ListViewColumnHeader) override;
	};

	class UI_API FListViewItemCellTemplate : public TControlTemplate<UListViewItemCell>
	{
	public:
		FListViewItemCellTemplate() = default;
		UElementRef LoadTemplateT(UListViewItemCell & ListViewItemCell) override;
	};

	class UI_API FListViewItemCellStyle : public TControlStyle<UListViewItemCell>
	{
	public:
		FListViewItemCellStyle() = default;
		void LoadStyleT(UListViewItemCell & ListViewItemCell) override;
	};

	class UI_API FListViewItemTemplate : public TControlTemplate<UListViewItem>
	{
	public:
		FListViewItemTemplate() = default;
		UElementRef LoadTemplateT(UListViewItem & ListViewItem) override;
	};

	class UI_API FListViewItemStyle : public TControlStyle<UListViewItem>
	{
	public:
		FListViewItemStyle() = default;
		void LoadStyleT(UListViewItem & ListViewItem) override;
	};

	class UI_API FListViewTemplate : public TControlTemplate<UListView>
	{
	public:
		FListViewTemplate() = default;
		UElementRef LoadTemplateT(UListView & ListView) override;
	};

	class UI_API FListViewStyle : public TControlStyle<UListView>
	{
	public:
		FListViewStyle() = default;
		void LoadStyleT(UListView & ListView) override;
	};
}

