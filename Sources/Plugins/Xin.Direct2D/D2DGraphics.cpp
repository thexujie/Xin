#include "PCH.h"
#include "D2DGraphics.h"
#include "D2DBitmap.h"
#include "D2DPath.h"
#include "D2DPainter.h"
#include "WICBitmap.h"

namespace Xin::D2D
{
	FD2DGraphics::FD2DGraphics() : IGraphics(EGraphicsRHI::D2D)
	{
		InitializeD2D();

		HRESULT Result;
		HRESULT WINAPI D2D1CreateDevice(_In_ IDXGIDevice *dxgiDevice, _In_opt_ CONST D2D1_CREATION_PROPERTIES *creationProperties, _Outptr_ ID2D1Device **d2dDevice);

		//auto pfnD2D1CreateDevice = D2D1Library.GetProc<decltype(D2D1CreateDevice)>("D2D1CreateDevice");
		//if (!pfnD2D1CreateDevice)
		//{
		//	LogError(u8"Can't find D2D1CreateDevice in dxgi.dll, {}"V, ErrorDescription(GetLastError()));
		//	return;
		//}


		auto pfnD3D11CreateDevice = D3D11Library.GetProc<decltype(D3D11CreateDevice)>("D3D11CreateDevice");
		if (!pfnD3D11CreateDevice)
		{
			LogError(u8"Can't find D3D11CreateDevice in dxgi.dll, {}"V, ErrorDescription(GetLastError()));
			return;
		}

		auto pfnCreateDXGIFactory1 = DXGILibrary.GetProc<decltype(CreateDXGIFactory1)>("CreateDXGIFactory1");
		if (!pfnCreateDXGIFactory1)
		{
			LogError(u8"Can't find CreateDXGIFactory in dxgi.dll, {}"V, ErrorDescription(GetLastError()));
			return;
		}

		Result = pfnCreateDXGIFactory1(__uuidof(IDXGIFactory1), dxgiFactory1.GetVV());
		HRError(Result, u8"CreateDXGIFactory1 failed: {:x}"V, Result);

		UINT AdapterIndex = 0;
		TReferPtr<IDXGIAdapter> DXGIAdapter;
		while (dxgiFactory1->EnumAdapters(AdapterIndex++, DXGIAdapter.GetPP()) != DXGI_ERROR_NOT_FOUND)
		{
			D3D_FEATURE_LEVEL FeatureLevels[] =
			{
				D3D_FEATURE_LEVEL_11_1,
				D3D_FEATURE_LEVEL_11_0,
				D3D_FEATURE_LEVEL_10_1,
				D3D_FEATURE_LEVEL_10_0,
				D3D_FEATURE_LEVEL_9_3,
				D3D_FEATURE_LEVEL_9_2,
				D3D_FEATURE_LEVEL_9_1
			};

			UINT CreationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if XIN_DEBUG
			CreationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
			D3D_FEATURE_LEVEL SupportedFeatureLevel;
			Result = pfnD3D11CreateDevice(
				nullptr,
				D3D_DRIVER_TYPE_HARDWARE,
				0,
				CreationFlags,
				FeatureLevels,
				ARRAYSIZE(FeatureLevels),
				D3D11_SDK_VERSION,
				d3d11Device.GetPP(),
				&SupportedFeatureLevel,
				nullptr
			);

			if (FAILED(Result))
			{
				// If the initialization fails, fall back to the WARP device.
				// For more information on WARP, see:
				// http://go.microsoft.com/fwlink/?LinkId=286690
				Result = pfnD3D11CreateDevice(
					nullptr,
					D3D_DRIVER_TYPE_WARP,
					0,
					CreationFlags,
					FeatureLevels,
					ARRAYSIZE(FeatureLevels),
					D3D11_SDK_VERSION,
					d3d11Device.GetPP(),
					&SupportedFeatureLevel,
					nullptr
				);
			}

			HRError(Result, u8"D3D11CreateDevice failed: {:x}"V, Result);
			IDXGIDeviceRef DXGIDevice = d3d11Device;

			D2D1_CREATION_PROPERTIES D2D1CreationProperties  {D2D1_THREADING_MODE_SINGLE_THREADED };
#if XIN_DEBUG
			D2D1CreationProperties.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif
			D2D1CreationProperties.options = D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS;

			/**
			 * D2D1CreateDevice missing d2d1Factory, resources(geometry/brush/...) will be create on difference factory.
			 */
			Result = d2d1Factory1->CreateDevice(DXGIDevice, d2d1Device.GetPP());
			HRWarning(Result, u8"ID2D1Factory1::CreateDevice failed, {}"V, ErrorDescription(Result));

			//Result = pfnD2D1CreateDevice(DXGIDevice, &D2D1CreationProperties, d2d1Device.GetPP());
			//HRWarning(Result, u8"D2D1CreateDevice failed, {}"V, ErrorDescription(Result));

			if (Succeeded(Result))
			{
				Result = d2d1Device->CreateDeviceContext( D2D1_DEVICE_CONTEXT_OPTIONS_NONE, d2d1DeviceContext.GetPP());
				HRWarning(Result, u8"CreateDeviceContext failed, {}"V, ErrorDescription(GetLastError()));
			}
			else
			{
				d3d11Device = nullptr;
				d2d1Device = nullptr;
				d3d11Device = nullptr;
			}

			break;
		}
	}

	FD2DGraphics::FD2DGraphics(ID3D12CommandQueue * D3D12CommandQueue): IGraphics(EGraphicsRHI::D2D)
	{
		[[assume(D3D12CommandQueue)]];

		InitializeD2D();

		TReferPtr<IUnknown> Device_IUnknown;
		HRESULT Result = D3D12CommandQueue->GetDevice(IID_PPV_ARGS(Device_IUnknown.GetPP()));
		HRError(Result, u8"ID3D12CommandQueue::GetDevice failed: {:x}"V, Result);

		if (Device_IUnknown)
		{
			auto pfnD3D11On12CreateDevice = D3D11Library.GetProc<decltype(D3D11On12CreateDevice)>("D3D11On12CreateDevice");
			if (!pfnD3D11On12CreateDevice)
			{
				LogError(u8"Can't find D3D11On12CreateDevice in dxgi.dll, {}"V, ErrorDescription(GetLastError()));
				return;
			}

			TReferPtr<IUnknown> D3D12CommandQueue_IUnknown(D3D12CommandQueue);

			UINT D3D11DeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

			IUnknown * const UnknownCommandQueue = D3D12CommandQueue;
			Result = pfnD3D11On12CreateDevice(
				Device_IUnknown,
				D3D11DeviceFlags,
				nullptr,
				0,
				&UnknownCommandQueue,
				1,
				0,
				d3d11Device.GetPP(),
				d3d11DeviceContext.GetPP(),
				nullptr
			);
			d3d11On12Device = d3d11Device.QueryInterface<ID3D11On12Device>();

			if (d3d11On12Device)
			{
				IDXGIDeviceRef DXGIDevice { d3d11On12Device };
				Result = d2d1Factory1->CreateDevice(DXGIDevice.Get(), d2d1Device.GetPP());
				HRError(Result, u8"ID2D1Factory1::CreateDevice failed: {:x}"V, Result);

				D2D1_DEVICE_CONTEXT_OPTIONS DeviceContextOptions = D2D1_DEVICE_CONTEXT_OPTIONS_NONE;
				Result = d2d1Device->CreateDeviceContext(DeviceContextOptions, d2d1DeviceContext.GetPP());
				HRError(Result, u8"ID2D1Factory1::CreateDeviceContext failed: {:x}"V, Result);
			}
		}
	}

	FD2DGraphics::~FD2DGraphics()
	{
		d2d1DeviceContext = nullptr;
		d2d1Device = nullptr;

		d3d11DeviceContext = nullptr;
		d3d11Device = nullptr;
	}

	IPainterRef FD2DGraphics::CreatePainter()
	{
		if (!d2d1Device)
			return new FD2DWICPainter(*this);

		return new FD2DPainter(*this);
	}

	IPainterRef FD2DGraphics::CreateWindowPainter(voidp WindowHandle)
	{
		//return nullptr;
		return new FD2DWindowPainter(*this, (HWND)WindowHandle);
	}

	IPainterRef FD2DGraphics::CreateDCPainter(voidp HDCHandle, const RectU & Rect)
	{
		return new FD2DPainter(*this, (HDC)HDCHandle, Rect);
	}

	IFontFamilyRef FD2DGraphics::CreateFontFamily(FStringV FamilyName)
	{
		return DWTextShaper.CreateFontFamily(FamilyName);
	}

	IFontFaceRef FD2DGraphics::CreateFontFace(const FFontFace & FontFace)
	{
		return DWTextShaper.CreateFontFace(FontFace);
	}

	ITextBlobRef FD2DGraphics::CreateTextBlob(FStringV String, const FFontFace & FontFace, float32 FontSize, const SizeF & LayoutSize, ETextLayoutFlags LayoutFlags)
	{
		return DWTextShaper.CreateTextBlob(String, FontFace, FontSize, LayoutSize, LayoutFlags);
	}

	IPathRef FD2DGraphics::CreatePath()
	{
		return new FD2DPath(*this);
	}

	IPathRef FD2DGraphics::CreateTextPath(ITextBlob & TextBlob, const Point2F & Position)
	{
		FDWTextBlob & DWTextBlob = StaticCast<FDWTextBlob>(TextBlob);

		HRESULT Result = S_OK;

		TList<ID2D1TransformedGeometryRef> LayoutSpanGeometries { DWTextBlob.ClusterRuns.Size, EInitializeMode::Default };

		float32 LineY = Position.Y;
		for (sizet LineIndex = 0; LineIndex < TextBlob.LayoutLines.Size; ++LineIndex)
		{
			const FLayoutLine & LayoutLine = TextBlob.LayoutLines[LineIndex];
			float32 SpanX = Position.X;

			for (sizet SpanIndex = 0; SpanIndex < LayoutLine.RunLength; ++SpanIndex)
			{
				const FClusterRun & LayoutSpan = TextBlob.ClusterRuns[LayoutLine.RunIndex + SpanIndex];
				if (!LayoutSpan.GlyphIndices.Size)
					continue;

				// top
				//PointF SpanPosition { SpanX, LineY };
				// base line
				//PointF SpanPosition { SpanX, LineY + LayoutLine.Ascent - LayoutSpan.Font.Ascent * LayoutSpan.FontSize };
				// center
				PointF SpanPosition { SpanX, LineY + (LayoutLine.Height - LayoutSpan.FontFace.Height * LayoutSpan.FontSize) * 0.5f };
				if (LayoutSpan.BiDiLevel & EBiDiLevel::RTL)
					SpanPosition.X += LayoutSpan.Size.Width;

				ID2D1PathGeometry1Ref d2d1RunPathGeometry;
				Result = d2d1Factory1->CreatePathGeometry(d2d1RunPathGeometry.GetPP());

				ID2D1GeometrySinkRef d2d1RunPathGeometrySink;
				Result = d2d1RunPathGeometry->Open(d2d1RunPathGeometrySink.GetPP());

				FDWFontFace & DWFontFace = StaticCast<FDWFontFace>(LayoutSpan.FontFace);

				Result = DWFontFace.dwFontFace->GetGlyphRunOutline(
					LayoutSpan.FontSize,
					LayoutSpan.GlyphIndices.Data,
					LayoutSpan.GlyphAdvances.Data,
					(const DWRITE_GLYPH_OFFSET *)LayoutSpan.GlyphOffsets.Data,
					(uint32)LayoutSpan.GlyphIndices.Size,
					LayoutSpan.Rotate90,
					LayoutSpan.BiDiLevel & EBiDiLevel::RTL,
					d2d1RunPathGeometrySink);
				Result = d2d1RunPathGeometrySink->Close();

				D2D1::Matrix3x2F RunGeometryTransform = D2D1::Matrix3x2F(
					1.0f, 0.0f,
					0.0f, 1.0f,
					SpanPosition.X, SpanPosition.Y + LayoutSpan.FontFace.Ascent * LayoutSpan.FontSize
				);

				Result = d2d1Factory1->CreateTransformedGeometry(d2d1RunPathGeometry.Get(), RunGeometryTransform, LayoutSpanGeometries[LayoutLine.RunIndex + SpanIndex].GetPP());

				SpanX += LayoutSpan.Size.Width;
			}
			LineY += LayoutLine.Height;
		}

		TList<ID2D1Geometry *> D2D1Geometries { LayoutSpanGeometries };
		ID2D1GeometryGroupRef TextOutlineGeometry;
		Result = d2d1Factory1->CreateGeometryGroup(D2D1_FILL_MODE_ALTERNATE, D2D1Geometries.Data, D2D1Geometries.Size, TextOutlineGeometry.GetPP());
		return new FD2DPath(*this, TextOutlineGeometry.Get());
	}

	IBitmapRef FD2DGraphics::CreateBitmap(FStringV FilePath)
	{
		if (!d2d1Device)
			return new FWICBitmap(*this, FilePath);

		return new FWICBitmap(*this, FilePath);
	}

	IBitmapRef FD2DGraphics::CreateBitmap(EFormat Format, SizeU Size, EBitmapUsage BitmapUsage)
	{
		if (!d2d1Device)
			return new FWICBitmap(*this, Format, Size, BitmapUsage);

		return new FD2DBitmap(*this, Format, Size, BitmapUsage);
	}

	IBitmapRef FD2DGraphics::CreateBitmap(const IPixmap & Pixmap)
	{
		return new FWICBitmap(*this, Pixmap);
	}

	bool FD2DGraphics::SupportSharedBitmap(uint32 DeviceRHI) const
	{
		if (!d3d11On12Device)
			return false;
		return DeviceRHI == MakeFourCC("DX12");
	}

	IBitmapRef FD2DGraphics::CreateSharedBitmap(void * SharedHandle, EBitmapUsage BitmapUsage)
	{
		if (!d2d1Device || !d3d11On12Device)
			return nullptr;

		return new FD2DBitmap(*this, InterpretCast<ID3D12Resource>(SharedHandle), BitmapUsage);
	}

	IBitmapRef FD2DGraphics::CreateBitmapFromHBITMAP(HBITMAP hBitmap)
	{
		if (!d2d1Device)
			return nullptr;

		return new FWICBitmap(*this, hBitmap);
	}

	void FD2DGraphics::InitializeD2D()
	{
		GraphicsRHI = EGraphicsRHI::D2D;

		auto pfnD2D1CreateFactory = D2D1Library.GetProc<HRESULT(D2D1_FACTORY_TYPE, REFIID, const D2D1_FACTORY_OPTIONS *, void **)>("D2D1CreateFactory");
		if (!pfnD2D1CreateFactory)
			return;

		HRESULT Result;

		D2D1_FACTORY_OPTIONS D2DFactoryOptions {};
#if XIN_DEBUG
		D2DFactoryOptions.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif
		Result = pfnD2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(d2d1Factory1.Ref()), &D2DFactoryOptions, d2d1Factory1.GetVV());
		if (FAILED(Result))
			return;

		Result = CoCreateInstance(CLSID_WICImagingFactory2, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory2, wicImagingFactory2.GetVV());
		if (FAILED(Result))
			return;
	}
}
