#include "PCH.h"
#include "PostProcessRenderer.h"

#include "RenderGraph.h"
#include "DrawCommand/DrawCommand.h"

namespace Xin::Engine
{
	DEFINE_SIMPLE_DEFAULT_SHADER(FLuminancePS, u8"Engine/Shaders/Luminance.xsf"V, EShaderStage::Pixel);

	DEFINE_SIMPLE_DEFAULT_SHADER(FLuminanceCS, u8"Engine/Shaders/LuminanceCS.xsf"V, EShaderStage::Compute);

	DEFINE_SIMPLE_DEFAULT_SHADER(FHistogramPS, u8"Engine/Shaders/Histogram.xsf"V, EShaderStage::Pixel);

	DEFINE_SIMPLE_DEFAULT_SHADER(FHistogramCS, u8"Engine/Shaders/HistogramCS.xsf"V, EShaderStage::Compute);

	DEFINE_SIMPLE_DEFAULT_SHADER(FAdaptExposurePS, u8"Engine/Shaders/AdaptExposure.xsf"V, EShaderStage::Pixel);

	DEFINE_SIMPLE_DEFAULT_SHADER(FAdaptExposureCS, u8"Engine/Shaders/AdaptExposureCS.xsf"V, EShaderStage::Compute);

	class FToonMappingCS : public IShaderSource
	{
	public:
		DEFINE_SHADER_INSTANCE(FToonMappingCS)

		using Bloom = TShaderPermutationBool<u8"XIN_TONEMAPPING_BLOOM">;

		using FShaderVariance = TShaderVariance<Bloom>;

		FToonMappingCS() : IShaderSource(EShaderStage::Compute, u8"Engine/Shaders/ToneMappingCS.xsf"V)
		{
		}
	};

	FPostProcessRenderer::FPostProcessRenderer(FRenderGraph & RenderGraph)
		: RenderGraph(RenderGraph) , Bloom(RenderGraph)
	{
		SceneExposure = RenderGraph.FindResource(u8"Global.SceneExposure"V);
		AssertExpr(SceneExposure);
		SceneHistogram = RenderGraph.CreateResource(u8"PP.Histogram"V, { { 256, sizeof(uint1) }, EFormat::UInt1});

		SizeU ViewSize { RenderGraph.Scene.SceneView.ViewRect.Size };
		BloomSize = { ViewSize.Width > 2560u ? 1280u : 640u, ViewSize.Height > 1440u ? 768u : 384u };
		SceneLuminance = RenderGraph.CreateResource(u8"PP.SceneLuminance"V, { EFormat::R8UI, BloomSize });
	}

	FPostProcessRenderer::~FPostProcessRenderer()
	{
		
	}

	void FPostProcessRenderer::Render(FRGTexture * SceneColor)
	{
		FRenderGraph::FScopedEvent SSAORenderEvent(RenderGraph, u8"Post Process"V);

		if (RenderGraph.Scene.RenderSettings.BloomMode != EBloomMode::Disabled)
		{
			Bloom.Render(SceneColor, SceneExposure);
			GenerateHistogramCS(Bloom.BloomLuminance, SceneHistogram);
			AdaptExposureCS(SceneHistogram, Bloom.BloomLuminance);
		}
		else
		{
			GenerateLuminanceCS(SceneColor, SceneLuminance);
			GenerateHistogramCS(SceneLuminance, SceneHistogram);
			AdaptExposureCS(SceneHistogram, SceneLuminance);
		}
		ToneMappingCS(SceneColor);
	}

	void FPostProcessRenderer::GenerateLuminance(FRGTexture * Luminance, FRGTexture * Color)
	{
		struct FGenerateLuminanceCSInputs
		{
			FRGTexture * Luminance;
			FRGTexture * Input;
			FRGBuffer * Exposure;
		};
		FGenerateLuminanceCSInputs Inputs = { Luminance, Color, SceneExposure };
		RenderGraph.AddPass(u8"GenerateLuminance"V,
			[Inputs = Inputs](FCompileContext & CompileContext)
			{
				CompileContext.AccessResource(Inputs.Luminance, EResourceAccess::RenderTarget, EResourceLoad::Discard);
				CompileContext.AccessResource(Inputs.Input, EResourceAccess::PixelSampledTexture);
				CompileContext.AccessResource(Inputs.Exposure, EResourceAccess::PixelSampledTexture);
			},
			[Inputs = Inputs, BloomSize = BloomSize](FRGContext & RGContext)
			{
				FViewport Viewport { 0.0f, 0.0f, (float32)BloomSize.Width, (float32)BloomSize.Height, 0.0f, 1.0f };
				RGContext.BeginPass({ Inputs.Luminance }, nullptr, Viewport);

				IRHIResource * ShaderParameter = RGContext.RenderGraph.CreateShaderParameterT(float4 { 1.0f / Inputs.Luminance->Desc.Size });
			
				FMeshRenderState MeshRenderState;
				{
					MeshRenderState.Shaders[EShaderStage::Vertex] = RGContext.RenderGraph.Executor.Engine.ShaderBundle.GetShaderT<FScreenQuadVS>();
					MeshRenderState.Shaders[EShaderStage::Pixel] = RGContext.RenderGraph.Executor.Engine.ShaderBundle.GetShaderT<FLuminancePS>();
					MeshRenderState.InputElements = {};
					MeshRenderState.RasterizeState = FRasterizeState::Default;
					MeshRenderState.BlendState = FBlendState::Default;
					MeshRenderState.DepthStencilState = FDepthStencilState::Disabled;
				}

				FDrawCommand MeshCommand;

				MeshCommand.MeshRenderStateIndex = RGContext.RenderGraph.Executor.PipelineStateCache.IndexMeshRenderState(MeshRenderState);
				MeshCommand.VertexCount = 3;
				MeshCommand.Instanced.VertexOffset = 0;
				MeshCommand.Instanced.VertexCount = 3;

				MeshCommand.Parameters.Emplace(u8"ShaderParameter"N, ShaderParameter);

				MeshCommand.Parameters.Emplace(u8"Input"N, Inputs.Input->Resource);
				MeshCommand.Parameters.Emplace(u8"Exposure"N, Inputs.Exposure->Resource);

				RGContext.DispatchCommand(MeshCommand);
				RGContext.EndPass();
			});
	}

	void FPostProcessRenderer::GenerateHistogram(FRGBuffer * Histogram, FRGTexture * Luminance)
	{
		struct FGenerateHistogramCSInputs
		{
			FRGBuffer * Histogram;
			FRGTexture * Luminance;
		};
		FGenerateHistogramCSInputs Inputs = { Histogram, Luminance };
		RenderGraph.AddPass(u8"GenerateHistogram"V,
			[Inputs = Inputs](FCompileContext & CompileContext)
			{
				CompileContext.AccessResource(Inputs.Histogram, EResourceAccess::PixelUniformTexelBuffer);
				CompileContext.AccessResource(Inputs.Luminance, EResourceAccess::CopyWrite | EResourceAccess::PixelSampledTexture);
			},
			[Inputs = Inputs, BloomSize = BloomSize](FRGContext & RGContext)
			{
				RGContext.ClearUnorderedAccess(Inputs.Histogram->Resource, 0);

				FViewport Viewport { 0.0f, 0.0f, (float32)BloomSize.Width, (float32)BloomSize.Height, 0.0f, 1.0f };
				RGContext.BeginPass({}, nullptr, Viewport);

				FMeshRenderState MeshRenderState;
				{
					MeshRenderState.Shaders[EShaderStage::Vertex] = RGContext.RenderGraph.Executor.Engine.ShaderBundle.GetShaderT<FScreenQuadVS>();
					MeshRenderState.Shaders[EShaderStage::Pixel] = RGContext.RenderGraph.Executor.Engine.ShaderBundle.GetShaderT<FHistogramPS>();
					MeshRenderState.InputElements = {};
					MeshRenderState.RasterizeState = FRasterizeState::Default;
					MeshRenderState.BlendState = FBlendState::Default;
					MeshRenderState.DepthStencilState = FDepthStencilState::Disabled;
				}

				FDrawCommand MeshCommand;
				MeshCommand.MeshRenderStateIndex = RGContext.RenderGraph.Executor.PipelineStateCache.IndexMeshRenderState(MeshRenderState);
				MeshCommand.VertexCount = 3;
				MeshCommand.Instanced.VertexOffset = 0;
				MeshCommand.Instanced.VertexCount = 3;

				MeshCommand.Parameters.Emplace(u8"Histogram"N, Inputs.Histogram->Resource);
				MeshCommand.Parameters.Emplace(u8"Luminance"N, Inputs.Luminance->Resource);

				RGContext.DispatchCommand(MeshCommand);
				RGContext.EndPass();
			});
	}

	void FPostProcessRenderer::AdaptExposure(FRGBuffer * Histogram, FRGTexture * Luminance)
	{
		struct FShaderParameterStruct
		{
			float TargetLuminance;
			float AdaptationRate;
			float MinExposure;
			float MaxExposure;
			uint32 PixelCount;
		};

		uint32 PixelCount = Luminance->Desc.Size.X * Luminance->Desc.Size.Y;

		struct FGenerateHistogramCSInputs
		{
			FRGBuffer * Histogram;
			FRGBuffer * Exposure;
		};
		FGenerateHistogramCSInputs Inputs = { Histogram, SceneExposure };
		RenderGraph.AddPass(u8"AdaptExposure"V,
			[Inputs = Inputs](FCompileContext & CompileContext)
			{
				CompileContext.AccessResource(Inputs.Histogram, EResourceAccess::PixelSampledTexture);
				CompileContext.AccessResource(Inputs.Exposure, EResourceAccess::StorageTexelBuffer);
			},
			[Inputs = Inputs, PixelCount = PixelCount](FRGContext & RGContext)
			{
				FViewport Viewport { 0.0f, 0.0f, 2.0f, 2.0f, 0.0f, 1.0f };
				RGContext.BeginPass({}, nullptr, Viewport);

				FShaderParameterStruct ShaderParameterStruct;
				ShaderParameterStruct.TargetLuminance = 0.08f;
				ShaderParameterStruct.AdaptationRate = 0.05f;
				ShaderParameterStruct.MinExposure = 1.0f / 64.0f;
				ShaderParameterStruct.MaxExposure = 64.0f;
				ShaderParameterStruct.PixelCount = PixelCount;
				IRHIResource * ShaderParameter = RGContext.RenderGraph.CreateShaderParameterT(ShaderParameterStruct);

				FMeshRenderState MeshRenderState;
				{
					MeshRenderState.Shaders[EShaderStage::Vertex] = RGContext.RenderGraph.Executor.Engine.ShaderBundle.GetShaderT<FScreenQuadVS>();
					MeshRenderState.Shaders[EShaderStage::Pixel] = RGContext.RenderGraph.Executor.Engine.ShaderBundle.GetShaderT<FAdaptExposurePS>();
					MeshRenderState.InputElements = {};
					MeshRenderState.RasterizeState = FRasterizeState::Default;
					MeshRenderState.BlendState = FBlendState::Default;
					MeshRenderState.DepthStencilState = FDepthStencilState::Disabled;
				}

				FDrawCommand MeshCommand;
				MeshCommand.MeshRenderStateIndex = RGContext.RenderGraph.Executor.PipelineStateCache.IndexMeshRenderState(MeshRenderState);
				MeshCommand.VertexCount = 3;
				MeshCommand.Instanced.VertexOffset = 0;
				MeshCommand.Instanced.VertexCount = 3;

				MeshCommand.Parameters.Emplace(u8"ShaderParameter"N, ShaderParameter);
				MeshCommand.Parameters.Emplace(u8"Histogram"N, Inputs.Histogram->Resource);
				MeshCommand.Parameters.Emplace(u8"Exposure"N, Inputs.Exposure->Resource);

				RGContext.DispatchCommand(MeshCommand);
				RGContext.EndPass();
			});
	}

	void FPostProcessRenderer::GenerateLuminanceCS(FRGTexture * Color, FRGTexture * Luminance)
	{
		struct FGenerateLuminanceCSInputs
		{
			FRGTexture * Luminance;
			FRGTexture * Input;
			FRGBuffer * Exposure;
		};
		FGenerateLuminanceCSInputs Inputs = { Luminance, Color, SceneExposure };
		RenderGraph.AddPass(u8"GenerateLuminanceCS"V,
			[Inputs = Inputs](FCompileContext & CompileContext)
			{
				CompileContext.AccessResource(Inputs.Luminance, EResourceAccess::ComputeUniformTexelBuffer);
				CompileContext.AccessResource(Inputs.Input, EResourceAccess::ComputeSampledTexture);
				CompileContext.AccessResource(Inputs.Exposure, EResourceAccess::ComputeUniformTexelBuffer);
			},
			[Inputs = Inputs, BloomSize = BloomSize](FRGContext & RGContext)
			{
				IRHIResource * ShaderParameter = RGContext.RenderGraph.CreateShaderParameterT(float4 { 1.0f / Inputs.Luminance->Desc.Size });

				FMeshRenderState MeshRenderState;
				{
					MeshRenderState.Shaders[EShaderStage::Compute] = RGContext.RenderGraph.Executor.Engine.ShaderBundle.GetShaderT<FLuminanceCS>();
				}

				FDrawCommand DispatchCommand;
				DispatchCommand.MeshRenderStateIndex = RGContext.RenderGraph.Executor.PipelineStateCache.IndexMeshRenderState(MeshRenderState);
				DispatchCommand.Dispatch = { DivideCeil(BloomSize.X, 8u), DivideCeil(BloomSize.Y, 8u), 1 };

				DispatchCommand.Parameters.Emplace(u8"ShaderParameter"N, ShaderParameter);

				DispatchCommand.Parameters.Emplace(u8"Input"N, Inputs.Input->Resource);
				DispatchCommand.Parameters.Emplace(u8"Exposure"N, Inputs.Exposure->Resource);
				DispatchCommand.Parameters.Emplace(u8"Luminance"N, Inputs.Luminance->Resource);

				RGContext.DispatchCommand(DispatchCommand);
			});
	}

	void FPostProcessRenderer::GenerateHistogramCS(FRGTexture * Luminance, FRGBuffer * Histogram)
	{
		//RenderGraph.ClearUIntCS(Histogram, 0);

		struct FGenerateHistogramInputs
		{
			FRGTexture * Luminance;
			FRGBuffer * Histogram;
		};
		FGenerateHistogramInputs Inputs = { Luminance, Histogram };
		RenderGraph.AddPass(u8"GenerateHistogramCS"V,
			[Inputs = Inputs](FCompileContext & CompileContext)
			{
				CompileContext.AccessResource(Inputs.Luminance, EResourceAccess::ComputeSampledTexture);
				CompileContext.AccessResource(Inputs.Histogram, EResourceAccess::CopyWrite | EResourceAccess::StorageTexelBuffer);
			},
			[Inputs = Inputs](FRGContext & RGContext)
			{
				byte HistogramZeros[256 * sizeof(uint32)] = { };
				//RGContext.CommandList.UpdateResource(Inputs.Histogram->Resource, HistogramZeros);
				RGContext.ClearUnorderedAccess(Inputs.Histogram->Resource, 0);

				SizeU InputSize = Inputs.Luminance->Desc.Size;
				IRHIResource * ShaderParameter = RGContext.RenderGraph.CreateShaderParameterT(InputSize.Y);

				FMeshRenderState MeshRenderState;
				{
					MeshRenderState.Shaders[EShaderStage::Compute] = RGContext.RenderGraph.Executor.Engine.ShaderBundle.GetShaderT<FHistogramCS>();
				}

				FDrawCommand DispatchCommand;
				DispatchCommand.MeshRenderStateIndex = RGContext.RenderGraph.Executor.PipelineStateCache.IndexMeshRenderState(MeshRenderState);
				DispatchCommand.Dispatch = { DivideCeil(InputSize.X, 16u), 1u, 1u };

				DispatchCommand.Parameters.Emplace(u8"ShaderParameter"N, ShaderParameter);
				DispatchCommand.Parameters.Emplace(u8"Luminance"N, Inputs.Luminance->Resource);
				DispatchCommand.Parameters.Emplace(u8"Histogram"N, Inputs.Histogram->Resource);

				RGContext.DispatchCommand(DispatchCommand);
			});
	}

	void FPostProcessRenderer::AdaptExposureCS(FRGBuffer * Histogram, FRGTexture * Luminance)
	{
		uint32 PixelCount = Luminance->Desc.Size.X * Luminance->Desc.Size.Y;

		struct FAdaptExposureInputs
		{
			FRGBuffer * Histogram;
			FRGBuffer * Exposure;
		};
		FAdaptExposureInputs Inputs = { Histogram, SceneExposure };
		RenderGraph.AddPass(u8"AdaptExposureCS"V,
			[Inputs = Inputs](FCompileContext & CompileContext)
			{
				CompileContext.AccessResource(Inputs.Histogram, EResourceAccess::ComputeUniformTexelBuffer);
				CompileContext.AccessResource(Inputs.Exposure, EResourceAccess::ComputeStorageTexelBuffer);
			},
			[Inputs = Inputs, PixelCount = PixelCount](FRGContext & RGContext)
			{
			 //   NumVar TargetLuminance("Graphics/HDR/Key", 0.08f, 0.01f, 0.99f, 0.01f);
			 //   NumVar AdaptationRate("Graphics/HDR/Adaptation Rate", 0.05f, 0.01f, 1.0f, 0.01f);
				//ExpVar MinExposure("Graphics/HDR/Min Exposure", 1.0f / 64.0f, -8.0f, 0.0f, 0.25f);
				//ExpVar MaxExposure("Graphics/HDR/Max Exposure", 64.0f, 0.0f, 8.0f, 0.25f);
				if (false)
				{
					//ExpVar Exposure("Graphics/HDR/Exposure", 2.0f, -8.0f, 8.0f, 0.25f);
					float InitialExposure = 2.0f;

					float LuminanceMin = 0.0001f;
					float LuminanceMax = 16.0f;
					float LuminanceMinLog = log2(LuminanceMin);
					float LuminanceMaxLog = log2(LuminanceMax);

					float InitExposure[8] =
					{
						InitialExposure, 1.0f / InitialExposure, InitialExposure, 0.0f,
						LuminanceMinLog, LuminanceMaxLog, LuminanceMaxLog - LuminanceMinLog, 1.0f / (LuminanceMaxLog - LuminanceMinLog)
					};
					RGContext.CommandList.UpdateBuffer(Inputs.Exposure->Resource, ViewBytes(InitExposure));
				}
				struct FShaderParameterStruct
				{
					float TargetLuminance;
					float AdaptationRate;
					float MinExposure;
					float MaxExposure;
					uint32 PixelCount;
				};
				FShaderParameterStruct ShaderParameterStruct;
				ShaderParameterStruct.TargetLuminance = 0.08f;
				ShaderParameterStruct.AdaptationRate = 0.05f;
				ShaderParameterStruct.MinExposure = 1.0f / 64.0f;
				ShaderParameterStruct.MaxExposure = 64.0f;
				ShaderParameterStruct.PixelCount = PixelCount;
				IRHIResource * ShaderParameter = RGContext.RenderGraph.CreateShaderParameterT(ShaderParameterStruct);

				FMeshRenderState MeshRenderState;
				{
					MeshRenderState.Shaders[EShaderStage::Compute] = RGContext.RenderGraph.Executor.Engine.ShaderBundle.GetShaderT<FAdaptExposureCS>();
				}

				FDrawCommand DispatchCommand;
				DispatchCommand.MeshRenderStateIndex = RGContext.RenderGraph.Executor.PipelineStateCache.IndexMeshRenderState(MeshRenderState);
				DispatchCommand.Dispatch = { 1u, 1u, 1u };

				DispatchCommand.Parameters.Emplace(u8"ShaderParameter"N, ShaderParameter);
				DispatchCommand.Parameters.Emplace(u8"Histogram"N, Inputs.Histogram->Resource);
				DispatchCommand.Parameters.Emplace(u8"Exposure"N, Inputs.Exposure->Resource);

				RGContext.DispatchCommand(DispatchCommand);
			});
	}

	void FPostProcessRenderer::ToneMappingCS(FRGTexture * Color)
	{
		struct FToneMappingInputs
		{
			FRGBuffer * Exposure;
			FRGTexture * Bloom;
			FRGTexture * SceneColor;
		};
		FToneMappingInputs Inputs = { SceneExposure, Bloom.Bloom1xB, Color };
		RenderGraph.AddPass(u8"ToneMappingCS"V,
			[Inputs = Inputs](FCompileContext & CompileContext)
			{
				CompileContext.AccessResource(Inputs.Exposure, EResourceAccess::ComputeSampledTexture);
				CompileContext.AccessResource(Inputs.Bloom, EResourceAccess::ComputeSampledTexture);
				CompileContext.AccessResource(Inputs.SceneColor, EResourceAccess::ComputeSampledTexture | EResourceAccess::StorageTexture);
			},
			[Inputs = Inputs](FRGContext & RGContext)
			{
				//NumVar BloomStrength("Graphics/Bloom/Strength", 0.1f, 0.0f, 2.0f, 0.05f);		// A modulator controlling how much bloom is added back into the image
				float BloomStrength = 0.1f;
				SizeU InputSize = Inputs.SceneColor->Desc.Size;
				IRHIResource * ShaderParameter = RGContext.RenderGraph.CreateShaderParameterT(float3(1.0f / InputSize, BloomStrength));

				FMeshRenderState MeshRenderState;
				{
					FToonMappingCS::FShaderVariance ShaderVariance;
					ShaderVariance.SetValue<FToonMappingCS::Bloom>(RGContext.RenderGraph.Scene.RenderSettings.BloomMode != EBloomMode::Disabled);
					MeshRenderState.Shaders[EShaderStage::Compute] = RGContext.RenderGraph.Executor.Engine.ShaderBundle.GetShaderT<FToonMappingCS>();
				}

				FDrawCommand DispatchCommand;

				DispatchCommand.MeshRenderStateIndex = RGContext.RenderGraph.Executor.PipelineStateCache.IndexMeshRenderState(MeshRenderState);
				DispatchCommand.Dispatch = { DivideCeil(InputSize.X, 8u), DivideCeil(InputSize.Y, 8u), 1u };

				DispatchCommand.Parameters.Emplace(u8"ShaderParameter"N, ShaderParameter);

				DispatchCommand.Parameters.Emplace(u8"Exposure"N, Inputs.Exposure->Resource);
				DispatchCommand.Parameters.Emplace(u8"Bloom"N, Inputs.Bloom->Resource);

				DispatchCommand.Parameters.Emplace(u8"SceneColor"N, Inputs.SceneColor->Resource);

				RGContext.DispatchCommand(DispatchCommand);
			});
	}
}
