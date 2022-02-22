#pragma once

#include "SkiaModule.h"

#include <include/core/SkStream.h>

#include "Xin.Core/Xin.Core.h"

#include <include/core/SkSize.h>
#include <include/core/SkRect.h>
#include <include/core/SkPath.h>

#include <include/core/SkBitmap.h>
#include <include/core/SkImage.h>

#include <include/core/SkString.h>

#include <include/core/SkTypeface.h>
#include <include/core/SkFont.h>
#include <include/core/SkTextBlob.h>
#include <include/core/SkFontMgr.h>
#include <include/core/SkFontMetrics.h>

#include <include/core/SkSurface.h>
#include <include/core/SkCanvas.h>
#include <include/core/SkPaint.h>

#include <include/codec/SkCodec.h>
#include <include/codec/SkEncodedImageFormat.h>

#include <include/core/SkPathEffect.h>
#include <include/effects/SkDashPathEffect.h>

namespace Xin::Skia
{
	class FSkiaGraphics;
	class FSkiaPainter;
	class FSkiaBitmap;

	template<typename T = SkRefCnt>
	inline T * SkiaRref(T * Pointer)
	{
		if (Pointer)
			Pointer->ref();
		return Pointer;
	}

	template<typename T = SkRefCnt>
	inline void SkiaUnref(T * Pointer)
	{
		if (Pointer)
			Pointer->unref();
	}

	inline SkPoint FromPoint(const PointF & Point)
	{
		return SkPoint(Point.X, Point.Y);
	}

	inline SkIPoint FromPoint(const PointI & Point)
	{
		return SkIPoint(Point.X, Point.Y);
	}

	inline SkIRect FromRect(const RectI & Rect)
	{
		return SkIRect::MakeXYWH(Rect.X, Rect.Y, Rect.Width, Rect.Height);
	}

	inline SkRect FromRect(const RectF & Rect)
	{
		return SkRect::MakeXYWH(Rect.X, Rect.Y, Rect.Width, Rect.Height);
	}

	inline RectI ToRect(const SkIRect & Rect)
	{
		return { Rect.x(), Rect.y(), Rect.width(), Rect.height() };
	}

	inline RectF ToRect(const SkRect & Rect)
	{
		return { Rect.x(), Rect.y(), Rect.width(), Rect.height() };
	}

	inline SkMatrix FromTransform(const FTransform3x2F & Transform)
	{
		SkMatrix skMatrix;
		skMatrix.setTranslate(Transform.Tx, Transform.Ty);
		skMatrix.preScale(Transform.Sx, Transform.Sy);
		skMatrix.preSkew(Transform.Kx, Transform.Ky);
		return skMatrix;
	}

	forceinline EFormat ToFormat(SkColorType ColorType)
	{
		static constexpr EFormat InvalidFormat = EFormat::None;
		switch (ColorType)
		{
		case kAlpha_8_SkColorType: return EFormat::A8;
		case kRGB_565_SkColorType: return EFormat::R5G6B5;
		case kARGB_4444_SkColorType: return EFormat::A4R4G4B4;
		case kRGBA_8888_SkColorType: return EFormat::R8G8B8A8;
		case kRGB_888x_SkColorType:return EFormat::R8G8B8X8;
		case kBGRA_8888_SkColorType:return EFormat::B8G8R8A8;
		case kRGBA_1010102_SkColorType: return EFormat::R10G10B10A2;
		case kBGRA_1010102_SkColorType: return InvalidFormat;
		case kRGB_101010x_SkColorType: return EFormat::R10G10B10A2;
		case kBGR_101010x_SkColorType: return InvalidFormat;
		case kGray_8_SkColorType: return EFormat::GRAY8;
		case kRGBA_F16Norm_SkColorType: return EFormat::R16G16B16A16UF;
		case kRGBA_F16_SkColorType: return EFormat::R16G16B16A16F;
		case kRGBA_F32_SkColorType: return EFormat::R32G32B32A32F;
		case kR8G8_unorm_SkColorType: return EFormat::R8G8UF;
		case kA16_float_SkColorType: return EFormat::R16F;
		case kR16G16_float_SkColorType: return EFormat::R16G16F;
		case kA16_unorm_SkColorType: return EFormat::R16UF;
		case kR16G16_unorm_SkColorType:return EFormat::R16G16UF;
		case kR16G16B16A16_unorm_SkColorType: return EFormat::R16G16B16A16UF;
		case kSRGBA_8888_SkColorType:return EFormat::R8G8B8A8UFsRGB;
		case kR8_unorm_SkColorType: return EFormat::R8UF;
		default: return InvalidFormat;
		}
	}

	forceinline SkColorType FromFormat(EFormat Format)
	{
		switch (Format)
		{
		case EFormat::None:
		case EFormat::R8G8B8A8:
		case EFormat::R8G8B8A8UF:
		case EFormat::R8G8B8A8SF:
			return kRGBA_8888_SkColorType;
		case EFormat::R8G8B8A8UFsRGB:
			return kSRGBA_8888_SkColorType;
		case EFormat::R8G8B8A8UI:
		case EFormat::R8G8B8A8SI:
			return kRGBA_8888_SkColorType;
		case EFormat::R8G8B8X8:
			return kRGB_888x_SkColorType;

		case EFormat::B8G8R8A8:
		case EFormat::B8G8R8A8UF:
		case EFormat::B8G8R8A8UFsRGB:
		case EFormat::B8G8R8A8SF:
		case EFormat::B8G8R8X8:
		case EFormat::B8G8R8X8UF:
		case EFormat::B8G8R8X8UFsRGB:
			return kBGRA_8888_SkColorType;

		case EFormat::R10G10B10A2:
		case EFormat::R10G10B10A2UF:
		case EFormat::R10G10B10A2UI:
			return kRGBA_1010102_SkColorType;
		case EFormat::R11G11B10F:
			break;
		case EFormat::R16G16:
		case EFormat::R16G16F:
		case EFormat::R16G16UF:
		case EFormat::R16G16SF:
		case EFormat::R16G16UI:
		case EFormat::R16G16SI:
			return kR16G16_float_SkColorType;
		case EFormat::R16G16B16A16:
			return kR16G16B16A16_unorm_SkColorType;
		case EFormat::R16G16B16A16F:
			return kRGBA_F16_SkColorType;
		case EFormat::R16G16B16A16UF:
			return kRGBA_F16Norm_SkColorType;
		case EFormat::R16G16B16A16SF:
			return kRGBA_F16_SkColorType;
		case EFormat::R16G16B16A16UI:
		case EFormat::R16G16B16A16SI:
			return kR16G16B16A16_unorm_SkColorType;
		case EFormat::R32G32:
			break;
		case EFormat::R32G32F:
			break;
		case EFormat::R32G32UI:
			break;
		case EFormat::R32G32SI:
			break;
		case EFormat::R32G32B32:
			break;
		case EFormat::R32G32B32F:
			break;
		case EFormat::R32G32B32UI:
			break;
		case EFormat::R32G32B32SI:
			break;
		case EFormat::R32G32B32A32:
			break;
		case EFormat::R32G32B32A32F:
			break;
		case EFormat::R32G32B32A32UF:
			break;
		case EFormat::R32G32B32A32SF:
			break;
		case EFormat::R32G32B32A32UI:
			break;
		case EFormat::R32G32B32A32SI:
			break;
		case EFormat::A8:
		case EFormat::A8UF:
			return kAlpha_8_SkColorType;
			
		case EFormat::R8:
			break;
		case EFormat::R8UI:
			break;
		case EFormat::R8SI:
			break;
		case EFormat::R8UF:
			break;
		case EFormat::R8SF:
			break;
		case EFormat::R16:
		case EFormat::R16F:
		case EFormat::R16UI:
		case EFormat::R16SI:
		case EFormat::R16UF:
		case EFormat::R16SF:
			return kA16_float_SkColorType;
		case EFormat::R24G8:
			break;
		case EFormat::R24X8:
			break;
		case EFormat::R32:
		case EFormat::R32F:
		case EFormat::R32UI:
		case EFormat::R32SI:
			return kRGBA_F32_SkColorType;
		case EFormat::BC1:
			break;
		case EFormat::BC1UF:
			break;
		case EFormat::BC1UFsRGB:
			break;
		case EFormat::BC1SF:
			break;
		case EFormat::BC2:
			break;
		case EFormat::BC2UF:
			break;
		case EFormat::BC2UFsRGB:
			break;
		case EFormat::BC3:
			break;
		case EFormat::BC3UF:
			break;
		case EFormat::BC3UFsRGB:
			break;
		case EFormat::BC4:
			break;
		case EFormat::BC4UF:
			break;
		case EFormat::BC4SF:
			break;
		case EFormat::BC5:
			break;
		case EFormat::BC5UF:
			break;
		case EFormat::BC5SF:
			break;
		case EFormat::BC6H:
			break;
		case EFormat::BC6H_UF16:
			break;
		case EFormat::BC6H_SF16:
			break;
		case EFormat::BC7:
			break;
		case EFormat::BC7UF:
			break;
		case EFormat::BC7UFsRGB:
			break;
		case EFormat::D16UF:
			break;
		case EFormat::D32F:
			break;
		case EFormat::D24UF_S8UI:
			break;
		case EFormat::R3G3B2:
			break;
		case EFormat::R8G8:
		case EFormat::R8G8UF:
		case EFormat::R8G8SF:
		case EFormat::R8G8UI:
		case EFormat::R8G8SI:
			return kR8G8_unorm_SkColorType;

		case EFormat::R5G6B5:
		case EFormat::R5G6B5UF:
			return kRGB_565_SkColorType;
		case EFormat::R5G5B5A1:
			break;
		case EFormat::R5G5B5A1UF:
			break;
		case EFormat::B5G6R5:
			break;
		case EFormat::B5G5R5A1:
			break;
		case EFormat::A1R5G5B5:
			break;
		case EFormat::A1B5G5R5:
			break;
		case EFormat::X1R5G5B5:
			break;
		case EFormat::X1B5G5R5:
			break;
		case EFormat::A4R4G4B4:
		case EFormat::X4R4G4B4:
			return kARGB_4444_SkColorType;
		case EFormat::A8R3G3B2:
			break;
		case EFormat::B4G4R4A4:
			break;
		case EFormat::R8G8B8:
			break;
		case EFormat::B8G8R8:
			break;
		case EFormat::A8R5G6B5:
			break;
		case EFormat::A8B5G6R5:
			break;
		case EFormat::A8X1R5G5B5:
			break;
		case EFormat::A8X1B5G5R5:
			break;
		case EFormat::A2R10G10B10:
			break;
		case EFormat::A2B10G10R10:
			break;
		case EFormat::INDEX4_A8R8G8B8:
			break;
		case EFormat::INDEX8_A8R8G8B8:
			break;
		case EFormat::Short1:
			break;
		case EFormat::Short2:
			break;
		case EFormat::Short3:
			break;
		case EFormat::Short4:
			break;
		case EFormat::UShort1:
			break;
		case EFormat::UShort2:
			break;
		case EFormat::UShort3:
			break;
		case EFormat::UShort4:
			break;
		case EFormat::Int1:
			break;
		case EFormat::Int2:
			break;
		case EFormat::Int3:
			break;
		case EFormat::Int4:
			break;
		case EFormat::UInt1:
			break;
		case EFormat::UInt2:
			break;
		case EFormat::UInt3:
			break;
		case EFormat::UInt4:
			break;
		case EFormat::Half1:
			break;
		case EFormat::Half2:
			break;
		case EFormat::Half3:
			break;
		case EFormat::Half4:
			break;
		case EFormat::Float1:
			break;
		case EFormat::Float2:
			break;
		case EFormat::Float3:
			break;
		case EFormat::Float4:
			break;
		case EFormat::Struct:
			break;
		case EFormat::GRAY1:
			break;
		case EFormat::GRAY2:
			break;
		case EFormat::GRAY4:
			break;
		case EFormat::GRAY8:
			return kGray_8_SkColorType;
		default:
			return kUnknown_SkColorType;
		}
		return kUnknown_SkColorType;
	}

	inline SkEncodedImageFormat FromImageFormat(EImageFormat ImageFormat)
	{
		switch (ImageFormat)
		{
		case EImageFormat::BMP: return SkEncodedImageFormat::kBMP;
		case EImageFormat::GIF: return SkEncodedImageFormat::kGIF;
		case EImageFormat::ICO: return SkEncodedImageFormat::kICO;
		case EImageFormat::JPEG: return SkEncodedImageFormat::kJPEG;
		case EImageFormat::PNG: return SkEncodedImageFormat::kPNG;
		case EImageFormat::WEBP: return SkEncodedImageFormat::kWEBP;
		case EImageFormat::ASTC: return SkEncodedImageFormat::kASTC;
		case EImageFormat::HEIF: return SkEncodedImageFormat::kHEIF;
		default: return SkEncodedImageFormat::kBMP;
		}
	}

	forceinline SkPaint::Join FromLineJoin(ELineJoin LineJoin)
	{
		switch (LineJoin)
		{
		default:
		case ELineJoin::Miter: return SkPaint::Join::kMiter_Join;
		case ELineJoin::Bevel: return SkPaint::Join::kBevel_Join;
		case ELineJoin::Round: return SkPaint::Join::kRound_Join;
		}
	}

	forceinline SkFontStyle::Weight FromFontWeight(EFontWeight FontWeight)
	{
		switch (FontWeight)
		{
		case Thin:				return SkFontStyle::Weight::kThin_Weight;
		case ExtraLight:		return SkFontStyle::Weight::kExtraLight_Weight;
		case Light:				return SkFontStyle::Weight::kLight_Weight;
		case SemiLight:			throw 0;
		case Regular:			return SkFontStyle::Weight::kNormal_Weight;
		case Medium:			return SkFontStyle::Weight::kMedium_Weight;
		case SemiBold:			return SkFontStyle::Weight::kSemiBold_Weight;
		case Bold:				return SkFontStyle::Weight::kBold_Weight;
		case ExtraBold:			return SkFontStyle::Weight::kExtraBold_Weight;
		case Heavy:				return SkFontStyle::Weight::kBlack_Weight;
		default:				return SkFontStyle::Weight::kNormal_Weight;
		}
	}

	forceinline EFontWeight ToFontWeight(SkFontStyle::Weight skFontWeight)
	{
		switch (skFontWeight)
		{
		case SkFontStyle::Weight::kThin_Weight: return EFontWeight::Thin;
		case SkFontStyle::Weight::kExtraLight_Weight: return EFontWeight::ExtraLight;
		case SkFontStyle::Weight::kLight_Weight: return EFontWeight::Light;
		case SkFontStyle::Weight::kNormal_Weight: return EFontWeight::Regular;
		case SkFontStyle::Weight::kMedium_Weight: return EFontWeight::Medium;
		case SkFontStyle::Weight::kSemiBold_Weight: return EFontWeight::SemiBold;
		case SkFontStyle::Weight::kBold_Weight: return EFontWeight::Bold;
		case SkFontStyle::Weight::kExtraBold_Weight: return EFontWeight::ExtraBold;
		case SkFontStyle::Weight::kBlack_Weight: return EFontWeight::Heavy;
		default: return EFontWeight(skFontWeight);
		}
	}

	forceinline SkFontStyle::Slant FromFontStyle(EFontStyle FontStyle)
	{
		if (FontStyle & EFontStyle::Italic)
			return SkFontStyle::Slant::kItalic_Slant;
		return SkFontStyle::Slant::kUpright_Slant;
	}

	forceinline EFontStyle ToFontStyle(SkFontStyle::Slant skFontSlant)
	{
		if (skFontSlant == SkFontStyle::kItalic_Slant)
			return EFontStyle::Italic;
		else
			return EFontStyle::None;
	}
}
