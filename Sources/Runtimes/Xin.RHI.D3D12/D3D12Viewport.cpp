#include "PCH.h"
#include "D3D12Viewport.h"
#include "D3D12CommandQueue.h"
#include "D3D12Device.h"
#include "D3D12Resource.h"

namespace Xin::RHI::D3D12
{
	FD3D12Viewport::FD3D12Viewport(FD3D12CommandQueue & CommandQueue, voidp WindowHandle, SizeU Size, uint32 NumBackBuffers,
		EFormat Format, EPresentMode PresentMode, FMSAA MSAA)
		: CommandQueue(CommandQueue), WindowHandle(WindowHandle), SwapchainSize(Size), NumBackBuffers(NumBackBuffers)
		, SwapchainFormat(Format), PresentMode(PresentMode), MSAA(MSAA)
	{
		//AllowTearing = true;
		EnableFrameLatency = true;
		Initialize();
	}

	void FD3D12Viewport::SetDebugName(FStringV Name)
	{
		IRHIViewport::SetDebugName(Name);
		for (uint32 BackBufferIndex = 0; BackBufferIndex < BackBuffers.Size; ++BackBufferIndex)
			SetD3D12ObjectName(BackBuffers[BackBufferIndex]->d3d12Resource, Xin::Format(u8"{}.BackBuffer[{}]"V, Name, BackBufferIndex));
	}

	FD3D12Viewport::~FD3D12Viewport()
	{
		if (FrameLatencyWaitableObject)
		{
			CloseHandle(FrameLatencyWaitableObject);
			FrameLatencyWaitableObject = NULL;
		}
	}

	void FD3D12Viewport::Initialize()
	{
		SwapChainFlag = 0;
		if (EnableFrameLatency)
			SwapChainFlag = DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT;
		if (AllowTearing)
			SwapChainFlag = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
		//SwapChainFlag |= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		DXGI_SWAP_CHAIN_DESC1 SwapChainDesc = {};
		SwapChainDesc.BufferCount = NumBackBuffers;
		SwapChainDesc.Width = SwapchainSize.Width;
		SwapChainDesc.Height = SwapchainSize.Height;
		SwapChainDesc.Format = FromFormat(SwapchainFormat);
		SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_BACK_BUFFER;
		SwapChainDesc.SwapEffect = FromSwapEffect(PresentMode);
		SwapChainDesc.SampleDesc.Count = MSAA.Level;
		SwapChainDesc.SampleDesc.Quality = MSAA.Quality;
		SwapChainDesc.Scaling = DXGI_SCALING_NONE;
		SwapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
		SwapChainDesc.Flags = SwapChainFlag;

		// https://learn.microsoft.com/en-us/windows/win32/direct3ddxgi/hardware-support-for-direct3d-12-1-formats
		HRESULT Result = CommandQueue.D3D12Devcie.dxgiFactory3->CreateSwapChainForHwnd(
			CommandQueue.d3d12CommandQueue.Get(),
			HWND(WindowHandle),
			&SwapChainDesc,
			nullptr,
			nullptr,
			DXGISwapChain1.GetPP()
		);
		if (FAILED(Result))
		{
			LogError(u8"CreateSwapChainForHwnd failed, {}"V, ErrorDescription(Result));
			return;
		}

		DXGISwapChain = DXGISwapChain1.QueryInterface<IDXGISwapChain3>();
		DXGISwapChain3 = DXGISwapChain1.QueryInterface<IDXGISwapChain3>();
		//Result = DXGISwapChain3->SetMaximumFrameLatency(10);

		Result = CommandQueue.D3D12Devcie.dxgiFactory3->MakeWindowAssociation((HWND)WindowHandle, DXGI_MWA_NO_ALT_ENTER);
		if (FAILED(Result))
		{
			LogWarning(u8"MakeWindowAssociation(DXGI_MWA_NO_ALT_ENTER) failed, {}"V, ErrorDescription(Result & 0xFFFF));
		}

		if (SwapChainDesc.Flags & DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT)
			FrameLatencyWaitableObject = DXGISwapChain3->GetFrameLatencyWaitableObject();

		InitializeBackBuffers();
	}

	uint32 FD3D12Viewport::BeginFrame()
	{
		SizeU ClientSize = Windows::GetClientSize(HWND(WindowHandle));
		if (ClientSize != SwapchainSize)
		{
			SwapchainSize = ClientSize;
			LogInfo(u8"ResizeBuffers to {}x{}"V, ClientSize.Width, ClientSize.Height);

			CommandQueue.QueueFence.WaitFence(NullIndex);
			CommandQueue.UpdateContext();

			BackBuffers.Clear();
			DXGISwapChain3->ResizeBuffers(NumBackBuffers, ClientSize.Width, ClientSize.Height, FromFormat(SwapchainFormat), SwapChainFlag);
			InitializeBackBuffers();
			return DXGISwapChain3->GetCurrentBackBufferIndex();;
		}
		else
		{
			uint32 BackBufferIndex = DXGISwapChain3->GetCurrentBackBufferIndex();
			CommandQueue.QueueFence.WaitFence(FrameFenceValues[BackBufferIndex]);
			return BackBufferIndex;
		}
	}

	void FD3D12Viewport::EndFrame(uint32 SyncInterval)
	{
		if (!DXGISwapChain3)
			return;

		uint32 BackBufferIndex = DXGISwapChain3->GetCurrentBackBufferIndex();
		if (FrameLatencyWaitableObject)
		{
			DWORD WaitResult = ::WaitForSingleObject(FrameLatencyWaitableObject, 0);
			if (WaitResult == WAIT_OBJECT_0)
				DXGISwapChain->Present(SyncInterval, DXGI_PRESENT_DO_NOT_WAIT);
		}
		else
		{
			uint32 PresentFlags = 0;
			if (AllowTearing && !SyncInterval)
				PresentFlags |= DXGI_PRESENT_ALLOW_TEARING;
			DXGISwapChain->Present(SyncInterval, PresentFlags);
		}

		FrameFenceValues[BackBufferIndex] = ++CommandQueue.QueueFence.SignalValue;
		CommandQueue.d3d12CommandQueue->Signal(CommandQueue.QueueFence.d3d12Fence, FrameFenceValues[BackBufferIndex]);
	}

	void FD3D12Viewport::Resize(SizeU NewSize)
	{
		if (DXGISwapChain3)
		{
			SwapchainSize = NewSize;
			BackBuffers.Clear();

			auto DXGISwapChain1 = DXGISwapChain3.QueryInterface<IDXGISwapChain1>();
			DXGISwapChain1->ResizeBuffers(NumBackBuffers, NewSize.Width, NewSize.Height, FromFormat(SwapchainFormat), SwapChainFlag);
			InitializeBackBuffers();
		}
	}

	void FD3D12Viewport::InitializeBackBuffers()
	{
		FrameFenceValues.ResizeTo(NumBackBuffers, 0);
		BackBuffers.ResizeTo(NumBackBuffers);
		for (uint32 BufferIndex = 0; BufferIndex < NumBackBuffers; BufferIndex++)
		{
			TReferPtr<ID3D12Resource> D3D12Resource;
			HRESULT Result = DXGISwapChain3->GetBuffer(BufferIndex, __uuidof(ID3D12Resource), D3D12Resource.GetVV());
			if (FAILED(Result))
			{
				LogError(u8"GetBuffer failed, {}"V, ErrorDescription(Result & 0xFFFF));
				return;
			}

			SetD3D12ObjectName(D3D12Resource.Get(), Xin::Format(L"BackBuffer [{0}]"V, BufferIndex));
			BackBuffers[BufferIndex] = new FD3D12Resource(CommandQueue.D3D12Devcie, D3D12Resource, EResourceAccess::Present);
		}
	}
}

