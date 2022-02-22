#include "pch.h"
#include "RHIWindow.h"

namespace Xin::RHI::UI
{
	FRHIWindow::FRHIWindow(IRHIDevice & Device, IGraphics & OfflineGraphic)
		: Device(Device), Engine(Device), Executor(Engine), RHIGraphics(Executor)
	{
		RHIGraphics.Initialize(&OfflineGraphic);
	}

	FRHIWindow::~FRHIWindow()
	{
		Executor.WaitFinish();
	}

	void FRHIWindow::OnConstruct()
	{
		UWindow::OnConstruct();
	}

	void FRHIWindow::RunLoop()
	{
		FApplication::Instance().Loop([&]
			{
				if (Shown)
				{
					UpdateViewport();
					Render();
				}
			});
	}

	void FRHIWindow::Render()
	{
		uint32 BackBufferIndex = Viewport->BeginFrame();
		if (BackBufferIndex != NullIndex)
		{
			PaintWindows(BackBitmaps[BackBufferIndex]);
			Viewport->EndFrame();
			Viewport->EndFrame(false);
		}
	}

	void FRHIWindow::UpdateViewport()
	{
		SizeU ViewportSize = WindowSize;
		if (!Viewport)
		{
			Viewport = Engine.Device.CreateViewport(Executor.PrimaryQueue.Get(), voidp(WindowHandle->GetWindowHandle()), ViewportSize, BackBufferCount, EFormat::R10G10B10A2UF, EPresentMode::FlipDiscard);
			Viewport->SetDebugName(u8"MainViewport"V);
			BackBitmaps.Clear();
			BackBuffers.Clear();
		}
		else
		{
			if (Viewport->GetSize() != ViewportSize)
			{
				Executor.WaitRefresh();
				BackBitmaps.Clear();
				BackBuffers.Clear();
				Viewport->Resize(ViewportSize);
			}
		}

		if (BackBuffers.Size == 0)
		{
			BackBitmaps.Reset(Viewport->GetNumBackBuffers(), nullptr);
			BackBuffers.Reset(Viewport->GetNumBackBuffers(), nullptr);

			for (uint32 BufferIndex = 0; BufferIndex < Viewport->GetNumBackBuffers(); ++BufferIndex)
			{
				BackBuffers[BufferIndex] = Viewport->GetBackBuffer(BufferIndex);
				BackBitmaps[BufferIndex] = RHIGraphics.CreateBitmap(BackBuffers[BufferIndex], EBitmapUsage::RenderTarget);
				//BackBitmaps[BufferIndex] = RHIGraphics.CreateBitmap(EFormat::R8G8B8A8UF, WindowSize, EBitmapUsage::RenderTarget);
			}
		}
	}

	void FRHIWindow::PaintWindows(IBitmap * Bitmap)
	{
		FRHIPainter RHIPainter(RHIGraphics);
		RHIPainter.BeginPaint(Bitmap);
		Paint(RHIPainter);
		RHIPainter.EndPaint();
	}
}
