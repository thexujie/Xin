#include "PCH.h"
#include "Line.h"

namespace Xin::UI
{
	FSize ULine::OnMeasure(FSize ConstrainedSize)
	{
		//FSize MeasuredSize { Max(From.X, To.X), Max(From.Y, To.Y) };
		//if (Width.IsAbsolute())
		//	MeasuredSize.Width = Width.Value * MeasuredSize.Width;
		//if (Height.IsAbsolute())
		//	MeasuredSize.Height = Height.Value * MeasuredSize.Height;
		return FSize::Zero;
	}

	void ULine::OnPaint(IPainter & Painter)
	{
		if (Stroke && StrokeThickness > 0)
		{
			switch(CoordinateMode)
			{
			default:
			case ECoordinateMode::Absolute:
				Painter.DrawLine(From, To, IPen(*Stroke, StrokeThickness));
				break;
			case ECoordinateMode::Relative:
			{
				Painter.DrawLine(From * FinalRect.Size, To * FinalRect.Size, IPen(*Stroke, StrokeThickness));
				break;
			}
			}
		}
	}

	FSize UHLine::OnMeasure(FSize ConstrainedSize)
	{
		return { 0dim, StrokeThickness };
	}

	void UHLine::OnPaint(IPainter & Painter)
	{
		if (Stroke && StrokeThickness > 0)
			Painter.DrawLine(FPoint { 0.0f, FinalSize.Height * 0.5f }, FPoint { FinalSize.Width, FinalSize.Height * 0.5f}, IPen(*Stroke, StrokeThickness));
	}

	FSize UVLine::OnMeasure(FSize ConstrainedSize)
	{
		return { StrokeThickness, NaN<dimenx> };
	}

	void UVLine::OnPaint(IPainter & Painter)
	{
		if (Stroke && StrokeThickness > 0)
			Painter.DrawLine(FPoint { FinalSize.Width * 0.5f, 0.0f }, FPoint { FinalSize.Width * 0.5f, FinalSize.Height }, IPen(*Stroke, StrokeThickness));
	}
}

