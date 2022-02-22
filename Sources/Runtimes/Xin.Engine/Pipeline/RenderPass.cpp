#include "PCH.h"
#include "RenderPass.h"
#include "RenderGraph.h"

namespace Xin::Engine
{
	void FCompileContext::AccessResource(FRGResource * Resource, EResourceAccess ResourceAccess, EResourceLoad ResourceLoad)
	{
		if (!Resource)
			return;

		if (ResourceLoad == EResourceLoad::None)
		{
			if (ResourceAccess & EResourceAccess::SampledTexture)
				ResourceLoad = EResourceLoad::Load;
			else if (ResourceAccess & EResourceAccess::DepthRead)
				ResourceLoad = EResourceLoad::Load;
			else
				ResourceLoad = EResourceLoad::Discard;
		}

		Resource->ResourceAccess |= ResourceAccess;
		if (!Resource->InitialPass)
			Resource->InitialPass = &RenderPass;
		Resource->FinalPass = &RenderPass;
		RenderPass.PassResources.Add({ Resource, ResourceAccess, ResourceLoad });
	}

	void FCompileContext::AccessResourceView(FRGResourceView * ResourceView, EResourceAccess ResourceAccess, EResourceLoad ResourceLoad)
	{
		if (ResourceView)
		{
			if (ResourceLoad == EResourceLoad::None)
			{
				if (ResourceAccess & EResourceAccess::SampledTexture)
					ResourceLoad = EResourceLoad::Load;
				else if (ResourceAccess & EResourceAccess::DepthRead)
					ResourceLoad = EResourceLoad::Load;
				else
					ResourceLoad = EResourceLoad::Discard;
			}

			AssertExpr(ResourceView->RGResource);
			RenderPass.PassResourceViews.Add({ ResourceView, ResourceAccess, ResourceLoad });
			AccessResource(ResourceView->RGResource, ResourceAccess, ResourceLoad);
		}
	}

	FRGContext::FRGContext(FRenderGraph & RenderGraph, FCommandList & CommandList)
		: RenderGraph(RenderGraph), CommandList(CommandList) {}

	void FRGContext::ClearUnorderedAccess(IRHIResource * RHIResource, uint32 ClearValue)
	{
		CommandList.ClearUnorderedAccess(RHIResource, ClearValue);
	}

	void FRGContext::BeginPass(const TArray<FRGRenderPassTarget, MaxRenderTargets> & RenderTargets, const FRGRenderPassTarget & DepthStencil, const FViewport & Viewport)
	{
		FRenderPassState RenderPassState {};
		for (uintx RTIndex = 0; RTIndex < RenderTargets.Size; ++RTIndex)
		{
			const FRGRenderPassTarget & RGRenderPassTarget = RenderTargets[RTIndex];
			if (RGRenderPassTarget)
			{
				RenderPassState.RenderTargets[RTIndex].ResourceView = RGRenderPassTarget.ResourceView;
				RenderPassState.RenderTargets[RTIndex].LoadAction = RGRenderPassTarget.LoadAction;
				RenderPassState.RenderTargets[RTIndex].ClearValue = RGRenderPassTarget.ClearValue;
			}
		}

		if (DepthStencil)
		{
			RenderPassState.DepthStencil.ResourceView = DepthStencil.ResourceView;
			RenderPassState.DepthStencil.LoadAction = DepthStencil.LoadAction;
			RenderPassState.DepthStencil.ClearValue = DepthStencil.ClearValue;
		}
		RenderPassState.Viewports[0] = Viewport;

		CommandList.BeginPass(RenderPassState);
	}

	void FRGContext::EndPass()
	{
		CommandList.EndPass();
	}

	inline void FRGContext::DispatchCommand(const FDrawCommand & MeshCommand)
	{
#ifdef XIN_DEBUG
		//IRHICommandList::FScopedEvent ScopedEvent(CommandList, MeshCommand.RenderPrimitive ?
		//	MeshCommand.RenderPrimitive->GetPrimitiveName() + u8" ("V + InterpretCastRef<const FMaterial>(MeshCommand.Material).ResourcePath + u8")"V : FStringView::Empty);
#endif

		CommandList.SetMeshRenderState(MeshCommand.MeshRenderStateIndex);

		for (const FCommandParameter & ResourceParameter : MeshCommand.Parameters)
		{
			if (ResourceParameter.RGResource)
				CommandList.SetParameter(ResourceParameter.Name, ResourceParameter.RGResource->Resource);
			else if (ResourceParameter.RGResource)
				CommandList.SetParameter(ResourceParameter.Name, ResourceParameter.RGResource->Resource);
			else if (ResourceParameter.RGResourceView)
				CommandList.SetParameter(ResourceParameter.Name, ResourceParameter.RGResourceView->ResourceView);
			else if (ResourceParameter.Resource)
				CommandList.SetParameter(ResourceParameter.Name, ResourceParameter.Resource);
			else if (ResourceParameter.ResourceView)
				CommandList.SetParameter(ResourceParameter.Name, ResourceParameter.ResourceView);
			else
				AssertFailed(u8"Invalid FResourceParameter."V);
		}

		if (MeshCommand.Dispatch.GorupSizeX)
		{
			CommandList.Dispatch({ MeshCommand.Dispatch.GorupSizeX, MeshCommand.Dispatch.GorupSizeY, MeshCommand.Dispatch.GorupSizeZ });
		}
		else if (MeshCommand.DispatchIndirect.IndirectBuffer)
		{
			CommandList.DispatchIndirect(MeshCommand.DispatchIndirect.IndirectSignature, MeshCommand.DispatchIndirect.IndirectBuffer, MeshCommand.DispatchIndirect.IndirectBufferOffset, MeshCommand.DispatchIndirect.MaxCommandCount);
		}
		else
		{
			CommandList.IASetVertexBuffers(0, MeshCommand.VertexBuffers.View(0, MeshCommand.VertexBufferCount), MeshCommand.VertexStrides, MeshCommand.VertexCount);
			if (MeshCommand.IndexCount > 0)
			{
				CommandList.IASetIndexBuffer(MeshCommand.IndexBuffer, MeshCommand.IndexFormat, MeshCommand.IndexCount);
				CommandList.DrawIndexedInstanced(MeshCommand.IndexedInstanced.IndexOffset, MeshCommand.IndexedInstanced.IndexCount, MeshCommand.IndexedInstanced.VertexOffset);
			}
			else
			{
				CommandList.IASetIndexBuffer(nullptr, EIndexFormat::None, 0);
				CommandList.DrawInstanced(MeshCommand.Instanced.VertexOffset, MeshCommand.Instanced.VertexCount);
			}
		}
	}

	void FRGContext::DispatchCommands(TView<FDrawCommand> MeshCommands)
	{
		// Dispatch Draw Calls
		for (uintx Index = 0; Index < MeshCommands.Size; ++Index)
		{
			const FDrawCommand & VisibleMeshCommand = MeshCommands[Index];
			DispatchCommand(VisibleMeshCommand);
		}
		//CommandList.FlushCommands();
	}
}
