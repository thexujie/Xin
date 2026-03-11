#include "PCH.h"
#include "RenderGraph.h"
#include "DrawCommand/DrawCommand.h"

namespace Xin::Engine
{
	FRenderGraph::FScopedEvent::FScopedEvent(FRenderGraph & RenderGraph, FStringV EventName, FColor Color) : RenderGraph(RenderGraph)
	{
		if (EventName.Size)
		{
			RenderGraph.AddPass({}, [EventName = FString(EventName), Color = Color](FRGContext & RGContext)
			{
				RGContext.BeginEvent(EventName, Color);
			});
			Triggered = true;
		}
	}

	FRenderGraph::FScopedEvent::~FScopedEvent()
	{
		if (Triggered)
		{
			RenderGraph.AddPass({}, [](FRGContext & RGContext)
			{
				RGContext.EndEvent();
			});
		}
	}

	FRenderGraph::FRenderGraph(FCommandExecutor & Executor, FScene & Scene, FCommandList & CommandList)
		: Executor(Executor), Scene(Scene), CommandList(CommandList)
	{
	}

	void FRenderGraph::Setup()
	{

	}

	void FRenderGraph::Execute()
	{
		if (ImmediatelyPass)
			return;

		if (Scene.RenderSettings.TransientResourceEnabled)
		{
			TransientAllocator = Executor.AcquireTransientAllocator();
			TransientAllocator->SetDebugName(u8"TransientAllocator[{}]"V.Format(Scene.SceneView.FrameIndex));
			TransientAllocator->TransientBegin();
			//AssertExpresion(TransientAllocator->GetMemoryUsage().Y == 0);
		}

		Allocator.BeginFrame();

		for (FRenderPassRef & RenderPass : RenderPasses)
		{
			FCompileContext CompileContext(RenderPass.Ref());
			RenderPass->Compile(CompileContext);
		}

		for (FRenderPassRef & RenderPass : RenderPasses)
		{
			for (FRenderPass::FPassResource & PassResource : RenderPass->PassResources)
			{
				PassResource.RGResource->InitialPass->InitialResources.AddUnique(PassResource.RGResource);
				PassResource.RGResource->FinalPass->FinalResources.AddUnique(PassResource.RGResource);
			}
		}

		FRGContext RGContext(*this, CommandList);
		for (uintx RenderPassIndex = 0; RenderPassIndex < RenderPasses.Size; ++RenderPassIndex)
		{
			FRenderPassRef & RenderPass = RenderPasses[RenderPassIndex];
			{
				FCommandList::FScopedEvent ScopedEvent(CommandList, RenderPass->PassName);

				BeginPass(RenderPass.Ref());
				RenderPass->Excute(RGContext);
				EndPass(RenderPass.Ref());
			}
			//CommandList.Flush();

			//CommandList.FlushCommands();
		}

		CommandList.Flush();

		SemanticResources.Clear();
		GraphRGResourceViews.Clear();

		for (IRHIResourceRef & PooledTexture : FramePooledTextures)
			Executor.ResourcePool.ForfeitResource(PooledTexture);
		FramePooledTextures.Clear();

		for (IRHIResourceRef & Resource : FrameResources)
			Executor.FenceResource(Resource);
		FrameResources.Clear();

		for (IRHIResourceViewRef & ResourceView : FrameResourceViews)
			Executor.FenceResourceView(ResourceView);
		FrameResourceViews.Clear();

		//for (IRHIResourceRef & RegsiteredResource : FrameRegisteredResources)
		//	Executor.FenceResource(RegsiteredResource);
		FrameRegisteredResources.Clear();

		if (TransientAllocator)
		{
			Executor.FenceTransientAllocator(TransientAllocator);
			TransientAllocator->TransientEnd();
			TransientAllocator = nullptr;
		}

		Allocator.EndFrame();
	}

	inline void FRenderGraph::BeginPass(FRenderPass & RenderPass)
	{
		for (FRGResource * RGResource : RenderPass.InitialResources)
		{
			if (RGResource->Resource == nullptr)
			{
				FResourceDesc ResourceDesc = RGResource->Desc;
				ResourceDesc.Usage = GetResourceUsageFromResourceAccess(RGResource->ResourceAccess);

				// Not an external Texture2D
				if (RGResource->Desc.Dimension == EResourceDimension::Texture2D)
				{
					/*if (ResourceDesc.Usage * EResourceUsage::Standalone)
					{
						FResourceRef Resource = Engine.Allocator.CreateResource(ResourceDesc);
						GraphResources.Add(Resource);

						Resource->SetName(Format(u8"{}.{}"V, RGResource->Semantic, RenderViewport.FrameIndex));
						RGResource->Resource = Resource.Get();
						RGResource->InitialAccess = Resource->Desc.InitialAccess;
						RGResource->CurrentAccess = Resource->Desc.InitialAccess;
					}
					else */
					if (TransientAllocator)
					{
						FTransientResourceRef TransientResource = TransientAllocator->AcquireResource(ResourceDesc);
						TransientResource->SetDebugName(Format(u8"{}.{}"V, RGResource->Semantic, Scene.SceneView.FrameIndex));

						if (TransientResource->ResourcesBefore.Size)
						{
							for (FTransientResource * ResourcesBefore : TransientResource->ResourcesBefore)
							{
								AssertExpr(ResourcesBefore->Memory.Location.Offset <= TransientResource->Memory.Location.Offset + TransientResource->Memory.Location.Size);
								AssertExpr(TransientResource->Memory.Location.Offset <= ResourcesBefore->Memory.Location.Offset + ResourcesBefore->Memory.Location.Size);
								CommandList.ResourceBarrierAlias(ResourcesBefore->Resource, TransientResource->Resource);
							}
						}
						else
							CommandList.ResourceBarrierAlias(nullptr, TransientResource->Resource);

						RGResource->TransientResource = TransientResource;
						RGResource->Resource = TransientResource->Resource;
						//RGResource->InitialAccess = RHIResource->ResourceDesc.InitialAccess;
						//RGResource->CurrentAccess = RHIResource->ResourceDesc.InitialAccess;
					}
					else
					{
						// Pool or Placed
						IRHIResourceRef Resource = Executor.ResourcePool.AcquireResource(ResourceDesc);
						FramePooledTextures.Add(Resource);

						RGResource->Resource = Resource;
						//RGResource->InitialAccess = Resource->RHIResource->ResourceDesc.InitialAccess;
						//RGResource->CurrentAccess = Resource->RHIResource->ResourceDesc.InitialAccess;
					}
				}
				else
				{
					IRHIResourceRef Resource = Executor.ResourcePool.AcquireResource(ResourceDesc);
					FramePooledTextures.Add(Resource);
					Resource->SetDebugName(Format(u8"{}.{}"V, RGResource->Semantic, Scene.SceneView.FrameIndex));
					RGResource->Resource = Resource;

					//IRHIResourceRef RHIResource = Engine.RHIContext.CreateResource(ResourceDesc);
					//GraphResources.Add(RHIResource);
					//RHIResource->SetResourceLabel(Format(u8"{}.{}"V, RGResource->Semantic, Scene.Viewport.FrameIndex));
					//RGResource->Resource = RHIResource;

					//RGResource->InitialAccess = Resource->RHIResource->ResourceDesc.InitialAccess;
					//RGResource->CurrentAccess = Resource->RHIResource->ResourceDesc.InitialAccess;
				}
			}

			RGResource->Resource->SetDebugName(Format(u8"{}.{}"V, RGResource->Semantic, Scene.SceneView.FrameIndex));
			//if (!GraphTrackedResources.FindBy([&](const FGraphAccess & GraphResource) { return GraphResource.RGResource == RGResource; }))
			//	GraphTrackedResources.Add({ RGResource, RGResource->ResourceAccess });
		}

		for (FRenderPass::FPassResource & PassResource : RenderPass.PassResources)
		{
			CommandList.ResourceAccess(PassResource.RGResource->Resource, PassResource.PassResourceAccess);

			if (PassResource.ResourceLoad == EResourceLoad::Discard)
			{
				// We don't need BarrierUAV for discard UAV access.
				if (PassResource.PassResourceAccess & EResourceAccess::StorageMask)
					CommandList.ResourceBarrierUAV(PassResource.RGResource->Resource);
				else
					CommandList.DiscardResource(PassResource.RGResource->Resource);
			}
			else
			{
			}
		}
		//CommandList.FlushBarriers();

		for (FRenderPass::FPassResourceView & PassResourceView : RenderPass.PassResourceViews)
		{
			if (PassResourceView.RGResourceView->ResourceView == nullptr)
			{
				AssertExpr(PassResourceView.RGResourceView->RGResource->Resource);
				IRHIResourceView * ResourceViewFound = PassResourceView.RGResourceView->RGResource->Resource->GetResourceView(PassResourceView.RGResourceView->Desc.SubresourceIndex);

				if (ResourceViewFound && ResourceViewFound->ViewDesc == PassResourceView.RGResourceView->Desc)
					PassResourceView.RGResourceView->ResourceView = ResourceViewFound;
				else
				{
					IRHIResourceViewRef RHIResourceView = Executor.Engine.Device.CreateResourceView(PassResourceView.RGResourceView->RGResource->Resource, PassResourceView.RGResourceView->Desc);
					FrameResourceViews.Add(RHIResourceView);
					PassResourceView.RGResourceView->ResourceView = RHIResourceView;
				}
			}
		}
	}

	inline void FRenderGraph::EndPass(FRenderPass & RenderPass)
	{
		for (FRGResource * RGResource : RenderPass.FinalResources)
		{
			if (RGResource->TransientResource)
			{
				TransientAllocator->ForfietResource(RGResource->TransientResource);
			}
		}
		//CommandList.FlushBarriers();
	}

	FRGResource * FRenderGraph::FindResource(FStringV Semantic)
	{
		if (auto Iter = SemanticResources.Find(Semantic); Iter != SemanticResources.End())
			return Iter->Second.Get();
		return nullptr;
	}

	FRGTexture * FRenderGraph::CreateResource(FStringV Semantic, const FRGResourceDesc & ResourceDesc)
	{
		AssertExpr(ResourceDesc.Depth != 0);
		AssertExpr(ResourceDesc.MipLevels != 0);

		if (SemanticResources.Find(Semantic) != SemanticResources.End())
		{
			LogWarning(u8"RegisterTexture failed, {0} is registed already.", Semantic);
			return nullptr;
		}

		FRGTextureRef RGTexture = new FRGTexture(Semantic, ResourceDesc);
		SemanticResources[Semantic] = RGTexture.Get();
		return RGTexture.Get();
	}

	FRGTexture * FRenderGraph::RegisterTexture(FStringV Semantic, IRHIResource * Texture, EResourceAccess InputAccess, EResourceAccess OutputAccess)
	{
		if (!Texture)
			return nullptr;

		if (SemanticResources.Find(Semantic) != SemanticResources.End())
		{
			LogWarning(u8"RegisterTexture failed, {0} is registed already.", Semantic);
			return nullptr;
		}

		FRGTextureRef RGTexture = new FRGTexture(Semantic, Texture);
		RGTexture->ResourceAccess = GetDefaultResourceAccess(Texture->ResourceDesc.Usage);
		RGTexture->InitialAccess = InputAccess;
		RGTexture->CurrentAccess = InputAccess;
		SemanticResources[Semantic] = RGTexture.Get();

		GraphTrackedResources.Add({ RGTexture.Get(), InputAccess });
		return RGTexture.Get();
	}

	FRGBuffer * FRenderGraph::RegisterBuffer(FStringV Semantic, IRHIResource * Buffer, EResourceAccess InputAccess)
	{
		//if (InputAccess == EResourceAccess::None)
		//	InputAccess = Buffer->RHIResource->ResourceDesc.InitialAccess;

		if (SemanticResources.Find(Semantic) != SemanticResources.End())
		{
			LogWarning(u8"RegisterBuffer failed, {0} is registed already.", Semantic);
			return nullptr;
		}

		Buffer->SetDebugName(Semantic);
		FRGResourceRef RGBuffer = new FRGResource(Semantic, Buffer);
		RGBuffer->ResourceAccess = GetResourceAccessFromResourceUsage(Buffer->ResourceDesc.Usage);
		//RGBuffer->InitialAccess = InputAccess == EResourceAccess::None ? Buffer->RHIResource->ResourceDesc.InitialAccess : InputAccess;
		RGBuffer->CurrentAccess = RGBuffer->InitialAccess;
		SemanticResources[Semantic] = RGBuffer.Get();
		FrameRegisteredResources.Add(Buffer);
		return RGBuffer.Get();
	}

	FRGResourceView * FRenderGraph::CreateResourceView(FRGResource * RGResource, const FResourceViewDesc & ResourceViewDesc)
	{
		FRGResourceViewRef RGResourceView = new FRGResourceView(RGResource, ResourceViewDesc);
		GraphRGResourceViews.Add(RGResourceView);
		return RGResourceView.Get();
	}

	FRGResourceView * FRenderGraph::CreateResourceView(FRGResource * RGResource, uint32 SubresouceIndex)
	{
		FRGResourceViewRef RGResourceView = new FRGResourceView(RGResource, SubresouceIndex);
		GraphRGResourceViews.Add(RGResourceView);
		return RGResourceView.Get();
	}

	FRGTexture * FRenderGraph::BlitTexture(const FRGBlitParameters & Parameters)
	{
		struct FPassInputs
		{
			FRGTexture * Input;
			FRGTexture * Output;
			FRGResourceView * InputSRV;
			FRGResourceView * OutputRTV;

			IShaderInstance * Shader;
			IRHIResource * ShaderParameter;
			EBlendMode BlendMode;
			EBlitTextureFlag Flags;
			RectI ViewRect;
		};

		AssertExpr(Parameters.Output || Parameters.OutputRTV);
		AssertExpr(Parameters.Input || Parameters.InputSRV);

		FPassInputs PassInputs {};
		if (Parameters.OutputRTV)
		{
			PassInputs.Output = Parameters.OutputRTV->RGResource;
			PassInputs.OutputRTV = Parameters.OutputRTV;
		}
		else
		{
			PassInputs.Output = Parameters.Output;
			PassInputs.OutputRTV = CreateResourceView(Parameters.Output);
		}

		if (Parameters.InputSRV)
		{
			PassInputs.Input = Parameters.InputSRV->RGResource;
			PassInputs.InputSRV = Parameters.InputSRV;
		}
		else
		{
			PassInputs.Input = Parameters.Input;
			PassInputs.InputSRV = CreateResourceView(Parameters.Input);
		}
		PassInputs.Shader = Parameters.Shader;
		PassInputs.ShaderParameter = Parameters.ShaderParameter;
		PassInputs.BlendMode = Parameters.BlendMode;
		PassInputs.Flags = Parameters.Flags;
		PassInputs.ViewRect = Parameters.ViewRect;

		FString PassName = (Parameters.Flags & EBlitTextureFlag::PassName) ? (Format(u8"{}  {}->{}"V, u8"CollectionName"V, Parameters.Input->Semantic, Parameters.Output->Semantic)) : FStringV::None;
		AddPass(PassName,
			[Inputs = PassInputs](FCompileContext & CompileContext)
			{
				CompileContext.AccessResourceView(Inputs.InputSRV, EResourceAccess::PixelSampledTexture);
				CompileContext.AccessResourceView(Inputs.OutputRTV, EResourceAccess::RenderTarget, AnyFlags(Inputs.Flags, EBlitTextureFlag::Center | EBlitTextureFlag::Clear) ? EResourceLoad::Clear : EResourceLoad::Discard);
			},
			[Inputs = PassInputs](FRGContext & RGContext)
			{
				EResourceLoad OutputLoad = AnyFlags(Inputs.Flags, EBlitTextureFlag::Center | EBlitTextureFlag::Clear) ? EResourceLoad::Clear : EResourceLoad::None;

				SizeI InputSize { Inputs.Input->Desc.Size };
				SizeI OutputSize { Inputs.Output->Desc.Size };
				if (Inputs.Flags & EBlitTextureFlag::Center)
				{
					RectI LocalViewRect;
					if (OutputSize.Width * InputSize.Height > InputSize.Width * OutputSize.Height)
					{
						LocalViewRect.Width = OutputSize.Height * InputSize.Width / InputSize.Height;
						LocalViewRect = { (OutputSize.Width - LocalViewRect.Width) / 2, 0, LocalViewRect.Width, OutputSize.Height };
					}
					else
					{
						LocalViewRect.Height = OutputSize.Width * InputSize.Height / InputSize.Width;
						LocalViewRect = { 0, (OutputSize.Height - LocalViewRect.Height) / 2, OutputSize.Width, LocalViewRect.Height };
					}

					FViewport Viewport { float32(LocalViewRect.X), float32(LocalViewRect.Y), float32(LocalViewRect.Width), float32(LocalViewRect.Height), 0.0f, 1.0f };
					RGContext.BeginPass({ { Inputs.OutputRTV, OutputLoad } }, nullptr, Viewport);
				}
				else if (Inputs.ViewRect.Width && Inputs.ViewRect.Height)
				{
					FViewport Viewport { float32(Inputs.ViewRect.X), float32(Inputs.ViewRect.Y), float32(Inputs.ViewRect.Width), float32(Inputs.ViewRect.Height), 0.0f, 1.0f };
					RGContext.BeginPass({ { Inputs.OutputRTV, OutputLoad } }, nullptr, Viewport);
				}
				else
				{
					FViewport Viewport { 0.0f, 0.0f, float32(OutputSize.Width), float32(OutputSize.Height), 0.0f, 1.0f };
					RGContext.BeginPass({ { Inputs.OutputRTV, OutputLoad } }, nullptr, Viewport);
				}


				FMeshRenderState MeshRenderState;
				{
					MeshRenderState.Shaders = { RGContext.RenderGraph.Executor.Engine.ShaderBundle.GetShaderT<FScreenQuadVS>(), Inputs.Shader, };
					MeshRenderState.InputElements = {};
					MeshRenderState.RasterizeState = FRasterizeState::Default;
					if (Inputs.BlendMode == EBlendMode::Transparent)
						MeshRenderState.BlendState = FBlendState::Transparent;
					else if (Inputs.BlendMode == EBlendMode::Premultiplied)
						MeshRenderState.BlendState = FBlendState::Premultiplied;
					else
						MeshRenderState.BlendState = FBlendState::Default;
					MeshRenderState.DepthStencilState = FDepthStencilState::Disabled;
				}

				FDrawCommand MeshCommand;
				MeshCommand.MeshRenderStateIndex = RGContext.RenderGraph.Executor.PipelineStateCache.IndexMeshRenderState(MeshRenderState);
				MeshCommand.VertexCount = 3;
				MeshCommand.Instanced.VertexOffset = 0;
				MeshCommand.Instanced.VertexCount = 3;

				if (Inputs.ShaderParameter)
					MeshCommand.Parameters.Emplace(u8"ShaderParameter"N, Inputs.ShaderParameter);
				MeshCommand.Parameters.Emplace(u8"Source"N, Inputs.InputSRV);
				RGContext.DispatchCommand(MeshCommand);
				RGContext.EndPass();
			});
		return PassInputs.Output;
	}

	void FRenderGraph::BlurCS(FRGTexture * Output, FRGTexture * Input, FStringV PassName)
	{
		struct FBlurInputs
		{
			FRGTexture * Input;
			FRGTexture * Output;
		};
		FBlurInputs Inputs = { Input, Output };
		AddPass(PassName.Size ? PassName : Format(u8"Blur {} -> {}"V, Input->Semantic, Output->Semantic),
			[Inputs = Inputs](FCompileContext & CompileContext)
			{
				CompileContext.AccessResource(Inputs.Input, EResourceAccess::ComputeSampledTexture);
				CompileContext.AccessResource(Inputs.Output, EResourceAccess::ComputeStorageTexture);
			},
			[this, Inputs = Inputs](FRGContext & RGContext)
			{
				SizeU InputSize = Inputs.Input->Desc.Size;
				FMeshRenderState MeshRenderState;
				{
					MeshRenderState.Shaders[EShaderStage::Compute] = Executor.Engine.ShaderBundle.GetShaderT<FGussainBlurCS>();
				}

				FDrawCommand DispatchCommand;
				DispatchCommand.MeshRenderStateIndex = RGContext.RenderGraph.Executor.PipelineStateCache.IndexMeshRenderState(MeshRenderState);
				DispatchCommand.Dispatch = { DivideCeil(InputSize.X, 8u), DivideCeil(InputSize.Y, 8u), 1u };

				DispatchCommand.Parameters.Emplace(u8"Input"N, Inputs.Input->Resource);
				DispatchCommand.Parameters.Emplace(u8"Output"N, Inputs.Output->Resource);

				RGContext.DispatchCommand(DispatchCommand);
			});
	}

	void FRenderGraph::ClearUIntCS(FRGTexture * Source, uint32 ClearValue, FStringV PassName)
	{
		struct FInputs
		{
			FRGTexture * Source;
			uint32 ClearValue;
		};
		FInputs Inputs = { Source, ClearValue };
		AddPass(PassName.Size ? PassName : Format(u8"ClearUIntCS {}"V, Source->Semantic),
			[Inputs = Inputs](FCompileContext & CompileContext)
			{
				CompileContext.AccessResource(Inputs.Source, EResourceAccess::ComputeStorageBuffer);
			},
			[this, Inputs = Inputs](FRGContext & RGContext)
			{
				IRHIResource * ShaderParameter = RGContext.RenderGraph.CreateShaderParameterT(Inputs.ClearValue);

				SizeU InputSize = Inputs.Source->Desc.Size;
				FMeshRenderState MeshRenderState;
				{
					MeshRenderState.Shaders[EShaderStage::Compute] = Executor.Engine.ShaderBundle.GetShaderT<FClearUIntCS>();
				}

				FDrawCommand DispatchCommand;
				DispatchCommand.MeshRenderStateIndex = RGContext.RenderGraph.Executor.PipelineStateCache.IndexMeshRenderState(MeshRenderState);
				DispatchCommand.Dispatch = { DivideCeil(InputSize.X, 8u), DivideCeil(InputSize.Y, 8u), 1u };

				DispatchCommand.Parameters.Emplace(u8"ShaderParameter"N, ShaderParameter);
				DispatchCommand.Parameters.Emplace(u8"Source"N, Inputs.Source->Resource);

				RGContext.DispatchCommand(DispatchCommand);
			});
	}

	IRHIResource * FRenderGraph::CreateShaderParameter(uint32 Size, TView<byte> InitialBytes)
	{
		IRHIResourceRef RHIResource = Executor.CreateBuffer(FBufferDesc(Size, EResourceUsage::UniformBuffer | EResourceUsage::HostWrite), InitialBytes);
		FrameResources.Add(RHIResource);
		return RHIResource;
	}

	IRHIResource * FRenderGraph::CreateShaderParameter(Size2U Size, EFormat Format, TView<byte> InitialBytes)
	{
		IRHIResourceRef RHIResource = Executor.CreateBuffer(FBufferDesc(Size, Format, EResourceUsage::UniformBuffer | EResourceUsage::HostWrite), InitialBytes);
		FrameResources.Add(RHIResource);
		return RHIResource;
	}
}
