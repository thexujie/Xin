#pragma once

#include "D2DModule.h"

#include "Xin.Core/Xin.Core.h"
#include "Xin.DirectWrite/Xin.DirectWrite.h"

#include <wincodec.h>

#include <d2d1.h>
#include <d2d1_2.h>
#include <d2d1_3.h>
#include <dwrite.h>
#include <dwrite_1.h>
#include <dwrite_2.h>
#include <dwrite_3.h>

#include <d3d11.h>

#if XIN_DEBUG
#define XIN_DWRITE_DEBUG 1
#else
#define XIN_DWTEXT_DEBUG 0
#endif

namespace Xin::D2D
{
	forceinline DXGI_FORMAT FromFormat(EFormat Format)
	{
		return (DXGI_FORMAT)DXGIFormatFromFormat(Format);
	}

	forceinline EFormat ToFormat(DXGI_FORMAT Format)
	{
		return FormatFromDXGIFormat((EDXGIFormat)Format);
	}
}

namespace Xin::D2D
{
	using namespace Xin::Windows;
	using namespace DWrite;

	class FD2DGraphics;
	class FD2DPainter;
	class FD2DBitmap;
	
	using IDXGIFactoryRef = TReferPtr<IDXGIFactory>;
	using IDXGIFactory1Ref = TReferPtr<IDXGIFactory1>;

	using IDXGIAdapterRef = TReferPtr<IDXGIAdapter>;
	using IDXGIAdapter1Ref = TReferPtr<IDXGIAdapter1>;

	using IDXGIDeviceRef = TReferPtr<IDXGIDevice>;

	using IDXGIDeviceRef = TReferPtr<IDXGIDevice>;
	using IDXGISurfaceRef = TReferPtr<IDXGISurface>;

	using ID2D1FactoryRef = TReferPtr<ID2D1Factory>;
	using ID2D1Factory1Ref = TReferPtr<ID2D1Factory1>;
	using ID2D1Factory2Ref = TReferPtr<ID2D1Factory2>;
	using ID2D1Factory3Ref = TReferPtr<ID2D1Factory3>;

	using ID3D11DeviceRef = TReferPtr<ID3D11Device>;
	using ID3D11DeviceContextRef = TReferPtr<ID3D11DeviceContext>;
	using ID3D11ResourceRef = TReferPtr<ID3D11Resource>;

	using ID2D1DeviceRef = TReferPtr<ID2D1Device>;
	using ID2D1Device1Ref = TReferPtr<ID2D1Device1>;
	using ID2D1Device2Ref = TReferPtr<ID2D1Device2>;
	using ID2D1Device3Ref = TReferPtr<ID2D1Device3>;
	using ID2D1Device4Ref = TReferPtr<ID2D1Device4>;
	using ID2D1Device5Ref = TReferPtr<ID2D1Device5>;
	using ID2D1Device6Ref = TReferPtr<ID2D1Device6>;

	using ID2D1DeviceContextRef = TReferPtr<ID2D1DeviceContext>;
	using ID2D1DeviceContext1Ref = TReferPtr<ID2D1DeviceContext1>;
	using ID2D1DeviceContext2Ref = TReferPtr<ID2D1DeviceContext2>;
	using ID2D1DeviceContext3Ref = TReferPtr<ID2D1DeviceContext3>;
	using ID2D1DeviceContext4Ref = TReferPtr<ID2D1DeviceContext4>;
	using ID2D1DeviceContext5Ref = TReferPtr<ID2D1DeviceContext5>;
	using ID2D1DeviceContext6Ref = TReferPtr<ID2D1DeviceContext6>;

	using ID2D1BitmapRef = TReferPtr<ID2D1Bitmap>;
	using ID2D1Bitmap1Ref = TReferPtr<ID2D1Bitmap1>;

	using ID2D1RenderTargetRef = TReferPtr<ID2D1RenderTarget>;
	using ID2D1LayerRef = TReferPtr<ID2D1Layer>;

	using ID2D1BrushRef = TReferPtr<ID2D1Brush>;
	using ID2D1SolidColorBrushRef = TReferPtr<ID2D1SolidColorBrush>;

	using ID2D1StrokeStyleRef = TReferPtr<ID2D1StrokeStyle>;
	using ID2D1StrokeStyle1Ref = TReferPtr<ID2D1StrokeStyle1>;

	using IWICImagingFactoryRef = TReferPtr<IWICImagingFactory>;
	using IWICImagingFactory2Ref = TReferPtr<IWICImagingFactory2>;

	using IWICBitmapRef = TReferPtr<IWICBitmap>;

	using ID2D1GeometryRef = TReferPtr<ID2D1Geometry>;
	using ID2D1PathGeometryRef = TReferPtr<ID2D1PathGeometry>;
	using ID2D1PathGeometry1Ref = TReferPtr<ID2D1PathGeometry1>;
	using ID2D1TransformedGeometryRef = TReferPtr<ID2D1TransformedGeometry>;

	using ID2D1GeometrySinkRef = TReferPtr<ID2D1GeometrySink>;
	using ID2D1GeometryGroupRef = TReferPtr<ID2D1GeometryGroup>;

	constexpr D2D1_MATRIX_3X2_F D2D1MatrixIdentity = { { { 1, 0, 0, 1, 0, 0 } } };
	constexpr float32 D2DHalfOffsetValue = 0.0f;

	forceinline D2D1_COLOR_F AffineColor(const FColor & Color)
	{
		return D2D1_COLOR_F { Color.R / 255.0f, Color.G / 255.0f, Color.B / 255.0f, Color.A / 255.0f };
	}

	forceinline D2D1_POINT_2F AffinePoint(const PointF & Point)
	{
		return { Point.X + D2DHalfOffsetValue, Point.Y + D2DHalfOffsetValue };
	}

	forceinline D2D1_RECT_F AffineRect(const RectF & Rect)
	{
		return { Rect.X + D2DHalfOffsetValue, Rect.Y + D2DHalfOffsetValue, Rect.X + D2DHalfOffsetValue + Rect.Width, Rect.Y + D2DHalfOffsetValue + Rect.Height };
	}

	forceinline D2D1_ROUNDED_RECT AffineRoundRect(const RectF & Rect, const SizeF & Radius)
	{
		return { AffineRect(Rect), Radius.X, Radius.Y };
	}

	forceinline D2D1_RECT_F AffineClipRect(const RectF & Rect)
	{
		return { Floor(Rect.X) + D2DHalfOffsetValue, Floor(Rect.Y) + D2DHalfOffsetValue, Ceil(Rect.X + Rect.Width) + D2DHalfOffsetValue, Ceil(Rect.Y + Rect.Height) + D2DHalfOffsetValue };
	}

	forceinline D2D1_MATRIX_3X2_F AffineTransform(const FTransform3x2F & Transform)
	{
		static_assert(sizeof(D2D1_MATRIX_3X2_F) == sizeof(FTransform3x2F));
		if constexpr (D2DHalfOffsetValue == 0)
		{
			return *(const D2D1_MATRIX_3X2_F *)(&Transform);
		}
		else
		{
			D2D1_MATRIX_3X2_F D2D1Matrix;
			D2D1Matrix.m11 = Transform.M11;
			D2D1Matrix.m12 = Transform.M12;
			D2D1Matrix.m21 = Transform.M21;
			D2D1Matrix.m22 = Transform.M22;
			D2D1Matrix.dx = Transform.Trans.X + D2DHalfOffsetValue;
			D2D1Matrix.dy = Transform.Trans.Y + D2DHalfOffsetValue;
			return D2D1Matrix;
		}
	}

	forceinline D2D1_ALPHA_MODE AlphaModeFromFormat(EFormat Format)
	{
		switch (Format)
		{
		case EFormat::BGRX:
			return D2D1_ALPHA_MODE_IGNORE;
		case EFormat::BGRA:
		case EFormat::BGRAUF:
		case EFormat::BGRAUFsRGB:
		case EFormat::BGRXUFsRGB:
		case EFormat::RGBA:
		case EFormat::RGBAUF:
		case EFormat::RGBAUFsRGB:
		case EFormat::RGBAUI:
		case EFormat::A8:
		case EFormat::A8UF:
			return D2D1_ALPHA_MODE_PREMULTIPLIED;
		default:
			return D2D1_ALPHA_MODE_UNKNOWN;
		}
	}

	forceinline D2D1_MAP_OPTIONS FromBitmapLock(EBitmapLock BitmapLock)
	{
		switch (BitmapLock)
		{
		case EBitmapLock::Read:return D2D1_MAP_OPTIONS::D2D1_MAP_OPTIONS_READ;
		case EBitmapLock::Write:return D2D1_MAP_OPTIONS::D2D1_MAP_OPTIONS_WRITE;
		case EBitmapLock::Discard:return D2D1_MAP_OPTIONS::D2D1_MAP_OPTIONS_DISCARD;
		default: return D2D1_MAP_OPTIONS::D2D1_MAP_OPTIONS_NONE;
		}
	}

	class D2D_API FD2DBitmapBase : public IBitmap
	{
	public:
		FD2DBitmapBase() = default;
		FD2DBitmapBase(EFormat Format, SizeU Size, EBitmapUsage Usage) : IBitmap(Format, Size, Usage) {}
		FD2DBitmapBase(const FBitmapDesc & BitmapDesc) : IBitmap(BitmapDesc){}
		~FD2DBitmapBase() = default;

		virtual bool IsWICBitmap() const { return false; }
	};
}
