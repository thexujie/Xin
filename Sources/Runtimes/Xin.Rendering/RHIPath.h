#pragma once

#include "Xin.Rendering.Types.h"
#include "RHIVertex.h"
#include "CommandList.h"

namespace Xin::Rendering
{
	class FRHIGraphics;

	class FRHIPathVertices : public IPathTessellateSink
	{
	public:
		void AddTriangles(TView<Vec2F> Points) override
		{
			for (const Vec2F & Point : Points)
				Vertices.Emplace(FVector4{ Point.X, Point.Y, 0.0f, 1.0f });
		}

		void AddTriangles(TView<Vec3F> Points) override
		{
			for (const Vec3F & Point : Points)
				Vertices.Emplace(FVector4{ Point.X, Point.Y, 0.0f, Point.Z });
		}

		TList<FRHIVertex2D> Vertices;
	};

	class RENDERING_API FRHIPath : public IPath
	{
	public:
		FRHIPath(FRHIGraphics & RHIGraphics);
		~FRHIPath() override;

	public:
		bool Open() override;
		void Close() override;
		void BeginFigure(const PointF & Point) override;
		void EndFigure(bool Close) override;
		void AddLine(const PointF & Point) override;
		void AddLines(TView<PointF> Points) override;
		void AddBezier(const PointF & ControlPoint1, const PointF & ControlPoint2, const PointF & EndPoint) override;
		void AddQuadraticBezier(const PointF & ControlPoint, const PointF & EndPoint) override;
		void AddArc(const SizeF & Radius, float32 RotationAngle, Point2F EndPoint, bool LargeArc, bool CCW) override;
		void Transform(const FTransform3x2F & Transform) override;
		void Transform(const FTransform3x2F & Transform, IPath & OutputPath) override;

	public:
		float32 GetLength(float32 StrokeThickness) const override { return Length; }
		float32 GetArea(float32 StrokeThickness) const override { return 0.0f; }
		RectF GetBounds() const override { return Bounds; }

	public:
		void Tessellate(IPathTessellateSink & GeometryTesselateSink, bool AntiAlias, float32 Tolerance) override {}
		void Widen(IPathTessellateSink & PathTessellateSink, float32 Thickness, bool AntiAlias, const FStrokeStyle & StrokeStyle, float32 Tolerance) override {}

	private:
		void UpdateVertices();

	public:
		FRHIGraphics & RHIGraphics;
		TList<FRHIVertex2D> Vertices;

	public:
		IPathRef OfflinePath;
		RectF Bounds = RectF::Zero;
		float32 Length = 0.0f;
	};

	using FRHIPathRef = TReferPtr<FRHIPath>;
}
