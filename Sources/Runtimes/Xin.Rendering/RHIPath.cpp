#include "PCH.h"
#include "RHIPath.h"

#include "RHIGraphics.h"

namespace Xin::Rendering
{
	FRHIPath::FRHIPath(FRHIGraphics & RHIGraphics) : RHIGraphics(RHIGraphics) {}

	FRHIPath::~FRHIPath() {}

	bool FRHIPath::Open()
	{
		OfflinePath = RHIGraphics.OfflineGraphics->CreatePath();
		OfflinePath->Open();
		return !!OfflinePath;
	}

	void FRHIPath::Close() 
	{
		if (!OfflinePath)
			return;

		OfflinePath->Close();
		UpdateVertices();
	}

	void FRHIPath::BeginFigure(const PointF & Point) 
	{
		OfflinePath->BeginFigure(Point);
	}

	void FRHIPath::EndFigure(bool Close) 
	{
		OfflinePath->EndFigure(Close);
	}

	void FRHIPath::AddLine(const PointF & Point) 
	{
		OfflinePath->AddLine(Point);
	}

	void FRHIPath::AddLines(TView<PointF> Points)
	{
		OfflinePath->AddLines(Points);
	}

	void FRHIPath::AddBezier(const PointF & ControlPoint1, const PointF & ControlPoint2, const PointF & EndPoint) 
	{
		OfflinePath->AddBezier(ControlPoint1, ControlPoint2, EndPoint);
	}

	void FRHIPath::AddQuadraticBezier(const PointF & ControlPoint, const PointF & EndPoint) 
	{
		OfflinePath->AddQuadraticBezier(ControlPoint, EndPoint);
	}

	void FRHIPath::AddArc(const SizeF & Radius, float32 RotationAngle, Point2F EndPoint, bool LargeArc, bool CCW) 
	{
		OfflinePath->AddArc(Radius, RotationAngle, EndPoint, LargeArc, CCW);
	}

	void FRHIPath::Transform(const FTransform3x2F & Transform) 
	{
		if (OfflinePath)
		{
			OfflinePath->Transform(Transform);
			UpdateVertices();
		}
	}

	void FRHIPath::Transform(const FTransform3x2F & Transform, IPath & OutputPath) 
	{
		if (OfflinePath)
		{
			FRHIPath & RHIOutputPath = StaticCast<FRHIPath>(OutputPath);
			RHIOutputPath.OfflinePath = RHIGraphics.OfflineGraphics->CreatePath();
			OfflinePath->Transform(Transform, *RHIOutputPath.OfflinePath);
			RHIOutputPath.UpdateVertices();
		}
	}

	void FRHIPath::UpdateVertices()
	{
		Vertices.Clear();

		FRHIPathVertices RHIPathVertices;
		OfflinePath->Tessellate(RHIPathVertices, true);
		Vertices = Move(RHIPathVertices.Vertices);

		Bounds = OfflinePath->GetBounds();
		Length = OfflinePath->GetLength(1.0f);
	}
}
