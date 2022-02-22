#pragma once

#include "Control.h"

namespace Xin::UI
{
	class FDragStartedEventArgs : public FEventArgs
	{
	public:
		FDragStartedEventArgs(FPoint Position) : Position(Position) { }

		FPoint Position;
	};

	class FDragDeltaEventArgs : public FEventArgs
	{
	public:
		FDragDeltaEventArgs(FVec2 Delta, FPoint Position, FPoint StartPosition) : Delta(Delta), Position(Position), StartPosition(StartPosition) { }

		FVec2 Delta;
		FPoint Position;
		FPoint StartPosition;
	};

	class FDragCompletedEventArgs : public FEventArgs
	{
		
	};

	//[[Meta(OverrideMetadata(Bckground.AffectsRender, true)]]
	class UI_API UThumb : public UControl
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

		UThumb();
		UThumb(FName Name) : UControl(Name) {}
		~UThumb() = default;

	public:
		void OnConstruct() override;
		void OnPaint(IPainter & Painter) override;

		void OnMouseEnter(FMouseEventArgs & Args) override;
		void OnMouseLeave(FMouseEventArgs & Args) override;

		void OnMouseDown(FMouseButtonEventArgs & Args) override;
		void OnMouseUp(FMouseButtonEventArgs & Args) override;
		void OnMouseMove(FMouseEventArgs & Args) override;

	public:
		void OnMouseCapturedChanged(bool ValueBefore) override;

	private:
		// in view coordinate
		FPoint DragStartPoint = FPoint::NaN;
		FPoint DragLastPoint = FPoint::NaN;

	public:
		TEvent<void(FDragStartedEventArgs &)> DragStarted;
		TEvent<void(FDragDeltaEventArgs &)> DragDelta;
		TEvent<void(FDragCompletedEventArgs &)> DragCompleted;
	};
	using UThumbRef = TReferPtr<UThumb>;
}
