#include "PCH.h"
#include "D2DPath.h"
#include "D2DGraphics.h"

namespace Xin::D2D
{
	FD2DPath::FD2DPath(FD2DGraphics & D2DDevice) : D2DGraphics(D2DDevice) {}

	FD2DPath::FD2DPath(FD2DGraphics & D2DDevice, ID2D1Geometry * d2d1Geometry) : D2DGraphics(D2DDevice), d2d1Geometry(d2d1Geometry) {}

	FD2DPath::~FD2DPath() { }

	bool FD2DPath::Open()
	{
		AssertExpr(!d2d1GeometrySink);
		HRESULT Result;
		if (!d2d1PathGeometry)
		{
			Result = D2DGraphics.d2d1Factory1->CreatePathGeometry(d2d1PathGeometry.GetPP());
			d2d1Geometry = d2d1PathGeometry;
		}

		Result = d2d1PathGeometry->Open(d2d1GeometrySink.GetPP());
		return Succeeded(Result);
	}

	void FD2DPath::Close()
	{
		if (d2d1GeometrySink)
		{
			AssertExpr(Figuring == 0);
			d2d1GeometrySink->Close();
			d2d1GeometrySink = nullptr;
		}
	}

	void FD2DPath::BeginFigure(const PointF & Point)
	{
		AssertExpr(d2d1GeometrySink);
		++Figuring;
		d2d1GeometrySink->BeginFigure({ Point.X, Point.Y }, D2D1_FIGURE_BEGIN_FILLED);
	}

	void FD2DPath::EndFigure(bool Close)
	{
		++FigureIndex;
		AssertExpr(Figuring > 0);
		AssertExpr(Figuring == 1);
		AssertExpr(d2d1GeometrySink);
		--Figuring;
		d2d1GeometrySink->EndFigure(Close ? D2D1_FIGURE_END_CLOSED : D2D1_FIGURE_END_OPEN);
	}

	void FD2DPath::AddLine(const PointF & Point)
	{
		AssertExpr(Figuring);
		AssertExpr(d2d1GeometrySink);
		d2d1GeometrySink->AddLine({ Point.X, Point.Y });
	}

	void FD2DPath::AddLines(TView<PointF> Points)
	{
		AssertExpr(Figuring);
		AssertExpr(d2d1GeometrySink);
		for (uintx PointIndex = 0; PointIndex < Points.Size; ++PointIndex)
			d2d1GeometrySink->AddLine({ Points[PointIndex].X, Points[PointIndex].Y});
	}

	void FD2DPath::AddBezier(const PointF & ControlPoint1, const PointF & ControlPoint2, const PointF & EndPoint)
	{
		AssertExpr(Figuring);
		AssertExpr(d2d1GeometrySink);
		d2d1GeometrySink->AddBezier(D2D1_BEZIER_SEGMENT {
			{ ControlPoint1.X, ControlPoint1.Y },
			{ ControlPoint2.X, ControlPoint2.Y },
			{ EndPoint.X, EndPoint.Y }
			});
	}

	void FD2DPath::AddQuadraticBezier(const PointF & ControlPoint, const PointF & EndPoint)
	{
		AssertExpr(Figuring);
		AssertExpr(d2d1GeometrySink);
		d2d1GeometrySink->AddQuadraticBezier(D2D1_QUADRATIC_BEZIER_SEGMENT {
			{ ControlPoint.X, ControlPoint.Y },
			{ EndPoint.X, EndPoint.Y }
		});
	}

	void FD2DPath::AddArc(const SizeF & Radius, float32 RotationAngle, Point2F EndPoint, bool LargeArc, bool CCW)
	{
		AssertExpr(Figuring);
		AssertExpr(d2d1GeometrySink);
		d2d1GeometrySink->AddArc(D2D1_ARC_SEGMENT {
			{ EndPoint.X, EndPoint.Y },
			{ Radius.X, Radius.Y },
			RotationAngle,
			CCW ? D2D1_SWEEP_DIRECTION_COUNTER_CLOCKWISE : D2D1_SWEEP_DIRECTION_CLOCKWISE,
			LargeArc ? D2D1_ARC_SIZE_LARGE : D2D1_ARC_SIZE_SMALL
		});
	}

	float32 FD2DPath::GetLength(float32 StrokeThickness) const
	{
		if (!IsNaN(Length))
			return Length;

		if (!d2d1Geometry)
		{
			Length = 0.0f;
			return Length;
		}

		float32 ComputedLength;
		HRESULT Result = d2d1Geometry->ComputeLength(nullptr, StrokeThickness, &ComputedLength);
		AssertExpr(Succeeded(Result));
		Length = ComputedLength;
		return Length;
	}

	float32 FD2DPath::GetArea(float32 StrokeThickness) const
	{
		if (!IsNaN(Area))
			return Area;

		if (!d2d1Geometry)
		{
			Area = 0.0f;
			return Area;
		}

		float32 ComputedArea;
		HRESULT Result = d2d1Geometry->ComputeArea(nullptr, StrokeThickness, &ComputedArea);
		AssertExpr(Succeeded(Result));
		Length = ComputedArea;
		return Length;
	}

	Vec4F FD2DPath::GetBounds() const
	{
		if (!Bounds.AnyNaN())
			return Bounds;

		if (!d2d1Geometry)
		{
			Bounds = RectF::Zero;
			return Bounds;
		}

		D2D1_RECT_F ComputedBounds;
		HRESULT Result = d2d1Geometry->GetBounds(nullptr, &ComputedBounds);
		AssertExpr(Succeeded(Result));
		Bounds = { ComputedBounds.left, ComputedBounds.top, ComputedBounds.right, ComputedBounds.bottom  };
		return Bounds;
	}

	void FD2DPath::Transform(const FTransform3x2F & Transform)
	{
		if (!d2d1Geometry)
			return;

		ID2D1TransformedGeometryRef d2d1TransformedPathGeometry;
		D2DGraphics.d2d1Factory1->CreateTransformedGeometry(d2d1Geometry, AffineTransform(Transform), d2d1TransformedPathGeometry.GetPP());

		d2d1TransformedGeometry = d2d1TransformedPathGeometry;
		d2d1Geometry = d2d1TransformedPathGeometry;

		Bounds = RectF::NaN;
		Length = NaN<float32>;
	}

	void FD2DPath::Transform(const FTransform3x2F & Transform, IPath & OutputPath)
	{
		if (!d2d1Geometry)
			return;

		ID2D1TransformedGeometryRef d2d1TransformedPathGeometry;
		D2DGraphics.d2d1Factory1->CreateTransformedGeometry(d2d1Geometry, AffineTransform(Transform), d2d1TransformedPathGeometry.GetPP());

		StaticCast<FD2DPath>(OutputPath).d2d1Geometry = d2d1TransformedPathGeometry;
		StaticCast<FD2DPath>(OutputPath).d2d1PathGeometry = nullptr;
		StaticCast<FD2DPath>(OutputPath).d2d1TransformedGeometry = d2d1TransformedPathGeometry;
	}

	static void D2DTessellateGeometry(ID2D1Geometry & d2d1Geometry, IPathTessellateSink & PathTessellateSink, float32 Tolerance)
	{
		class FD2D1TessellationSink : public TUnknown<ID2D1TessellationSink>
		{
		public:
			FD2D1TessellationSink(IPathTessellateSink & GeometryTesselateSink) : GeometryTesselateSink(GeometryTesselateSink) {}

			void AddTriangles(const D2D1_TRIANGLE * Triangles, UINT32 TrianglesCount) override
			{
				const PointF * Points = (const PointF *)Triangles;
				GeometryTesselateSink.AddTriangles({ Points, TrianglesCount * 3 });
			}

			HRESULT Close() override
			{
				return S_OK;
			}

		public:
			IPathTessellateSink & GeometryTesselateSink;
		};

		FD2D1TessellationSink TessellationSink { PathTessellateSink };
		d2d1Geometry.Tessellate(nullptr, Tolerance, &TessellationSink);
	}

	void FD2DPath::Tessellate(IPathTessellateSink & PathTessellateSink, bool AntiAlias, float32 Tolerance)
	{
		if (!d2d1Geometry)
			return;

		D2DTessellateGeometry(*d2d1Geometry, PathTessellateSink, Tolerance);
	}

	void FD2DPath::Widen(IPathTessellateSink & PathTessellateSink, float32 Thickness, bool AntiAlias, const FStrokeStyle & StrokeStyle, float32 Tolerance)
	{
		if (!d2d1Geometry)
			return;

		ID2D1PathGeometryRef d2d1PathGeometryWiden;
		HRESULT Result = D2DGraphics.d2d1Factory1->CreatePathGeometry(d2d1PathGeometryWiden.GetPP());

		ID2D1GeometrySinkRef d2d1GeometrySinkWiden;
		Result = d2d1PathGeometryWiden->Open(d2d1GeometrySinkWiden.GetPP());
		Result = d2d1Geometry->Widen(Thickness, nullptr, nullptr, d2d1GeometrySinkWiden);
		d2d1GeometrySinkWiden->Close();

		D2DTessellateGeometry(*d2d1PathGeometryWiden, PathTessellateSink, Tolerance);
	}
}
