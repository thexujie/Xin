#include "PCH.h"
#include "Xin.RHI.D3D12.h"
#include "Xin.RHI.D3D12.Private.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

namespace Xin::RHI::D3D12
{
	IRHIDeviceRef CreateDevice(ECreateDeviceFlag CreateDeviceFlags)
	{
		return CreateD3D12RHIDevice(CreateDeviceFlags);
	}

	FD3D12DeviceRef CreateD3D12RHIDevice(ECreateDeviceFlag CreateDeviceFlags)
	{
		FModule DXGILibrary { u8"dxgi.dll"V };
		if (!DXGILibrary)
		{
			LogError(u8"load dxgi.dll failed, {}"V, ErrorDescription(GetLastError()));
			return nullptr;
		}

		HRESULT Result = S_OK;

		auto pfnCreateDXGIFactory = DXGILibrary.GetProc<decltype(CreateDXGIFactory)>("CreateDXGIFactory");
		if (!pfnCreateDXGIFactory)
		{
			LogError(u8"Can't find CreateDXGIFactory in dxgi.dll, {}"V, ErrorDescription(GetLastError()));
			return nullptr;
		}

		IDXGIFactory3Ref DXGIFactory3;
		Result = pfnCreateDXGIFactory(__uuidof(IDXGIFactory3), DXGIFactory3.GetVV());
		if (FAILED(Result))
		{
			LogError(u8"CreateDXGIFactory<IDXGIFactory3> faild, {}"V, ErrorDescription(GetLastError()));
			return nullptr;
		}

		TList<IRHIAdapterRef> Adapters;
		UINT AdapterIndex = 0;
		TReferPtr<IDXGIAdapter> DXGIAdapter;
		while (DXGIFactory3->EnumAdapters(AdapterIndex++, DXGIAdapter.GetPP()) != DXGI_ERROR_NOT_FOUND)
		{
			//TReferPtr<ID3D12Device> D3D12Device;
			//HRESULT Result = D3D12CreateDevice(DXGIAdapter.Get(), D3D_FEATURE_LEVEL_12_0, __uuidof(ID3D12Device), D3D12Device.GetVV());
			//if (FAILED(Result))
			//{
			//	DXGIAdapter.Reset();
			//	LogWarning(u8"D3D12CreateDevice failed."V);
			//	continue;
			//}

			return new FD3D12Device(DXGIFactory3.Get(), DXGIAdapter.Get(), CreateDeviceFlags);
			//DXGIAdapter.Reset();
		}
		return nullptr;
	}
}
