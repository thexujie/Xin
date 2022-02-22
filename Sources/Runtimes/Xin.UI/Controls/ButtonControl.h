#pragma once

#include "Xin.UI.Types.h"
#include "ContentControl.h"

namespace Xin::UI
{
	enum class EClickMode
	{
		None = 0,
		Press,
		Release,
	};

	class UI_API UButtonControl : public UContentControl
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

		UButtonControl();
		UButtonControl(FName Name) : UContentControl(Name) {}
		~UButtonControl() = default;

	public:
		void OnConstruct() override;
		void OnPaint(IPainter & Painter) override;
		void UpdateState() override;

		void OnMouseDown(FMouseButtonEventArgs & Args) override;
		void OnMouseUp(FMouseButtonEventArgs & Args) override;
		void OnMouseEnter(FMouseEventArgs & Args) override;
		void OnMouseLeave(FMouseEventArgs & Args) override;

	public:
		void OnClick()
		{
			FRoutedEventArgs RoutedEventArgs { *this };
			OnClick(RoutedEventArgs);
		}

		virtual void OnClick(FRoutedEventArgs & Args);

	public:
		EClickMode ClickMode = EClickMode::Release;

	public:
		[[Meta(AffectsState)]]
		DependencyPropertyRW(bool, IsPressed) = false;

		DependencyPropertyRW(ICommandRef, Command);

	public:
		FRoutedEvent Click;
	};
	using UButtonBaseRef = TReferPtr<UButtonControl>;
}
