#pragma once

#include "../../Xin.Core.Types.h"

#include "WindowsPrivate.h"
#include <wincodec.h>

namespace Xin::WIC
{
	using IWICImagingFactoryRef = TReferPtr<IWICImagingFactory>;
	using IWICImagingFactory2Ref = TReferPtr<IWICImagingFactory2>;

	using IWICBitmapSourceRef = TReferPtr<IWICBitmapSource>;
	using IWICBitmapRef = TReferPtr<IWICBitmap>;

	using IWICBitmapEncoderRef = TReferPtr<IWICBitmapEncoder>;

	using IWICStreamRef = TReferPtr<IWICStream>;

	const TPair<EFormat, GUID> WICFormatMappings[] =
	{
		{ EFormat::RGB, GUID_WICPixelFormat24bppRGB },
		{ EFormat::BGR, GUID_WICPixelFormat24bppBGR },

		{ EFormat::BGRX, GUID_WICPixelFormat32bppBGR },
		{ EFormat::RGBX, GUID_WICPixelFormat32bppRGB },
		{ EFormat::BGRA, GUID_WICPixelFormat32bppBGRA },
		{ EFormat::BGRAUF, GUID_WICPixelFormat32bppBGRA },
		{ EFormat::BGRAUFsRGB, GUID_WICPixelFormat32bppBGRA },
		
		{ EFormat::RGBA, GUID_WICPixelFormat32bppRGBA },
		{ EFormat::RGBAUI, GUID_WICPixelFormat32bppRGBA },
		{ EFormat::RGBAUF, GUID_WICPixelFormat32bppRGBA },
		{ EFormat::RGBAUFsRGB, GUID_WICPixelFormat32bppRGBA },
	};

	const TPair<EFormat, GUID> WICPremultipliedFormatMappings[] =
	{
		{ EFormat::B8G8R8X8, GUID_WICPixelFormat32bppBGR },
		{ EFormat::B8G8R8X8UF, GUID_WICPixelFormat32bppBGR },
		{ EFormat::B8G8R8X8UFsRGB, GUID_WICPixelFormat32bppBGR },

		{ EFormat::BGRA, GUID_WICPixelFormat32bppPBGRA },
		{ EFormat::BGRAUF, GUID_WICPixelFormat32bppPBGRA },
		{ EFormat::BGRAUFsRGB, GUID_WICPixelFormat32bppPBGRA },

		{ EFormat::RGBA, GUID_WICPixelFormat32bppPRGBA },
		{ EFormat::RGBAUI, GUID_WICPixelFormat32bppPRGBA },
		{ EFormat::RGBAUF, GUID_WICPixelFormat32bppPRGBA },
		{ EFormat::RGBAUFsRGB, GUID_WICPixelFormat32bppPRGBA },
	};

	inline const GUID & WICFormatFromFormat(EFormat Format)
	{
		for (const auto & WICFormatMapping : WICFormatMappings)
		{
			if (WICFormatMapping.First == Format)
				return WICFormatMapping.Second;
		}
		return GUID_WICPixelFormatUndefined;
	}

	inline const GUID & WICPremultipliedFormatFromFormat(EFormat Format)
	{
		for (const auto & WICFormatMapping : WICPremultipliedFormatMappings)
		{
			if (WICFormatMapping.First == Format)
				return WICFormatMapping.Second;
		}
		return GUID_WICPixelFormatUndefined;
	}

	inline EFormat WICFormatToFormat(const GUID & WICFormat)
	{
		for (const auto & WICFormatMapping : WICFormatMappings)
		{
			if (WICFormatMapping.Second == WICFormat)
				return WICFormatMapping.First;
		}
		for (const auto & WICFormatMapping : WICPremultipliedFormatMappings)
		{
			if (WICFormatMapping.Second == WICFormat)
				return WICFormatMapping.First;
		}
		return EFormat::None;
	}

	const TPair<EImageFormat, GUID> WICContainerFormatMappings[] =
	{
		{ EImageFormat::BMP, GUID_ContainerFormatBmp },
		{ EImageFormat::JPEG, GUID_ContainerFormatJpeg },
		{ EImageFormat::PNG, GUID_ContainerFormatPng },
		{ EImageFormat::DDS, GUID_ContainerFormatDds },
		{ EImageFormat::WEBP, GUID_ContainerFormatWebp },
		{ EImageFormat::RAW, GUID_ContainerFormatRaw },
		{ EImageFormat::HEIF, GUID_ContainerFormatHeif },
	};

	inline const GUID & WICContainerFormatFromImageFormat(EImageFormat ImageFormat)
	{
		for (const auto & WICContainerFormatMapping : WICContainerFormatMappings)
		{
			if (WICContainerFormatMapping.First == ImageFormat)
				return WICContainerFormatMapping.Second;
		}
		return GUID_ContainerFormatBmp;
	}
}
