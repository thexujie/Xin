#include "pch.h"
#include "00.Test.RHI.h"

int TestRHI_ColorPS(IRHIDeviceRef Device)
{
	HWND WindowHandle = TestRHI_CreateWindow();

	IRHICommandQueueRef CommandQueue = Device->CreateCommandQueue({ ECommandType::Direct });
	IRHICommandListRef CommandList = Device->CreateCommandList(ECommandType::Direct);
	IRHIViewportRef Viewport = Device->CreateViewport(CommandQueue, WindowHandle, Windows::GetClientSize(WindowHandle), NumBackBuffers, EFormat::R8G8B8A8UF);

	IRHIShaderCompiler & ShaderCompiler = Device->GetShaderCompiler();

	FShaderCompilerContext ShaderCompilerContext;
	ShaderCompiler.CompilerFromFile(u8"Contents/Shaders/ColorPS"V, ShaderCompilerContext);

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

			CommandQueue->UpdateContext();

			{
				CommandList->BeginCommand(CommandQueue);
				CommandList->ResourceAccess(BackBuffer, EResourceAccess::RenderTarget);
				{
					CommandList->BeginPass({ { { BackBuffer, EResourceLoad::Clear, Colors::Gray } }, { } });
					CommandList->EndPass();
				}
				CommandList->ResourceAccess(BackBuffer, EResourceAccess::Present);
				CommandList->ExecuteCommands(CommandQueue);

				CommandList->EndCommand(CommandQueue);
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

	CommandQueue->WaitFinish();

	return 0;
}
