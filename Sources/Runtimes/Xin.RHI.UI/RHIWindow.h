#pragma once

#include "RHI.UI.Types.h"

namespace Xin::RHI::UI
{
	class RHIUI_API FRHIWindow : public UWindow
	{
	public:
		FRHIWindow(IRHIDevice & Device, IGraphics & OfflineGraphic);
		~FRHIWindow();

	public:
		void OnConstruct() override;
		IGraphics & GetGraphics() const final { return ConstCast(RHIGraphics); }
		void OnPaintWindow() final { }

	public:
		void RunLoop();

	public:
		void Render();
		void UpdateViewport();

	private:
		void PaintWindows(IBitmap * Bitmap);

	public:
		IRHIDevice & Device;

		FEngine Engine;
		FCommandExecutor Executor;
		FRHIGraphics RHIGraphics;

	public:
		static constexpr uint32 BackBufferCount = 3;
		IRHIViewportRef Viewport;
		TList<IRHIResourceRef> BackBuffers;
		TList<IBitmapRef> BackBitmaps;
	};
}
