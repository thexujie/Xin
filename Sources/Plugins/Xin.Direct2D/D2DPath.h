#pragma once

#include "D2DTypes.h"

namespace Xin::D2D
{
	class D2D_API FD2DPath final : public IPath
	{
	public:
		FD2DPath(FD2DGraphics & D2DDevice);
		FD2DPath(FD2DGraphics & D2DDevice, ID2D1Geometry * d2d1Geometry);
		~FD2DPath() override;
		FTypeid Typeid() const override { return typeid(FD2DPath); }

	public:
		float32 GetLength(float32 StrokeThickness) const override;
		float32 GetArea(float32 StrokeThickness) const override;
		Vec4F GetBounds() const override;

	public:
		bool Open() override;
		void Close() override;
		void BeginFigure(const PointF & Point)override;
		void EndFigure(bool Close)override;

		void AddLine(const PointF & Point) override;
		void AddLines(TView<PointF> Points) override;
		void AddBezier(const PointF & ControlPoint1, const PointF & ControlPoint2, const PointF & EndPoint) override;
		void AddQuadraticBezier(const PointF & ControlPoint, const PointF & EndPoint) override;
		void AddArc(const SizeF & Radius, float32 RotationAngle, Point2F EndPoint, bool LargeArc, bool CCW) override;

	public:
		void Transform(const FTransform3x2F & Transform) override;
		void Transform(const FTransform3x2F & Transform, IPath & OutputPath) override;
		void Tessellate(IPathTessellateSink & PathTessellateSink, bool AntiAlias = false, float32 Tolerance = X2DFlatteningTolerance) override;
		void Widen(IPathTessellateSink & PathTessellateSink, float32 Thickness, bool AntiAlias = false, const FStrokeStyle & StrokeStyle = {}, float32 Tolerance = X2DFlatteningTolerance) override;

	public:
		FD2DGraphics & D2DGraphics;
		ID2D1GeometryRef d2d1Geometry;

	private:
		ID2D1PathGeometryRef d2d1PathGeometry;
		ID2D1TransformedGeometryRef d2d1TransformedGeometry;

		ID2D1GeometrySinkRef d2d1GeometrySink;
		uintx Figuring = 0;
		uintx FigureIndex = 0;

		mutable float32 Length = NaN<float32>;
		mutable float32 Area = NaN<float32>;
		mutable RectF Bounds = RectF::NaN;
	};
	using FD2DBitmapRef = TReferPtr<FD2DBitmap>;
}
