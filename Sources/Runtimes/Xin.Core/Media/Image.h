#pragma once

#include "Media.Types.h"

namespace Xin
{
	struct FImageSlice
	{
		EFormat Format = EFormat::None;
		uint32 Width = 0;
		uint32 Height = 0;
		uint32 RowPitch = 0;
		uint32 SlicePitch = 0;
	};

	struct FImageData
	{
		EFormat Format = EFormat::None;
		uint32 Width = 0;
		uint32 Height = 0;
		uint32 MipLevels = 1;
		uint32 NumLayers = 1;

		TView<byte> Bytes;
		TView<FImageSlice> Slices;
	};

	struct FCompressConfigASTC
	{
		bool IsNormalMap = false;
		bool IsHDR = false;
		float32 ComporessQuality = 60.0f/*ASTCENC_PRE_MEDIUM*/;
	};

	class IImage;
	using IImageRef = TReferPtr<IImage>;
	class CORE_API IImage : public ISharedRefer
	{
	public:
		IImage() = default;
		~IImage() = default;

		EImageFormat ImageFormat = EImageFormat::None;
		FImageData ImageData;

	public:
		static bool Save(const FImageData & Image, FStringV FilePath, EImageFormat ImageFormat = EImageFormat::JPEG);
		static IImageRef Convert(FImageData & Image, EFormat Format = EFormat::R8G8B8A8);
		static IImageRef CompressDDS(FImageData & Image, EFormat Format = EFormat::BC1UFsRGB, uint32 Miplevels = 0);
		static IImageRef CompressASTC(FImageData & Image, EFormat Format = EFormat::BC1UFsRGB, uint32 Miplevels = 0, const FCompressConfigASTC & CompressConfig = { });

		static EImageFormat GetImageFormat(FStringV FilePath);
		static IImageRef Load(IDataStream & DataStream);
		static IImageRef Load(FStringV FileName);
		static bool Save(IImage & Image, FStringV FilePath, EImageFormat ImageFormat = EImageFormat::JPEG)
		{
			return Save(Image.ImageData, FilePath, ImageFormat);
		}
		static bool Convert(FStringV SourceFileName, FStringV TargetFileName, EImageFormat ImageFormat = EImageFormat::JPEG);

		static bool CompressDDS(FStringV SourceFileName, FStringV TargetFileName, EFormat Format = EFormat::BC1UFsRGB, uint32 Miplevels = 0)
		{
			IImageRef CompressedImage = CompressDDS(*Load(SourceFileName), Format, Miplevels);
			if (!CompressedImage)
				return false;
			return Save(CompressedImage->ImageData, TargetFileName, EImageFormat::DDS);
		}
		static IImageRef CompressDDS(IImage & Image, EFormat Format = EFormat::BC1UFsRGB, uint32 Miplevels = 0)
		{
			return CompressDDS(Image.ImageData, Format, Miplevels);
		}

		static IImageRef CompressASTC(IImage & Image, EFormat Format = EFormat::BC1UFsRGB, uint32 Miplevels = 0)
		{
			return CompressASTC(Image.ImageData, Format, Miplevels);
		}
	};
}
