#pragma once

#include "Pipeline.Types.h"
#include "RenderPass.h"

namespace Xin::Engine
{
	class FRenderPass;
	class FRenderGraph;

	DEFINE_SIMPLE_DEFAULT_SHADER(FScreenQuadVS, u8"Engine/Shaders/ScreenQuad.xsf"V, EShaderStage::Vertex);

	DEFINE_SIMPLE_DEFAULT_SHADER(FZoomPS, u8"Engine/Shaders/ZoomPS.xsf"V, EShaderStage::Pixel);

	DEFINE_SIMPLE_DEFAULT_SHADER(FRGBAVisualizePS, u8"Engine/Shaders/RGBAVisualize.xsf"V, EShaderStage::Pixel);

	DEFINE_SIMPLE_DEFAULT_SHADER(FRRR1VisualizePS, u8"Engine/Shaders/RRR1Visualize.xsf"V, EShaderStage::Pixel);

	DEFINE_SIMPLE_DEFAULT_SHADER(FZndcVisualizePS, u8"Engine/Shaders/ZndcVisualize.xsf"V, EShaderStage::Pixel);

	DEFINE_SIMPLE_DEFAULT_SHADER(FHDR2SDRPS, u8"Engine/Shaders/HDR2SDR.xsf"V, EShaderStage::Pixel);


	class FGussainBlurPS : public IShaderSource
	{
	public:
		DEFINE_SHADER_INSTANCE(FGussainBlurPS)

		using BorderSample = TShaderPermutationBool<u8"XIN_BLUR_GUSSAIN_BORDER_SAMPLE">;

		using FShaderVariance = TShaderVariance<BorderSample>;

		FGussainBlurPS() : IShaderSource(EShaderStage::Pixel, u8"Engine/Shaders/GussainBlur.xsf"V)
		{
		}
	};


	class FGussainBlurCS : public IShaderSource
	{
	public:
		DEFINE_SHADER_INSTANCE(FGussainBlurCS)

		using FShaderVariance = FEmptyShaderVariance;

		FGussainBlurCS() : IShaderSource(EShaderStage::Compute, u8"Engine/Shaders/GussainBlurCS.xsf"V)
		{
		}
	};


	class FClearUIntCS : public IShaderSource
	{
	public:
		DEFINE_SHADER_INSTANCE(FClearUIntCS)

		using FShaderVariance = FEmptyShaderVariance;

		FClearUIntCS() : IShaderSource(EShaderStage::Compute, u8"Engine/Shaders/ClearUIntCS.xsf"V)
		{
		}
	};

	enum class EBlitTextureFlag
	{
		None = 0,
		Fill = 0x0000,
		// default fill output, will not keep aspect ratio
		Center = 0x0002,
		PassName = 0x10000,
		Clear = 0x20000,
	};

	enum class EBlendMode
	{
		None = 0,
		Transparent,
		Premultiplied,
	};

	struct FRGBlitParameters
	{
		FRGTexture * Output = nullptr;
		FRGResourceView * OutputRTV = nullptr;
		FRGTexture * Input = nullptr;
		FRGResourceView * InputSRV = nullptr;
		IShaderInstance * Shader = nullptr;
		IRHIResource * ShaderParameter = nullptr;
		EBlendMode BlendMode = EBlendMode::None;
		EBlitTextureFlag Flags = EBlitTextureFlag::None;
		RectI ViewRect = {};
	};

	class ENGINE_API FRGAllocator
	{
	public:
		void BeginFrame()
		{}

		void EndFrame()
		{
			for (void * Pointer : Blocks)
				Free(Pointer);
		}

		template<typename T>
		T * Alloc()
		{
			T * Pointer = (T *)Malloc(sizeof(T), alignof(T));
			Blocks.Add(Pointer);
			return Pointer;
		}

		template<typename T>
		T & AllocRef()
		{
			return *Alloc<T>();
		}

	private:
		TList<void *> Blocks;
	};

	class ENGINE_API FRenderGraph
	{
	public:
		class FScopedEvent
		{
		public:
			FScopedEvent(FRenderGraph & RenderGraph, FStringV EventName, FColor Color = Colors::None);
			~FScopedEvent();

		public:
			FRenderGraph & RenderGraph;
			bool Triggered = false;
		};

	public:
		FRenderGraph(FCommandExecutor & Executor, FScene & Scene, FCommandList & CommandList);
		FRenderGraph(const FRenderGraph &) = delete;

	public:
		void Setup();
		void Execute();

	private:
		void BeginPass(FRenderPass & RenderPass);
		void EndPass(FRenderPass & RenderPass);

	public:
		bool ImmediatelyPass = false;

		template<RenderPassCompileLambdaT CompileLambdaT, RenderPassLambdaT ExecuteLambdaT>
		FRenderPass & AddPass(FStringV PassName, CompileLambdaT && CompileLambda, ExecuteLambdaT && ExecuteLambda)
		{
			FRenderPassRef RenderPass = new TRenderPass<CompileLambdaT, ExecuteLambdaT>(PassName, Forward<CompileLambdaT>(CompileLambda), Forward<ExecuteLambdaT>(ExecuteLambda));
			RenderPasses.Add(RenderPass);

			if (ImmediatelyPass)
			{
				FCompileContext CompileContext(*RenderPass);
				RenderPass->Compile(CompileContext);

				FRGContext RGContext(*this, CommandList);

				BeginPass(RenderPass.Ref());
				RenderPass->Excute(RGContext);
				EndPass(RenderPass.Ref());
			}
			return *RenderPass;
		}

		template<RenderPassLambdaT ExecuteLambdaT>
		FRenderPass & AddPass(FStringV PassName, ExecuteLambdaT && ExecuteLambda)
		{
			return AddPass(PassName, [](FCompileContext & CompileContext) {}, Forward<ExecuteLambdaT>(ExecuteLambda));
		}

	public:
		FRGResource * FindResource(FStringV Semantic);

		FRGTexture * CreateResource(FStringV Semantic, const FRGResourceDesc & TextureDesc);

		FRGTexture * RegisterTexture(FStringV Semantic, IRHIResource * Texture, EResourceAccess InputAccess = EResourceAccess::None, EResourceAccess OutputAccess = EResourceAccess::None);
		FRGBuffer * RegisterBuffer(FStringV Semantic, IRHIResource * Buffer, EResourceAccess InputAccess = EResourceAccess::None);

		FRGResourceView * CreateResourceView(FRGResource * RGResource, const FResourceViewDesc & ResourceViewDesc);
		FRGResourceView * CreateResourceView(FRGResource * RGResource, uint32 SubresouceIndex = NullIndex);

	public: // BlitTexture
		FRGTexture * BlitTexture(FRGTexture * Output, FRGTexture * Input, FRGResourceView * InputSRV, IShaderInstance * Shader, TView<byte> Parameter, EBlitTextureFlag Flags = EBlitTextureFlag::Fill)
		{
			FRGBlitParameters BlitParameters { };
			BlitParameters.Output = Output;
			BlitParameters.Input = Input;
			BlitParameters.InputSRV = InputSRV;
			BlitParameters.Shader = Shader;
			BlitParameters.ShaderParameter = Parameter.Size ? CreateShaderParameter(Parameter) : nullptr;
			BlitParameters.Flags = Flags;
			return BlitTexture(BlitParameters);
		}

		FRGTexture * BlitTexture(FRGTexture * Output, FRGTexture * Input, FRGResourceView * InputSRV, IShaderInstance * Shader, IRHIResource * ShaderParameter = nullptr, EBlitTextureFlag Flags = EBlitTextureFlag::Fill)
		{
			FRGBlitParameters BlitParameters { };
			BlitParameters.Output = Output;
			BlitParameters.Input = Input;
			BlitParameters.InputSRV = InputSRV;
			BlitParameters.Shader = Shader;
			BlitParameters.ShaderParameter = ShaderParameter;
			BlitParameters.Flags = Flags;
			return BlitTexture(BlitParameters);
		}

		FRGTexture * BlitTexture(FRGTexture * Output, FRGTexture * Input, IShaderInstance * Shader, IRHIResource * ShaderParameter = nullptr, EBlitTextureFlag Flags = EBlitTextureFlag::Fill)
		{
			FRGBlitParameters BlitParameters { };
			BlitParameters.Output = Output;
			BlitParameters.Input = Input;
			BlitParameters.Shader = Shader;
			BlitParameters.ShaderParameter = ShaderParameter;
			BlitParameters.Flags = Flags;
			return BlitTexture(BlitParameters);
		}

		FRGTexture * BlitTexture(const FRGBlitParameters & Parameters);

		template<typename T>
		FRGTexture * BlitTextureT(FRGTexture * Output, FRGTexture * Input, IShaderInstance * Shader, const T & Parameter, EBlitTextureFlag Flags = EBlitTextureFlag::Fill)
		{
			return BlitTexture(Output, Input, nullptr, Shader, ViewBytes<T>(Parameter), Flags);
		}

		template<typename T>
		FRGTexture * BlitTextureT(FRGTexture * Output, FRGTexture * Input, IRHIResourceView * InputSRV, IShaderInstance * Shader, const T & Parameter, EBlitTextureFlag Flags = EBlitTextureFlag::Fill)
		{
			return BlitTexture(Output, Input, InputSRV, Shader, ViewBytes<T>(Parameter), Flags);
		}

	public:
		void BlurCS(FRGTexture * Output, FRGTexture * Input, FStringV PassName = {});
		void ClearUIntCS(FRGTexture * Source, uint32 ClearValue, FStringV PassName = {});

		IRHIResource * CreateShaderParameter(uint32 Size, TView<byte> InitialBytes = { });
		IRHIResource * CreateShaderParameter(Size2U Size, EFormat Format, TView<byte> InitialBytes = { });
		IRHIResource * CreateShaderParameter(TView<byte> InitialBytes) { return CreateShaderParameter(uint32(InitialBytes.Size), InitialBytes); }
		template<typename T>
		IRHIResource * CreateShaderParameterT(const T & Value) { return CreateShaderParameter(AlignWith<uint32>(sizeof(T), sizeof(float4)), ViewBytes<T>(Value)); }

		TList<IRHIResourceRef> FrameResources;

	public:
		FCommandExecutor & Executor;
		FCommandList & CommandList;

		FScene & Scene;

		FRGAllocator Allocator;

		FTransientAllocatorRef TransientAllocator;

		TList<FRenderPassRef> RenderPasses;

		TMap<FString, FRGTextureRef> SemanticResources;

		TList<IRHIResourceRef> FramePooledTextures;
		TList<IRHIResourceRef> FrameRegisteredResources;
		TList<IRHIResourceViewRef> FrameResourceViews;
		TList<FRGResourceViewRef> GraphRGResourceViews;

		struct FGraphAccess
		{
			FRGResource * RGResource;
			EResourceAccess ResourceAccess;
		};

		TList<FGraphAccess> GraphTrackedResources;
	};
}
