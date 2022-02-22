#include "PCH.h"
#include "SkiaPath.h"
#include "SkiaGraphics.h"

#include <include/core/SkStrokeRec.h>

namespace Xin::Skia
{
	FSkiaPath::FSkiaPath(FSkiaGraphics & SkiaDevice) : SkiaDevice(SkiaDevice) {}

	FSkiaPath::FSkiaPath(FSkiaGraphics & SkiaDevice, TUniquePtr<SkPath> && skPath) : SkiaDevice(SkiaDevice), skPath(Move(skPath)) {}

	FSkiaPath::~FSkiaPath() {}

	bool FSkiaPath::Open()
	{
		return true;
	}

	void FSkiaPath::Close() {}

	void FSkiaPath::BeginFigure(const PointF & Point)
	{
		if (!skPath)
			skPath = new SkPath();
		skPath->moveTo(FromPoint(Point));
	}

	void FSkiaPath::EndFigure(bool Close)
	{
		AssertExpr(skPath);

		if (Close)
			skPath->close();
	}

	void FSkiaPath::AddLine(const PointF & Point)
	{
		AssertExpr(skPath);
		skPath->lineTo(FromPoint(Point));
	}

	void FSkiaPath::AddLines(TView<PointF> Points)
	{
		AssertExpr(skPath);
		for (uintx PointIndex = 0; PointIndex < Points.Size; ++PointIndex)
			skPath->lineTo({ Points[PointIndex].X, Points[PointIndex].Y });
	}

	void FSkiaPath::AddBezier(const PointF & ControlPoint1, const PointF & ControlPoint2, const PointF & EndPoint)
	{
		AssertExpr(skPath);
		skPath->cubicTo(FromPoint(ControlPoint1), FromPoint(ControlPoint2), FromPoint(EndPoint));
	}

	void FSkiaPath::AddQuadraticBezier(const PointF & ControlPoint, const PointF & EndPoint)
	{
		AssertExpr(skPath);
		skPath->cubicTo(FromPoint(ControlPoint), FromPoint(ControlPoint), FromPoint(EndPoint));
		//skPath->rCubicTo(FromPoint(ControlPoint1), FromPoint(ControlPoint2), FromPoint(EndPoint));
	}

	void FSkiaPath::AddArc(const SizeF & Radius, float32 RotationAngle, Point2F EndPoint, bool LargeArc, bool CCW)
	{
		AssertExpr(skPath);
		skPath->arcTo(FromPoint(Radius), RotationAngle, LargeArc ? SkPath::kLarge_ArcSize : SkPath::kSmall_ArcSize, CCW ? SkPathDirection::kCCW : SkPathDirection::kCW, FromPoint(EndPoint));
	}

	float32 FSkiaPath::GetLength(float32 StrokeThickness) const
	{
		return 0.0f;
	}

	float32 FSkiaPath::GetArea(float32 StrokeThickness) const
	{
		return 0.0f;
	}

	Vec4F FSkiaPath::GetBounds() const
	{
		if (Bounds.Size.AnyNaN())
			Bounds = skPath ? ToRect(skPath->getBounds()) : RectF();
		return Bounds;
	}

	void FSkiaPath::Transform(const FTransform3x2F & Transform)
	{
		skPath->transform(FromTransform(Transform));
		Bounds = RectF::NaN;
		Length = NaN<float32>;
	}

	void FSkiaPath::Transform(const FTransform3x2F & Transform, IPath & OutputPath)
	{
		FSkiaPath & OutputSkiaPath = StaticCast<FSkiaPath>(OutputPath);
		if (!OutputSkiaPath.skPath)
			OutputSkiaPath.skPath = new SkPath();

		skPath->transform(FromTransform(Transform), OutputSkiaPath.skPath);
	}

	class FSkiaPathTessellateSink : public SkPath::IPathTessellateSink
	{
	public:
		FSkiaPathTessellateSink(Xin::IPathTessellateSink & PathTessellateSink, bool AntiAlias)
			: PathTessellateSink(PathTessellateSink), AntiAlias(AntiAlias) {}

		void addTriangles(int32 numTriangles, const float32 * Vertices) override
		{
			if (AntiAlias)
				PathTessellateSink.AddTriangles({ (const Vec3F *)Vertices, uintx(numTriangles * 3) });
			else
				PathTessellateSink.AddTriangles({ (const Vec2F *)Vertices, uintx(numTriangles * 3) });
		}

	public:
		Xin::IPathTessellateSink & PathTessellateSink;
		bool AntiAlias = false;
	};

	void FSkiaPath::Tessellate(IPathTessellateSink & PathTessellateSink, bool AntiAlias, float32 Tolerance)
	{
		if (!skPath || skPath->isEmpty())
			return;

		FSkiaPathTessellateSink PathTessellate { PathTessellateSink, AntiAlias };

		if (AntiAlias)
			skPath->toAATriangles(Tolerance, SkRect(), &PathTessellate);
		else
			skPath->toTriangles(Tolerance, SkRect(), &PathTessellate);
	}

	void FSkiaPath::Widen(IPathTessellateSink & PathTessellateSink, float32 Thickness, bool AntiAlias, const FStrokeStyle & StrokeStyle, float32 Tolerance)
	{
		if (!skPath || skPath->isEmpty())
			return;

		SkStrokeRec skStrokeRec { SkStrokeRec::InitStyle::kFill_InitStyle };
		skStrokeRec.setStrokeStyle(Thickness, false);

		SkPath skPathStroked;
		skStrokeRec.applyToPath(&skPathStroked, *skPath);

		FSkiaPathTessellateSink PathTessellate { PathTessellateSink, AntiAlias };

		if (AntiAlias)
			skPathStroked.toAATriangles(Tolerance, SkRect(), &PathTessellate);
		else
			skPathStroked.toTriangles(Tolerance, SkRect(), &PathTessellate);
	}
}
