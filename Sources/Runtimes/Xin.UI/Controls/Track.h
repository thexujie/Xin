#pragma once

#include "Thumb.h"
#include "RepeatButton.h"

namespace Xin::UI
{
	/**
	 * \brief
	 * \ref https://learn.microsoft.com/zh-cn/dotnet/api/system.windows.controls.primitives.track
	 */
	class UI_API UTrack : public UElement
	{
	public:
		static FType & StaticType();

		FType & Type() const override { return StaticType(); }

		static constexpr FName Name_Thumb = u8"PART_Thumb"N;
		static constexpr FName Name_Increase = u8"PART_Increase"N;
		static constexpr FName Name_Decrease = u8"PART_Decrease"N;

	public:
		UTrack();
		~UTrack() = default;

	public:
		void OnInitialize() override;
		void OnFinalize() override;
		FSize OnMeasure(FSize ConstrainedSize) override;
		void OnArrange() override;
		void OnPaint(IPainter & Painter) override;

		void OnMouseDown(FMouseButtonEventArgs & Args) override;
		void OnMouseUp(FMouseButtonEventArgs & Args) override;
		void OnMouseEnter(FMouseEventArgs & Args) override;
		void OnMouseLeave(FMouseEventArgs & Args) override;

	public:
		void ArrangeParts();

	private:
		void OnThumbDragStarted(FDragStartedEventArgs & Args);
		void OnThumbDragDelta(FDragDeltaEventArgs & Args);

	public:
		[[Meta(AffectsArrange)]]
		DependencyPropertyRW(EOrientation, Orientation) = EOrientation::Horizontal;

		[[Meta(Hierarchical)]]
		DependencyPropertyRW(IBrushRef, Background) = noval;

	public:
		[[Meta(AffectsArrange)]]
		DependencyPropertyRW(dimenx, ViewportSize) = 0;

		[[Meta(AffectsArrange)]]
		DependencyPropertyRW(dimenx, Maximum) = 1;

		[[Meta(AffectsArrange)]]
		DependencyPropertyRW(dimenx, Minimum) = 0;

		[[Meta(AffectsArrange)]]
		DependencyPropertyRW(dimenx, Value) = 0;

	private:
		dimenx DimenPerPixel = 0.0f;
		FPoint DragStartPosition {};
		dimenx DragStartValue = NaN<dimenx>;;
		mutable TArray<UElementRef, 3> Elements;

	public:
		void OnThumbChanged(UThumbRef ThumbBefore);
		ProxyPropertyRW(UThumbRef, Thumb);

		void OnIncreaseButtonChanged(URepeatButtonRef IncreaseButtonBefore);
		ProxyPropertyRW(URepeatButtonRef, IncreaseButton);

		void OnDecreaseButtonChanged(URepeatButtonRef DecreaseButtonBefore);
		ProxyPropertyRW(URepeatButtonRef, DecreaseButton);
	};

	using UTrackRef = TReferPtr<UTrack>;
}
