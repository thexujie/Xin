#include "PCH.h"
#include "MeshPrimitive.h"

namespace Xin::Engine
{
	FMeshPrimitive::FMeshPrimitive(IRHIDevice & Device) : IPrimitive(Device)
	{
	}

	void FMeshPrimitive::InitializeRHI(FCommandExecutor & CommandExecutor)
	{
	}

	void FMeshPrimitive::UpdateRHI()
	{
	}
}
