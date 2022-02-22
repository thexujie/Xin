#pragma once

#include "Xin.Rendering.Types.h"

#include "Shader.h"
#include "ShaderCollection.h"

namespace Xin::Rendering
{
	class RENDERING_API IFileAssetStream : public IAssetStream
	{
	public:
		IFileAssetStream(FStringV AssetPath, FStringV FilePath);

		uintx Read(byte * Data, uintx Size) override
		{
			return FileStream.Read(Data, Size);
		}

		uintx Write(const byte * Data, uintx Size) override
		{
			return FileStream.Write(Data, Size);
		}

		uintx Peek(byte * Data, uintx Size) override
		{
			return FileStream.Peek(Data, Size);
		}

		uintx Seek(EStreamSeek StreamSeek, intx Distance) override
		{
			return FileStream.Seek(StreamSeek, Distance);
		}

		uintx Position() const override
		{
			return FileStream.Position();
		}

		uintx Size() const override
		{
			return FileStream.Size();
		}

	protected:
		FFileStream FileStream;
	};

	class RENDERING_API FFileAssetStream : public IFileAssetStream
	{
	public:
		FFileAssetStream(FStringV AssetPath, FStringV FilePath);
	};

	using FFileAssetStreamRef = TReferPtr<FFileAssetStream>;

	class RENDERING_API IAssetLoader : public IRenderingObject
	{
	public:
		virtual bool Exists(FStringV AssetPath) const = 0;
		virtual IAssetStreamRef LoadAsset(FStringV AssetPath) = 0;

	protected:
		IAssetStreamRef LoadAssetPrivate(FStringV AssetPath, FStringV FilePath);

		TMap<FString, IAssetStreamRef> AssetStreams;
	};

	using IAssetLoaderRef = TReferPtr<IAssetLoader>;

	class RENDERING_API FGlobalAssetLoader : public IAssetLoader
	{
	public:
		FGlobalAssetLoader() {}

		bool Exists(FStringV AssetPath) const override;
		IAssetStreamRef LoadAsset(FStringV AssetPath) override;
	};

	class RENDERING_API FLocalAssetLoader : public IAssetLoader
	{
	public:
		FLocalAssetLoader(FStringV AssetsPath) : AssetsPath(AssetsPath) {}

		bool Exists(FStringV AssetPath) const override;
		IAssetStreamRef LoadAsset(FStringV AssetPath) override;

	public:
		FString AssetsPath;
	};

	class RENDERING_API FEngine : public IRenderingObject
	{
	public:
		FEngine(IRHIDevice & Device);
		~FEngine();

	public:
		virtual FStringV GetDocumentsPath() const { return DocumentsPath; }

		virtual void AddAssetsPath(FStringV AssetsPath);

		virtual IAssetStreamRef LoadStream(FStringV AssetPath);

	public:
		IRHIDevice & Device;
		FShaderBundle ShaderBundle { *this };

	protected:
		IAssetLoaderRef GlobalAssetLoader;
		IAssetLoaderRef EngineAssetLoader;
		IAssetLoaderRef ProjectAssetLoader;

		TList<IAssetLoaderRef> AssetLoaders;

	public:
		FString SavedPath;
		FString DocumentsPath;
	};

	using FEngineRef = TReferPtr<FEngine>;
}
