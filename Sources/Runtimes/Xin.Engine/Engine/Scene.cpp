#include "PCH.h"
#include "Scene.h"

namespace Xin::Engine
{
	FSceneRenderer::FSceneRenderer(FCommandExecutor & CommandExecutor)
		: CommandExecutor(CommandExecutor)
	{
		
	}

	void FSceneRenderer::Update()
	{
		if (PenddingAddedPrimitives.Size)
		{
			for (IPrimitiveRef & RenderPrimitive : PenddingAddedPrimitives)
				RenderPrimitive->InitializeRHI(CommandExecutor);

			Primitives.AddRange(PenddingAddedPrimitives);
			PenddingAddedPrimitives.Clear();
		}

		for (uintx PrimitiveIndex = 0; PrimitiveIndex < Primitives.Size; ++PrimitiveIndex)
		{
			IPrimitive & RenderPrimitive = Primitives[PrimitiveIndex].Ref();
			RenderPrimitive.UpdateRHI();
		}
	}

	void FSceneRenderer::AddPrimitive(IPrimitiveRef RenderPrimitive)
	{
		PenddingAddedPrimitives.Add(RenderPrimitive);
	}
}
