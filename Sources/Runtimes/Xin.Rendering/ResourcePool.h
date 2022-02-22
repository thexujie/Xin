#pragma once

#include "Xin.Rendering.Types.h"

namespace Xin::Rendering
{
	class RENDERING_API FResourcePool : public IRenderingObject
	{
	public:
		FResourcePool(FCommandExecutor & Executor);

		IRHIResourceRef AcquireResource(const FResourceDesc & ResourceDesc);
		void ForfeitResource(IRHIResource * Resource);

		void Update(float32 DeltaTime);
		void UpdateContext();
		void ClearContext();
		
	private:
		FCommandExecutor & Executor;

		TList<TPair<uint64, IRHIResourceRef>> Resources;
		TList<TPair<uint64, IRHIResourceRef>> FencedResources;
	};
}
