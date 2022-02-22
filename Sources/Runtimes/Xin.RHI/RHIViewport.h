#pragma once

#include "RHITypes.h"
#include "RHIResource.h"

namespace Xin::RHI
{
	class IRHIFence;
	class IRHICommandList;

	class RHI_API IRHIViewport : public IRHIObject
	{
	public:
		IRHIViewport() = default;

	public:
		virtual uint32 BeginFrame() = 0;
		virtual void EndFrame(uint32 SyncInterval = 0) = 0;

		virtual uint32 GetNumBackBuffers() const = 0;
		virtual uint32 GetCurrentBufferIndex() const = 0;
		virtual IRHIResource * GetBackBuffer(uint32 BufferIndex) const = 0;

		virtual SizeU GetSize() const = 0;
		virtual void Resize(SizeU Size) = 0;
	};
	using IRHIViewportRef = TReferPtr<IRHIViewport>;
}
