#pragma once

#include "../../Xin.Core.Types.h"
#include "Xin.Base/Platform/Windows/WindowsHeader.h"

#include "CoMemPointer.h"
#include "Unknown.h"

namespace Xin::Windows
{
	struct FHandle
	{
		FHandle() = default;
		FHandle(HANDLE Handle) : Handle(Handle) {}
		~FHandle()
		{
			if (Handle != INVALID_HANDLE_VALUE)
			{
				::CloseHandle(Handle);
				Handle = INVALID_HANDLE_VALUE;
			}
		}

		HANDLE * operator &() { return &Handle; }
		operator HANDLE() const { return Handle; }

		HANDLE Handle = INVALID_HANDLE_VALUE;
	};

	class CORE_API FCOMInitializer
	{
	public:
		FCOMInitializer();
		~FCOMInitializer();
	};

	CORE_API HINSTANCE Instance();

	CORE_API FStringV ErrorDescription(int32 ErrorCode);
	CORE_API HMODULE GetAddressModule(const void * Address);
	CORE_API FStringV GetModuleFilePath(HMODULE hModule = NULL);

	enum class EWin32Resource
	{
		// "FILE"
		File = 0,
		Count
	};
	TView<byte> LoadResource(uint32 ResourceId, EWin32Resource ResourceType = EWin32Resource::File);

	struct FWindowsVersion
	{
		uint32 Major;
		uint32 Minor;
		uint32 Pack;
		uint32 Build;

		int32 operator-(const FWindowsVersion & Another) const
		{
			int32 Difference = 0;
			if (Major > Another.Major) Difference += 1000;
			else if (Major < Another.Major) Difference -= 1000;
			else {}
			if (Minor > Another.Minor) Difference += 100;
			else if (Minor < Another.Minor) Difference -= 100;
			else {}
			if (Pack > Another.Pack) Difference += 10;
			else if (Pack < Another.Pack) Difference -= 10;
			else {}
			if (Build > Another.Build) Difference += 1;
			else if (Build < Another.Build) Difference -= 1;
			else {}
			return Difference;
		}

		bool operator > (const FWindowsVersion & Another) const { return operator-(Another) > 0; }
		bool operator < (const FWindowsVersion & Another) const { return operator-(Another) < 0; }
		bool operator >= (const FWindowsVersion & Another) const { return operator-(Another) >= 0; }
		bool operator <= (const FWindowsVersion & Another) const { return operator-(Another) <= 0; }
		bool operator == (const FWindowsVersion & Another) const { return operator-(Another) == 0; }
		bool operator != (const FWindowsVersion & Another) const { return operator-(Another) != 0; }
	};

	constexpr FWindowsVersion WindowsVersion10 = { 10, 0, 0, 0 };
	constexpr FWindowsVersion WindowsVersion8_1 = { 6, 3, 0, 0 };
	constexpr FWindowsVersion WindowsVersion8 = { 6, 2, 0, 0 };
	constexpr FWindowsVersion WindowsVersion7 = { 6, 1, 0, 0 };
	constexpr FWindowsVersion WindowsVersionVista = { 6, 0, 0, 0 };
	constexpr FWindowsVersion WindowsVersionServer2003 = { 5, 2, 0, 0 };
	constexpr FWindowsVersion WindowsVersionXP_SP3 = { 5, 1, 3, 0 };
	constexpr FWindowsVersion WindowsVersionXP_SP2 = { 5, 1, 2, 0 };
	constexpr FWindowsVersion WindowsVersionXP_SP1 = { 5, 1, 1, 0 };
	constexpr FWindowsVersion WindowsVersionXP = { 5, 1, 0, 0 };
	constexpr FWindowsVersion WindowsVersion2000 = { 5, 0, 0, 0 };
	CORE_API FWindowsVersion Version();

	CORE_API SizeU GetWindowSize(HWND WindowHandle);
	CORE_API RectI GetWindowRect(HWND WindowHandle);
	CORE_API SizeU GetClientSize(HWND WindowHandle);
	CORE_API RectI GetClientRect(HWND WindowHandle);
	CORE_API EKeyCode VirtualKeyToKeyCode(uint32 VirtualKey);

	inline std::string HrHexString(HRESULT hResult)
	{
		char s_str[64] = {};
		sprintf_s(s_str, "HRESULT of 0x%08X", static_cast<UINT>(hResult));
		return std::string(s_str);
	}

	class FHrError : public std::runtime_error
	{
	public:
		FHrError(HRESULT hr) : std::runtime_error(HrHexString(hr)), _hr(hr) {}
		HRESULT error() const { return _hr; }
	private:
		const HRESULT _hr;
	};

	inline void ThrowIfFailed(HRESULT hResult)
	{
		if (FAILED(hResult))
		{
			throw FHrError(hResult);
		}
	}
}

#include "../../X2D/Bitmap.h"

namespace Xin::Windows
{
	class CORE_API FPixmapHBITMAP : public IPixmap
	{
	public:
		FPixmapHBITMAP(HBITMAP hBitmap) : hBitmap(hBitmap)
		{
			BITMAP winBitmap;
			GetObjectW(hBitmap, sizeof(winBitmap), &winBitmap);
			AssertExpr(winBitmap.bmWidthBytes >= 0);

			Format = winBitmap.bmBitsPixel == 32 ? EFormat::B8G8R8A8UF : EFormat::B8G8R8X8UF;
			Size = { uint32(winBitmap.bmWidth), uint32(winBitmap.bmHeight) };
			Bytes = { (byte *)winBitmap.bmBits, uintx(winBitmap.bmWidthBytes) * winBitmap.bmHeight };
			Pitch = winBitmap.bmWidthBytes;
		}

		~FPixmapHBITMAP()
		{
			DeleteObject(hBitmap);
		}

	public:
		HBITMAP hBitmap = NULL;
	};
}