#pragma once

#include "Panel.h"
#include "StackPanel.h"

namespace Xin::UI
{
	class UI_API FSplitterPanelIndicatingIndexChagnedEventArgs : public FEventArgs
	{
	public:
		FSplitterPanelIndicatingIndexChagnedEventArgs(uintx IndicatingIndex, uintx IndicatingIndexBefore) : IndicatingIndex(IndicatingIndex), IndicatingIndexBefore(IndicatingIndexBefore) { }

		uintx IndicatingIndex;
		uintx IndicatingIndexBefore;
	};

	class UI_API FSplitterPanelResizingIndexChagnedEventArgs : public FEventArgs
	{
	public:
		FSplitterPanelResizingIndexChagnedEventArgs(uintx ResigingIndex, uintx ResigingIndexBefore) : ResigingIndex(ResigingIndex), ResigingIndexBefore(ResigingIndexBefore) { }

		uintx ResigingIndex;
		uintx ResigingIndexBefore;
	};

	class UI_API USplitterPanel : public UStackPanel
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

	public:
		USplitterPanel() = default;
		USplitterPanel(FName Name) : UStackPanel(Name) {}
		~USplitterPanel() = default;

	public:
		void OnConstruct() override;
		void OnInitialize() override;
		FSize OnMeasure(FSize ConstrainedSize) override;
		void OnArrange() override;


		UElement * HitElement(FPoint Point) const override;
		ECursor QueryCursor(FPoint Point) const override;

	public:
		void OnMouseDown(FMouseButtonEventArgs & Args) override;
		void OnMouseUp(FMouseButtonEventArgs & Args) override;
		void OnMouseMove(FMouseEventArgs & Args) override;
		void OnMouseLeave(FMouseEventArgs & Args) override;

	private:
		uintx FindResizingItem(FPoint Position) const;

	public:
		dimenx SplitterSize = 4.0f;

	private:
		FSize DragStartSize = FSize::NaN;
		FPoint DragStartPoint = FPoint::NaN;

	public:
		AttachedPropertyRW(bool, ResizeEnabled) = true;

		void OnIndicatingIndexChanged(uintx IndicatingIndexBefore);
		ProxyPropertyRW(uintx, IndicatingIndex) = NullIndex;

		void OnResigingIndexChanged(uintx ResigingIndexBefore);
		ProxyPropertyRW(uintx, ResizingIndex) = NullIndex;

	public:
		TEvent<void(FSplitterPanelIndicatingIndexChagnedEventArgs & EventArgs)> IndicatingIndexChanged;
		TEvent<void(FSplitterPanelResizingIndexChagnedEventArgs & EventArgs)> ResigingIndexChanged;
	};
	using USplitterPanelRef = TReferPtr<USplitterPanel>;
}
