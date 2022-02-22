#include "PCH.h"
#include "WindowsPrivate.h"
#include <combaseapi.h>

namespace Xin::Windows
{
	static intx COMEnvironmentInitialized = 0;

	FCOMInitializer::FCOMInitializer()
	{
		intx ModifiedValue = Atomics::IncFetch(COMEnvironmentInitialized);
		if (ModifiedValue == 1)
			::CoInitializeEx(nullptr, 0);
	}

	FCOMInitializer::~FCOMInitializer()
	{
		AssertExpr(COMEnvironmentInitialized > 0);

		intx ModifiedValue = Atomics::DecFetch(COMEnvironmentInitialized);
		if (ModifiedValue == 0)
			::CoUninitialize();
	}

	HINSTANCE Instance()
	{
		return GetModuleHandleW(NULL);
	}

	FStringV ErrorDescription(int32 ErrorCode)
	{
		if (High(ErrorCode) == FACILITY_WIN32)
			return ErrorDescription(Low(ErrorCode));

		TString<wchar_t> & Buffer = GetThreadLocalStringBuffer<wchar_t>();
		int Size = ::FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, NULL, ErrorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), Buffer.GetData(), (DWORD)Buffer.GetCapacityWithoutNull(), NULL);
		while (!Size)
		{
			if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
			{
				uintx Capacity = Buffer.GetCapacity() > 0 ? Buffer.GetCapacity() * 2 : 64;
				Buffer.Resize(Capacity - 1, Capacity);
				Size = ::FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, NULL, ErrorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), Buffer.GetData(), (DWORD)Buffer.GetCapacityWithoutNull(), NULL);
				continue;
			}
			return Format(u8"[0x{:X}] Unkown error"V, BitCast<uint32>(ErrorCode));
		}

		Buffer.Resize(Size - 2);
		Buffer.Insert(0, Format(L"[0x{:X}] "V, BitCast<uint32>(ErrorCode)));
		return FEncoding::ToUtf8(Buffer);
	}

	HMODULE GetAddressModule(const void * Address)
	{
		HMODULE hModule = NULL;
		if (GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCWSTR)Address, &hModule))
			return hModule;
		else
			return NULL;
	}

	FStringV GetModuleFilePath(HMODULE hModule)
	{
		FWString & Buffer = GetThreadLocalStringBuffer<wchar_t>();
		int ModuleFileNameLength = GetModuleFileNameW(hModule, Buffer.Data, (DWORD)Buffer.GetCapacityWithoutNull());
		while (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			Buffer.Resize(0, Max<uintx>(Buffer.Capacity, 128) * 2);
			ModuleFileNameLength = GetModuleFileNameW(hModule, Buffer.Data, (DWORD)Buffer.GetCapacityWithoutNull());
			Buffer.Resize(ModuleFileNameLength);
		}
		return FEncoding::ToUtf8(Buffer.View(0, ModuleFileNameLength));
	}

	SizeU GetWindowSize(HWND WindowHandle)
	{
		if (!WindowHandle)
			return {};

		RECT WindowRect = {};
		::GetWindowRect(WindowHandle, &WindowRect);
		SizeU WindowSize = { uint32(WindowRect.right - WindowRect.left), uint32(WindowRect.bottom - WindowRect.top) };
		return WindowSize;
	}

	RectI GetWindowRect(HWND WindowHandle)
	{
		if (!WindowHandle)
			return {};

		RECT WindowRect = {};
		::GetWindowRect(WindowHandle, &WindowRect);
		RectI ClientRect = { WindowRect.left, WindowRect.top, WindowRect.right - WindowRect.left, WindowRect.bottom - WindowRect.top };
		return ClientRect;
	}

	SizeU GetClientSize(HWND WindowHandle)
	{
		if (!WindowHandle)
			return {};

		RECT ClientRect = {};
		::GetClientRect(HWND(WindowHandle), &ClientRect);
		SizeU ClientSize = { uint32(ClientRect.right - ClientRect.left), uint32(ClientRect.bottom - ClientRect.top) };
		return ClientSize;
	}

	RectI GetClientRect(HWND WindowHandle)
	{
		if (!WindowHandle)
			return {};

		POINT ZeroPoint { 0, 0 };
		::ClientToScreen(WindowHandle, &ZeroPoint);

		RECT ClientRect = {};
		::GetClientRect(HWND(WindowHandle), &ClientRect);
		return { 0, 0, ClientRect.right - ClientRect.left, ClientRect.bottom - ClientRect.top };
		RECT WindowRect = {};
		::GetWindowRect(WindowHandle, &WindowRect);

		POINT LoalOffset { ZeroPoint.x - WindowRect.left, ZeroPoint.y - WindowRect.top };

		return { LoalOffset.x + ClientRect.left, LoalOffset.y + ClientRect.top, ClientRect.right - ClientRect.left, ClientRect.bottom - ClientRect.top };
	}

	static const EKeyCode Win32VirtualKey2KeyCodeMap[0xff] =
	{
		//! 不可用的按键码。
		/*0x00*/EKeyCode::None,
		/*0x01*/EKeyCode::None, // vk_lbutton
		/*0x02*/EKeyCode::None, // vk_rbutton
		/*0x03*/EKeyCode::None, // vk_cancel
		/*0x04*/EKeyCode::None, // vm_mbutton
		/*0x05*/EKeyCode::None, // vk_xbutton1
		/*0x06*/EKeyCode::None, // vk_xbutton2
		/*0x07*/EKeyCode::None,
		/*0x08*/EKeyCode::BackSpace, // vk_back
		/*0x09*/EKeyCode::Tab, // vk_tab
		/*0x0a*/EKeyCode::None,
		/*0x0b*/EKeyCode::None,
		/*0x0c*/EKeyCode::Clear,
		/*0x0d*/EKeyCode::Enter,
		/*0x0e*/EKeyCode::None,
		/*0x0f*/EKeyCode::None,

		/*0x10*/EKeyCode::Shift,
		/*0x11*/EKeyCode::Ctrl,
		/*0x12*/EKeyCode::Alt,
		/*0x13*/EKeyCode::PauseBreak,
		/*0x14*/EKeyCode::Caps,
		/*0x15*/EKeyCode::ImeKana,
		/*0x16*/EKeyCode::ImeHangul,
		/*0x17*/EKeyCode::ImeJunja,
		/*0x18*/EKeyCode::ImeFinal,
		/*0x19*/EKeyCode::ImeKanji,
		/*0x1a*/EKeyCode::None,
		/*0x1b*/EKeyCode::Escape,
		/*0x1c*/EKeyCode::ImeConvert,
		/*0x1d*/EKeyCode::ImeNonconvert,
		/*0x1e*/EKeyCode::ImeAccept,
		/*0x1f*/EKeyCode::ImeModechange,

		/*0x20*/EKeyCode::Space,
		/*0x21*/EKeyCode::Pageup,
		/*0x22*/EKeyCode::Pagedown,
		/*0x23*/EKeyCode::End,
		/*0x24*/EKeyCode::Home,
		/*0x25*/EKeyCode::Left,
		/*0x26*/EKeyCode::Up,
		/*0x27*/EKeyCode::Right,
		/*0x28*/EKeyCode::Down,
		/*0x29*/EKeyCode::Select,
		/*0x2a*/EKeyCode::Print,
		/*0x2b*/EKeyCode::Excute,
		/*0x2c*/EKeyCode::PrintScreen,
		/*0x2d*/EKeyCode::Insert,
		/*0x2e*/EKeyCode::Del,
		/*0x2f*/EKeyCode::Help,

		/*0x30*/EKeyCode::Num0,
		/*0x31*/EKeyCode::Num1,
		/*0x32*/EKeyCode::Num2,
		/*0x33*/EKeyCode::Num3,
		/*0x34*/EKeyCode::Num4,
		/*0x35*/EKeyCode::Num5,
		/*0x36*/EKeyCode::Num6,
		/*0x37*/EKeyCode::Num7,
		/*0x38*/EKeyCode::Num8,
		/*0x39*/EKeyCode::Num9,
		/*0x3a*/EKeyCode::None,
		/*0x3b*/EKeyCode::None,
		/*0x3c*/EKeyCode::None,
		/*0x3d*/EKeyCode::None,
		/*0x3e*/EKeyCode::None,
		/*0x3f*/EKeyCode::None,

		/*0x40*/EKeyCode::None,
		/*0x41*/EKeyCode::A,
		/*0x42*/EKeyCode::B,
		/*0x43*/EKeyCode::C,
		/*0x44*/EKeyCode::D,
		/*0x45*/EKeyCode::E,
		/*0x46*/EKeyCode::F,
		/*0x47*/EKeyCode::G,
		/*0x48*/EKeyCode::H,
		/*0x49*/EKeyCode::I,
		/*0x4a*/EKeyCode::J,
		/*0x4b*/EKeyCode::K,
		/*0x4c*/EKeyCode::L,
		/*0x4d*/EKeyCode::M,
		/*0x4e*/EKeyCode::N,
		/*0x4f*/EKeyCode::O,
		/*0x50*/EKeyCode::P,
		/*0x51*/EKeyCode::Q,
		/*0x52*/EKeyCode::R,
		/*0x53*/EKeyCode::S,
		/*0x54*/EKeyCode::T,
		/*0x55*/EKeyCode::U,
		/*0x56*/EKeyCode::V,
		/*0x57*/EKeyCode::W,
		/*0x58*/EKeyCode::X,
		/*0x59*/EKeyCode::Y,
		/*0x5a*/EKeyCode::Z,
		/*0x5b*/EKeyCode::WinL,
		/*0x5c*/EKeyCode::WinR,
		/*0x5d*/EKeyCode::Apps,
		/*0x5e*/EKeyCode::None,
		/*0x5f*/EKeyCode::Sleep,

		/*0x60*/EKeyCode::Numpad0,
		/*0x61*/EKeyCode::Numpad1,
		/*0x62*/EKeyCode::Numpad2,
		/*0x63*/EKeyCode::Numpad3,
		/*0x64*/EKeyCode::Numpad4,
		/*0x65*/EKeyCode::Numpad5,
		/*0x66*/EKeyCode::Numpad6,
		/*0x67*/EKeyCode::Numpad7,
		/*0x68*/EKeyCode::Numpad8,
		/*0x69*/EKeyCode::Numpad9,
		/*0x6a*/EKeyCode::NumpadMul,
		/*0x6b*/EKeyCode::NumpadAdd,
		/*0x6c*/EKeyCode::NumpadEnter,
		/*0x6d*/EKeyCode::NumpadSub,
		/*0x6e*/EKeyCode::NumpadDecimal,
		/*0x6f*/EKeyCode::NumpadDiv,

		/*0x70*/EKeyCode::F1,
		/*0x71*/EKeyCode::F2,
		/*0x72*/EKeyCode::F3,
		/*0x73*/EKeyCode::F4,
		/*0x74*/EKeyCode::F5,
		/*0x75*/EKeyCode::F6,
		/*0x76*/EKeyCode::F7,
		/*0x77*/EKeyCode::F8,
		/*0x78*/EKeyCode::F9,
		/*0x79*/EKeyCode::F10,
		/*0x7a*/EKeyCode::F11,
		/*0x7b*/EKeyCode::F12,
		/*0x7c*/EKeyCode::F13,
		/*0x7d*/EKeyCode::F14,
		/*0x7e*/EKeyCode::F15,
		/*0x7f*/EKeyCode::F16,
		/*0x80*/EKeyCode::F17,
		/*0x81*/EKeyCode::F18,
		/*0x82*/EKeyCode::F19,
		/*0x83*/EKeyCode::F20,
		/*0x84*/EKeyCode::F21,
		/*0x85*/EKeyCode::F22,
		/*0x86*/EKeyCode::F23,
		/*0x87*/EKeyCode::F24,
		/*0x88*/EKeyCode::None,
		/*0x89*/EKeyCode::None,
		/*0x8a*/EKeyCode::None,
		/*0x8b*/EKeyCode::None,
		/*0x8c*/EKeyCode::None,
		/*0x8d*/EKeyCode::None,
		/*0x8e*/EKeyCode::None,
		/*0x8f*/EKeyCode::None,

		/*0x90*/EKeyCode::NumpadLock,
		/*0x91*/EKeyCode::ScrollLock,
		/*0x92*/EKeyCode::NumpadEqual,
		/*0x93*/EKeyCode::None,
		/*0x94*/EKeyCode::None,
		/*0x95*/EKeyCode::None,
		/*0x96*/EKeyCode::None,
		/*0x97*/EKeyCode::None,
		/*0x98*/EKeyCode::None,
		/*0x99*/EKeyCode::None,
		/*0x9a*/EKeyCode::None,
		/*0x9b*/EKeyCode::None,
		/*0x9c*/EKeyCode::None,
		/*0x9d*/EKeyCode::None,
		/*0x9e*/EKeyCode::None,
		/*0x9f*/EKeyCode::None,

		/*0xa0*/EKeyCode::ShiftL,
		/*0xa1*/EKeyCode::ShiftR,
		/*0xa2*/EKeyCode::CtrlL,
		/*0xa3*/EKeyCode::CtrlR,
		/*0xa4*/EKeyCode::AltL,
		/*0xa5*/EKeyCode::AltR,
		/*0xa6*/EKeyCode::BrowserBack,
		/*0xa7*/EKeyCode::BrowserForward,
		/*0xa8*/EKeyCode::BrowserForward,
		/*0xa9*/EKeyCode::BrowserStop,
		/*0xaa*/EKeyCode::BrowserSearch,
		/*0xab*/EKeyCode::BrowserFavorites,
		/*0xac*/EKeyCode::BrowserHome,
		/*0xad*/EKeyCode::VolumeMute,
		/*0xae*/EKeyCode::VolumeDown,
		/*0xaf*/EKeyCode::VolumeUp,

		/*0xb0*/EKeyCode::MediaNext,
		/*0xb1*/EKeyCode::MediaPrev,
		/*0xb2*/EKeyCode::MediaStop,
		/*0xb3*/EKeyCode::MediaPlaypause,
		/*0xb4*/EKeyCode::LaunchMail,
		/*0xb5*/EKeyCode::LaunchMediaSelect,
		/*0xb6*/EKeyCode::LaunchApp1,
		/*0xb7*/EKeyCode::LaunchApp2,
		/*0xb8*/EKeyCode::None,
		/*0xb9*/EKeyCode::None,
		/*0xba*/EKeyCode::SemiColon,
		/*0xbb*/EKeyCode::Equal,
		/*0xbc*/EKeyCode::Comma,
		/*0xbd*/EKeyCode::Sub,
		/*0xbe*/EKeyCode::Period,
		/*0xbf*/EKeyCode::Slash,

		/*0xc0*/EKeyCode::Grave,
		/*0xc1*/EKeyCode::Reserved,
		/*0xc2*/EKeyCode::Reserved,
		/*0xc3*/EKeyCode::Reserved,
		/*0xc4*/EKeyCode::Reserved,
		/*0xc5*/EKeyCode::Reserved,
		/*0xc6*/EKeyCode::Reserved,
		/*0xc7*/EKeyCode::Reserved,
		/*0xc8*/EKeyCode::Reserved,
		/*0xc9*/EKeyCode::Reserved,
		/*0xca*/EKeyCode::Reserved,
		/*0xcb*/EKeyCode::Reserved,
		/*0xcc*/EKeyCode::Reserved,
		/*0xcd*/EKeyCode::Reserved,
		/*0xce*/EKeyCode::Reserved,
		/*0xcf*/EKeyCode::Reserved,

		/*0xd0*/EKeyCode::Reserved,
		/*0xd1*/EKeyCode::Reserved,
		/*0xd2*/EKeyCode::Reserved,
		/*0xd3*/EKeyCode::Reserved,
		/*0xd4*/EKeyCode::Reserved,
		/*0xd5*/EKeyCode::Reserved,
		/*0xd6*/EKeyCode::Reserved,
		/*0xd7*/EKeyCode::Reserved,
		/*0xd8*/EKeyCode::UnAssigned,
		/*0xd9*/EKeyCode::UnAssigned,
		/*0xda*/EKeyCode::UnAssigned,
		/*0xdb*/EKeyCode::BracketL,
		/*0xdc*/EKeyCode::BackSlash,
		/*0xdd*/EKeyCode::BracketR,
		/*0xde*/EKeyCode::Apostrophe,
		/*0xdf*/EKeyCode::None,

		/*0xe0*/EKeyCode::Reserved,
		/*0xe1*/EKeyCode::NotMatch,
		/*0xe2*/EKeyCode::Comma,
		/*0xe3*/EKeyCode::NotMatch,
		/*0xe4*/EKeyCode::NotMatch,
		/*0xe5*/EKeyCode::NotMatch,
		/*0xe6*/EKeyCode::NotMatch,
		/*0xe7*/EKeyCode::NotMatch,
		/*0xe8*/EKeyCode::NotMatch,
		/*0xe9*/EKeyCode::NotMatch,
		/*0xea*/EKeyCode::NotMatch,
		/*0xeb*/EKeyCode::NotMatch,
		/*0xec*/EKeyCode::NotMatch,
		/*0xed*/EKeyCode::NotMatch,
		/*0xee*/EKeyCode::NotMatch,
		/*0xef*/EKeyCode::NotMatch,

		/*0xf0*/EKeyCode::NotMatch,
		/*0xf1*/EKeyCode::NotMatch,
		/*0xf2*/EKeyCode::NotMatch,
		/*0xf3*/EKeyCode::NotMatch,
		/*0xf4*/EKeyCode::NotMatch,
		/*0xf5*/EKeyCode::NotMatch,
		/*0xf6*/EKeyCode::NotMatch,
		/*0xf7*/EKeyCode::NotMatch,
		/*0xf8*/EKeyCode::NotMatch,
		/*0xf9*/EKeyCode::NotMatch,
		/*0xfa*/EKeyCode::NotMatch,
		/*0xfb*/EKeyCode::NotMatch,
		/*0xfc*/EKeyCode::NotMatch,
		/*0xfd*/EKeyCode::NotMatch,
		/*0xfe*/EKeyCode::NotMatch,
		/*0xff*/
	};

	EKeyCode VirtualKeyToKeyCode(uint32 VirtualKey)
	{
		if (VirtualKey >= 0xff)
			return EKeyCode::None;
		return Win32VirtualKey2KeyCodeMap[VirtualKey];
	}

	TView<byte> LoadResource(uint32 ResourceId, EWin32Resource ResourceType)
	{
		// It is not necessary to unlock resources because the system automatically deletes them when the process that created them terminates.
		static constexpr chara ResourceTypeNames[uint32(EWin32Resource::Count)][16] = { "FILE" };

		HRSRC hResource = FindResourceA(NULL, (const chara *)(uintmax_t)ResourceId, ResourceTypeNames[uint32(ResourceType)]);
		if (hResource)
		{
			HGLOBAL hResourceData = LoadResource(NULL, hResource);
			if (hResourceData)
			{
				byte * Bytes = (byte *)LockResource(hResourceData);
				uint32 NumBytes = SizeofResource(NULL, hResource);
				return { Bytes, NumBytes };
			}
		}
		return {};
	}

	FWindowsVersion Version()
	{
		static FWindowsVersion WindowsVersion = {};
		if (WindowsVersion.Major == 0)
		{
			typedef LONG (WINAPI * RtlGetVersionFuncT)(RTL_OSVERSIONINFOEXW *);
			typedef void (WINAPI * NtVersionNumbersFuncT)(DWORD *, DWORD *, DWORD *);

			RTL_OSVERSIONINFOEXW RtlVersion = { sizeof(RTL_OSVERSIONINFOEXW) };
			RtlGetVersionFuncT pfnRtlGetVersion = (RtlGetVersionFuncT)(::GetProcAddress(::GetModuleHandleW(L"ntdll.dll"), "RtlGetVersion"));
			if (pfnRtlGetVersion)
			{
				pfnRtlGetVersion(&RtlVersion);
			}

			if (RtlVersion.dwMajorVersion > 5)
			{
				WindowsVersion.Major = RtlVersion.dwMajorVersion;
				WindowsVersion.Minor = RtlVersion.dwMinorVersion;
				WindowsVersion.Pack = RtlVersion.wServicePackMajor;
				WindowsVersion.Build = RtlVersion.dwBuildNumber;
			}
			else
			{
				NtVersionNumbersFuncT pfnRtlGetNtVersionNumbers = (NtVersionNumbersFuncT)(::GetProcAddress(::GetModuleHandleW(L"ntdll.dll"), "RtlGetNtVersionNumbers"));
				if (pfnRtlGetNtVersionNumbers)
				{
					DWORD dwMajor = 0;
					DWORD dwMinor = 0;
					DWORD dwBuild = 0;
					pfnRtlGetNtVersionNumbers(&dwMajor, &dwMinor, &dwBuild);
					dwBuild &= 0xffff;
					WindowsVersion.Major = dwMajor;
					WindowsVersion.Minor = dwMinor;
					WindowsVersion.Pack = 0;
					WindowsVersion.Build = dwBuild;
				}
				else
				{
					WindowsVersion.Major = RtlVersion.dwMajorVersion;
					WindowsVersion.Minor = RtlVersion.dwMinorVersion;
					WindowsVersion.Pack = RtlVersion.dwMajorVersion;
					WindowsVersion.Build = RtlVersion.dwBuildNumber;
				}
			}
		}

		return WindowsVersion;
	}
}
