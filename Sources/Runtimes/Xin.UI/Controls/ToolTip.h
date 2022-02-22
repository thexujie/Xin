#pragma once

#include "Xin.UI.Types.h"
#include "Popup.h"

namespace Xin::UI
{
	class UI_API UToolTip : public UPopup
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

	public:
		UToolTip();
		~UToolTip();

	public:
		void OnConstruct() override;
		void OnInitialize() override;
		void OnFinalize() override;
		void OnHidden() override;
		void OnTrackElementChanged(UElementObj TrackElementBefore) override;

	public:
		void ShowElementToolTip(UElement * Element);

	private:
		void OnTimerTick();
		void OnHostMouseMove(FMouseEventArgs & EventArgs);

	private:
		FDispatcherTimer ToolTipTimer;
		UElementObj ToolTipElement;

	private:
		static const uint32 DefaultToolTipDelay = 500;
		static const uint32 DefaultToolTipDuration = 5000;
	};
	using UToolTipRef = TReferPtr<UToolTip>;
	using UToolTipObj = TReferPtr<UToolTip>;
}
