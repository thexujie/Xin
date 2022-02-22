#include "PCH.h"
#include "SkiaBitmap.h"
#include "SkiaGraphics.h"
#include "SkiaPainter.h"

#include "include/codec/SkBmpDecoder.h"
#include "include/codec/SkCodec.h"
#include "include/codec/SkIcoDecoder.h"
#include "include/codec/SkJpegDecoder.h"
#include "include/codec/SkPngDecoder.h"
#include "include/codec/SkWbmpDecoder.h"
#include "include/codec/SkWebpDecoder.h"

#include "include/encode/SkJpegEncoder.h"
#include "include/encode/SkPngEncoder.h"
#include "include/encode/SkWebpEncoder.h"

#include "include/core/SkData.h"
#include "include/core/SkImageInfo.h"
#include "include/core/SkStream.h"

namespace Xin::Skia
{
	sk_sp<SkImage> FSkiaImageCodec::LoadFromFile(FStringV FilePath)
	{
		std::unique_ptr<SkFILEStream> skFILEStream = SkFILEStream::Make(FEncoding::AsAnsi(FilePath));
		if (!skFILEStream || !skFILEStream->isValid())
		{
			return nullptr;
		}

		sk_sp<SkData> skData = SkData::MakeFromStream(skFILEStream.get(), skFILEStream->getLength());
		std::unique_ptr<SkCodec> skCodec = nullptr;
		if (SkBmpDecoder::IsBmp(skData->bytes(), skData->size()))
		{
			skCodec = SkBmpDecoder::Decode(skData, nullptr);
		}
		else if (SkIcoDecoder::IsIco(skData->bytes(), skData->size()))
		{
			skCodec = SkIcoDecoder::Decode(skData, nullptr);
		}
		else if (SkJpegDecoder::IsJpeg(skData->bytes(), skData->size()))
		{
			skCodec = SkJpegDecoder::Decode(skData, nullptr);
		}
		else if (SkPngDecoder::IsPng(skData->bytes(), skData->size()))
		{
			skCodec = SkPngDecoder::Decode(skData, nullptr);
		}
		else if (SkWbmpDecoder::IsWbmp(skData->bytes(), skData->size()))
		{
			skCodec = SkWbmpDecoder::Decode(skData, nullptr);
		}
		else if (SkWebpDecoder::IsWebp(skData->bytes(), skData->size()))
		{
			skCodec = SkWebpDecoder::Decode(skData, nullptr);
		}
		else
		{
			LogInfo(u8"Unsupported file format: {}"V, FilePath);
			return nullptr;
		}

		auto [skCodecImage, Result] = skCodec->getImage();
		return skCodecImage;
	}

	bool FSkiaImageCodec::SaveToFile(const SkPixmap & skPixmap, FStringV FilePath, EImageFormat ImageFormat)
	{
		switch (ImageFormat)
		{
		case EImageFormat::ASTC:
			break;
		case EImageFormat::BMP:
			break;
		case EImageFormat::HEIF:
			break;
		case EImageFormat::ICO:
			break;
		case EImageFormat::JPEG:
			{
			SkJpegEncoder::Options Options { };
			SkFILEWStream skFileWStream((const chara *)FilePath.Data);
			return SkJpegEncoder::Encode(&skFileWStream, skPixmap, Options);
		}
		case EImageFormat::JNG:
			break;
		case EImageFormat::KOALA:
			break;
		case EImageFormat::LBM:
			break;
		case EImageFormat::IFF:
			break;
		case EImageFormat::MNG:
			break;
		case EImageFormat::PBM:
			break;
		case EImageFormat::PBMRAW:
			break;
		case EImageFormat::PCD:
			break;
		case EImageFormat::PCX:
			break;
		case EImageFormat::PGM:
			break;
		case EImageFormat::PGMRAW:
			break;
		case EImageFormat::PNG:
			{
			SkPngEncoder::Options Options { };
			SkFILEWStream skFileWStream((const chara *)FilePath.Data);
			return SkPngEncoder::Encode(&skFileWStream, skPixmap, Options);
		}
		case EImageFormat::PPM:
			break;
		case EImageFormat::PPMRAW:
			break;
		case EImageFormat::RAS:
			break;
		case EImageFormat::TARGA:
			break;
		case EImageFormat::TIFF:
			break;
		case EImageFormat::WBMP:
			break;
		case EImageFormat::PSD:
			break;
		case EImageFormat::CUT:
			break;
		case EImageFormat::XBM:
			break;
		case EImageFormat::XPM:
			break;
		case EImageFormat::DDS:
			break;
		case EImageFormat::GIF:
			break;
		case EImageFormat::HDR:
			break;
		case EImageFormat::FAXG3:
			break;
		case EImageFormat::SGI:
			break;
		case EImageFormat::EXR:
			break;
		case EImageFormat::J2K:
			break;
		case EImageFormat::JP2:
			break;
		case EImageFormat::PFM:
			break;
		case EImageFormat::PICT:
			break;
		case EImageFormat::RAW:
			break;
		case EImageFormat::WEBP:
		{
			SkWebpEncoder::Options Options { };
			SkFILEWStream skFileWStream((const chara *)FilePath.Data);
			return SkWebpEncoder::Encode(&skFileWStream, skPixmap, Options);
		}
		case EImageFormat::JXR:
			break;
		default:
			return false;
		}
		return false;
	}

	bool FSkiaImageCodec::SaveToFile(const SkImage & skImage, FStringV FilePath, EImageFormat ImageFormat)
	{
		SkPixmap skPixmap;
		if (skImage.peekPixels(&skPixmap))
			return FSkiaImageCodec::SaveToFile(skPixmap, FilePath, ImageFormat);
		return false;
	}

	static FBitmapDesc BitmapDescFromSkImageInfo(const SkImageInfo & skImageInfo)
	{
		FBitmapDesc BitmapDesc;
		BitmapDesc.Size = { uint32(skImageInfo.width()), uint32(skImageInfo.height()) };
		BitmapDesc.Format = ToFormat(skImageInfo.colorType());
		BitmapDesc.Usage = EBitmapUsage::None;
		return BitmapDesc;
	}

	FSkiaBitmap::FSkiaBitmap(FSkiaGraphics & SkiaGraphics) : Graphics(SkiaGraphics) {}

	FSkiaBitmap::FSkiaBitmap(FSkiaGraphics & SkiaGraphics, const IPixmap & Pixmap): IBitmap(Pixmap.Format, Pixmap.Size), Graphics(SkiaGraphics)
	{
		SkImageInfo skImageInfo = SkImageInfo::Make(int32(Pixmap.Size.Width), int32(Pixmap.Size.Height), FromFormat(Pixmap.Format), IsAlphaFormat(Pixmap.Format) ? kPremul_SkAlphaType : kOpaque_SkAlphaType);

		SkPixmap skPixmap { skImageInfo, Pixmap.Bytes.Data, Pixmap.Pitch };
		skBitmap = new SkBitmap();
		skBitmap->installPixels(skPixmap);

		skImage = skBitmap->asImage();
	}

	FSkiaBitmap::FSkiaBitmap(FSkiaGraphics & SkiaGraphics, TUniquePtr<SkBitmap> && skBitmap)
		: IBitmap(BitmapDescFromSkImageInfo(skBitmap ? skBitmap->info() : SkImageInfo{})), Graphics(SkiaGraphics), skBitmap(Move(skBitmap)) {}

	FSkiaBitmap::FSkiaBitmap(FSkiaGraphics & SkiaGraphics, sk_sp<SkImage> skImage)
		: IBitmap(BitmapDescFromSkImageInfo(skImage ? skImage->imageInfo() : SkImageInfo{})), Graphics(SkiaGraphics), skImage(Move(skImage))
	{
		
	}

	FSkiaBitmap::FSkiaBitmap(FSkiaGraphics & SkiaGraphics, EFormat Format, SizeU Size, EBitmapUsage BitmapUsage)
		: IBitmap(Format, Size, BitmapUsage), Graphics(SkiaGraphics)
	{
		AssertExpr(!HasFlags(BitmapUsage, EBitmapUsage::ReadBack) || !HasFlags(BitmapUsage, EBitmapUsage::RenderTarget));
		//SkiaPainter.Skia1RenderTarget->CreateBitmap();
		SkImageInfo skImageInfo = SkImageInfo::Make(int32(Size.Width), int32(Size.Height), FromFormat(Format), IsAlphaFormat(Format) ? kPremul_SkAlphaType : kOpaque_SkAlphaType);

		skBitmap = new SkBitmap();
		skBitmap->allocPixels(skImageInfo);

		skImage = skBitmap->asImage();
	}

	FSkiaBitmap::~FSkiaBitmap() {}

	void FSkiaBitmap::AcquireSurface() {}

	void FSkiaBitmap::ReleaseSurface() {}

	FLockedBitmap FSkiaBitmap::Lock(EBitmapLock BitmapLock)
	{
		if (skBitmap->peekPixels(&skPixmap))
			return { this, (byte *)skPixmap.addr(0, 0), uint32(skPixmap.rowBytes()) };
		else
			return {};
	}

	void FSkiaBitmap::Unlock()
	{
		skPixmap.reset();
	}

	bool FSkiaBitmap::Save(FStringV FilePath, EImageFormat ImageFormat) const
	{
		if (skImage)
			return FSkiaImageCodec::SaveToFile(*skImage, FilePath, ImageFormat);
		else
			return FSkiaImageCodec::SaveToFile(skPixmap, FilePath, ImageFormat);
	}
}
