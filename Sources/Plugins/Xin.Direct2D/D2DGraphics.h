#pragma once

#include "D2DTypes.h"

#include <d3d11on12.h>

namespace Xin::D2D
{
	using ID3D11On12DeviceRef = TReferPtr<ID3D11On12Device>;

	class D2D_API FD2DGraphics : public IGraphics
	{
	public:
		FD2DGraphics();
		FD2DGraphics(ID3D12CommandQueue * D3D12CommandQueue);
		~FD2DGraphics();

	public:
		IPainterRef CreatePainter() override;
		IPainterRef CreateWindowPainter(voidp WindowHandle) override;
		IPainterRef CreateDCPainter(voidp HDCHandle, const RectU & Rect) override;

	public:
		IFontFamilyRef CreateFontFamily(FStringV FamilyName) override;
		IFontFaceRef CreateFontFace(const FFontFace & FontFace) override;

		ITextBlobRef CreateTextBlob(FStringV String, const FFontFace & FontFace, float32 FontSize, const SizeF & LayoutSize, ETextLayoutFlags LayoutFlags) override;

		IPathRef CreatePath() override;
		IPathRef CreateTextPath(ITextBlob & TextBlob, const Point2F & Position) override;

	public: // Image
		IBitmapRef CreateBitmap(FStringV FilePath) override;
		IBitmapRef CreateBitmap(EFormat Format, SizeU Size, EBitmapUsage BitmapUsage) override;
		IBitmapRef CreateBitmap(const IPixmap & Pixmap) override;

		bool SupportSharedBitmap(uint32 DeviceRHI) const override;
		IBitmapRef CreateSharedBitmap(void * SharedHandle, EBitmapUsage BitmapUsage) override;

	public:
		IBitmapRef CreateBitmapFromHBITMAP(HBITMAP hBitmap);

	private:
		void InitializeD2D();

	public:
		Windows::FCOMInitializer COMInitializer;

		FModule DXGILibrary { u8"dxgi.dll"V };
		FModule D3D11Library { u8"d3d11.dll"V };
		FModule D2D1Library { u8"d2d1.dll"V };

		ID2D1Factory1Ref d2d1Factory1;
		IWICImagingFactory2Ref wicImagingFactory2;

		IDXGIFactory1Ref dxgiFactory1;

		ID3D11DeviceRef d3d11Device;
		ID3D11DeviceContextRef d3d11DeviceContext;

		ID3D11On12DeviceRef d3d11On12Device;
		ID2D1DeviceRef d2d1Device;
		ID2D1DeviceContextRef d2d1DeviceContext;

	public:
		FDWTextShaper DWTextShaper;
	};

	using FD2DGraphicsRef = TReferPtr<FD2DGraphics>;
}
