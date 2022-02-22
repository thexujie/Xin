#include "PCH.h"

#include "Core/Dispatcher.h"
#include "Core/DispatcherContext.h"
#include "Threading/Mutex.h"
#include "Threading/Lock.h"
#include "Core/DispatcherTimer.h"

#ifdef XIN_WINDOWS
#include "Platform/Windows/WindowsPrivate.h"
#endif

namespace Xin
{
	static LRESULT CALLBACK DispatcherWndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);

	class FWindowsDispatcherContext : public IDispatcherContext
	{
	public:
		static constexpr wchar_t Windows_ClassName_Dispatcher[] = L"Dispatcher.{2A84CC0A-7D4F-42AB-9311-DEA336A1F289}";
		static constexpr wchar_t Message_Dispatcher[] = L"Message.Dispatcher.{C12F3FD9-A71B-427F-8958-7D0DF04BB518}";

	public:
		FWindowsDispatcherContext()
		{
			HINSTANCE hInstance = GetModuleHandleW(NULL);
			WNDCLASSEXW WindowClass = { sizeof(WNDCLASSEXW) };
			WindowClass.style = CS_HREDRAW | CS_VREDRAW;
			WindowClass.lpfnWndProc = DispatcherWndProc;
			WindowClass.cbClsExtra = 0;
			WindowClass.cbWndExtra = sizeof(voidp);
			WindowClass.hInstance = hInstance;
			WindowClass.hIcon = NULL;
			WindowClass.hCursor = NULL;
			WindowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
			WindowClass.lpszMenuName = NULL;
			WindowClass.lpszClassName = Windows_ClassName_Dispatcher;
			WindowClass.hIconSm = NULL;
			RegisterClassExW(&WindowClass);

			hWnd = CreateWindowExW(0, Windows_ClassName_Dispatcher, L"Dispatcher Message Window", WS_OVERLAPPEDWINDOW, 0, 0, CW_DEFAULT, CW_DEFAULT, HWND_MESSAGE, NULL, hInstance, NULL);
			::SetWindowLongPtrW(hWnd, 0, (LONG_PTR)(voidp)this);

			DispatchInvokeMessage = ::RegisterWindowMessageW(Message_Dispatcher);
		}

		LRESULT WndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
		{
			if (Message == DispatchInvokeMessage)
				FDispatcher::Current().Dispatch();
			return DefWindowProcW(hWnd, Message, wParam, lParam);
		}

		void DispatchInvoke() override
		{
			::PostMessageW(hWnd, DispatchInvokeMessage, 0, 0);
		}

		uint32 StartTimer(FDispatcherTimer * DispatcherTimer) override
		{
			TUniqueLock Lock { Mutex };
			Timers.Emplace(DispatcherTimer);
			::SetTimer(hWnd, (UINT_PTR)DispatcherTimer, DispatcherTimer->Interval, TimerCallBack);
			return FDateTime::SteadyMilliseconds();
		}

		uint32 StopTimer(FDispatcherTimer * DispatcherTimer) override
		{
			TUniqueLock Lock { Mutex };
#if XIN_DEBUG
			DeadTimers.Add(DispatcherTimer);
#endif
			AssertExpr(Timers.Contains(DispatcherTimer));
			if (auto Iter = Timers.Find(DispatcherTimer); Iter != Timers.End())
				Timers.Erase(Iter);
			::KillTimer(hWnd, (UINT_PTR)DispatcherTimer);
			uint32 Time = FDateTime::SteadyMilliseconds();
			return Time;
		}

		void OnTimerCallback(FDispatcherTimer * DispatcherTimer)
		{
#if XIN_DEBUG
			AssertExpr(DeadTimers.Contains(DispatcherTimer) || Timers.Contains(DispatcherTimer));
#else
			AssertExpr(Timers.Contains(DispatcherTimer));
#endif
			if (auto Iter = Timers.Find(DispatcherTimer); Iter != Timers.End())
				DispatcherTimer->OnTimer();
		}

	public:
		static void CALLBACK TimerCallBack(HWND hWnd, UINT Message /*WM_TIMER*/, UINT_PTR TimerId, DWORD SystemTime)
		{
			StaticCast<FWindowsDispatcherContext>(IDispatcherContext::Instance()).OnTimerCallback((FDispatcherTimer *)TimerId);
		}

		static void CALLBACK TimerCallBackSingle(HWND, UINT, UINT_PTR timerId, DWORD);
		

	private:
		HWND hWnd;
		FMutex Mutex;

		UINT DispatchInvokeMessage = 0;
		TList<FDispatcherTimerRef> Timers;
#if XIN_DEBUG
		TList<FDispatcherTimer *> DeadTimers;
#endif
	};

	IDispatcherContext & IDispatcherContext::Instance()
	{
		thread_local FWindowsDispatcherContext WindowsDispatcherContext;
		return WindowsDispatcherContext;
	}

	static LRESULT CALLBACK DispatcherWndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
	{
		FWindowsDispatcherContext * WindowsDispatcherContext = (FWindowsDispatcherContext *)::GetWindowLongPtrW(hWnd, 0);
		if (WindowsDispatcherContext)
			return WindowsDispatcherContext->WndProc(hWnd, Message, wParam, lParam);
		else
			return DefWindowProcW(hWnd, Message, wParam, lParam);
	}
}
