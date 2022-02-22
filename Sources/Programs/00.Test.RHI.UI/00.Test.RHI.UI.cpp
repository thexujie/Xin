#include "pch.h"
#include "00.Test.RHI.UI.h"

#include "Xin.RHI.D3D12/Xin.RHI.D3D12.h"
#include "Xin.Skia/Xin.Skia.h"
#include "Xin.Direct2D/Xin.Direct2D.h"

#include "Xin.Core/Platform/Windows/WindowsPrivate.h"

void InitialWindow_FileList(UWindow & Window);
void InitialWindow_Atlas(UWindow & Window);

class FMainWindow : public RHI::UI::FRHIWindow
{
public:
	FMainWindow(IRHIDevice & Device, IGraphics & OfflineGraphic) : FRHIWindow(Device, OfflineGraphic)
	{
		
	}

	~FMainWindow()
	{
		
	}
};

int main(int argc, const char * args[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE | _CRTDBG_MODE_WNDW);
	_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
	//_CrtSetBreakAlloc(10874);

	Windows::FCOMInitializer COMInitializer;

	new char[9] { 't', 'e', 's', 't', ' ', 'l', 'e', 'a', 'k' };

	//IGraphicsRef Graphics = D2D::D2DCreateGraphics();
	IGraphicsRef Graphics = Skia::SkiaCreateGraphics();
	IDesktop::Default().SetDefaultGraphics(Graphics);

	Xin::RHI::IRHIDeviceRef Device = Xin::RHI::D3D12::CreateDevice();
	FMainWindow MainWindow { *Device, *Graphics };
	MainWindow.WindowSize = { 1280, 720 };
	MainWindow.GroundingFill = Brushs::White;
	//InitialWindow_FileList(MainWindow);
	InitialWindow_Atlas(MainWindow);

	MainWindow.Closed = [&](const auto & Args)
		{
			FApplication::Instance().Exit(0);
		};

	MainWindow.Show();
	MainWindow.RunLoop();

	IDesktop::Default().SetDefaultGraphics(nullptr);

	return 0;
}
