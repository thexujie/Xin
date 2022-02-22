#include "PCH.h"
#include "FXAA.h"

#include "../RenderGraph.h"

namespace Xin::Engine
{
	class FFXAAPS : public IShaderSource
	{
	public:
		DEFINE_SHADER_INSTANCE(FFXAAPS)

		using QualityLevel = TShaderPermutationIntRange<u8"XIN_FXAA_QUALITY_LEVEL", 0, 16>;
		using Console = TShaderPermutationBool<u8"XIN_FXAA_CONSOLE">;
		using Debug = TShaderPermutationBool<u8"XIN_FXAA_DEBUG">;

		using FShaderVariance = TShaderVariance<QualityLevel, Console, Debug>;

		FFXAAPS() : IShaderSource(EShaderStage::Pixel, u8"Engine/Shaders/FXAA.xsf"V)
		{
		}
	};

	class FFXAAPS_3_11 : public IShaderSource
	{
	public:
		DEFINE_SHADER_INSTANCE(FFXAAPS_3_11)

		using QualityLevel = TShaderPermutationIntRange<u8"XIN_FXAA_QUALITY_LEVEL", 0, 16>;
		using Console = TShaderPermutationBool<u8"XIN_FXAA_CONSOLE">;
		using Debug = TShaderPermutationBool<u8"XIN_FXAA_DEBUG">;

		using FShaderVariance = TShaderVariance<QualityLevel, Console, Debug>;

		FFXAAPS_3_11() : IShaderSource(EShaderStage::Pixel, u8"Engine/Shaders/FXAA.xsf"V, u8"FXAA_3_11_PixelShader")
		{
		}
	};

	class FFXAAPrepareCS : public IShaderSource
	{
	public:
		DEFINE_SHADER_INSTANCE(FFXAAPrepareCS)

		using DataQueuePacked = TShaderPermutationBool<u8"XIN_FXAA_DATAQUEUE_PACKED">;

		using FShaderVariance = TShaderVariance<DataQueuePacked>;

		FFXAAPrepareCS() : IShaderSource(EShaderStage::Compute, u8"Engine/Shaders/FXAACS_Prepare.xsf"V)
		{
		}
	};

	DEFINE_SIMPLE_DEFAULT_SHADER(FFXAAResolveCS, u8"Engine/Shaders/FXAACS_Resolve.xsf"V, EShaderStage::Compute);

	class FFXAAFinalCS : public IShaderSource
	{
	public:
		DEFINE_SHADER_INSTANCE(FFXAAFinalCS)

		using QualityLevel = TShaderPermutationIntRange<u8"XIN_FXAA_QUALITY_LEVEL", 0, 16>;
		using DataQueuePacked = TShaderPermutationBool<u8"XIN_FXAA_DATAQUEUE_PACKED">;
		using VerticalOrientation = TShaderPermutationBool<u8"XIN_FXAA_FINAL_VERTICAL_ORIENTATION">;

		using FShaderVariance = TShaderVariance<QualityLevel, DataQueuePacked, VerticalOrientation>;

		FFXAAFinalCS() : IShaderSource(EShaderStage::Compute, u8"Engine/Shaders/FXAACS_Final.xsf"V)
		{
		}
	};

	FFXAARenderer::FFXAARenderer(FRenderGraph & RenderGraph, EFXAAFlag Flags)
		: RenderGraph(RenderGraph), Flags(Flags)
	{
		BlockTile = { 1, 1 };
		//DataQueuePacked = 1;
	}

	FFXAARenderer::~FFXAARenderer()
	{
		
	}

	void FFXAARenderer::Render(FRGTexture * InputColor, FRGTexture * OutputColor)
	{
		SizeU InputSize = InputColor->Desc.Size;
		BlockSize = InputSize / BlockTile;
		uint32 NumBlocks = (BlockSize.X * BlockSize.Y) + 128;

		//TArray<uint1, 2> InitialWorkCounter { 0, 0 };
		//WorkCounter = RenderGraph.CreateBufferT(u8"WorkCounter"V, { { 4, sizeof(uint1) }, EFormat::None }, InitialWorkCounter);
		WorkCounter = RenderGraph.CreateResource(u8"FXAA.WorkCounter"V, { { 2, sizeof(uint1) }, EFormat::UInt1 });
		WorkQueue = RenderGraph.CreateResource(u8"FXAA.WorkQueue"V, { { NumBlocks, sizeof(uint1) }, EFormat::UInt1 });
		DataQueue = RenderGraph.CreateResource(u8"FXAA.DataQueue"V, { { NumBlocks, uint1(DataQueuePacked ? sizeof(uint1) : sizeof(float2)) }, DataQueuePacked ? EFormat::UInt1 : EFormat::Float2 });
		//RenderGraph.CommandContext.ClearUnorderedAccess(WorkCounter->RHIBuffer->UAView.Get(), {});

		//TArray<uint1, 8> InitialIndirectParameters { 0, 1, 1, 1, 0, 1, 1, 1 };
		// [DispatchX, DispatchY, DispatchZ] * 2
		//IndirectParameters = RenderGraph.CreateBufferT(u8"IndirectParameters"V, { { 8, sizeof(uint1) }, EFormat::None }, InitialIndirectParameters);
		IndirectParameters = RenderGraph.CreateResource(u8"FXAA.IndirectParameters"V, { { 8, sizeof(uint1) }, EFormat::UInt1 });
		//RenderGraph.CommandContext.ClearUnorderedAccess(IndirectParameters->RHIBuffer->UAView.Get(), { 1, 1, 1, 1 });

		if (RenderGraph.Scene.RenderSettings.FXAAMode == EFXAAMode::CS)
			RenderCS(InputColor, OutputColor);
		else
			RenderPS(InputColor, OutputColor);
	}

	void FFXAARenderer::RenderPS(FRGTexture * InputColor, FRGTexture * OutputColor)
	{
		struct FFXAAInputs
		{
			FRGTexture * InputColor;
			FRGTexture * OutputColor;
		};

		FFXAAInputs Inputs = { InputColor, OutputColor };
		RenderGraph.AddPass(u8"FXAA"V,
			[Inputs = Inputs](FCompileContext & CompileContext)
			{
				CompileContext.AccessResource(Inputs.InputColor, EResourceAccess::PixelSampledTexture);
				CompileContext.AccessResource(Inputs.OutputColor, EResourceAccess::RenderTarget, EResourceLoad::Clear);
			},
			[Inputs = Inputs, DebugRender = !!(Flags & EFXAAFlag::Debug)](FRGContext & RGContext)
			{
				SizeU InputSize = Inputs.InputColor->Desc.Size;
				FViewport Viewport { 0.0f, 0.0f, (float32)InputSize.Width, (float32)InputSize.Height, 0.0f, 1.0f };
				RGContext.BeginPass({ { Inputs.OutputColor, EResourceLoad::Clear } }, nullptr, Viewport);

				FMeshRenderState MeshRenderState;
				{
					FFXAAPS::FShaderVariance ShaderVariance;
					ShaderVariance.SetValue<FFXAAPS::QualityLevel>(int32(RGContext.RenderGraph.Scene.RenderSettings.FXAALevel));
					ShaderVariance.SetValue<FFXAAPS::Console>(RGContext.RenderGraph.Scene.RenderSettings.FXAAMode == EFXAAMode::PS_Console);
					ShaderVariance.SetValue<FFXAAPS::Debug>(DebugRender);

					MeshRenderState.Shaders[EShaderStage::Vertex] = RGContext.RenderGraph.Executor.Engine.ShaderBundle.GetShaderT<FScreenQuadVS>();
					if (RGContext.RenderGraph.Scene.RenderSettings.FXAAMode == EFXAAMode::PS_3_11)
						MeshRenderState.Shaders[EShaderStage::Pixel] = RGContext.RenderGraph.Executor.Engine.ShaderBundle.GetShaderT<FFXAAPS_3_11>(ShaderVariance);
					else
						MeshRenderState.Shaders[EShaderStage::Pixel] = RGContext.RenderGraph.Executor.Engine.ShaderBundle.GetShaderT<FFXAAPS>(ShaderVariance);

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

				struct
				{
					float2 FXAATexelSize;
					float1 FXAAEdgeThresholdMin;
					float1 FXAAEdgeThreshold;
					float1 FXAASubpix;
				}FXAAShaderParameter;
				FXAAShaderParameter.FXAATexelSize = 1.0f / Inputs.InputColor->Desc.Size;
				FXAAShaderParameter.FXAAEdgeThresholdMin = 0.0312f;
				FXAAShaderParameter.FXAAEdgeThreshold = 0.063f;
				FXAAShaderParameter.FXAASubpix = 0.75f;

				IRHIResource * ShaderParameter = RGContext.RenderGraph.CreateShaderParameterT(FXAAShaderParameter);
				MeshCommand.Parameters.Emplace(u8"ShaderParameter"N, ShaderParameter);
				MeshCommand.Parameters.Emplace(u8"Source"N, Inputs.InputColor->Resource);

				RGContext.DispatchCommand(MeshCommand);
				RGContext.EndPass();
			});
	}

	void FFXAARenderer::RenderCS(FRGTexture * InputColor, FRGTexture * OutputColor)
	{
		FRenderGraph::FScopedEvent FXAAEvent(RenderGraph, u8"FXAACS"V);

		if (!(Flags & EFXAAFlag::Debug))
			RenderGraph.BlitTexture(OutputColor, InputColor, RenderGraph.Executor.Engine.ShaderBundle.GetShaderT<FRGBAVisualizePS>());
		else
		{
			RenderGraph.AddPass(FStringV::None,
				[OutputColor = OutputColor](FRGContext & RGContext)
				{
					RGContext.ClearRenderTarget(OutputColor->Resource, OutputColor->Resource->ResourceDesc.ClearValue.Color);
				});
		}

		FFXAACSShaderParameter FXAACSShaderParameter;
		FXAACSShaderParameter.FXAATexelSize = 1.0f / InputColor->Desc.Size;
		FXAACSShaderParameter.FXAAEdgeThresholdMin = 0.0312f;
		FXAACSShaderParameter.FXAAEdgeThreshold = 0.063f;
		FXAACSShaderParameter.FXAASubpix = 0.75f;
		FXAACSShaderParameter.LastQueueIndex = WorkQueue->Desc.Size.X - 1;
		for (uint32 BlockIndexY = 0; BlockIndexY < BlockTile.Y; ++BlockIndexY)
		{
			for (uint32 BlockIndexX = 0; BlockIndexX < BlockTile.X; ++BlockIndexX)
			{
				FRenderGraph::FScopedEvent FXAACSPassEvent(RenderGraph, Format(u8"FXAACS Pass [{}, {}]"V, BlockIndexX, BlockIndexY));
				FXAACSShaderParameter.StartPixel.X = BlockIndexX * BlockSize.X;
				FXAACSShaderParameter.StartPixel.Y = BlockIndexY * BlockSize.Y;
				RenderCS_Prepare(InputColor, OutputColor, FXAACSShaderParameter);
				RenderCS_Resolve(InputColor, OutputColor, FXAACSShaderParameter);
				RenderCS_Final(InputColor, OutputColor, FXAACSShaderParameter);
			}
		}
	}

	void FFXAARenderer::RenderCS_Prepare(FRGTexture * InputColor, FRGTexture * OutputColor, const FFXAACSShaderParameter & FXAACSShaderParameter)
	{
		struct FXAACS_PrepareInputs
		{
			FRGTexture * Source;
			FRGBuffer * WorkCounter;
			FRGBuffer * WorkQueue;
			FRGBuffer * DataQueue;
		};
		FXAACS_PrepareInputs Inputs = { InputColor, WorkCounter, WorkQueue, DataQueue };
		RenderGraph.AddPass(u8"FXAA.PrepareCS"V,
			[Inputs = Inputs](FCompileContext & CompileContext)
			{
				CompileContext.AccessResource(Inputs.Source, EResourceAccess::ComputeSampledTexture);
				CompileContext.AccessResource(Inputs.WorkCounter, EResourceAccess::CopyWrite | EResourceAccess::StorageTexelBuffer);
				CompileContext.AccessResource(Inputs.WorkQueue, EResourceAccess::StorageTexelBuffer);
				CompileContext.AccessResource(Inputs.DataQueue, EResourceAccess::StorageTexelBuffer);
			},
			[FXAACSShaderParameter = FXAACSShaderParameter, Inputs = Inputs, BlockSize = BlockSize, DataQueuePacked = DataQueuePacked](FRGContext & RGContext)
			{
				RGContext.ClearUnorderedAccess(Inputs.WorkCounter->Resource, 0);
				IRHIResource * ShaderParameter = RGContext.RenderGraph.CreateShaderParameterT(FXAACSShaderParameter);

				FMeshRenderState MeshRenderState;
				{
					FFXAAPrepareCS::FShaderVariance ShaderVariance;
					ShaderVariance.SetValue<FFXAAPrepareCS::DataQueuePacked>(!!DataQueuePacked);
					MeshRenderState.Shaders[EShaderStage::Compute] = RGContext.RenderGraph.Executor.Engine.ShaderBundle.GetShaderT<FFXAAPrepareCS>(ShaderVariance);
				}
				FDrawCommand DispatchCommand;

				DispatchCommand.MeshRenderStateIndex = RGContext.RenderGraph.Executor.PipelineStateCache.IndexMeshRenderState(MeshRenderState);
				DispatchCommand.Dispatch = { DivideCeil((uint32)BlockSize.X, 8u), DivideCeil((uint32)BlockSize.Y, 8u), 1u };

				DispatchCommand.Parameters.Emplace(u8"ShaderParameter"N, ShaderParameter);

				DispatchCommand.Parameters.Emplace(u8"Source"N, Inputs.Source->Resource);

				DispatchCommand.Parameters.Emplace(u8"WorkCounter"N, Inputs.WorkCounter->Resource);
				DispatchCommand.Parameters.Emplace(u8"WorkQueue"N, Inputs.WorkQueue->Resource);
				DispatchCommand.Parameters.Emplace(u8"DataQueue"N, Inputs.DataQueue->Resource);

				RGContext.DispatchCommand(DispatchCommand);
			});
	}

	void FFXAARenderer::RenderCS_Resolve(FRGTexture * InputColor, FRGTexture * OutputColor, const FFXAACSShaderParameter & FXAACSShaderParameter)
	{
		struct FXAACS_ResolveInputs
		{
			FRGBuffer * WorkCounter;
			FRGBuffer * WorkQueue;
			FRGBuffer * IndirectParameters;
		};
		FXAACS_ResolveInputs Inputs = { WorkCounter, WorkQueue, IndirectParameters };
		RenderGraph.AddPass(u8"FXAA.ResolveCS"V,
			[Inputs = Inputs](FCompileContext & CompileContext)
			{
				CompileContext.AccessResource(Inputs.WorkCounter, EResourceAccess::ComputeUniformTexelBuffer);
				CompileContext.AccessResource(Inputs.WorkQueue, EResourceAccess::ComputeStorageTexelBuffer);
				CompileContext.AccessResource(Inputs.IndirectParameters, EResourceAccess::CopyWrite | EResourceAccess::ComputeStorageTexelBuffer);
			},
			[FXAACSShaderParameter = FXAACSShaderParameter, Inputs = Inputs](FRGContext & RGContext)
			{
				RGContext.ClearUnorderedAccess(Inputs.IndirectParameters->Resource, 1);
				IRHIResource * ShaderParameter = RGContext.RenderGraph.CreateShaderParameterT(FXAACSShaderParameter);

				FMeshRenderState MeshRenderState;
				{
					MeshRenderState.Shaders[EShaderStage::Compute] = RGContext.RenderGraph.Executor.Engine.ShaderBundle.GetShaderT<FFXAAResolveCS>();
				}
				FDrawCommand DispatchCommand;

				DispatchCommand.MeshRenderStateIndex = RGContext.RenderGraph.Executor.PipelineStateCache.IndexMeshRenderState(MeshRenderState);
				DispatchCommand.Dispatch = { 1, 1, 1 };

				DispatchCommand.Parameters.Emplace(u8"ShaderParameter"N, ShaderParameter);

				DispatchCommand.Parameters.Emplace(u8"WorkCounter"N, Inputs.WorkCounter->Resource);
				DispatchCommand.Parameters.Emplace(u8"WorkQueue"N, Inputs.WorkQueue->Resource);
				DispatchCommand.Parameters.Emplace(u8"IndirectParameters"N, Inputs.IndirectParameters->Resource);

				RGContext.DispatchCommand(DispatchCommand);
			});
	}

	void FFXAARenderer::RenderCS_Final(FRGTexture * InputColor, FRGTexture * OutputColor, const FFXAACSShaderParameter & FXAACSShaderParameter)
	{
		struct FXAACS_ResolveInputs
		{
			FRGTexture * Source;
			FRGBuffer * WorkQueue;
			FRGBuffer * DataQueue;
			FRGBuffer * IndirectParameters;
			FRGTexture * Target;
		};
		FXAACS_ResolveInputs Inputs = { InputColor, WorkQueue, DataQueue, IndirectParameters, OutputColor };
		RenderGraph.AddPass(u8"FXAA.FinalCS"V,
			[Inputs = Inputs](FCompileContext & CompileContext)
			{
				CompileContext.AccessResource(Inputs.Source, EResourceAccess::ComputeSampledTexture);
				CompileContext.AccessResource(Inputs.WorkQueue, EResourceAccess::ComputeUniformTexelBuffer);
				CompileContext.AccessResource(Inputs.DataQueue, EResourceAccess::ComputeUniformTexelBuffer);
				CompileContext.AccessResource(Inputs.Target, EResourceAccess::ComputeStorageTexture);
				CompileContext.AccessResource(Inputs.IndirectParameters, EResourceAccess::ComputeIndirectBuffer);
			},
			[FXAACSShaderParameter = FXAACSShaderParameter, Inputs = Inputs, DataQueuePacked = DataQueuePacked](FRGContext & RGContext)
			{
				IRHIResource * ShaderParameter = RGContext.RenderGraph.CreateShaderParameterT(FXAACSShaderParameter);

				FFXAAFinalCS::FShaderVariance ShaderVariance;
				ShaderVariance.SetValue<FFXAAFinalCS::QualityLevel>(int32(RGContext.RenderGraph.Scene.RenderSettings.FXAALevel));
				ShaderVariance.SetValue<FFXAAFinalCS::DataQueuePacked>(!!DataQueuePacked);

				// Final H
				{
					FMeshRenderState MeshRenderState;
					{
						ShaderVariance.SetValue<FFXAAFinalCS::VerticalOrientation>(false);
						MeshRenderState.Shaders[EShaderStage::Compute] = RGContext.RenderGraph.Executor.Engine.ShaderBundle.GetShaderT<FFXAAFinalCS>(ShaderVariance);
					}

					FDrawCommand DispatchCommand;

					DispatchCommand.MeshRenderStateIndex = RGContext.RenderGraph.Executor.PipelineStateCache.IndexMeshRenderState(MeshRenderState);

					FIndirectSignature IndirectSignature { sizeof(uint4), { { EIndirectParameterType::Dispatch } } };
					DispatchCommand.DispatchIndirect = { RGContext.RenderGraph.Executor.PipelineStateCache.FetchIndirectSignature(IndirectSignature), Inputs.IndirectParameters->Resource, 0 };

					DispatchCommand.Parameters.Emplace(u8"ShaderParameter"N, ShaderParameter);

					DispatchCommand.Parameters.Emplace(u8"Source"N, Inputs.Source->Resource);
					DispatchCommand.Parameters.Emplace(u8"WorkQueue"N, Inputs.WorkQueue->Resource);
					DispatchCommand.Parameters.Emplace(u8"DataQueue"N, Inputs.DataQueue->Resource);
					DispatchCommand.Parameters.Emplace(u8"Target"N, Inputs.Target->Resource);
					RGContext.DispatchCommand(DispatchCommand);
				}

				// Final V
				{
					FMeshRenderState MeshRenderState;
					{
						ShaderVariance.SetValue<FFXAAFinalCS::VerticalOrientation>(true);
						MeshRenderState.Shaders[EShaderStage::Compute] = RGContext.RenderGraph.Executor.Engine.ShaderBundle.GetShaderT<FFXAAFinalCS>(ShaderVariance);
					}

					FDrawCommand DispatchCommand;

					DispatchCommand.MeshRenderStateIndex = RGContext.RenderGraph.Executor.PipelineStateCache.IndexMeshRenderState(MeshRenderState);

					FIndirectSignature IndirectSignature { sizeof(uint4), { { EIndirectParameterType::Dispatch } } };
					DispatchCommand.DispatchIndirect = { RGContext.RenderGraph.Executor.PipelineStateCache.FetchIndirectSignature(IndirectSignature), Inputs.IndirectParameters->Resource, 16 };

					DispatchCommand.Parameters.Emplace(u8"ShaderParameter"N, ShaderParameter);

					DispatchCommand.Parameters.Emplace(u8"Source"N, Inputs.Source->Resource);
					DispatchCommand.Parameters.Emplace(u8"WorkQueue"N, Inputs.WorkQueue->Resource);
					DispatchCommand.Parameters.Emplace(u8"DataQueue"N, Inputs.DataQueue->Resource);
					DispatchCommand.Parameters.Emplace(u8"Target"N, Inputs.Target->Resource);
					RGContext.DispatchCommand(DispatchCommand);
				}
			});
	}

}
