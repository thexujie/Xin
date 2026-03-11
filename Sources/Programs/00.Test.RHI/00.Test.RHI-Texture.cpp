#include "pch.h"
#include "00.Test.RHI.h"

#include "Primitive-Plane.h"
#include "00.Test.RHI-Utils.h"


class FImageVS : public IShaderSource
{
public:
	DEFINE_SHADER_INSTANCE(FImageVS);

	FImageVS() : IShaderSource(EShaderStage::Vertex, FPath::Combine(GetProjectAssetsPath(), u8"Shaders/Image.xsf"V), u8"MainVS"V) {}
};

class FImagePS : public IShaderSource
{
public:
	DEFINE_SHADER_INSTANCE(FImagePS);

	FImagePS() : IShaderSource(EShaderStage::Pixel, FPath::Combine(GetProjectAssetsPath(), u8"Shaders/Image.xsf"V), u8"MainPS"V) {}
};


int TestRHI_Texture(FEngine & Engine)
{
	TestRHI_Initialize();

	FCommandExecutor CommandExecutor { Engine };
	FCommandList CommandList { CommandExecutor };

	HWND WindowHandle = TestRHI_CreateWindow();

	SizeU ClientSize = Windows::GetClientSize(WindowHandle);
	IRHIViewportRef Viewport = Engine.Device.CreateViewport(CommandExecutor.PrimaryQueue, WindowHandle, ClientSize, NumBackBuffers, EFormat::R8G8B8A8UF);

	IShaderInstance * VertexShader = Engine.ShaderBundle.GetShaderT<FImageVS>();
	//VertexShader->SetResourceLabel(u8"MainVS"V);
	IShaderInstance * PixelShader = Engine.ShaderBundle.GetShaderT<FImagePS>();
	//PixelShader->SetResourceLabel(u8"MainPS"V);

	FPipelineStateDesc PipelineStateDesc;
	PipelineStateDesc.Shaders[EShaderStage::Vertex] = VertexShader->GetShaderModule();
	PipelineStateDesc.Shaders[EShaderStage::Pixel] = PixelShader->GetShaderModule();
	PipelineStateDesc.Topology = ETopology::TriangleList;
	PipelineStateDesc.InputFormat.Streams[0].Stride = sizeof(float3) + sizeof(float2);
	PipelineStateDesc.InputFormat.Elements[0] = { EInputSemantic::Position, 0, EFormat::Float3, 0, 0 };
	PipelineStateDesc.InputFormat.Elements[1] = { EInputSemantic::Texcoord, 0, EFormat::Float2, 0, sizeof(float3) };
	PipelineStateDesc.OutputFormat.RenderTargets[0] = EFormat::R8G8B8A8UF;

	PipelineStateDesc.RasterizeState.CullMode = ECullMode::Back;

	PipelineStateDesc.StaticSamplers[0] = { EShaderRelevance::Pixel, 0, 0, ESampleFilter::Linear, EAddressMode::Clamp, EAddressMode::Clamp, EAddressMode::Clamp };

	IRHIPipelineStateRef PipelineState = Engine.Device.CreatePipelineState(PipelineStateDesc);

	IImageRef Image = IImage::Load(u8"../../Data/Cat.jpg"V);
	IRHIResourceRef TextureA = CommandExecutor.CreateTexture({ Image->ImageData.Format, { Image->ImageData.Width, Image->ImageData.Height }, EResourceUsage::SampledTexture | EResourceUsage::CopyWrite }, FTextureImageSource { Image->ImageData });
	IRHIResourceRef TextureB = CommandExecutor.CreateTexture({ Image->ImageData.Format, { Image->ImageData.Width, Image->ImageData.Height }, EResourceUsage::SampledTexture | EResourceUsage::CopyWrite }, FTextureImageSource { Image->ImageData });

	IRHIResourceRef VertexBuffer = CommandExecutor.CreateBuffer(FBufferDesc { sizeof(PlanePureVertices), EResourceUsage::VertexBuffer | EResourceUsage::CopyWrite }, ReinterpretView<byte>(PlanePureVertices.View()));

	FMatrix WorldMatrix = FMatrix::Scale({ 500, 200, 1 }) * FMatrix::Translate(-400, 0, 0);
	//FMatrix WorldMatrix = FMatrix::Scale({float32(ImageData->Width), float32(ImageData->Height), 1});
	IRHIResourceRef WorldParameters = CommandExecutor.CreateBuffer(FBufferDesc { sizeof(FMatrix), EResourceUsage::UniformBuffer | EResourceUsage::HostWrite }, ViewBytes(WorldMatrix));

	FMatrix ViewMatrix = FMatrix::LookToLH({}, { 0, 0, 1 }, { 0, 1, 0 });
	FMatrix ProjectionMatrix = FMatrix::OrthographicLH(ClientSize.Width, ClientSize.Height, 0.01f, 100.0f);
	FMatrix ViewProjectionMatrix = ViewMatrix * ProjectionMatrix;
	//ViewMatrix = FMatrix::Identity;
	IRHIResourceRef ViewParameters = CommandExecutor.CreateBuffer(FBufferDesc { sizeof(FMatrix), EResourceUsage::UniformBuffer | EResourceUsage::HostWrite }, ViewBytes(ViewProjectionMatrix));

	float4 Color = { 1, 1, 1, 1 };
	IRHIResourceRef PixelParameters = CommandExecutor.CreateBuffer(FBufferDesc { sizeof(float4), EResourceUsage::UniformBuffer | EResourceUsage::HostWrite }, ViewBytes(Color));

	CommandList.Flush();

	float64 Time = FDateTime::Steady();
	MSG msg = {};
	FCounter<float32, 3> FpsCounter { 1000 };
	float64 LastTitleUpdateTime = 0;
	while (msg.message != WM_QUIT)
	{
		//::GetMessageW(&msg, NULL, 0, 0);
		//TranslateMessage(&msg);
		//DispatchMessageW(&msg);

		float64 CurrentTime = FDateTime::Steady();
		float32 DeltaTime = float32(CurrentTime - Time);
		Time = CurrentTime;

		uint32 BackBufferIndex = Viewport->BeginFrame();
		if (BackBufferIndex != NullIndex)
		{
			IRHIResource * BackBuffer = Viewport->GetBackBuffer(BackBufferIndex);

			CommandExecutor.Update(DeltaTime);

			{
				CommandList.ResourceAccess(BackBuffer, EResourceAccess::RenderTarget);
				{
					CommandList.BeginPass({ { { BackBuffer, EResourceLoad::Clear, Colors::Gray } } });

					CommandList.SetViewPorts({ { 0, 0, float32(ClientSize.Width), float32(ClientSize.Height), 0.0f, 1.0f } });
					CommandList.SetScissorRects({ { 0, 0, int32(ClientSize.Width), int32(ClientSize.Height) } });

					CommandList.SetPipelineState(PipelineState);

					CommandList.IASetVertexBuffers(0, { VertexBuffer }, { sizeof(FPlaneVertex) }, 6);

					CommandList.SetParameter(u8"WorldParameters"N, WorldParameters);
					CommandList.SetParameter(u8"ViewParameters"N, ViewParameters);
					CommandList.SetParameter(u8"PixelParameters"N, PixelParameters);
					CommandList.SetParameter(u8"ImageA"N, TextureA);
					CommandList.SetParameter(u8"ImageB"N, TextureB);

					CommandList.DrawInstanced(0, 6);

					CommandList.EndPass();
				}
				CommandList.ResourceAccess(BackBuffer, EResourceAccess::Present);
				CommandList.Flush();
				Viewport->EndFrame();
			}
			Viewport->EndFrame();
		}

		DWORD dwWait = MsgWaitForMultipleObjectsEx(0, NULL, 0, QS_ALLEVENTS, MWMO_ALERTABLE);
		switch (dwWait)
		{
		default:
			break;
		}

		//Sleep(20);

		FpsCounter.Accumulate(1);
		if (CurrentTime - LastTitleUpdateTime >= 1)
		{
			FWString Title = Format(L"FPS={:.2f}"V, FpsCounter.Average());
			::SetWindowTextW(WindowHandle, Title.Data);
			LastTitleUpdateTime = CurrentTime;
		}

		while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}

	CommandExecutor.WaitFinish();

	return 0;
}
