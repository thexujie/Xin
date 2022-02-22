#include "pch.h"
#include "00.Test.RHI.D3D12.h"

#include "Programs/00.Test.RHI/00.Test.RHI.h"
#include "Xin.RHI.D3D12/Xin.RHI.D3D12.h"

int main(int argc, const char * args[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(11113);

	Xin::RHI::IRHIDeviceRef Device = Xin::RHI::D3D12::CreateDevice();

	TestRHI_Texture(Device);
	//TestRHI_Mesh(Device);

	return 0;
}
