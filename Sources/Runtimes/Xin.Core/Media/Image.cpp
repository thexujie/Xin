#include "PCH.h"
#include "Image.h"
#include "Platform/Windows/WindowsPrivate.h"

#include "FreeImage/Include/FreeImage.h"
#include <DirectXTex/Include/DirectXTex.h>
#include <astc-encoder/Include/astcenc.h>

namespace Xin
{
	using namespace Xin::Windows;

	static constexpr EImageFormat FromFreeImageFormat(FREE_IMAGE_FORMAT FiFormat)
	{
		switch (FiFormat)
		{
		case FIF_BMP: return EImageFormat::BMP;
		case FIF_ICO: return EImageFormat::ICO;
		case FIF_JPEG: return EImageFormat::JPEG;
		case FIF_JNG: return EImageFormat::JNG;
		case FIF_KOALA: return EImageFormat::KOALA;
		case FIF_LBM: return EImageFormat::LBM;
		case FIF_MNG: return EImageFormat::MNG;
		case FIF_PBM: return EImageFormat::PBM;
		case FIF_PBMRAW: return EImageFormat::PBMRAW;
		case FIF_PCD: return EImageFormat::PCD;
		case FIF_PCX: return EImageFormat::PCX;
		case FIF_PGM: return EImageFormat::PGM;
		case FIF_PGMRAW: return EImageFormat::PGMRAW;
		case FIF_PNG: return EImageFormat::PNG;
		case FIF_PPM: return EImageFormat::PPM;
		case FIF_PPMRAW: return EImageFormat::PPMRAW;
		case FIF_RAS: return EImageFormat::RAS;
		case FIF_TARGA: return EImageFormat::TARGA;
		case FIF_TIFF: return EImageFormat::TIFF;
		case FIF_WBMP: return EImageFormat::WBMP;
		case FIF_PSD: return EImageFormat::PSD;
		case FIF_CUT: return EImageFormat::CUT;
		case FIF_XBM: return EImageFormat::XBM;
		case FIF_XPM: return EImageFormat::XPM;
		case FIF_DDS: return EImageFormat::DDS;
		case FIF_GIF: return EImageFormat::GIF;
		case FIF_HDR: return EImageFormat::HDR;
		case FIF_FAXG3: return EImageFormat::FAXG3;
		case FIF_SGI: return EImageFormat::SGI;
		case FIF_EXR: return EImageFormat::EXR;
		case FIF_J2K: return EImageFormat::J2K;
		case FIF_JP2: return EImageFormat::JP2;
		case FIF_PFM: return EImageFormat::PFM;
		case FIF_PICT: return EImageFormat::PICT;
		case FIF_RAW: return EImageFormat::RAW;
		case FIF_WEBP: return EImageFormat::WEBP;
		case FIF_JXR: return EImageFormat::JXR;
		default: return EImageFormat::None;
		}
	}

	FREE_IMAGE_FORMAT FromImageFormat(EImageFormat ImageFormat)
	{
		switch (ImageFormat)
		{
		case EImageFormat::BMP: return FIF_BMP;
		case EImageFormat::ICO: return FIF_ICO;
		case EImageFormat::JPEG: return FIF_JPEG;
		case EImageFormat::JNG: return FIF_JNG;
		case EImageFormat::KOALA: return FIF_KOALA;
		case EImageFormat::LBM: return FIF_LBM;
		case EImageFormat::MNG: return FIF_MNG;
		case EImageFormat::PBM: return FIF_PBM;
		case EImageFormat::PBMRAW: return FIF_PBMRAW;
		case EImageFormat::PCD: return FIF_PCD;
		case EImageFormat::PCX: return FIF_PCX;
		case EImageFormat::PGM: return FIF_PGM;
		case EImageFormat::PGMRAW: return FIF_PGMRAW;
		case EImageFormat::PNG: return FIF_PNG;
		case EImageFormat::PPM: return FIF_PPM;
		case EImageFormat::PPMRAW: return FIF_PPMRAW;
		case EImageFormat::RAS: return FIF_RAS;
		case EImageFormat::TARGA: return FIF_TARGA;
		case EImageFormat::TIFF: return FIF_TIFF;
		case EImageFormat::WBMP: return FIF_WBMP;
		case EImageFormat::PSD: return FIF_PSD;
		case EImageFormat::CUT: return FIF_CUT;
		case EImageFormat::XBM: return FIF_XBM;
		case EImageFormat::XPM: return FIF_XPM;
		case EImageFormat::DDS: return FIF_DDS;
		case EImageFormat::GIF: return FIF_GIF;
		case EImageFormat::HDR: return FIF_HDR;
		case EImageFormat::FAXG3: return FIF_FAXG3;
		case EImageFormat::SGI: return FIF_SGI;
		case EImageFormat::EXR: return FIF_EXR;
		case EImageFormat::J2K: return FIF_J2K;
		case EImageFormat::JP2: return FIF_JP2;
		case EImageFormat::PFM: return FIF_PFM;
		case EImageFormat::PICT: return FIF_PICT;
		case EImageFormat::RAW: return FIF_RAW;
		case EImageFormat::WEBP: return FIF_WEBP;
		case EImageFormat::JXR: return FIF_JXR;
		default: return FIF_UNKNOWN;
		}
	}

	class FFreeImageImage : public IImage
	{
	public:
		FFreeImageImage(FIBITMAP * Bitmap) : Bitmap(Bitmap)
		{
			uint32 Pitch = FreeImage_GetPitch(Bitmap);

			ImageData.Format = FreeImage_IsTransparent(Bitmap) ? EFormat::B8G8R8A8UF : EFormat::B8G8R8X8UF;
			ImageData.Width = FreeImage_GetWidth(Bitmap);
			ImageData.Height = FreeImage_GetHeight(Bitmap);
			ImageData.MipLevels = 1;
			ImageData.NumLayers = 1;
			ImageData.Bytes = { FreeImage_GetBits(Bitmap), Pitch * ImageData.Height };
			{
				ImageSlice.Format = ImageData.Format;
				ImageSlice.Width = ImageData.Width;
				ImageSlice.Height = ImageData.Height;
				ImageSlice.RowPitch = Pitch;
				ImageSlice.SlicePitch = Pitch * ImageData.Height;
			}
			ImageData.Slices = { &ImageSlice, 1 };
		}

		~FFreeImageImage()
		{
			if (Bitmap)
			{
				FreeImage_Unload(Bitmap);
				Bitmap = nullptr;
			}
		}

	public:
		FIBITMAP * Bitmap;
		FImageSlice ImageSlice;
	};

	class FDirectXTexImage : public IImage
	{
	public:
		FDirectXTexImage(DirectX::ScratchImage && ScratchImage_) : ScratchImage(Move(ScratchImage_))
		{
			const DirectX::TexMetadata & Metadata = ScratchImage.GetMetadata();
			ImageData.Format = FormatFromDXGIFormat(EDXGIFormat(Metadata.format));
			ImageData.Width = uint32(Metadata.width);
			ImageData.Height = uint32(Metadata.height);

			FImageInfo ImageInfo = GetImageInfo(ImageData.Format, ImageData.Width, ImageData.Width);
			ImageData.MipLevels = uint32(Metadata.mipLevels);
			ImageData.NumLayers = uint32(Metadata.depth);
			Slices.ResizeTo(ImageData.MipLevels * ImageData.NumLayers, EInitializeMode::Zero);
			for (uintx LayerIndex = 0; LayerIndex < ImageData.NumLayers; ++LayerIndex)
			{
				for (uintx MipmapIndex = 0; MipmapIndex < ImageData.MipLevels; ++MipmapIndex)
				{
					const DirectX::Image * dxtImage = ScratchImage.GetImage(MipmapIndex, LayerIndex, 0);
					AssertExpr(dxtImage);

					FImageSlice & ImageSlice = Slices[ImageData.MipLevels * LayerIndex + MipmapIndex];
					ImageSlice.Format = FormatFromDXGIFormat(EDXGIFormat(dxtImage->format));
					ImageSlice.Width = uint32(dxtImage->width);
					ImageSlice.Height = uint32(dxtImage->height);
					ImageSlice.RowPitch = uint32(dxtImage->rowPitch);
					ImageSlice.SlicePitch = uint32(dxtImage->slicePitch);
				}
			}

			AssertExpr(ScratchImage.GetPixelsSize() % Metadata.arraySize == 0);
			ImageData.Bytes = { ScratchImage.GetPixels(), ScratchImage.GetPixelsSize() };
			ImageData.Slices = Slices;
		}

		~FDirectXTexImage()
		{
			ScratchImage.Release();
		}

	public:
		DirectX::ScratchImage ScratchImage;
		TList<FImageSlice> Slices;
	};

	//static TTypeRegister<ITextShaper> TypeRegister_ITextShaper;

	static unsigned DLL_CALLCONV FIReadProc(void * buffer, unsigned size, unsigned count, fi_handle handle)
	{
		IDataStream & DataStream = *(IDataStream *)handle;
		return (unsigned)DataStream.Read((byte *)buffer, size * count);
	}

	static unsigned DLL_CALLCONV FIWriteProc(void * buffer, unsigned size, unsigned count, fi_handle handle)
	{
		IDataStream & DataStream = *(IDataStream *)handle;
		return (unsigned)DataStream.Write((const byte *)buffer, size * count);
	}

	static int DLL_CALLCONV FISeekProc(fi_handle handle, long offset, int origin)
	{
		IDataStream & DataStream = *(IDataStream *)handle;
		switch (origin)
		{
		case SEEK_SET:
			return (int)DataStream.Seek(EStreamSeek::Begin, offset);
		case SEEK_CUR:
			return (int)DataStream.Seek(EStreamSeek::Current, offset);
		case SEEK_END:
			return (int)DataStream.Seek(EStreamSeek::End, offset);
		default:
			return (int)DataStream.Seek(EStreamSeek::Current, 0);
		}
	}

	static long DLL_CALLCONV FITellProc(fi_handle handle)
	{
		IDataStream & DataStream = *(IDataStream *)handle;
		return (int)DataStream.Position();
	}
}

namespace Xin
{
	bool IImage::Save(const FImageData & Image, FStringV FilePath, EImageFormat ImageFormat)
	{
		if (ImageFormat == EImageFormat::DDS)
		{
			FWString FilePathW = FEncoding::ToWide(FilePath);
			TList<DirectX::Image> dxImages { Image.NumLayers * Image.MipLevels };
			uint8_t * Pixels = (uint8_t *)Image.Bytes.Data;
			for (uintx ItemIndex = 0; ItemIndex < Image.NumLayers; ++ItemIndex)
			{
				for (uintx MipmapIndex = 0; MipmapIndex < Image.MipLevels; ++MipmapIndex)
				{
					uintx SliceIndex = Image.MipLevels * ItemIndex + MipmapIndex;
					DirectX::Image & dxImage = dxImages[SliceIndex];
					if (Image.Slices.Size)
					{
						const FImageSlice & ImageSlice = Image.Slices[SliceIndex];

						dxImage.width = ImageSlice.Width;
						dxImage.height = ImageSlice.Height;
						dxImage.format = DXGI_FORMAT(DXGIFormatFromFormat(ImageSlice.Format));
						dxImage.rowPitch = ImageSlice.RowPitch;
						dxImage.slicePitch = ImageSlice.SlicePitch;
						dxImage.pixels = Pixels;
					}
					else
					{
						dxImage.width = Image.Width;
						dxImage.height = Image.Height;
						dxImage.format = DXGI_FORMAT(DXGIFormatFromFormat(Image.Format));
						FImageInfo ImageInfo = GetImageInfo(Image.Format, Image.Width, Image.Height);
						dxImage.rowPitch = ImageInfo.RowPitch;
						dxImage.slicePitch = ImageInfo.TotalBytes;
						dxImage.pixels = Pixels;
					}
					Pixels += dxImage.slicePitch;
				}
			}

			DirectX::TexMetadata dxtMetadata {};
			{
				dxtMetadata.width = Image.Width;
				dxtMetadata.height = Image.Height;
				dxtMetadata.depth = 1;
				dxtMetadata.arraySize = Image.NumLayers;
				dxtMetadata.mipLevels = Image.MipLevels;
				dxtMetadata.format = DXGI_FORMAT(DXGIFormatFromFormat(Image.Format));
				dxtMetadata.dimension = DirectX::TEX_DIMENSION_TEXTURE2D;
			}

			HRESULT Result = DirectX::SaveToDDSFile(dxImages.Data, dxImages.Size * Image.MipLevels, dxtMetadata, DirectX::DDS_FLAGS_NONE, FilePathW.Data);
			//HRESULT Result = DirectX::SaveToDDSFile(dxImages[0], DirectX::DDS_FLAGS_NONE, FilePathW.Data);
			return Succeeded(Result);
		}
		else
		{
			FImageInfo ImageInfo = GetImageInfo(Image.Format, Image.Width, Image.Height);
			FIBITMAP * FiBitmap = FreeImage_ConvertFromRawBits(
				(BYTE *)Image.Bytes.Data,
				Image.Width, Image.Height, (int32)ImageInfo.RowPitch,
				ImageInfo.BitsPerPixel,
				FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK,
				TRUE);
			FWString FilePathW = FEncoding::ToWide(FilePath);
			BOOL Result = FreeImage_SaveU(FromImageFormat(ImageFormat), FiBitmap, FilePathW.Data, 0);
			FreeImage_Unload(FiBitmap);
			return Result;
		}
	}

	IImageRef IImage::Convert(FImageData & Image, EFormat Format)
	{
		if (IsDXTCompressFormat(Image.Format))
		{
			TList<DirectX::Image> dxImages { Image.NumLayers * Image.MipLevels };
			uint8_t * Pixels = (uint8_t *)Image.Bytes.Data;
			for (uintx ItemIndex = 0; ItemIndex < Image.NumLayers; ++ItemIndex)
			{
				for (uintx MipmapIndex = 0; MipmapIndex < Image.MipLevels; ++MipmapIndex)
				{
					uintx SliceIndex = Image.MipLevels * ItemIndex + MipmapIndex;
					DirectX::Image & dxImage = dxImages[SliceIndex];
					if (Image.Slices.Size)
					{
						const FImageSlice & ImageSlice = Image.Slices[SliceIndex];

						dxImage.width = ImageSlice.Width;
						dxImage.height = ImageSlice.Height;
						dxImage.format = DXGI_FORMAT(DXGIFormatFromFormat(ImageSlice.Format));
						dxImage.rowPitch = ImageSlice.RowPitch;
						dxImage.slicePitch = ImageSlice.SlicePitch;
						dxImage.pixels = Pixels;
					}
					else
					{
						dxImage.width = Image.Width;
						dxImage.height = Image.Height;
						dxImage.format = DXGI_FORMAT(DXGIFormatFromFormat(Image.Format));
						FImageInfo ImageInfo = GetImageInfo(Image.Format, Image.Width, Image.Height);
						dxImage.rowPitch = ImageInfo.RowPitch;
						dxImage.slicePitch = ImageInfo.TotalBytes;
						dxImage.pixels = Pixels;
					}
					Pixels += dxImage.slicePitch;
				}
			}

			DirectX::TexMetadata dxtMetadata {};
			{
				dxtMetadata.width = Image.Width;
				dxtMetadata.height = Image.Height;
				dxtMetadata.depth = 1;
				dxtMetadata.arraySize = Image.NumLayers;
				dxtMetadata.mipLevels = Image.MipLevels;
				dxtMetadata.format = DXGI_FORMAT(DXGIFormatFromFormat(Image.Format));
				dxtMetadata.dimension = DirectX::TEX_DIMENSION_TEXTURE2D;
			}

			DirectX::ScratchImage dxtDecompressedImage;
			HRESULT Result = DirectX::Decompress(dxImages.Data, dxImages.Size, dxtMetadata, DXGI_FORMAT(DXGIFormatFromFormat(Format)), dxtDecompressedImage);
			HRError(Result, u8"DirectX::Convert failed"V);

			return new FDirectXTexImage(Move(dxtDecompressedImage));
		}
		else
		{
			if (!IsSwizzleFormat(Image.Format) || !IsSwizzleFormat(Format))
				return nullptr;

			FImageInfo ImageInfo = GetImageInfo(Image.Format, Image.Width, Image.Height);
			FIBITMAP * fiBitmap = FreeImage_ConvertFromRawBits(
				(BYTE *)Image.Bytes.Data,
				Image.Width, Image.Height, (int32)ImageInfo.RowPitch,
				ImageInfo.BitsPerPixel,
				FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK,
				TRUE);
			if (!fiBitmap)
				return nullptr;
			FreeImage_FlipVertical(fiBitmap);

			auto ConvertedImage = new FFreeImageImage(fiBitmap);
			ConvertedImage->ImageFormat = EImageFormat::None;
			return ConvertedImage;
		}
	}

	IImageRef IImage::CompressDDS(FImageData & Image, EFormat Format, uint32 Miplevels)
	{
		if (!IsDXTCompressFormat(Format))
			return nullptr;

		if (Miplevels == 0)
			Miplevels = BitFindForward(Max(Image.Width, Image.Height)) + 1;

		const FImageSlice & ImageSlice = Image.Slices[0];

		DirectX::Image dxtBaseImage;
		{
			dxtBaseImage.format = DXGI_FORMAT(DXGIFormatFromFormat(ImageSlice.Format));
			dxtBaseImage.width = ImageSlice.Width;
			dxtBaseImage.height = ImageSlice.Height;
			dxtBaseImage.rowPitch = ImageSlice.RowPitch;
			dxtBaseImage.slicePitch = ImageSlice.SlicePitch;
			dxtBaseImage.pixels = (uint8_t *)Image.Bytes.Data;
		}

		DirectX::TexMetadata dxtSourceMetadata {};
		{
			dxtSourceMetadata.width = ImageSlice.Width;
			dxtSourceMetadata.height = ImageSlice.Height;
			dxtSourceMetadata.depth = 1;
			dxtSourceMetadata.arraySize = 1;
			dxtSourceMetadata.mipLevels = Miplevels;
			dxtSourceMetadata.format = dxtBaseImage.format;
			dxtSourceMetadata.dimension = DirectX::TEX_DIMENSION_TEXTURE2D;
		}
		HRESULT Result = S_OK;

		DirectX::ScratchImage dxtMipmapedImage;
		Result = DirectX::GenerateMipMaps(dxtBaseImage, DirectX::TEX_FILTER_DEFAULT, Miplevels, dxtMipmapedImage, false);
		HRError(Result, u8"DirectX::GenerateMipMaps failed"V);

		DXGI_FORMAT CompressedFormat = DXGI_FORMAT(DXGIFormatFromFormat(Format));
		DirectX::ScratchImage dxtCompressedImage;
		Result = DirectX::Compress(dxtMipmapedImage.GetImages(), dxtMipmapedImage.GetImageCount(), dxtSourceMetadata, CompressedFormat, DirectX::TEX_COMPRESS_FLAGS::TEX_COMPRESS_DEFAULT, DirectX::TEX_THRESHOLD_DEFAULT, dxtCompressedImage);
		HRError(Result, u8"DirectX::Compress failed"V);

		return new FDirectXTexImage(Move(dxtCompressedImage));
	}

	IImageRef IImage::CompressASTC(FImageData & Image, EFormat Format, uint32 Miplevels, const FCompressConfigASTC & CompressConfig)
	{
		if (!IsASTCompressFormat(Format))
			return nullptr;

		if (!IsSwizzleFormat(Image.Format))
		{
			IImageRef SwizzleImage = Convert(Image, EFormat::R8G8B8A8UF);
			return CompressASTC(SwizzleImage->ImageData, Format, Miplevels);
		}

		uint3 BlockSize {};
		switch (Format)
		{
		case EFormat::ASTC4x4: BlockSize = { 4, 4, 1 }; break;
		case EFormat::ASTC5x4: BlockSize = { 5, 4, 1 }; break;
		case EFormat::ASTC5x5: BlockSize = { 5, 5, 1 }; break;
		case EFormat::ASTC6x5: BlockSize = { 6, 5, 1 }; break;
		case EFormat::ASTC6x6: BlockSize = { 6, 6, 1 }; break;
		case EFormat::ASTC8x5: BlockSize = { 8, 5, 1 }; break;
		case EFormat::ASTC8x6: BlockSize = { 8, 5, 1 }; break;
		default:
			LogError(u8"ASTC format is invlaid."V);
		}

		uint32 CompressFlags = 0;
		if (CompressConfig.IsNormalMap)
			CompressFlags |= ASTCENC_FLG_MAP_NORMAL;

		astcenc_config EncodeConfig;
		astcenc_error Status = astcenc_config_init(astcenc_profile::ASTCENC_PRF_LDR, BlockSize.X, BlockSize.Y, BlockSize.Z, CompressConfig.ComporessQuality, 0, &EncodeConfig);

		if (Status != ASTCENC_SUCCESS)
		{
			LogError(u8"astcenc_config_init has failed: {}"V, (const char8 *)astcenc_get_error_string(Status));
			return nullptr;
		}

		astcenc_swizzle EncSwizzle { ASTCENC_SWZ_R, ASTCENC_SWZ_G, ASTCENC_SWZ_B, ASTCENC_SWZ_A };
		if (CompressConfig.IsHDR)
		{
			// BC6H does not support A
			EncSwizzle.a = ASTCENC_SWZ_1;
		}

		if (!IsAlphaFormat(Format))
		{
			EncSwizzle.a = ASTCENC_SWZ_1;
		}

		astcenc_context* EncContext = nullptr;
		{
			uint32 EncThreadCount = 1;
			Status = astcenc_context_alloc(&EncodeConfig, EncThreadCount, &EncContext);
			if (Status != ASTCENC_SUCCESS)
			{
				LogError(u8"astcenc_context_alloc has failed: {}"V, (const char8 *)astcenc_get_error_string(Status));
				return nullptr;
			}
		}

		const int AlignedSizeX = AlignWith(Image.Width, BlockSize.Width);
		const int AlignedSizeY = AlignWith(Image.Height, BlockSize.Height);
		const int WidthInBlocks = AlignedSizeX / BlockSize.Width;
		const int HeightInBlocks = AlignedSizeY / BlockSize.Height;

		const int64 SizePerSlice = (int64)WidthInBlocks * HeightInBlocks * 16;

		TList<uint8 *> ImageSrcData;
		ImageSrcData.ReserveTo(Image.NumLayers);

		const uint8 * Pixels = Image.Bytes.Data;
		for (uint32 SliceIdx = 0; SliceIdx < Image.NumLayers; SliceIdx++)
		{
			const FImageSlice & ImageSlice = Image.Slices[SliceIdx];
			ImageSrcData.Add((uint8 *)Pixels);

			Pixels += ImageSlice.SlicePitch;
		}

		astcenc_image EncImage;
		EncImage.dim_x = Image.Width;
		EncImage.dim_y = Image.Height;
		EncImage.dim_z = Image.NumLayers;
		EncImage.data = (void**)ImageSrcData.GetData();
		EncImage.data_type = (CompressConfig.IsHDR ? ASTCENC_TYPE_F16 : ASTCENC_TYPE_U8);

		TList<byte> CompressedBytes(SizePerSlice * Image.NumLayers, EInitializeMode::None);
		Status = astcenc_compress_image(EncContext, &EncImage, &EncSwizzle, CompressedBytes.Data, CompressedBytes.Size, 0);
		if (Status != ASTCENC_SUCCESS)
		{
			LogError(u8"astcenc_context_alloc has failed: {}"V, (const char8 *)astcenc_get_error_string(Status));
			return nullptr;
		}
		return nullptr;
	}

	EImageFormat IImage::GetImageFormat(FStringV FilePath)
	{
		FWStringV FilePathW = FEncoding::ToWide(FilePath);
		FREE_IMAGE_FORMAT FiFormat = FreeImage_GetFileTypeU(FilePathW.GetData());
		return FromFreeImageFormat(FiFormat);
	}

	IImageRef IImage::Load(IDataStream & DataStream)
	{
		FreeImageIO FIIO { FIReadProc, FIWriteProc, FISeekProc, FITellProc };

		//FreeImage_GetFileTypeFromMemory();
		FREE_IMAGE_FORMAT FiFormat = FreeImage_GetFileTypeFromHandle(&FIIO, &DataStream);
		if (FiFormat == FIF_UNKNOWN)
			return nullptr;

		if (FiFormat == FIF_DDS)
		{
			DataStream.Seek(EStreamSeek::Begin, 0);

			uintx BufferSize = DataStream.Size();
			TList<byte> Buffer(BufferSize, EInitializeMode::None);
			DataStream.Read(Buffer.Data, BufferSize);

			DirectX::TexMetadata dxtMetadata;
			DirectX::ScratchImage dxtImage;
			HRESULT Result = DirectX::LoadFromDDSMemory(Buffer.Data, BufferSize, DirectX::DDS_FLAGS_NONE, &dxtMetadata, dxtImage);
			HRError(Result, u8"DirectX::LoadFromDDSMemory failed"V);

			auto ImageData = new FDirectXTexImage(Move(dxtImage));
			ImageData->ImageFormat = EImageFormat::DDS;
			return ImageData;
		}
		else
		{
			uint32 LoadFlags = /*JPEG_DEFAULT*/0;
			FIBITMAP * FileFIBitmap = FreeImage_LoadFromHandle(FiFormat, &FIIO, &DataStream, LoadFlags);
			FIBITMAP * FIBitmap = FreeImage_ConvertTo32Bits(FileFIBitmap);
			FreeImage_Unload(FileFIBitmap);
			if (!FIBitmap)
				return nullptr;

			auto ImageData = new FFreeImageImage(FIBitmap);
			ImageData->ImageFormat = FromFreeImageFormat(FiFormat);
			FreeImage_FlipVertical(FIBitmap);

			return ImageData;
		}
	}

	IImageRef IImage::Load(FStringV FileName)
	{
		FFileStream FileStream(FileName, EFileModes::Input);
		return Load(FileStream);
	}

	bool IImage::Convert(FStringV SourceFileName, FStringV TargetFileName, EImageFormat ImageFormat)
	{
		IImageRef Image = Load(SourceFileName);
		return Save(*Image, TargetFileName, ImageFormat);
	}
}
