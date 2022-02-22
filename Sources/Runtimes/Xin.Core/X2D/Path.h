#pragma once

#include "X2D.Types.h"
#include "Transform3x2.h"

namespace Xin
{
	class IPathTessellateSink
	{
	public:
		virtual ~IPathTessellateSink() = default;
		virtual void AddTriangles(TView<Vec2F> Points) = 0;
		virtual void AddTriangles(TView<Vec3F> Points) = 0;
	};

	// https://learn.microsoft.com/en-us/dotnet/desktop/wpf/graphics-multimedia/path-markup-syntax
	enum class EPathCommand
	{
		None,
		Move,
		// M m
		Line,
		// L l
		HorizontalLine,
		// H h
		VerticalLine,
		// V v
		CubicBezier,
		// C c
		QuadraticBezier,
		// Q q
		SmoothCubicBezier,
		// S s
		SmoothQuadraticBezier,
		// T t
		Arc,
		// A a
		Close,
		// Z z
	};

	class CORE_API IPath : public IX2DObject
	{
	public:
		virtual bool Open() = 0;
		virtual void Close() = 0;
		virtual void BeginFigure(const PointF & Point) = 0;
		virtual void EndFigure(bool Close) = 0;

	public:
		virtual void AddLine(const PointF & Point) = 0;
		virtual void AddLines(TView<PointF>  Points) = 0;
		virtual void AddBezier(const PointF & ControlPoint1, const PointF & ControlPoint2, const PointF & EndPoint) = 0;
		virtual void AddQuadraticBezier(const PointF & ControlPoint, const PointF & EndPoint) = 0;
		virtual void AddArc(const SizeF & Radius, float32 RotationAngle, Point2F EndPoint, bool LargeArc = false, bool CCW = false) = 0;

	public:
		virtual void Transform(const FTransform3x2F & Transform) = 0;
		virtual void Transform(const FTransform3x2F & Transform, IPath & OutputPath) = 0;
		virtual void Tessellate(IPathTessellateSink & PathTessellateSink, bool AntiAlias = false, float32 Tolerance = X2DFlatteningTolerance) = 0;
		virtual void Widen(IPathTessellateSink & PathTessellateSink, float32 Thickness, bool AntiAlias = false, const FStrokeStyle & StrokeStyle = {}, float32 Tolerance = X2DFlatteningTolerance) = 0;

	public:
		virtual float32 GetLength(float32 StrokeThickness) const = 0;
		virtual float32 GetArea(float32 StrokeThickness) const = 0;
		virtual RectF GetBounds() const = 0;

	public:
		static uintx ParsePathCommands(FStringV String, IPath & Path);
	};
	using IPathRef = TReferPtr<IPath>;
}
