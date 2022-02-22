#pragma once

#include "Asset.h"

namespace Xin::Engine
{
	constexpr FStringV TexturePostfix = u8".xtex"V;

	class ENGINE_API UTexture : public IAssetResource
	{
	public:
		UTexture(FEngine & Engine, FStringV AssetPath);
		~UTexture() { }

	public:
		bool LoadAsset(FJsonObject & Json) override;
		void LoadFromImage(const FImageData & ImageData);

	public:
		FTextureData TextureData {};
	};
	ENGINE_API  UTextureRef LoadTexture(FEngine & Engine, FStringV AssetPath);

	class ENGINE_API FTextureResource : public IRenderResource
	{
	public:
		FTextureResource(FEngine & Engine, FStringV ResourcePath);
		~FTextureResource() { }

	public:
		IRHIResourceRef RHIResource;
	};
	using FTextureResourceRef = TReferPtr<FTextureResource>;


	class ENGINE_API FTexture : public FTextureResource
	{
	public:
		FTexture(FEngine & Engine, UTexture & Texture);

		void InitializeRHI(FCommandExecutor & CommandExecutor) override;
		void FinalizeRHI() override {}

	public:
		UTexture & Texture;
	};


	class ENGINE_API FSourceTexture : public FTextureResource
	{
	public:
		FSourceTexture(FEngine & Engine, const FTextureData & TextureData, FStringV SourcePath);
		FSourceTexture(FEngine & Engine, FTextureData && TextureData, FStringV SourcePath);

		void InitializeRHI(FCommandExecutor & CommandExecutor) override;
		void FinalizeRHI() override {}

	public:
		FTextureData TextureData;
	};
}
