#include "pch.h"
#include "00.Test.RHI.h"

int TestRHI_Empty(IRHIDeviceRef Device)
{
	TestRHI_Initialize();

	HWND WindowHandle = TestRHI_CreateWindow();

	SizeU ClientSize = Windows::GetClientSize(WindowHandle);
	IRHICommandQueueRef CommandQueue = Device->CreateCommandQueue({ ECommandType::Direct });
	IRHICommandListRef CommandList = Device->CreateCommandList(ECommandType::Direct);
	IRHIViewportRef Viewport = Device->CreateViewport(CommandQueue, WindowHandle, ClientSize, NumBackBuffers, EFormat::R8G8B8A8UF);

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
					CommandList->BeginPass({ { { BackBuffer, EResourceLoad::Clear, Colors::LightSkyBlue } }, { },  { { 0.0f, 0.0f, float32(ClientSize.Width), float32(ClientSize.Height), 0.0f, 1.0f } } });
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
