#include "PCH.h"
#include "WindowsCore.h"

#include "Core/Application.h"
#include "X2D/Font.h"

#include "WindowsPrivate.h"
#include <combaseapi.h>

namespace Xin
{
	static FFontFormat GetDefaultFontFormat()
	{
		FFontFormat FontFormat;
		HDC hdcScreen = ::GetDC(NULL);
		int32 DpiY = GetDeviceCaps(hdcScreen, LOGPIXELSY);
		ReleaseDC(NULL, hdcScreen);

		NONCLIENTMETRICSW NonClientMetrics;
		NonClientMetrics.cbSize = sizeof(NONCLIENTMETRICSW);
		SystemParametersInfoW(SPI_GETNONCLIENTMETRICS, NonClientMetrics.cbSize, &NonClientMetrics, 0);
		FontFormat.FontFace.FaceName = FEncoding::ToUtf8(FWStringV { NonClientMetrics.lfMessageFont.lfFaceName, NullIndex });
		if (NonClientMetrics.lfMessageFont.lfHeight < 0)
		{
			float32 InchSize = -NonClientMetrics.lfMenuFont.lfHeight / 72.0f;

			// lfHeight = -MulDiv(PointSize, GetDeviceCaps(hDC, LOGPIXELSY), 72);
			float32 PointSize = -NonClientMetrics.lfMenuFont.lfHeight * 72.0f / DpiY;

			// The logical size of the font in DIPs (equals 1/96 inch)
			float32 LogicalSize = PointSize * USER_DEFAULT_SCREEN_DPI / 72.0f;

			//FontFormat.FontSize = LogicalSize;
			FontFormat.FontSize = 11 * USER_DEFAULT_SCREEN_DPI / 72.0f;
		}
		else
			FontFormat.FontSize = float32(NonClientMetrics.lfMessageFont.lfHeight);
		return FontFormat;
	}

	FFontFormat FFontFormat::Default = GetDefaultFontFormat();

	FFontFace FFontFace::Default = FFontFormat::Default.FontFace;


	int32 FApplication::Loop(TFunction<void()> OnIdle)
	{
		MSG Message = {};
		if (OnIdle)
		{
			while (Message.message != WM_QUIT)
			{
				DWORD dwWait = MsgWaitForMultipleObjectsEx(0, NULL, 0, QS_ALLINPUT, MWMO_ALERTABLE);
				switch (dwWait)
				{
				default:
					break;
				}

				OnIdle();

				while (PeekMessageW(&Message, NULL, 0, 0, PM_REMOVE))
				{
					if (Message.message == WM_QUIT)
						break;
					TranslateMessage(&Message);
					DispatchMessageW(&Message);
				}
			}
		}
		else
		{
			while (GetMessageW(&Message, NULL, 0, 0))
			{
				TranslateMessage(&Message);
				DispatchMessageW(&Message);
			}
		}
		return (int32)Message.wParam;
	}

	void FApplication::Exit(int32 ExitCode)
	{
		PostQuitMessage(ExitCode);
	}
}
