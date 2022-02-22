#pragma once

#include "SkiaTypes.h"

namespace Xin::Skia
{
	class SKIA_API FSkiaPath final : public IPath
	{
	public:
		FSkiaPath(FSkiaGraphics & SkiaDevice);
		FSkiaPath(FSkiaGraphics & SkiaDevice, TUniquePtr<SkPath> && skPath);
		~FSkiaPath() override;
		FTypeid Typeid() const override { return typeid(FSkiaPath); }

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
		float32 GetLength(float32 StrokeThickness) const override;
		float32 GetArea(float32 StrokeThickness) const override;
		RectF GetBounds() const override;

	public:
		void Transform(const FTransform3x2F & Transform) override;
		void Transform(const FTransform3x2F & Transform, IPath & OutputPath) override;
		void Tessellate(IPathTessellateSink & PathTessellateSink, bool AntiAlias = false, float32 Tolerance = X2DFlatteningTolerance) override;
		void Widen(IPathTessellateSink & PathTessellateSink, float32 Thickness, bool AntiAlias = false, const FStrokeStyle & StrokeStyle = {}, float32 Tolerance = X2DFlatteningTolerance) override;

		FSkiaGraphics & SkiaDevice;
		TUniquePtr<SkPath> skPath;

	private:
		mutable float32 Length = NaN<float32>;
		mutable float32 Area = NaN<float32>;
		mutable RectF Bounds = RectF::NaN;
	};
	using FSkiaBitmapRef = TReferPtr<FSkiaBitmap>;
}
