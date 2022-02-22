#include "PCH.h"
#include "Graph.h"

#define NANOSVG_IMPLEMENTATION
#include <Externals/nanosvg/include/nanosvg/nanosvg.h>

namespace Xin::UI
{
	void UGraph::OnInitialize()
	{
		UElement::OnInitialize();
		LoadSvg(SvgFilePath);
	}

	void UGraph::OnFinalize()
	{
		Shapes.Clear();
		UElement::OnFinalize();
	}

	FSize UGraph::OnMeasure(FSize ConstrainedSize)
	{
		return
		{
			(IsNaN(SvgSize.Width) || LessThanOrCloseZero(SvgSize.Width))? SvgBounds.Right : SvgSize.Width,
			(IsNaN(SvgSize.Height) || LessThanOrCloseZero(SvgSize.Height) )? SvgBounds.Bottom : SvgSize.Height
		};
	}

	void UGraph::OnPaint(IPainter & Painter)
	{
		for (const FShape & Shape : Shapes)
		{
			Painter.DrawPath(Shape.Path.Ref(), FPoint(),
				Shape.FillBrush ? Shape.FillBrush.Ref() : IBrush::None, { Shape.StrokeBrush ? Shape.StrokeBrush.Ref() : IBrush::None, Shape.StrokeThickness });
		}
	}

	static uint32 MakeColorFromSvgColor(uint32 color, float32 opacity)
	{
		return
			(Clamp((int32)opacity * 255, 0, 255) << 24) |
			((color >> 16) & 0xFF) |
			(color & 0x00FF00) |
			((color >> 0) & 0xFF) << 16;
	}

	void UGraph::LoadSvg(FStringV FilePath)
	{
		struct FUserData
		{
			UGraph & VectorGraph;
			IPathRef Path;

			IBrushRef FillBrush;
			IBrushRef StrokeBrush;
			float32 StrokeThickness;
			FTransform3x2F ShapeTransform;

			void BeginShape(NSVGattrib * Attr)
			{
				AssertExpr(!Path);

				Path = VectorGraph.Graphics.CreatePath();
				Path->Open();
				if (Attr->hasStroke)
				{
					StrokeBrush = MakeRefer<FSolidColorBrush>(MakeColorFromSvgColor(Attr->strokeColor, Attr->strokeOpacity));
					StrokeThickness = Attr->strokeWidth;
				}

				if (Attr->hasFill)
					FillBrush = MakeRefer<FSolidColorBrush>(MakeColorFromSvgColor(Attr->fillColor, Attr->fillOpacity));

				ShapeTransform = { { Attr->xform[0] * VectorGraph.SvgScale.X, Attr->xform[3] * VectorGraph.SvgScale.Y }, { Attr->xform[4] * VectorGraph.SvgScale.X, Attr->xform[5] * VectorGraph.SvgScale.Y } };
			}

			void EndPath(bool Close)
			{
				Path->EndFigure(Close);
			}

			void EndShape()
			{
				AssertExpr(Path);

				FShape & Shape = VectorGraph.Shapes.AddDefault();
				Path->Close();

				Path->Transform(ShapeTransform);

				Shape.Path = Move(Path);
				Shape.FillBrush = Move(FillBrush);
				Shape.StrokeBrush = Move(StrokeBrush);
				Shape.StrokeThickness = StrokeThickness;
			}
		};

		FUserData UserData { *this };

		NSVGVtbl Vtbl;
		Vtbl.ud = &UserData;
		Vtbl.moveTo = [](void * ud, float x, float y) { return ((FUserData *)ud)->Path->BeginFigure({ x, y }); };
		Vtbl.lineTo = [](void * ud, float x, float y) { return ((FUserData *)ud)->Path->AddLine({ x, y }); };
		Vtbl.cubicBezTo = [](void * ud, float cx1, float cy1, float cx2, float cy2, float x2, float y2)
			{ return ((FUserData *)ud)->Path->AddBezier({ cx1, cy1 }, { cx2, cy2 }, { x2, y2 }); };
		Vtbl.quadBezTo = [](void * ud, float cx, float cy, float x2, float y2)
			{ return ((FUserData *)ud)->Path->AddQuadraticBezier({ cx, cy }, { x2, y2 }); };
		Vtbl.arcTo = [](void * ud, float rx, float ry, float rotx, float x2, float y2, bool l, bool sd)
			{ return ((FUserData *)ud)->Path->AddArc({ rx, ry }, rotx, { x2, y2 }, l, !sd); };

		Vtbl.beginPath = [](void * ud) {};
		Vtbl.endPath = [](void * ud, bool c) { ((FUserData *)ud)->EndPath(c); };

		Vtbl.beginShape = [](void * ud, struct NSVGattrib * a) {((FUserData *)ud)->BeginShape(a); };
		Vtbl.endShape = [](void * ud) { ((FUserData *)ud)->EndShape(); };

		FString Content = FFile::ReadString(FilePath);

		NSVGparser Parser;
		nsvg__initParser(&Parser);
		Parser.vtbl = &Vtbl;
		Parser.dpi = 96;

		nsvgParse(&Parser, (chara *)Content.Data);

		SvgSize = FSize { Parser.width, Parser.height } * SvgScale;

		Vec4F Bounds = { Vec2F::Infinity, -Vec2F::Infinity };
		for (const FShape & Shape : Shapes)
		{
			Vec4F ShapeBounds = Shape.Path->GetBounds();
			Bounds.X = Min(Bounds.X, ShapeBounds.X);
			Bounds.Y = Min(Bounds.Y, ShapeBounds.Y);
			Bounds.Z = Max(Bounds.Z, ShapeBounds.Z);
			Bounds.W = Max(Bounds.W, ShapeBounds.W);
		}
		SvgBounds = Bounds;
	}
}

