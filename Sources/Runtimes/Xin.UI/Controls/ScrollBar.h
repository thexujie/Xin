#pragma once

#include "Control.h"
#include "Track.h"

namespace Xin::UI
{
	/**
	 * \brief
	 * \ref https://learn.microsoft.com/zh-cn/dotnet/api/system.windows.controls.primitives.scrollbar
	 */
	class UI_API UScrollBar : public UControl
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

	public:
		static constexpr FName Name_Track = u8"PART_Track"N;

	public:
		UScrollBar();
		UScrollBar(FName Name) : UControl(Name) {}
		~UScrollBar() = default;
		void OnConstruct() override;

	public:
		void OnInitialize() override;
		FSize OnMeasure(FSize ConstrainedSize) override;
		void OnArrange() override;

	public:
		void OnPaint(IPainter & Painter) override;

		void OnMouseDown(FMouseButtonEventArgs & Args) override;
		void OnMouseUp(FMouseButtonEventArgs & Args) override;
		void OnMouseEnter(FMouseEventArgs & Args) override;
		void OnMouseLeave(FMouseEventArgs & Args) override;

	public:
		void OnLineUpCommand(FRoutedEventArgs & RoutedEventArgs);
		void OnLineDownCommand(FRoutedEventArgs & RoutedEventArgs);
		void OnPageUpCommand(FRoutedEventArgs & RoutedEventArgs);
		void OnPageDownCommand(FRoutedEventArgs & RoutedEventArgs);

	public:
		DependencyPropertyRW(EOrientation, Orientation) = EOrientation::Horizontal;
		DependencyPropertyRW(dimenx, ViewportSize) = 0;
		DependencyPropertyRW(dimenx, Maximum) = 1;
		DependencyPropertyRW(dimenx, Minimum) = 0;
		DependencyPropertyRW(dimenx, Value) = 0;

	public:
		FDelegate LineUp;
		FDelegate LineDown;
		FDelegate PageUp;
		FDelegate PageDown;

	public:
		static ICommandRef LineUpCommand;
		static ICommandRef LineDownCommand;
		static ICommandRef PageUpCommand;
		static ICommandRef PageDownCommand;
	};
	using UScrollBarRef = TReferPtr<UScrollBar>;
}
