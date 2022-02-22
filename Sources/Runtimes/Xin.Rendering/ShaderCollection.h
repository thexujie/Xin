#pragma once

#include "Shader.h"

namespace Xin::Rendering
{
	constexpr uint32 ShaderArchiveMagicNumber = MakeFourCC("xshd");

	struct RENDERING_API FShaderArchive
	{
		friend IDataStream & operator >>(IDataStream & DataStream, FShaderArchive & ShaderArchive);
		friend IDataStream & operator <<(IDataStream & DataStream, FShaderArchive & ShaderArchive);

		EShaderStage ShaderStage = EShaderStage::None;

		FString FilePath;
		FString EntryName;

		TList<FString> FilesInclude;

		TList<byte> ShaderCode;
		FReflectParameters ReflectParameters;
	};

	class FShaderCollection;
	class RENDERING_API FShaderInstance : public IShaderInstance
	{
	public:
		FShaderInstance(FEngine & Engine, const FShaderCollection & ShaderCollection, const FShaderCompilerEnvironment & Environment);

	public:
		EShaderStage GetShaderStage() const override { return ShaderArchive.ShaderStage; }
		FStringV GetFilePath() const override { return ShaderArchive.FilePath; }
		FStringV GetEntryName() const override { return ShaderArchive.EntryName; }

		TView<byte> GetShaderCode() const override { return ShaderArchive.ShaderCode; }
		const FReflectParameters & GetReflectParameters() const override { return ShaderArchive.ReflectParameters; }
		IRHIShaderModule * GetShaderModule() override { return ShaderModule; }

	public:
		FEngine & Engine;
		const FShaderCollection & ShaderCollection;
		FShaderCompilerEnvironment Environment;

	public:
		FShaderArchive ShaderArchive;
		IRHIShaderModuleRef ShaderModule;
	};
	using FShaderInstanceRef = TReferPtr<FShaderInstance>;


	class FShaderCollection: public IRenderingObject
	{
	public:
		FShaderCollection(FEngine & Engine, const IShaderSource & ShaderSource) : Interface(Engine), ShaderSource(ShaderSource)
		{
			ShaderInstances.Resize(ShaderSource.GetVarianceCount());
		}

	public:
		IShaderInstance * GetShader(const IShaderVariance & ShaderVariance = FEmptyShaderVariance()) const
		{
			uintx ShaderVarianceIndex = ShaderVariance.ComputeShaderVarianceIndex();
			if (!ShaderInstances[ShaderVarianceIndex])
			{
				FShaderCompilerEnvironment Environment;
				ShaderVariance.AffectEnvironment(Environment);
				ShaderInstances[ShaderVarianceIndex] = new FShaderInstance(Interface, *this, Environment);
			}
			return ShaderInstances[ShaderVarianceIndex];
		}

	public:
		FEngine & Interface;
		const IShaderSource & ShaderSource;
		mutable TList<FShaderInstanceRef> ShaderInstances;
	};
	using FShaderCollectionRef = TReferPtr<FShaderCollection>;


	class FShaderBundle : public IRenderingObject
	{
	public:
		FShaderBundle(FEngine & Engine) : Engine(Engine)
		{
		}

		IShaderInstance * GetShader(const IShaderSource & ShaderSource, const IShaderVariance & ShaderVariance = FEmptyShaderVariance()) const
		{
			if (auto Iter = ShaderCollections.Find(&ShaderSource); Iter != ShaderCollections.End())
				return Iter->Second->GetShader(ShaderVariance);
			else
			{
				auto [InsertIter, Success] = ShaderCollections.Insert({ &ShaderSource, new FShaderCollection(Engine, ShaderSource) });
				return InsertIter->Second->GetShader(ShaderVariance);
			}
		}

		template<typename ShaderT>
		IShaderInstance * GetShaderT()
		{
			return GetShader(ShaderT::Instance());
		}

		template<typename ShaderT>
		IShaderInstance * GetShaderT(const typename ShaderT::FShaderVariance & ShaderVariance)
		{
			return GetShader(ShaderT::Instance(), ShaderVariance);
		}

	private:
		FEngine & Engine;
		mutable TMap<TPointerPtr<const IShaderSource>, FShaderCollectionRef> ShaderCollections;
	};
	using FShaderCollectionRef = TReferPtr<FShaderCollection>;
}
