#pragma once

#include "Pipeline.Types.h"
#include "../Engine/Scene.h"

namespace Xin::Engine
{
	struct FDrawCommand;
	class FRenderPass;
	class FRenderGraph;

	struct FRGResourceDesc
	{
		EResourceDimension Dimension = EResourceDimension::None;
		EFormat Format = EFormat::None;

		Size2U Size = { 0, 1 };
		uint32 Depth = 1;
		uint32 MipLevels = 1;
		FMSAA MSAA;

		FClearValue ClearValue;

		EResourceFlags Flags = EResourceFlags::None;

		bool operator ==(const FRGResourceDesc &) const = default;
		auto operator <=>(const FRGResourceDesc &) const = default;

		FRGResourceDesc() = default;
		FRGResourceDesc(const FRGResourceDesc &) = default;
		FRGResourceDesc & operator =(const FRGResourceDesc &) = default;

		FRGResourceDesc(const FResourceDesc & ResourceDesc)
			: Dimension(ResourceDesc.Dimension), Format(ResourceDesc.Format), Size(ResourceDesc.Size)
			, Depth(ResourceDesc.Layers), MipLevels(ResourceDesc.Mipmaps), MSAA(ResourceDesc.MSAA)
			, ClearValue(ResourceDesc.ClearValue), Flags(ResourceDesc.Flags) { }

		FRGResourceDesc & operator =(const FResourceDesc & ResourceDesc)
		{
			Dimension = ResourceDesc.Dimension;
			Format = ResourceDesc.Format;
			Size = ResourceDesc.Size;
			Depth = ResourceDesc.Layers;
			MipLevels = ResourceDesc.Mipmaps;
			MSAA = ResourceDesc.MSAA;
			ClearValue = ResourceDesc.ClearValue;
			Flags = ResourceDesc.Flags;
			return *this;
		}

		operator FResourceDesc() const
		{
			FResourceDesc ResourceDesc;
			ResourceDesc.Dimension = Dimension;
			ResourceDesc.Format = Format;
			ResourceDesc.Size = Size;
			ResourceDesc.Layers = Depth;
			ResourceDesc.Mipmaps = MipLevels;
			ResourceDesc.MSAA = MSAA;
			ResourceDesc.ClearValue = ClearValue;
			ResourceDesc.Flags = Flags;
			return ResourceDesc;
		}

		// Texture Desc Starts With (Format, Size)
		FRGResourceDesc(EFormat Format, Size4U Size, FClearValue ClearValue = {}, EResourceFlags Flags = EResourceFlags::None)
		{
			this->Dimension = EResourceDimension::Texture2D;
			this->Format = Format;
			this->Size = Size.XY;
			this->Depth = Size.Z > 1 ? Size.Z : 1;
			this->MipLevels = Size.W > 1 ? Size.W : 1;
			this->ClearValue = ClearValue;
			this->Flags = Flags;
		}

		FRGResourceDesc(EFormat Format, Size2U Size, FClearValue ClearValue = {}, EResourceFlags Flags = EResourceFlags::None) : FRGResourceDesc(Format, { Size, 1, 1 }, ClearValue, Flags) { }

		FRGResourceDesc(uint32 BufferSize, EFormat Format = EFormat::None)
		{
			this->Dimension = EResourceDimension::Buffer;
			this->Format = Format;
			this->Size.X = BufferSize;
			this->Size.Y = 1;
		}

		FRGResourceDesc(Size2U TextureSize, EFormat Format = EFormat::None, EResourceFlags Flags = EResourceFlags::None)
		{
			this->Dimension = EResourceDimension::Buffer;
			this->Format = Format;
			this->Size = TextureSize;
			this->Flags = Flags;
		}
	};

	class ENGINE_API FRGResource : public IEngineObject
	{
	public:
		FRGResource(FString Semantic, const FRGResourceDesc & Desc) : Semantic(Semantic), Desc(Desc) {}

		FRGResource(FString Semantic, IRHIResource * Resource)
			: Semantic(Semantic), Desc(Resource->ResourceDesc), Resource(Resource) {}

	public:
		FString Semantic;
		FRGResourceDesc Desc;
		IRHIResourceRef Resource;
		FTransientResourceRef TransientResource;

		EResourceAccess ResourceAccess = EResourceAccess::None;
		EResourceAccess InitialAccess = EResourceAccess::None;
		EResourceAccess CurrentAccess = EResourceAccess::None;

		FRenderPass * InitialPass = nullptr;
		FRenderPass * FinalPass = nullptr;
	};
	using FRGResourceRef = TReferPtr<FRGResource>;

	class ENGINE_API FRGResourceView : public IEngineObject
	{
	public:
		FRGResourceView(FRGResource * RGResource, IRHIResourceView * ResourceView) : RGResource(RGResource), Desc(ResourceView->ViewDesc), ResourceView(ResourceView) {}
		FRGResourceView(FRGResource * RGResource, const FResourceViewDesc & Desc) : RGResource(RGResource), Desc(Desc) {}
		FRGResourceView(FRGResource * RGResource, uint32 SubresourceIndex) : RGResource(RGResource), Desc(RGResource->Desc.Format, SubresourceIndex) {}

	public:
		FRGResource * RGResource = nullptr;
		FResourceViewDesc Desc;
		IRHIResourceView * ResourceView = nullptr;
	};
	using FRGResourceViewRef = TReferPtr<FRGResourceView>;

	struct FRGRenderPassTarget
	{
		IRHIResourceView * ResourceView = nullptr;
		EResourceLoad LoadAction = EResourceLoad::None;
		FClearValue ClearValue = { };

		FRGRenderPassTarget() = default;

		FRGRenderPassTarget(nullptr_t) {}

		FRGRenderPassTarget(IRHIResourceView * ResourceView, EResourceLoad LoadAction = EResourceLoad::None, FClearValue ClearValue = {})
			: ResourceView(ResourceView), LoadAction(LoadAction), ClearValue(ClearValue) { }

		FRGRenderPassTarget(FRGResourceView * ResourceView, EResourceLoad LoadAction = EResourceLoad::None, FClearValue ClearValue = {})
			: ResourceView(ResourceView->ResourceView), LoadAction(LoadAction), ClearValue(ClearValue) { }

		FRGRenderPassTarget(FRGResource * Resource, EResourceLoad LoadAction = EResourceLoad::None, FClearValue ClearValue = {})
			: LoadAction(LoadAction), ClearValue(ClearValue)
		{
			ResourceView = Resource->Resource->GetResourceView();
		}

		constexpr operator bool() const { return !!ResourceView; }
	};


	class FRGContext;
	class FCompileContext;

	class ENGINE_API FRenderPass : public IEngineObject
	{
	public:
		FRenderPass(FStringV PassName) : PassName(PassName) {}

		virtual void Compile(FCompileContext & CompileContext) {}
		virtual void Excute(FRGContext & Context) {}

	public:
		FString PassName;

		struct FPassResource
		{
			FRGResource * RGResource;
			EResourceAccess PassResourceAccess;
			EResourceLoad ResourceLoad;
		};

		TList<FPassResource> PassResources;

		TList<FRGResource *> InitialResources;
		TList<FRGResource *> FinalResources;

		struct FPassResourceView
		{
			FRGResourceView * RGResourceView = nullptr;
			EResourceAccess ResourceViewAccess;
			EResourceLoad ResourceViewLoad;
		};

		TList<FPassResourceView> PassResourceViews;
	};

	using FRenderPassPtr = FRenderPass *;
	using FRenderPassRef = TReferPtr<FRenderPass>;

	class FRenderGraph;
	constexpr uintx MaxSceneDownSamples = 8;

	class ENGINE_API FCompileContext
	{
	public:
		FCompileContext(FRenderPass & RenderPass) : RenderPass(RenderPass) {}

		void AccessResource(FRGResource * Resource, EResourceAccess ResourceAccess, EResourceLoad ResourceLoad = EResourceLoad::None);
		void AccessResourceView(FRGResourceView * ResourceView, EResourceAccess ResourceAccess, EResourceLoad ResourceLoad = EResourceLoad::None);

	public:
		FRenderPass & RenderPass;
	};

	class ENGINE_API FRGContext
	{
	public:
		FRGContext(FRenderGraph & RenderGraph, FCommandList & CommandList);

	public:
		void SetMarker(FStringV Event, const FColor & Color = {}) { CommandList.SetMarker(Event, Color); }
		void BeginEvent(FStringV Event, const FColor & Color = {}) { CommandList.BeginLabel(Event, Color); }
		void EndEvent() { CommandList.EndLabel(); }

		void ClearDepthStencil(IRHIResource * RHIResource, float32 Depth, uint8 Stencial) { CommandList.ClearDepthStencil(RHIResource->GetResourceView(), Depth, Stencial); }
		void ClearRenderTarget(IRHIResource * RHIResource, FColorF Color) { CommandList.ClearRenderTarget(RHIResource->GetResourceView(), Color); }
		void ClearUnorderedAccess(IRHIResource * RHIResource, uint32 ClearValue);

		void SetViewPorts(TView<FViewport> ViewPorts) { CommandList.SetViewPorts(ViewPorts); }
		void SetScissorRects(TView<RectI> ScissorRects) { CommandList.SetScissorRects(ScissorRects); }

		void BeginPass(const TArray<FRGRenderPassTarget, MaxRenderTargets> & RenderTargets, const FRGRenderPassTarget & DepthStencil, const FViewport & Viewport);
		void EndPass();

	public:
		void DispatchCommand(const FDrawCommand & MeshCommand);
		void DispatchCommands(TView<FDrawCommand> MeshCommands);

	public:
		FRenderGraph & RenderGraph;
		FCommandList & CommandList;
	};


	template<typename T>
	concept RenderPassCompileLambdaT = requires(T Lambda, FCompileContext & CompileContext)
	{
		Lambda(CompileContext);
	};

	template<typename T>
	concept RenderPassLambdaT = requires(T Lambda, FRGContext & Context)
	{
		Lambda(Context);
	};

	template<RenderPassCompileLambdaT CompileLambdaT, RenderPassLambdaT ExecuteLambdaT>
	class TRenderPass : public FRenderPass
	{
	public:
		TRenderPass(FStringV PassName, CompileLambdaT && CompileLambda, ExecuteLambdaT && ExecuteLambda) : FRenderPass(PassName), CompileLambda(Move(CompileLambda)), ExecuteLambda(Move(ExecuteLambda)) { }

		void Compile(FCompileContext & CompileContext) override
		{
			CompileLambda(CompileContext);
		}

		void Excute(FRGContext & Context) override
		{
			ExecuteLambda(Context);
		}

	public:
		CompileLambdaT CompileLambda;
		ExecuteLambdaT ExecuteLambda;
	};
}
