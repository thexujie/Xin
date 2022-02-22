#pragma once

#include "D3D12Types.h"
#include "D3D12Resource.h"

namespace Xin::RHI::D3D12
{
	class FD3D12Device;
	class FD3D12CommandQueue;

	class RHID3D12_API FD3D12Viewport : public IRHIViewport
	{
	public:
		FD3D12Viewport(FD3D12CommandQueue & CommandQueue, voidp WindowHandle, SizeU Size, uint32 NumBackBuffers,
			EFormat Format = EFormat::BGRAUF, EPresentMode PresentMode = EPresentMode::FlipDiscard, FMSAA MSAA = {});
		~FD3D12Viewport();
		void SetDebugName(FStringV Name) override;

		void Initialize();

	public:
		uint32 BeginFrame() override;
		void EndFrame(uint32 SyncInterval) override;

	public:
		uint32 GetNumBackBuffers() const override { return NumBackBuffers; }
		uint32 GetCurrentBufferIndex() const override { return DXGISwapChain3->GetCurrentBackBufferIndex(); }
		IRHIResource * GetBackBuffer(uint32 BufferIndex) const override { return BackBuffers[BufferIndex].Get(); }

		SizeU GetSize() const override { return SwapchainSize; }
		void Resize(SizeU NewSize) override;

	private:
		void InitializeBackBuffers();

	public:
		FD3D12CommandQueue & CommandQueue;

		voidp WindowHandle = nullptr;
		SizeU SwapchainSize;
		EFormat SwapchainFormat = EFormat::None;
		uint32 NumBackBuffers;
		EPresentMode PresentMode;
		FMSAA MSAA;
		uint32 SwapChainFlag = 0;

		IDXGISwapChainRef DXGISwapChain;
		IDXGISwapChain1Ref DXGISwapChain1;
		IDXGISwapChain3Ref DXGISwapChain3;

		TList<uint64> FrameFenceValues;
		TList<FD3D12ResourceRef> BackBuffers;
		HANDLE FrameLatencyWaitableObject = NULL;

		bool AllowTearing = false;
		bool EnableFrameLatency = false;
	};
}
