#pragma once

#include "Xin.UI.Types.h"
#include "Popup.h"

namespace Xin::UI
{
	class UContextMenu;
	using UContextMenuRef = TReferPtr<UContextMenu>;
	using UContextMenuObj = TEntryPtr<UContextMenu>;


	class UI_API UContextMenu : public UPopup
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

	public:
		UContextMenu();
		~UContextMenu();

	public:
		void OnConstruct() override;
		void OnInitialize() override;
		void OnFinalize() override;
		void OnHidden() override;

	protected:
		TFunction<void()> Closed;

	public:
		void ShowMenu(UElement * Element, const FVariant & Menu, EPopupPosition PlacementMode, TFunction<void()> OnClosed);
	};
}
