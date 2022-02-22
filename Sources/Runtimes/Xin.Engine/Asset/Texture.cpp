#include "PCH.h"
#include "Texture.h"

#include "Engine.h"

namespace Xin::Engine
{
	UTexture::UTexture(FEngine & Engine, FStringV AssetPath) : IAssetResource(Engine, AssetPath)
	{
		
	}

	bool UTexture::LoadAsset(FJsonObject & Json)
	{
		FStringV DataFilePath = AssetPath + GetTextureCompressionFileExtention(/*Engine.Device.GetTextureCompression()*/ETextureCompression::DXTC);
		if (IAssetStreamRef DataStream = Engine.LoadStream(DataFilePath))
			*DataStream >> TextureData;
		return true;
	}

	void UTexture::LoadFromImage(const FImageData & ImageData)
	{
		TextureData = ImageData;
	}

	UTextureRef LoadTexture(FEngine & Engine, FStringV AssetPath)
	{
		IAssetStreamRef AssetStream = Engine.LoadStream(AssetPath);
		if (!AssetStream)
			return nullptr;

		FJsonDocument Json;
		*AssetStream >> Json;

		UTextureRef Texture = new UTexture(Engine, AssetPath);
		if (AssetStream->AssetPath)
		{
			Texture->LoadAsset(Json);
		}
		else
		{
			//IImage::Loa
			//EImageFormat ImageFormat = ImageCodec.GetImageFormat(AssetStream->FilePath);
			//FImageDataRef ImageData = ImageCodec.LoadImage(AssetStream.Ref(), ImageFormat);
			//if (!ImageData)
			//	return nullptr;

			////ImageCodec->CodeImage(ImageData.Ref(), u8"D:/Test.png"V, EImageFormat::BMP);
			//Texture->LoadFromImage(ImageData.Ref());
		}

		return Texture;
	}

	FTexture::FTexture(FEngine & Engine, UTexture & Texture)
		: FTextureResource(Engine, Texture.AssetPath), Texture(Texture)
	{

	}

	FTextureResource::FTextureResource(FEngine & Engine, FStringV ResourcePath)
		: IRenderResource(Engine, ResourcePath)
	{
		
	}

	void FTexture::InitializeRHI(FCommandExecutor & CommandExecutor)
	{
		if (!RHIResource)
		{
			RHIResource = CommandExecutor.CreateTexture(FTextureDesc(Texture.TextureData.Format, Texture.TextureData.Dimension, EResourceUsage::SampledTexture | EResourceUsage::CopyWrite), Texture.TextureData);
			RHIResource->SetDebugName(ResourcePath);
		}
	}

	FSourceTexture::FSourceTexture(FEngine & Engine, const FTextureData & TextureData, FStringV SourcePath)
		: FTextureResource(Engine, SourcePath), TextureData(TextureData)
	{
	}

	FSourceTexture::FSourceTexture(FEngine & Engine, FTextureData && TextureData, FStringV SourcePath)
		: FTextureResource(Engine, SourcePath), TextureData(Move(TextureData))
	{
		
	}

	void FSourceTexture::InitializeRHI(FCommandExecutor & CommandExecutor)
	{
		if (!RHIResource)
		{
			RHIResource = CommandExecutor.CreateTexture({ TextureData.Format, TextureData.Dimension, EResourceUsage::SampledTexture }, TextureData);
			RHIResource->SetDebugName(ResourcePath.ToString());
		}
	}
}
