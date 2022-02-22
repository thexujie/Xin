#pragma once

#include "ContentControl.h"
#include "Xin.UI.Types.h"
#include "ListControl.h"

namespace Xin::UI
{
	class UI_API UMenuItem : public UContentControl
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

	public:
		UMenuItem() = default;

	public:
		void OnConstruct() override;
		void UpdateState() override;

	public:
		void OnMouseEnter(FMouseEventArgs & Args) override;
		void OnMouseLeave(FMouseEventArgs & Args) override;
		void OnMouseDown(FMouseButtonEventArgs & Args) override;
		void OnMouseUp(FMouseButtonEventArgs & Args) override;

	public:
		UMenuObj Owner;

	private:
		bool SubmenuShowing = false;

	public:
		ProxyPropertyRW(intx, Index) = NullIndex;

		ProxyPropertyRW(bool, SelecteAble) = true;

		[[Meta(AffectsState)]]
		DependencyPropertyRW(FVariant, Submenu) = noval;

		[[Meta(AffectsState)]]
		DependencyPropertyRW(bool, Selected) = false;

		void OnIndicatingChanged(bool IndicatingBefore);
		[[Meta(AffectsState)]]
		DependencyPropertyRW(bool, Indicating) = false;

	public:
		FRoutedEvent Click;
	};
	using UMenuItemRef = TReferPtr<UMenuItem>;

	class UI_API UMenuSaperator : public UMenuItem
	{
		META_DECL
	public:
		UMenuSaperator() = default;
		~UMenuSaperator() = default;

	public:
		void OnConstruct() override;
	};

	class UI_API FMenuItemSelectedEventArgs : public FRoutedEventArgs
	{
	public:
		FMenuItemSelectedEventArgs(IObject & Sender, UMenuItem & MenuItem) : FRoutedEventArgs(Sender), MenuItem(MenuItem) { }

		UMenuItem & MenuItem;
	};

	class UI_API UMenu : public UListControl, TIItemAddable<UMenuItemRef>
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

	public:
		UMenu();
		~UMenu() = default;

	public:
		FStringV Describe() const override;
		void OnConstruct() override;
		void OnInitialize() override;
		void OnFinalize() override;
		void OnHidden() override;
		void PreKeyDown(FKeyboardEventArgs & Args) override;

	public:
		void OnItemMouseEnter(UMenuItem & MenuItem, FMouseEventArgs & Args);
		void OnItemMouseLeave(UMenuItem & MenuItem, FMouseEventArgs & Args);
		void OnItemMouseDown(UMenuItem & MenuItem, FMouseButtonEventArgs & Args);
		void OnItemMouseUp(UMenuItem & MenuItem, FMouseButtonEventArgs & Args);

		void OnItemSelected(FMenuItemSelectedEventArgs & EventArgs);

	private:
		void PresentItems();
		void UnpresentItems();

	public:
		TItemCollection<UMenuItemRef> Items { *this };
		bool ItemsPresented = false;

	public:
		void ShowSubmenu();
		void HideSubmenu();

	public:
		IControlTemplateRef ItemTemplate;

	public:
		ProxyPropertyRW(ESelectMethod, SelectMethod) = ESelectMethod::Release;

		void OnIndicatingIndexChanged(intx IndicatingIndexBefore);
		ProxyPropertyRW(intx, IndicatingIndex) = NullIndex;

		void OnHoveringIndexChanged(intx HoveringIndexBefore);
		ProxyPropertyRW(intx, HoveringIndex) = NullIndex;

	public:
		UMenuObj MenuParent;

	private:
		bool SubmenuShowing = false;

	public:
		TEvent<void(FMenuItemSelectedEventArgs &)> ItemSelected;
	};

	UI_API UMenuRef Menu(TView<FVariant> Items);

	inline UMenuItemRef MenuItem(const FVariant & Content, TFunction<void(const FRoutedEventArgs &)> OnClick = nullptr, const FVariant & Submenu = noval)
	{
		UMenuItemRef MenuItem = MakeRefer<UMenuItem>();
		MenuItem->Content = Content;
		MenuItem->Submenu = Submenu;
		if (OnClick)
			MenuItem->Click += OnClick;
		return MenuItem;
	}

	inline UMenuItemRef MenuSaperator()
	{
		UMenuItemRef MenuItem = MakeRefer<UMenuSaperator>();
		return MenuItem;
	}
}
