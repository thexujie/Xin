#pragma once

#include "Asset.h"
#include "Texture.h"

namespace Xin::Engine
{
	class FMaterialResource;

	enum class EAlphaMode
	{
		None = 0,
		Blend,
		Mask,
	};

	struct FMaterialSettings
	{
		FMaterialSettings() = default;
		//FMaterialSettings(const FMaterialSettings & Another) : Flag0(Another.Flag0) {}
		FMaterialSettings(const FMaterialSettings & Another) = default;

		FMaterialSettings & operator =(const FMaterialSettings & Another)
		{
			Flag0 = Another.Flag0;
			return *this;
		}

		auto operator <=>(const FMaterialSettings & Another) const noexcept { return Flag0 <=> Another.Flag0; }
		bool operator ==(const FMaterialSettings & Another) const noexcept { return Flag0 == Another.Flag0; }

		union
		{
			uint32 Flag0 = 0;

			struct
			{
				uint32 SSAO : 1;
				uint32 CascadeCount : 3;
			};
		};
	};

	class UMaterialTemplate;

	class ENGINE_API UMaterial : public IAssetResource
	{
	public:
		UMaterial(FEngine & Engine, FStringV AssetPath);
		~UMaterial() { }

		bool LoadAsset(FJsonObject & Json) override;
		virtual UMaterialTemplate & GetTemplate() const = 0;

	public:
		TMap<FString, UTextureRef> TextureParameters;
		TMap<FString, int64> IntegerParameters;
		TMap<FString, FString> Definetions;
	};
	using UMaterialRef = TReferPtr<UMaterial>;

	ENGINE_API  UMaterialRef LoadMaterial(FEngine & Engine, FStringV AssetPath);


	class ENGINE_API UMaterialTemplate : public UMaterial
	{
	public:
		UMaterialTemplate(FEngine & Engine, FStringV AssetPath);
		~UMaterialTemplate() { }

	public:
		bool LoadAsset(FJsonObject & Json) override;
		UMaterialTemplate & GetTemplate() const override { return ConstCast(*this); }

	public:
		FString ShaderAssetPath;

	private:
		struct FShaderCacheKey
		{
			FMaterialSettings MaterialSettings;
			EMeshPass MeshPass;

			bool operator ==(const FShaderCacheKey &) const noexcept = default;
			auto operator <=>(const FShaderCacheKey &) const noexcept = default;
		};

		mutable TMap<FShaderCacheKey, TArray<IRHIShaderModule *, MaxShaderStages>> ShadersCache;
	};
	using UMaterialTemplateRef = TReferPtr<UMaterialTemplate>;


	class ENGINE_API UMaterialInstance : public UMaterial
	{
	public:
		UMaterialInstance(FEngine & Engine, FStringV AssetPath);
		~UMaterialInstance() { }

	public:
		bool LoadAsset(FJsonObject & Json) override;
		UMaterialTemplate & GetTemplate() const override { AssertExpr(Parent); return Parent->GetTemplate(); }

		UMaterialRef Parent;
	};
	using UMaterialInstanceRef = TReferPtr<UMaterialInstance>;


	class ENGINE_API FMaterialResource final : public IRenderingObject
	{
	public:
		FMaterialResource(FEngine & Engine, UMaterial & Material);

		TArray<IShaderInstance *, MaxShaderStages> GetShaders(
			const IShaderSource & VertexShader, const IShaderVariance & VertexShaderVariance,
			const IShaderSource & PixelShader, const IShaderVariance & PixelShaderVariance) const;
		void GetParameters(FCommandList & CommandList) const;

		FMaterialResource * GetFallback() const { return nullptr; }

		void InitializeRHI(FCommandExecutor & CommandExecutor);

	public:
		void SetTextureParameter(FStringV Name, FTextureResourceRef Texture)
		{
			Textures[Name] = Texture;
			TexturesChanged = true;
		}

	public:
		FEngine & Engine;
		UMaterial & Material;

		FShaderBundle ShaderBundle { Engine };

		TMap<FName, FTextureResourceRef> Textures;
		bool TexturesChanged = false;

	protected:
	};

	using FMaterialResourceRef = TReferPtr<FMaterialResource>;
}
