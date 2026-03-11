#include "PCH.h"
#include "Bloom.h"

#include "../RenderGraph.h"

namespace Xin::Engine
{
	DEFINE_SIMPLE_DEFAULT_SHADER(FBloomPreparePS, u8"Engine/Shaders/Bloom_Prepare.xsf"V, EShaderStage::Pixel);

	class FBloomDownsamplePS : public IShaderSource
	{
	public:
		DEFINE_SHADER_INSTANCE(FBloomDownsamplePS)

		using Downsample_x4x16 = TShaderPermutationBool<u8"XIN_BLOOM_DOWNSAMPLE_X4X16">;

		using FShaderVariance = TShaderVariance<Downsample_x4x16>;

		FBloomDownsamplePS() : IShaderSource(EShaderStage::Pixel, u8"Engine/Shaders/Bloom_Downsample.xsf"V)
		{
		}
	};

	DEFINE_SIMPLE_DEFAULT_SHADER(FBloomBlendPS, u8"Engine/Shaders/Bloom_Blend.xsf"V, EShaderStage::Pixel);


	DEFINE_SIMPLE_DEFAULT_SHADER(FBloomPrepareCS, u8"Engine/Shaders/BloomCS_Prepare.xsf"V, EShaderStage::Compute);

	class FBloomDownsampleCS : public IShaderSource
	{
	public:
		DEFINE_SHADER_INSTANCE(FBloomDownsampleCS)

		using Downsample_x2x4x8x16 = TShaderPermutationBool<u8"BLOOM_DOWNSAMPLE_2x4x8x16x">;

		using FShaderVariance = TShaderVariance<Downsample_x2x4x8x16>;

		FBloomDownsampleCS() : IShaderSource(EShaderStage::Compute, u8"Engine/Shaders/BloomCS_Downsample.xsf"V)
		{
		}
	};

	DEFINE_SIMPLE_DEFAULT_SHADER(FBloomBlendCS, u8"Engine/Shaders/BloomCS_Blend.xsf"V, EShaderStage::Compute);

	FBloom::FBloom(FRenderGraph & RenderGraph)
		: RenderGraph(RenderGraph)
	{
		SizeU ViewSize { RenderGraph.Scene.SceneView.ViewRect.Size };

		// Divisible by 128 so that after dividing by 16, we still have multiples of 8x8 tiles.  The bloom
		// dimensions must be at least 1/4 native resolution to avoid undersampling.
		BloomSize = { ViewSize.Width > 2560u ? 1280u : 640u, ViewSize.Height > 1440u ? 768u : 384u };

		BloomLuminance = RenderGraph.CreateResource(u8"Bloom.Luminance"V, { EFormat::R8UI, BloomSize });
		Bloom1xA = RenderGraph.CreateResource(u8"Bloom.1xA"V, { RenderGraph.Scene.RenderSettings.HDRColorFormat, BloomSize });
		Bloom1xB = RenderGraph.CreateResource(u8"Bloom.1xB"V, { RenderGraph.Scene.RenderSettings.HDRColorFormat, BloomSize });
		Bloom2xA = RenderGraph.CreateResource(u8"Bloom.2xA"V, { RenderGraph.Scene.RenderSettings.HDRColorFormat, BloomSize >> 1 });
		Bloom2xB = RenderGraph.CreateResource(u8"Bloom.2xB"V, { RenderGraph.Scene.RenderSettings.HDRColorFormat, BloomSize >> 1 });
		Bloom4xA = RenderGraph.CreateResource(u8"Bloom.4xA"V, { RenderGraph.Scene.RenderSettings.HDRColorFormat, BloomSize >> 2 });
		Bloom4xB = RenderGraph.CreateResource(u8"Bloom.4xB"V, { RenderGraph.Scene.RenderSettings.HDRColorFormat, BloomSize >> 2 });
		Bloom8xA = RenderGraph.CreateResource(u8"Bloom.8xA"V, { RenderGraph.Scene.RenderSettings.HDRColorFormat, BloomSize >> 3 });
		Bloom8xB = RenderGraph.CreateResource(u8"Bloom.8xB"V, { RenderGraph.Scene.RenderSettings.HDRColorFormat, BloomSize >> 3 });
		Bloom16xA = RenderGraph.CreateResource(u8"Bloom.16xA"V, { RenderGraph.Scene.RenderSettings.HDRColorFormat, BloomSize >> 4 });
		Bloom16xB = RenderGraph.CreateResource(u8"Bloom.16xB"V, { RenderGraph.Scene.RenderSettings.HDRColorFormat, BloomSize >> 4 });
	}

	FBloom::~FBloom()
	{
		
	}

	void FBloom::Render(FRGTexture * SceneColor, FRGBuffer * Exposure)
	{
		switch(RenderGraph.Scene.RenderSettings.BloomMode)
		{
		case EBloomMode::PS:
			RenderPS(SceneColor, Exposure);
			break;
		case EBloomMode::CS:
			RenderCS(SceneColor, Exposure);
			break;
		default: 
			break;
		}
	}

	void FBloom::RenderPS(FRGTexture * SceneColor, FRGBuffer * Exposure)
	{
		FRenderGraph::FScopedEvent SSAORenderEvent(RenderGraph, u8"BloomPS"V);

		FGussainBlurPS::FShaderVariance ShaderVariance;
		ShaderVariance.SetValue<FGussainBlurPS::BorderSample>(true);

		PreparePS(SceneColor, Exposure);
		if (RenderGraph.Scene.RenderSettings.BloomLevel == EBloomLevel::Level0)
		{
			DownsamplePS_4x16x();
			{
				FRenderGraph::FScopedEvent SSAORenderEvent_Blur_16xA_16xB_16xA(RenderGraph, u8"Bloom.BlurPS Bloom16xA->Bloom16xB->Bloom16xA"V);
				RenderGraph.BlitTextureT(Bloom16xB, Bloom16xA, RenderGraph.Executor.Engine.ShaderBundle.GetShaderT<FGussainBlurPS>(ShaderVariance), float2(1.0f / Bloom16xA->Desc.Size.X, 0));
				RenderGraph.BlitTextureT(Bloom16xA, Bloom16xB, RenderGraph.Executor.Engine.ShaderBundle.GetShaderT<FGussainBlurPS>(ShaderVariance), float2(0, 1.0f / Bloom16xB->Desc.Size.Y));
			}
			BlendBlurPS(Bloom4xB, Bloom4xA, Bloom16xA, RenderGraph.Scene.RenderSettings.BloomBlendFactor);
			BlendBlurPS(Bloom1xB, Bloom1xA, Bloom4xB, RenderGraph.Scene.RenderSettings.BloomBlendFactor);
		}
		else
		{
			DownsamplePS_2x4x8x16x();
			{
				FRenderGraph::FScopedEvent SSAORenderEvent_Blur_16xA_16xB_16xA(RenderGraph, u8"Bloom.BlurPS Bloom16xA->Bloom16xB->Bloom16xA"V);
				RenderGraph.BlitTextureT(Bloom16xB, Bloom16xA, RenderGraph.Executor.Engine.ShaderBundle.GetShaderT<FGussainBlurPS>(ShaderVariance), float2(1.0f / Bloom16xA->Desc.Size.X, 0));
				RenderGraph.BlitTextureT(Bloom16xA, Bloom16xB, RenderGraph.Executor.Engine.ShaderBundle.GetShaderT<FGussainBlurPS>(ShaderVariance), float2(0, 1.0f / Bloom16xB->Desc.Size.Y));
			}
			BlendBlurPS(Bloom8xB, Bloom8xA, Bloom16xA, RenderGraph.Scene.RenderSettings.BloomBlendFactor);
			BlendBlurPS(Bloom4xB, Bloom4xA, Bloom8xB, RenderGraph.Scene.RenderSettings.BloomBlendFactor);
			BlendBlurPS(Bloom2xB, Bloom2xA, Bloom4xB, RenderGraph.Scene.RenderSettings.BloomBlendFactor);
			BlendBlurPS(Bloom1xB, Bloom1xA, Bloom2xB, RenderGraph.Scene.RenderSettings.BloomBlendFactor);
		}
	}

	void FBloom::PreparePS(FRGTexture * SceneColor, FRGBuffer * Exposure)
	{
		FRenderGraph::FScopedEvent SSAORenderEvent(RenderGraph, u8"Bloom.PreparePS"V);
		struct FBloomPrepareInputs
		{
			FRGTexture * SceneColor;
			FRGBuffer * Exposure;
			FRGTexture * Bloom;
			FRGTexture * BloomLuminance;
		};
		FBloomPrepareInputs Inputs = { SceneColor, Exposure, Bloom1xA, BloomLuminance };
		RenderGraph.AddPass(FStringV::None,
			[Inputs = Inputs](FCompileContext & CompileContext)
			{
				CompileContext.AccessResource(Inputs.SceneColor, EResourceAccess::PixelSampledTexture);
				CompileContext.AccessResource(Inputs.Exposure, EResourceAccess::PixelSampledTexture);
				CompileContext.AccessResource(Inputs.Bloom, EResourceAccess::RenderTarget, EResourceLoad::Discard);
				CompileContext.AccessResource(Inputs.BloomLuminance, EResourceAccess::RenderTarget, EResourceLoad::Discard);
			},
			[this, BloomSize = BloomSize, Inputs = Inputs](FRGContext & RGContext)
			{
				FViewport Viewport { 0.0f, 0.0f, (float32)BloomSize.Width, (float32)BloomSize.Height, 0.0f, 1.0f };
				RGContext.BeginPass({ Inputs.Bloom, Inputs.BloomLuminance }, nullptr, Viewport);

				float BloomLuminanceThreshold = 1.0f;
				struct
				{
					float2 InputTexelSize;
					float BloomLuminanceThreshold;
				}BloomParamter;
				BloomParamter.InputTexelSize = 1.0f / BloomSize;
				BloomParamter.BloomLuminanceThreshold = BloomLuminanceThreshold;
				IRHIResource * ShaderParameter = RGContext.RenderGraph.CreateShaderParameterT(BloomParamter);

				FMeshRenderState MeshRenderState;
				{
					MeshRenderState.Shaders[EShaderStage::Vertex] = RGContext.RenderGraph.Executor.Engine.ShaderBundle.GetShaderT<FScreenQuadVS>();
					MeshRenderState.Shaders[EShaderStage::Pixel] = RGContext.RenderGraph.Executor.Engine.ShaderBundle.GetShaderT<FBloomPreparePS>();
					MeshRenderState.InputElements = {};
					MeshRenderState.RasterizeState = FRasterizeState::Default;
					MeshRenderState.BlendState = FBlendState::Default;
					MeshRenderState.DepthStencilState = FDepthStencilState::Disabled;
				}

				FDrawCommand MeshCommand;

				MeshCommand.MeshRenderStateIndex = RGContext.RenderGraph.CommandList.Executor.PipelineStateCache.IndexMeshRenderState(MeshRenderState);
				MeshCommand.VertexCount = 3;
				MeshCommand.Instanced.VertexOffset = 0;
				MeshCommand.Instanced.VertexCount = 3;

				MeshCommand.Parameters.Emplace(u8"ShaderParameter"N, ShaderParameter);

				MeshCommand.Parameters.Emplace(u8"SceneColor"N, Inputs.SceneColor->Resource);
				MeshCommand.Parameters.Emplace(u8"Exposure"N, Inputs.Exposure->Resource);

				RGContext.DispatchCommand(MeshCommand);
				RGContext.EndPass();
			});
	}

	void FBloom::DownsamplePS_4x16x()
	{
		FBloomDownsamplePS::FShaderVariance ShaderVariance;
		ShaderVariance.SetValue<FBloomDownsamplePS::Downsample_x4x16>(true);
		FRenderGraph::FScopedEvent SSAORenderEvent(RenderGraph, u8"Bloom.DownsamplePS_4x16x"V);
		RenderGraph.BlitTextureT(Bloom4xA, Bloom1xA, RenderGraph.Executor.Engine.ShaderBundle.GetShaderT<FBloomDownsamplePS>(ShaderVariance), 1.0f / Bloom1xA->Desc.Size);
		RenderGraph.BlitTextureT(Bloom16xA, Bloom4xA, RenderGraph.Executor.Engine.ShaderBundle.GetShaderT <FBloomDownsamplePS>(ShaderVariance), 1.0f / Bloom4xA->Desc.Size);
	}

	void FBloom::DownsamplePS_2x4x8x16x()
	{
		FBloomDownsamplePS::FShaderVariance ShaderVariance;
		ShaderVariance.SetValue<FBloomDownsamplePS::Downsample_x4x16>(false);

		FRenderGraph::FScopedEvent SSAORenderEvent(RenderGraph, u8"Bloom.DownsamplePS_2x4x8x16x"V);
		RenderGraph.BlitTextureT(Bloom2xA, Bloom1xA, RenderGraph.Executor.Engine.ShaderBundle.GetShaderT<FBloomDownsamplePS>(ShaderVariance), float4 { 1.0f / Bloom1xA->Desc.Size, RenderGraph.Scene.RenderSettings.BloomThreshold, 0.5f });
		RenderGraph.BlitTextureT(Bloom4xA, Bloom2xA, RenderGraph.Executor.Engine.ShaderBundle.GetShaderT<FBloomDownsamplePS>(ShaderVariance), float4 { 1.0f / Bloom2xA->Desc.Size, RenderGraph.Scene.RenderSettings.BloomThreshold, 0.5f });
		RenderGraph.BlitTextureT(Bloom8xA, Bloom4xA, RenderGraph.Executor.Engine.ShaderBundle.GetShaderT<FBloomDownsamplePS>(ShaderVariance), float4 { 1.0f / Bloom4xA->Desc.Size, RenderGraph.Scene.RenderSettings.BloomThreshold, 0.5f });
		RenderGraph.BlitTextureT(Bloom16xA, Bloom8xA, RenderGraph.Executor.Engine.ShaderBundle.GetShaderT<FBloomDownsamplePS>(ShaderVariance), float4 { 1.0f / Bloom8xA->Desc.Size, RenderGraph.Scene.RenderSettings.BloomThreshold, 0.5f });
	}

	void FBloom::BlendBlurPS(FRGTexture * Output, FRGTexture * Input, FRGTexture * InputBlend, float32 BlendFactor)
	{
		FRenderGraph::FScopedEvent SSAORenderEvent_Blur_16x(RenderGraph, Format(u8"Bloom.BlendBlurPS {} -> {}"V, Input->Semantic, Output->Semantic));

		struct FBloomPrepareInputs
		{
			FRGTexture * Output;
			FRGTexture * Bloom;
			FRGTexture * BloomBlend;
			float32 BlendFactor;
		};
		FBloomPrepareInputs Inputs = { Output, Input, InputBlend, BlendFactor };
		RenderGraph.AddPass(u8"Bloom.BlendBlurPS"V,
			[Inputs = Inputs](FCompileContext & CompileContext)
			{
				CompileContext.AccessResource(Inputs.Output, EResourceAccess::RenderTarget, EResourceLoad::Discard);
				CompileContext.AccessResource(Inputs.Bloom, EResourceAccess::PixelSampledTexture);
				CompileContext.AccessResource(Inputs.BloomBlend, EResourceAccess::PixelSampledTexture);
			},
			[this, Inputs = Inputs](FRGContext & RGContext)
			{
				SizeU InputSize = Inputs.Bloom->Desc.Size;
				FViewport Viewport { 0.0f, 0.0f, (float32)InputSize.Width, (float32)InputSize.Height, 0.0f, 1.0f };
				RGContext.BeginPass({ Inputs.Output }, nullptr, Viewport);

				IRHIResource * ShaderParameter = RGContext.RenderGraph.CreateShaderParameterT(float4 { 1.0f / InputSize , Inputs.BlendFactor });
				
				FMeshRenderState MeshRenderState;
				{
					MeshRenderState.Shaders[EShaderStage::Vertex] = RGContext.RenderGraph.Executor.Engine.ShaderBundle.GetShaderT<FScreenQuadVS>();
					MeshRenderState.Shaders[EShaderStage::Pixel] = RGContext.RenderGraph.Executor.Engine.ShaderBundle.GetShaderT<FBloomBlendPS>();
					MeshRenderState.InputElements = {};
					MeshRenderState.RasterizeState = FRasterizeState::Default;
					MeshRenderState.BlendState = FBlendState::Default;
					MeshRenderState.DepthStencilState = FDepthStencilState::Disabled;
				}
				FDrawCommand MeshCommand;

				MeshCommand.MeshRenderStateIndex = RenderGraph.CommandList.Executor.PipelineStateCache.IndexMeshRenderState(MeshRenderState);
				MeshCommand.VertexCount = 3;
				MeshCommand.Instanced.VertexOffset = 0;
				MeshCommand.Instanced.VertexCount = 3;
				MeshCommand.Parameters.Emplace(u8"ShaderParameter"N, ShaderParameter);
				MeshCommand.Parameters.Emplace(u8"Bloom"N, Inputs.Bloom->Resource);
				MeshCommand.Parameters.Emplace(u8"BloomBlend"N, Inputs.BloomBlend->Resource);
				RGContext.DispatchCommand(MeshCommand);
				RGContext.EndPass();
			});

		FGussainBlurPS::FShaderVariance ShaderVariance;
		ShaderVariance.SetValue<FGussainBlurPS::BorderSample>(true);
		RenderGraph.BlitTextureT(Input, Output, RenderGraph.Executor.Engine.ShaderBundle.GetShaderT<FGussainBlurPS>(ShaderVariance), float2(1.0f / Output->Desc.Size.X, 0), EBlitTextureFlag::PassName);
		RenderGraph.BlitTextureT(Input, Output, RenderGraph.Executor.Engine.ShaderBundle.GetShaderT<FGussainBlurPS>(ShaderVariance), float2(0, 1.0f / Output->Desc.Size.Y), EBlitTextureFlag::PassName);
	}

	void FBloom::RenderCS(FRGTexture * SceneColor, FRGBuffer * Exposure)
	{
		FRenderGraph::FScopedEvent SSAORenderEvent(RenderGraph, u8"BloomCS"V);

		PrepareCS(SceneColor, Exposure);
		if (RenderGraph.Scene.RenderSettings.BloomLevel == EBloomLevel::Level0)
		{
			DownsampleCS_4x16x();
			RenderGraph.BlurCS(Bloom16xB, Bloom16xA, Format(u8"Bloom.Blur {} -> {}"V, Bloom16xA->Semantic, Bloom16xB->Semantic));
			BlendBlurCS(Bloom4xB, Bloom4xA, Bloom16xB, RenderGraph.Scene.RenderSettings.BloomBlendFactor);
			BlendBlurCS(Bloom1xB, Bloom1xA, Bloom4xB, RenderGraph.Scene.RenderSettings.BloomBlendFactor);
		}
		else
		{
			DownsampleCS_2x4x8x16x();
			RenderGraph.BlurCS(Bloom16xB, Bloom16xA, Format(u8"Bloom.Blur {} -> {}"V, Bloom16xA->Semantic, Bloom16xB->Semantic));
			BlendBlurCS(Bloom8xB, Bloom8xA, Bloom16xB, RenderGraph.Scene.RenderSettings.BloomBlendFactor);
			BlendBlurCS(Bloom4xB, Bloom4xA, Bloom8xB, RenderGraph.Scene.RenderSettings.BloomBlendFactor);
			BlendBlurCS(Bloom2xB, Bloom2xA, Bloom4xB, RenderGraph.Scene.RenderSettings.BloomBlendFactor);
			BlendBlurCS(Bloom1xB, Bloom1xA, Bloom2xB, RenderGraph.Scene.RenderSettings.BloomBlendFactor);
		}
	}

	void FBloom::PrepareCS(FRGTexture * SceneColor, FRGBuffer * Exposure)
	{
		struct FBloomPrepareInputs
		{
			FRGTexture * SceneColor;
			FRGBuffer * Exposure;
			FRGTexture * Bloom;
			FRGTexture * BloomLuminance;
		};
		FBloomPrepareInputs Inputs = { SceneColor, Exposure, Bloom1xA, BloomLuminance };
		RenderGraph.AddPass(u8"Bloom.PrepareCS"V,
			[Inputs = Inputs](FCompileContext & CompileContext)
			{
				CompileContext.AccessResource(Inputs.SceneColor, EResourceAccess::ComputeSampledTexture);
				CompileContext.AccessResource(Inputs.Exposure, EResourceAccess::ComputeSampledTexture);
				CompileContext.AccessResource(Inputs.Bloom, EResourceAccess::ComputeStorageTexture);
				CompileContext.AccessResource(Inputs.BloomLuminance, EResourceAccess::ComputeStorageTexture);
			},
			[BloomSize = BloomSize, Inputs = Inputs](FRGContext & RGContext)
			{
				float BloomLuminanceThreshold = 1.0f;
				struct
				{
					float2 InputTexelSize;
					float BloomLuminanceThreshold;
				}BloomParamter;
				BloomParamter.InputTexelSize = 1.0f / BloomSize;
				BloomParamter.BloomLuminanceThreshold = BloomLuminanceThreshold;
				IRHIResource * ShaderParameter = RGContext.RenderGraph.CreateShaderParameterT(BloomParamter);

				FMeshRenderState MeshRenderState;
				{
					MeshRenderState.Shaders[EShaderStage::Compute] = RGContext.RenderGraph.Executor.Engine.ShaderBundle.GetShaderT<FBloomPrepareCS>();
				}
				FDrawCommand DispatchCommand;

				DispatchCommand.MeshRenderStateIndex = RGContext.RenderGraph.CommandList.Executor.PipelineStateCache.IndexMeshRenderState(MeshRenderState);

				DispatchCommand.Dispatch = { DivideCeil((uint32)BloomSize.X, 8u), DivideCeil((uint32)BloomSize.Y, 8u), 1u };

				DispatchCommand.Parameters.Emplace(u8"ShaderParameter"N, ShaderParameter);

				DispatchCommand.Parameters.Emplace(u8"SceneColor"N, Inputs.SceneColor->Resource);
				DispatchCommand.Parameters.Emplace(u8"Exposure"N, Inputs.Exposure->Resource);

				DispatchCommand.Parameters.Emplace(u8"Bloom"N, Inputs.Bloom->Resource);
				DispatchCommand.Parameters.Emplace(u8"BloomLuminance"N, Inputs.BloomLuminance->Resource);

				RGContext.DispatchCommand(DispatchCommand);
			});
	}

	void FBloom::DownsampleCS_4x16x()
	{
		struct FBloomDownsampleInputs
		{
			FRGTexture * InputBloom;
			FRGTexture * OutputBloom4x;
			FRGTexture * OutputBloom16x;
		};
		FBloomDownsampleInputs Inputs = { Bloom1xA, Bloom4xA, Bloom16xA };
		RenderGraph.AddPass(u8"Bloom.DownsampleCS_4x16x"V,
			[Inputs = Inputs](FCompileContext & CompileContext)
			{
				CompileContext.AccessResource(Inputs.InputBloom, EResourceAccess::ComputeSampledTexture);
				CompileContext.AccessResource(Inputs.OutputBloom4x, EResourceAccess::ComputeStorageTexture);
				CompileContext.AccessResource(Inputs.OutputBloom16x, EResourceAccess::ComputeStorageTexture);
			},
			[this, Inputs = Inputs](FRGContext & RGContext)
			{
				float BloomThreshold = 1.0f;
				struct
				{
					float2 InputTexelSize;
					float BloomThreshold;
				}BloomParamter;
				BloomParamter.InputTexelSize = 1.0f / Inputs.InputBloom->Desc.Size;
				BloomParamter.BloomThreshold = BloomThreshold;
				IRHIResource * ShaderParameter = RGContext.RenderGraph.CreateShaderParameterT(BloomParamter);

				FMeshRenderState MeshRenderState;
				{
					FBloomDownsampleCS::FShaderVariance ShaderVariance;
					ShaderVariance.SetValue<FBloomDownsampleCS::Downsample_x2x4x8x16>(false);

					MeshRenderState.Shaders[EShaderStage::Compute] = RGContext.RenderGraph.Executor.Engine.ShaderBundle.GetShaderT<FBloomDownsampleCS>(ShaderVariance);
				}
				FDrawCommand DispatchCommand;

				DispatchCommand.MeshRenderStateIndex = RGContext.RenderGraph.CommandList.Executor.PipelineStateCache.IndexMeshRenderState(MeshRenderState);

				DispatchCommand.Dispatch = { DivideCeil(BloomSize.X >> 1, 8u), DivideCeil(BloomSize.Y >> 1, 8u), 1u };

				DispatchCommand.Parameters.Emplace(u8"ShaderParameter"N, ShaderParameter);

				DispatchCommand.Parameters.Emplace(u8"InputBloom"N, Inputs.InputBloom->Resource);

				DispatchCommand.Parameters.Emplace(u8"OutputBloom4x"N, Inputs.OutputBloom4x->Resource);
				DispatchCommand.Parameters.Emplace(u8"OutputBloom16x"N, Inputs.OutputBloom16x->Resource);

				RGContext.DispatchCommand(DispatchCommand);
			});
	}
	
	void FBloom::DownsampleCS_2x4x8x16x()
	{
		struct FBloomDownsampleInputs
		{
			FRGTexture * OutputBloom2x;
			FRGTexture * OutputBloom4x;
			FRGTexture * OutputBloom8x;
			FRGTexture * OutputBloom16x;
			FRGTexture * InputBloom;
		};
		FBloomDownsampleInputs Inputs = { Bloom2xA, Bloom4xA, Bloom8xA, Bloom16xA, Bloom1xA };
		RenderGraph.AddPass(u8"Bloom.DownsampleCS_2x4x8x16x"V,
			[Inputs = Inputs](FCompileContext & CompileContext)
			{
				CompileContext.AccessResource(Inputs.InputBloom, EResourceAccess::ComputeSampledTexture);
				CompileContext.AccessResource(Inputs.OutputBloom2x, EResourceAccess::ComputeStorageTexture);
				CompileContext.AccessResource(Inputs.OutputBloom4x, EResourceAccess::ComputeStorageTexture);
				CompileContext.AccessResource(Inputs.OutputBloom8x, EResourceAccess::ComputeStorageTexture);
				CompileContext.AccessResource(Inputs.OutputBloom16x, EResourceAccess::ComputeStorageTexture);
			},
			[this, Inputs = Inputs](FRGContext & RGContext)
			{
				float BloomThreshold = 1.0f;
				struct
				{
					float2 InputTexelSize;
					float BloomThreshold;
				}BloomParamter;
				BloomParamter.InputTexelSize = 1.0f / Inputs.InputBloom->Desc.Size;
				BloomParamter.BloomThreshold = BloomThreshold;
				IRHIResource * ShaderParameter = RGContext.RenderGraph.CreateShaderParameterT(BloomParamter);

				FMeshRenderState MeshRenderState;
				{
					FBloomDownsampleCS::FShaderVariance ShaderVariance;
					ShaderVariance.SetValue<FBloomDownsampleCS::Downsample_x2x4x8x16>(true);

					MeshRenderState.Shaders[EShaderStage::Compute] = RGContext.RenderGraph.Executor.Engine.ShaderBundle.GetShaderT<FBloomDownsampleCS>(ShaderVariance);
				}
				FDrawCommand DispatchCommand;

				DispatchCommand.MeshRenderStateIndex = RGContext.RenderGraph.CommandList.Executor.PipelineStateCache.IndexMeshRenderState(MeshRenderState);

				DispatchCommand.Dispatch = { DivideCeil(BloomSize.X >> 1, 8u), DivideCeil(BloomSize.Y >> 1, 8u), 1u };

				DispatchCommand.Parameters.Emplace(u8"ShaderParameter"N, ShaderParameter);

				DispatchCommand.Parameters.Emplace(u8"InputBloom"N, Inputs.InputBloom->Resource);

				DispatchCommand.Parameters.Emplace(u8"OutputBloom2x"N, Inputs.OutputBloom2x->Resource);
				DispatchCommand.Parameters.Emplace(u8"OutputBloom4x"N, Inputs.OutputBloom4x->Resource);
				DispatchCommand.Parameters.Emplace(u8"OutputBloom8x"N, Inputs.OutputBloom8x->Resource);
				DispatchCommand.Parameters.Emplace(u8"OutputBloom16x"N, Inputs.OutputBloom16x->Resource);

				RGContext.DispatchCommand(DispatchCommand);
			});
	}

	void FBloom::BlendBlurCS(FRGTexture * Output, FRGTexture * Input, FRGTexture * InputBloomBlend, float32 BlendFactor)
	{
		struct FBloomBlurBlendInputs
		{
			FRGTexture * OutputBloom;
			FRGTexture * InputBloom;
			FRGTexture * InputBloomBlend;
		};
		FBloomBlurBlendInputs Inputs = { Output, Input, InputBloomBlend };

		RenderGraph.AddPass(InputBloomBlend ? Format(u8"Bloom.BlendBlurCS {} + {} -> {}"V, Input->Semantic, InputBloomBlend->Semantic, Output->Semantic):
				Format(u8"BlendBlurCS {} -> {}"V, Input->Semantic, Output->Semantic),
			[Inputs = Inputs](FCompileContext & CompileContext)
			{
				CompileContext.AccessResource(Inputs.InputBloom, EResourceAccess::ComputeSampledTexture);
				CompileContext.AccessResource(Inputs.InputBloomBlend, EResourceAccess::ComputeSampledTexture);
				CompileContext.AccessResource(Inputs.OutputBloom, EResourceAccess::ComputeStorageTexture, EResourceLoad::Discard);
			},
			[this, Inputs = Inputs, BlendFactor = BlendFactor](FRGContext & RGContext)
			{
				SizeU InputSize = Inputs.InputBloom->Desc.Size;
				FMeshRenderState MeshRenderState;
				{
					MeshRenderState.Shaders[EShaderStage::Compute] = RGContext.RenderGraph.Executor.Engine.ShaderBundle.GetShaderT<FBloomBlendCS>();
				}
				FDrawCommand DispatchCommand;

				DispatchCommand.MeshRenderStateIndex = RGContext.CommandList.Executor.PipelineStateCache.IndexMeshRenderState(MeshRenderState);
				DispatchCommand.Dispatch = { DivideCeil(InputSize.X, 8u), DivideCeil(InputSize.Y, 8u), 1u };

				IRHIResource * ShaderParameter = RGContext.RenderGraph.CreateShaderParameterT(float3(1.0f / InputSize, BlendFactor));
				DispatchCommand.Parameters.Emplace(u8"ShaderParameter"N, ShaderParameter);

				DispatchCommand.Parameters.Emplace(u8"OutputBloom"N, Inputs.OutputBloom->Resource);
				DispatchCommand.Parameters.Emplace(u8"InputBloom"N, Inputs.InputBloom->Resource);
				DispatchCommand.Parameters.Emplace(u8"InputBloomBlend"N, Inputs.InputBloomBlend->Resource);

				RGContext.DispatchCommand(DispatchCommand);
			});
	}
}
