#pragma once

#include "../Xin.Engine.Types.h"

namespace Xin::Engine
{
	constexpr uint32 AssetMagic = MakeFourCC("xast");
	constexpr uint32 AssetVersion = 0;
	constexpr FStringV EnginePrefix = u8"Engine/"V;
	constexpr FStringV AssetsPrefix = u8"/"V;

	enum class EAssetType : uint32
	{
		None = 0,
		Texture = MakeFourCC("xtex"),
		Mesh = MakeFourCC("xmsh"),
		SkinedMesh = MakeFourCC("xsmh"),
		Map = MakeFourCC("xmap"),
		Material = MakeFourCC("xmtl"),
	};

	struct FAssetHeader
	{
		uint32 Magic = 0;
		uint32 AssetVersion = 0;
		EAssetType AssetType = EAssetType::None;
	};

	class ENGINE_API IAssetResource : public IRenderingObject
	{
	public:
		IAssetResource(FEngine & Engine, FStringV AssetPath) : Engine(Engine), AssetPath(AssetPath) {}

		virtual bool LoadAsset(FJsonObject & Json) = 0;

	public:
		FEngine & Engine;
		FString AssetPath;
	};

	using IAssetResourceRef = TReferPtr<IAssetResource>;

	class ENGINE_API IRenderResource : public IRenderingObject
	{
	public:
		IRenderResource(FEngine & Engine, FStringV ResourcePath = {})
			: Engine(Engine), ResourcePath(ResourcePath) {}

		~IRenderResource() = default;

		virtual void InitializeRHI(FCommandExecutor & CommandExecutor) {}
		virtual void FinalizeRHI() {}

	public:
		FEngine & Engine;
		FString ResourcePath;
	};
}
