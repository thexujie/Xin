#include "PCH.h"
#include "BaseEngine.h"

namespace Xin::Rendering
{
	TArray<FStringV, 4> AssetExtensions =
	{
		u8".xmsh"V,
		u8".xmtt"V,
		u8".xmtl"V,
		u8".xtex"V,
	};

	IAssetStreamRef IAssetLoader::LoadAssetPrivate(FStringV AssetPath, FStringV FilePath)
	{
		return new FFileAssetStream(AssetPath, FilePath);
	}

	IFileAssetStream::IFileAssetStream(FStringV AssetPath, FStringV FilePath)
		: IAssetStream(AssetPath, FilePath)
	{
	}

	FFileAssetStream::FFileAssetStream(FStringV AssetPath, FStringV FilePath)
		: IFileAssetStream(AssetPath, FilePath)
	{
		FileStream.Open(FilePath, EFileModes::Input);
	}

	bool FGlobalAssetLoader::Exists(FStringV AssetPath) const
	{
		return FFile::Exists(AssetPath);
	}

	IAssetStreamRef FGlobalAssetLoader::LoadAsset(FStringV AssetPath)
	{
		FString FilePath = FPath::LexicallyNormal(AssetPath);
		if (!FFile::Exists(FilePath))
			return nullptr;

		return LoadAssetPrivate(AssetPath, FilePath);
	}

	bool FLocalAssetLoader::Exists(FStringV AssetPath) const
	{
		FStringV FilePath = AssetsPath ? FPath::Combine(AssetsPath, AssetPath) : AssetPath;
		return FFile::Exists(FilePath);
	}

	IAssetStreamRef FLocalAssetLoader::LoadAsset(FStringV AssetPath)
	{
		FStringV EngineAssetPath = AssetsPath ? FPath::Combine(AssetsPath, AssetPath) : AssetPath;
		if (!FFile::Exists(EngineAssetPath))
			return nullptr;

		FString FilePath = FPath::LexicallyNormal(EngineAssetPath);
		if (!FFile::Exists(FilePath))
			return nullptr;

		return LoadAssetPrivate(AssetPath, FilePath);
	}

	FEngine::FEngine(IRHIDevice & Device) : Device(Device)
	{
		FModule EngineModule = FModule::Current();
		FModule ApplicationModule = FModule::Application();

		constexpr FStringV Binaries_x64 = u8"Binaries/x64"V;
		AssertExpr(EngineModule.ModulePath.Contains(Binaries_x64));
		AssertExpr(ApplicationModule.ModulePath.Contains(Binaries_x64));

		FString EnginePath = FPath::GetAbsolutePath(FPath::GetDirectory(EngineModule.ModulePath), u8"../../");
		FString ProjectPath = FPath::GetAbsolutePath(FPath::GetDirectory(ApplicationModule.ModulePath), u8"../../");
		FConsole::WriteLine(u8"EnginePath: \"{}\"."V, EnginePath);
		FConsole::WriteLine(u8"ProjectPath: \"{}\"."V, ProjectPath);

		DocumentsPath = FPath::GetAbsolutePath(ProjectPath, u8"/Documents/");

		FString EngineAssetsPath = FPath::GetAbsolutePath(EnginePath, u8"/Xin/Assets/");
		FString ProjectAssetsPath = FPath::GetAbsolutePath(ProjectPath, u8"/Assets/");
		AssertExpr(FDirectory::Exists(EngineAssetsPath));
		AssertExpr(FDirectory::Exists(ProjectAssetsPath));

		GlobalAssetLoader = new FGlobalAssetLoader();
		EngineAssetLoader = new FLocalAssetLoader(EngineAssetsPath);
		ProjectAssetLoader = new FLocalAssetLoader(ProjectAssetsPath);

		AssetLoaders.AddRange({ EngineAssetLoader, ProjectAssetLoader });
	}

	FEngine::~FEngine() {}

	void FEngine::AddAssetsPath(FStringV AssetsPath)
	{
		AssetLoaders.Add(new FLocalAssetLoader(AssetsPath));
	}

	IAssetStreamRef FEngine::LoadStream(FStringV AssetPath)
	{
		if (auto AssetStream = GlobalAssetLoader->LoadAsset(AssetPath))
			return AssetStream;

		if (AssetPath.StartsWith(u8"Engine/"V))
			return EngineAssetLoader->LoadAsset(AssetPath.SubView(u8"Engine/"V.Size));

		for (auto & AssetLoader : AssetLoaders)
		{
			if (auto AssetStream = AssetLoader->LoadAsset(AssetPath))
				return AssetStream;
		}
		return nullptr;
	}
}
