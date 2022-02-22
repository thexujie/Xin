#include "PCH.h"
#include "Path.h"

namespace Xin::UI
{
	void UPath::OnInitialize()
	{
		UElement::OnInitialize();
	}

	void UPath::OnFinalize()
	{
		Path = nullptr;
		UElement::OnFinalize();
	}

	FSize UPath::OnMeasure(FSize ConstrainedSize)
	{
		if (!Path)
		{
			OriginPath = Path = Graphics.CreatePath();
			Path->Open();
			IPath::ParsePathCommands(Command, *Path);
			if (__PathTransform.HasValue)
				Path->Transform(__PathTransform);
			Path->Close();

			if (Path->GetBounds().Position != PointF::Zero)
			{
				OriginPath = Graphics.CreatePath();
				Path->Transform(FTransform3x2F(Vec2F::One, -Path->GetBounds().Position), *OriginPath);
				Path = OriginPath;
			}
		}

		RectF Bounds = Path->GetBounds();
		return Bounds.Size + StrokeThickness;
	}

	void UPath::OnArranged()
	{
		UElement::OnArranged();

		Vec2F StrokeOffset = Vec2F(StrokeThickness * 0.5f, ESwizzleComponnet::XXXX);
		if (Stretch != EStretch::None)
		{
			Vec2F Scale = Vec2F::One;
			switch (Stretch)
			{
			default:
				break;
			case EStretch::Fill:
			{
				Scale = (FinalSize - StrokeThickness) / OriginPath->GetBounds().Size;
				break;
			}
			case EStretch::Uniform:
			{
				Scale = (FinalSize - StrokeThickness) / OriginPath->GetBounds().Size;
				Scale = Vec2F(Min(Scale.X, Scale.Y), ESwizzleComponnet::XXXX);
				break;
			}
			case EStretch::UniformFill:
			{
				Scale = (FinalSize - StrokeThickness) / OriginPath->GetBounds().Size;
				Scale = Vec2F(Max(Scale.X, Scale.Y), ESwizzleComponnet::XXXX);
				break;
			}
			}

			if (Scale != StretchScale)
			{
				StretchScale = Scale;

				Path = Graphics.CreatePath();
				OriginPath->Transform(FTransform3x2F(Scale, Vec2F::Zero), *Path);
				StretchOffset = (FinalSize - Path->GetBounds().Size) * 0.5f;
			}
		}
		else
			StretchOffset = StrokeOffset;
	}

	void UPath::OnPaint(IPainter & Painter)
	{
		if (Path)
		{
			if (Fill || (Stroke && StrokeThickness))
				Painter.DrawPath(Path.Ref(), StretchOffset, { Fill.Ref() }, { Stroke.Ref(), StrokeThickness });
		}
	}

	void UPath::OnPathTransformChanged(const FTransform3x2F & ValueBefore) {}
}
