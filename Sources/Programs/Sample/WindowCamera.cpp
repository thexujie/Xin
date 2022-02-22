#include "PCH.h"
#include "WindowCamera.h"

#include "Render/ConstBuffer.h"
#include "Platform/Win32/Win32.h"

class FWindowSceneView : public FSceneView
{
public:
	FWindowSceneView(FScene & Scene_, ptr_t WindowHandle_) : FSceneView(Scene_), WindowHandle(WindowHandle_)
	{
		
	}

	void FinishRender_rdt(FRHICommandList & RHICommandList, FRHIResource * SceneColor) override
	{
		if (!RHISwapChain || RHISwapChain->GetSize() != ViewSize)
		{
			RHISwapChain.Reset();

			auto & Renderer = FRenderSystem::Instance().Renderer.Value();

			RHI::FSwapChainDesc SwapChainArgs;
			SwapChainArgs.WindowHandle = WindowHandle;
			SwapChainArgs.NumBuffers = /*NumBackBuffers*/3;
			SwapChainArgs.Size = ViewSize;
			RHISwapChain = Renderer.RHIDevice.CreateSwapChain(Renderer.RHICommandQueue.Get(), SwapChainArgs);
		}

		uint32_t FrameIndex = RHISwapChain->FrameIndex();
		RHI::FRHIResource * RHICurrentFrameResource = RHISwapChain->GetBuffer(FrameIndex);
		if (SceneColor->GetResourceArgs().MSAA.Level > 1)
		{
			RHICommandList.TransitionBarrier(RHICurrentFrameResource, EResourceState::ResolveDest);
			RHICommandList.TransitionBarrier(SceneColor, EResourceState::ResolveSource);
			RHICommandList.ResolveSubresource(RHICurrentFrameResource, 0, SceneColor, 0, EPixelFormat::BGRA);
			RHICommandList.TransitionBarrier(RHICurrentFrameResource, EResourceState::Present);
		}
		else
		{
			RHICommandList.TransitionBarrier(RHICurrentFrameResource, EResourceState::CopyDest);
			RHICommandList.TransitionBarrier(SceneColor, EResourceState::CopySource);
			RHICommandList.CopyResource(RHICurrentFrameResource, SceneColor);
			RHICommandList.TransitionBarrier(RHICurrentFrameResource, EResourceState::Present);
		}

		RHICommandList.Close();
		FRenderSystem::Instance().Renderer->RHICommandQueue->ExcuteCommandList(&RHICommandList);

		RHISwapChain->Present(0);
	}

	void Render_rdt(FRenderContext & RenderContext)
	{
		
	}

	ptr_t WindowHandle = nullptr;
	TSharedPtr<FRHISwapChain> RHISwapChain;

	FPresentPass PresentPass;

public:
	void Flush()
	{
		FRenderSystem & Engine = FRenderSystem::Instance();
		Engine.Renderer->CurrentRHICommandList();

		FRenderPassBuilder RenderGraphBuilder;
		IRenderPass * OutputPass = RenderGraphBuilder.Build_rdt(*this);
		PresentPass.Excute_rdt();
	}
};


FWindowCamera::FWindowCamera(AActor & Actor_, ptr_t WindowHandle_): UCameraComponent(Actor_), WindowHandle(WindowHandle_)
{
	_UpdateWindowSize();
}

TReferPtr<FSceneView> FWindowCamera::CreateSceneView()
{
	return MakeRefer<FWindowSceneView>(Actor.World.Scene, WindowHandle);
}

void FWindowCamera::Tick(float DeltaTime)
{
	UCameraComponent::Tick(DeltaTime);
	if (SceneView)
	{
		::SetWindowTextW((HWND)WindowHandle, Format(L"FPS={0:.3f}", SceneView->SceneViewStatics.FPS).Data);
		//Debug::WriteLine(Format(L"FPS={0:.3f}", SceneView->SceneViewStatics.FPS));
	}
}

void FWindowCamera::_UpdateWindowSize()
{
	RECT rcClient = {};
	GetClientRect(HWND(WindowHandle), &rcClient);

	ViewSize = FWindowUtils::GetWindowSize(WindowHandle);

	ViewPort.Left = 0;
	ViewPort.Top = 0;
	ViewPort.Width = float32_t(rcClient.right - rcClient.left);
	ViewPort.Height = float32_t(rcClient.bottom - rcClient.top);
	ViewPort.NearZ = 0.0f;
	ViewPort.FarZ = 1.0f;

	SccisorRect = { 0, 0, int32_t(ViewPort.Width), int32_t(ViewPort.Height) };

	OnCameraViewChanged();
}

void FWindowCamera::Cull()
{
	//VisiblePrimitives.Clear();
	//VisiblePrimitives.Add(Scene.ScenePrimitives);
	//
	//for (IScenePrimitive * ScenePrimitive : VisiblePrimitives)
	//{
	//	ScenePrimitive->SetSceneViewVisibility(*this, true);
	//}
}

void FWindowCamera::OnWindowUpdated()
{
	if (!WindowView)
		WindowView = MakeRefer<FWindowSceneView>(Actor.World.Scene, WindowHandle);
	WindowView->Flush();
}

