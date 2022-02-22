#pragma once

#include "Xin.UI.Types.h"
#include "ContentControl.h"

namespace Xin::UI
{
	class URadioButton;
	using URadioButtonRef = TReferPtr<URadioButton>;
	using URadioButtonObj = TEntryPtr<URadioButton>;

	class UI_API UView : public UContentControl
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

	public:
		UView();
		~UView();

	public:
		void OnConstruct() override;
		void OnInitialize() override;

		UView * GetView() const override { return ConstCast(this); }

		UElement * HitElement(FPoint Point) const override;

	public:
		virtual void UpdateCursor(ECursor Cursor, bool ForceUpdate) = 0;
		virtual void UpdateCapture(bool Capture) = 0;

	public:
		void SetHoveredElement(UElement * Element, FPoint Point);
		UElementObj HoveredElement = nullptr;

	public:
		void SetCapturedElement(UElement * Element);
		UElementObj CapturedElement = nullptr;

	public:
		void DispatchLayout();
		bool LayoutDispatching = false;

	public:
		void RegisterRadioButton(URadioButton * RadioButton);
		void UnregisterRadioButton(URadioButton * RadioButton);
		void CheckRadioButton(URadioButton * RadioButton);

		TList<URadioButtonRef> RadioButtons;
		TList<TPair<FName, TList<URadioButtonRef>>> RadioButtonGroups;

	public:
		TEvent<void(UElement * Element)> ElementHovered;
		TEvent<void(UElement * Element)> ElementCaptured;
	};
}
