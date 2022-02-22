#pragma once

#include "../Controls/Panel.h"
#include "Chrome.h"

namespace Xin::UI
{
	class UI_API UChromePanel : public UPanel, TIItemAddable<UChromeRef>
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

	public:
		UChromePanel() = default;
		UChromePanel(FName Name) : UPanel(Name) {}
		~UChromePanel() = default;

	public:
		void OnConstruct() override;
		void Initialize() override;
		void OnInitialize() override;
		FSize OnMeasure(FSize ConstrainedSize) override;
		void OnArrange() override;

	public:
		UElement * HitElement(FPoint Point) const override;
		ECursor QueryCursor(FPoint Point) const override;
		EHitResult HitTest(FPoint Point) const override;

	public:
		void OnMouseDown(FMouseButtonEventArgs & Args) override;
		void OnMouseUp(FMouseButtonEventArgs & Args) override;
		void OnMouseMove(FMouseEventArgs & Args) override;
		void OnMouseLeave(FMouseEventArgs & Args) override;

	public:
		UChromeRef FindChrome(FPoint Point) const;

		void ShowChromes();

		void BeginDragChrome(UChrome & Chrome);
		void EndDragChrome();

		void BeginResizeChrome(UChrome & Chrome, EHitResult ResizingComponnet);
		void EndResizeChrome();

		void EndAdjustChrome();

		void CloseChrome(UChrome & Chrome);
		void BringToTop(UChrome & Chrome);

	public:
		TItemCollection<UChromeRef> Chromes { *this };

	public:
		UChromeObj AdjustingChrome = nullptr;
		FPoint PreviousCursorPosition = FPoint::NaN;
		FPoint PreviousChromeLocation = FPoint::NaN;
		FSize PreviousChromeSize = FPoint::NaN;

		EHitResult ResizingChromeComponent = EHitResult::None;
	};
	using UChromePanelRef = TReferPtr<UChromePanel>;
}
