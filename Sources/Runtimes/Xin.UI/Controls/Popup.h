#pragma once

#include "Xin.UI.Types.h"
#include "Window.h"

namespace Xin::UI
{
	class UI_API UPopup : public UWindow
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

	public:
		UPopup();
		~UPopup();

	public:
		void OnInitialize() override;
		void OnFinalize() override;
		void OnDesiredSizeChanged(FSize DesiredSizeBefore) override;
		void OnShowing(FEventArgs & Args) override;

	protected:
		void UpdateWindowLocation();

	public:
		virtual void OnTrackElementChanged(UElementObj TrackElementBefore) {}
		ProxyPropertyRW(UElementObj, TrackElement);

	public:
		DependencyPropertyRW(EPopupPosition, PopupPosition) = EPopupPosition::Bottom;
		DependencyPropertyRW(FSize, PlacementOffset) = FPoint::Zero;
	};
}
