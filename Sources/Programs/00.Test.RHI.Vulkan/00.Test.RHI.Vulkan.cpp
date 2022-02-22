#include "pch.h"
#include "00.Test.RHI.Vulkan.h"

#include "Programs/00.Test.RHI/00.Test.RHI.h"

int main(int argc, const char * args[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//Xin::Windows::FCOMEnvironment COMEnvironment;

	Xin::RHI::IRHIDeviceRef Device = Xin::RHI::Vulkan::CreateDevice();

	TestRHI_Texture(Device);
	//TestRHI_Mesh(Device);

	return 0;
}
