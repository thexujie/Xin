#include "PCH.h"
#include "Polyline.h"

namespace Xin::UI
{
	void UPolyline::OnInitialize()
	{
		UShape::OnInitialize();

		if (Points.Size > 1)
		{
			IPathRef PolylinePath = Graphics.CreatePath();
			PolylinePath->Open();
			PolylinePath->BeginFigure(Points[0]);
			for (uintx Index = 1; Index < Points.Size; ++Index)
				PolylinePath->AddLine(Points[Index]);
			PolylinePath->EndFigure(false);
			PolylinePath->Close();
			Path = PolylinePath;
		}
	}

	void UPolyline::OnFinalize()
	{
		Path = nullptr;
		UShape::OnFinalize();
	}

	FSize UPolyline::OnMeasure(FSize ConstrainedSize)
	{
		if (Path)
			return Path->GetBounds().Size;
		else
			return FSize::Zero;
	}

	void UPolyline::OnPaint(IPainter & Painter)
	{
		if (Path && Stroke && StrokeThickness > 0)
		{
			if (Stretch == EStretch::UniformFill)
			{
				RectF Bounds = Path->GetBounds();
				dimenx Scale = Min(FinalSize.Width / Bounds.Right, FinalSize.Height / Bounds.Bottom);
				Painter.DrawPath(Path.Ref(), FTransform3x2F { (float32)Scale, FVec2::Zero }, IPen(Stroke.Ref(), StrokeThickness, StrokeStyle));
			}
			else
			{
				Painter.DrawPath(Path.Ref(), FPoint(), IPen(Stroke.Ref(), StrokeThickness, StrokeStyle));
			}
		}
	}
}
