#include "PCH.h"
#include "Material.h"

#include "Engine.h"

namespace Xin::Engine
{
	class FMaterialShaderVariance : public IShaderVariance
	{
	public:
		FMaterialShaderVariance(UMaterial & Material, const IShaderVariance & BaseShaderVariance) : Material(Material), BaseShaderVariance(BaseShaderVariance)
		{

		}

		uintx ComputeShaderVarianceIndex() const override { return BaseShaderVariance.ComputeShaderVarianceIndex(); }

		void AffectEnvironment(FShaderCompilerEnvironment & Environment) const override
		{
			Environment.IncludeFileMapping[u8"Material.xsh"V] = Material.GetTemplate().ShaderAssetPath;
			for (const auto & Definetion : Material.Definetions)
				Environment.Definitions.Add({ Definetion.First, Definetion.Second });
			BaseShaderVariance.AffectEnvironment(Environment);
		}

		UMaterial & Material;
		const IShaderVariance & BaseShaderVariance;
	};

	UMaterial::UMaterial(FEngine & Engine, FStringV AssetPath)
		: IAssetResource(Engine, AssetPath)
	{
		
	}

	bool UMaterial::LoadAsset(FJsonObject & Json)
	{
		if (FVariant & ParametersJson = Json[u8"Parameters"V].Value)
		{
			FJsonObject & ParametersJsonObject = ParametersJson.Get<FJsonObject &>();
			for (const FJsonItem & ParameterJson : ParametersJsonObject.Items)
			{
				if (ParameterJson.Value.Kind == EVariantKind::String)
				{
					if (ParameterJson.Value.StringValue.StartsWith(AssetsPrefix))
					{
						if (ParameterJson.Value.StringValue.EndsWith(TexturePostfix))
						{
							TextureParameters[ParameterJson.Name] = LoadTexture(Engine, ParameterJson.Value.StringValue);
							continue;
						}
					}
				}
				else if (ParameterJson.Value.Kind == EVariantKind::Integer)
					IntegerParameters[ParameterJson.Name] = ParameterJson.Value.Get<int64>();
				else { }
			}
		}

		if (FVariant & DefinetionsJson = Json[u8"Definetions"V].Value)
		{
			FJsonObject & DefinetionsJsonObject = DefinetionsJson.Get<FJsonObject &>();
			for (const FJsonItem & DefinetionJson : DefinetionsJsonObject.Items)
			{
				if (DefinetionJson.Value.Kind == EVariantKind::Integer)
					Definetions[DefinetionJson.Name] = Format(u8"{}"V, DefinetionJson.Value.IntegerValue);
				if (DefinetionJson.Value.Kind == EVariantKind::String)
					Definetions[DefinetionJson.Name] = DefinetionJson.Value.StringValue;
				else { }
			}
		}
		return true;
	}

	UMaterialRef LoadMaterial(FEngine & Engine, FStringV AssetPath)
	{
		IAssetStreamRef AssetStream = Engine.LoadStream(AssetPath);
		if (!AssetStream)
			return nullptr;

		FJsonDocument Json;
		*AssetStream >> Json;

		UMaterialRef Material;
		if (AssetPath.EndsWith(u8"xmtt"V))
			Material = new UMaterialTemplate(Engine, AssetPath);
		else
			Material = new UMaterialInstance(Engine, AssetPath);

		Material->LoadAsset(Json);
		return Material;
	}

	UMaterialTemplate::UMaterialTemplate(FEngine & Engine, FStringV AssetPath)
		: UMaterial(Engine, AssetPath)
	{
		
	}

	bool UMaterialTemplate::LoadAsset(FJsonObject & Json)
	{
		ShaderAssetPath = Json[u8"Shader"V].Value.StringValue;
		return UMaterial::LoadAsset(Json);
	}

	UMaterialInstance::UMaterialInstance(FEngine & Engine, FStringV AssetPath)
		: UMaterial(Engine, AssetPath)
	{

	}

	bool UMaterialInstance::LoadAsset(FJsonObject & Json)
	{
		FStringV MaterialParentAssetPath = Json[u8"Parent"V].Value.StringValue;
		Parent = LoadMaterial(Engine, MaterialParentAssetPath);
		TextureParameters = Parent->TextureParameters;
		IntegerParameters = Parent->IntegerParameters;
		Definetions = Parent->Definetions;
		return UMaterial::LoadAsset(Json);
	}


	FMaterialResource::FMaterialResource(FEngine & Engine, UMaterial & Material)
		: Engine(Engine), Material(Material)
	{
		for (const auto & TextureParameter : Material.TextureParameters)
			Textures[FName(TextureParameter.First)] = new FTexture(Engine, TextureParameter.Second.Ref());
	}

	void FMaterialResource::InitializeRHI(FCommandExecutor & CommandExecutor)
	{
		for (auto & TextureParameterIter : Textures)
		{
			TextureParameterIter.Second->InitializeRHI(CommandExecutor);
		}
	}

	TArray<IShaderInstance *, MaxShaderStages> FMaterialResource::GetShaders(
		const IShaderSource & VertexShader, const IShaderVariance & VertexShaderVariance,
		const IShaderSource & PixelShader, const IShaderVariance & PixelShaderVariance) const
	{
		TArray<IShaderInstance *, MaxShaderStages> Shaders { };
		Shaders[EShaderStage::Vertex] = ShaderBundle.GetShader(VertexShader, FMaterialShaderVariance { Material, VertexShaderVariance });
		Shaders[EShaderStage::Pixel] = ShaderBundle.GetShader(PixelShader, FMaterialShaderVariance { Material, PixelShaderVariance });
		return Shaders;
	}

	void FMaterialResource::GetParameters(FCommandList & CommandList) const
	{
		for (auto & TextureParameterIter : Textures)
			CommandList.SetParameter(TextureParameterIter.First, TextureParameterIter.Second->RHIResource);
	}
}
