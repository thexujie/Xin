#pragma once

#include "ContentControl.h"
#include "Xin.UI.Types.h"
#include "ListControl.h"

namespace Xin::UI
{
	class UListBox;
	using UListBoxRef = TReferPtr<UListBox>;
	using UListBoxObj = TEntryPtr<UListBox>;


	class UI_API UListBoxItem : public UContentControl
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

	public:
		void OnConstruct() override;
		void UpdateState() override;

	public:
		void OnMouseDown(FMouseButtonEventArgs & Args) override;
		void OnMouseUp(FMouseButtonEventArgs & Args) override;

	public:
		UListBoxObj Owner;
		uintx Index = NullIndex;

	public:
		[[Meta(AffectsState)]]
		DependencyPropertyRW(bool, Selected) = false;
	};
	using UListBoxItemRef = TReferPtr<UListBoxItem>;

	class UI_API UListBox : public UListControl, TIItemAddable<UListBoxItemRef>
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

	public:
		UListBox();
		~UListBox() = default;

	public:
		void OnConstruct() override;
		void OnInitialize() override;
		void OnInitialized() override;
		void OnFinalize() override;

	public:
		void OnItemMouseDown(UListBoxItem & ListBoxItem, FMouseButtonEventArgs & Args);
		void OnItemMouseUp(UListBoxItem & ListBoxItem, FMouseButtonEventArgs & Args);

	private:
		void PresentItems();
		void UnpresentItems();

	public:
		TItemCollection<UListBoxItemRef> Items { *this };
		bool ItemsPresented = false;

	public:
		IControlTemplateRef ItemTemplate;

	public:
		DependencyPropertyRW(TList<uintx>, SelectedIndices);
		DependencyPropertyRW(uintx, SelectedIndex) = NullIndex;

		void OnSelectedIndexChanged(uintx SelectedIndexBefore);

	public:
		DependencyPropertyRW(ESelectMethod, SelectMethod) = ESelectMethod::Press;
		DependencyPropertyRW(ESelectionMode, SelectionMode) = ESelectionMode::Single;
	};
}
