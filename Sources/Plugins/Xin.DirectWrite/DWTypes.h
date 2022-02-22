#pragma once

#include "Xin.Core/Xin.Core.h"
#include "Xin.Core/Platform/Windows/WindowsPrivate.h"

#if XIN_LIB_DYNAMIC
#if _Xin_DirectWrite_Module_
#define DWRITE_API XIN_EXPORT
#else
#define DWRITE_API XIN_IMPORT
#endif
#else
#define DWRITE_API
#endif

#include <d2d1.h>
#include <dwrite.h>
#include <dwrite_1.h>
#include <dwrite_2.h>
#include <dwrite_3.h>

#if XIN_DEBUG
#define XIN_DWRITE_DEBUG 1
#else
#define XIN_DWTEXT_DEBUG 0
#endif

namespace Xin::DWrite
{
	using namespace Xin::Windows;

	// Windows 7, Windows Vista with SP2 and Platform Update for Windows Vista
	using IDWriteFactoryRef = TReferPtr<IDWriteFactory>;
	// Windows 8 and Platform Update for Windows 7
	using IDWriteFactory1Ref = TReferPtr<IDWriteFactory1>;
	// Windows 8.1
	using IDWriteFactory2Ref = TReferPtr<IDWriteFactory2>;
	// Windows 10
	using IDWriteFactory3Ref = TReferPtr<IDWriteFactory3>;

	using IDWriteFontCollectionRef = TReferPtr<IDWriteFontCollection>;
	using IDWriteFontCollection1Ref = TReferPtr<IDWriteFontCollection1>;
	using IDWriteFontCollection2Ref = TReferPtr<IDWriteFontCollection2>;
	using IDWriteFontCollection3Ref = TReferPtr<IDWriteFontCollection3>;

	using IDWriteFontFamilyRef = TReferPtr<IDWriteFontFamily>;
	using IDWriteFontFamily1Ref = TReferPtr<IDWriteFontFamily1>;
	using IDWriteFontFamily2Ref = TReferPtr<IDWriteFontFamily2>;

	using IDWriteFontRef = TReferPtr<IDWriteFont>;
	using IDWriteFont1Ref = TReferPtr<IDWriteFont1>;
	using IDWriteFont2Ref = TReferPtr<IDWriteFont2>;
	using IDWriteFont3Ref = TReferPtr<IDWriteFont3>;

	using IDWriteFontFaceRef = TReferPtr<IDWriteFontFace>;
	using IDWriteFontFace1Ref = TReferPtr<IDWriteFontFace1>;
	using IDWriteFontFace2Ref = TReferPtr<IDWriteFontFace2>;
	using IDWriteFontFace3Ref = TReferPtr<IDWriteFontFace3>;

	using IDWriteTextFormatRef = TReferPtr<IDWriteTextFormat>;
	using IDWriteTextFormat1Ref = TReferPtr<IDWriteTextFormat1>;
	using IDWriteTextFormat2Ref = TReferPtr<IDWriteTextFormat2>;
	using IDWriteTextFormat3Ref = TReferPtr<IDWriteTextFormat3>;

	using IDWriteTextLayoutRef = TReferPtr<IDWriteTextLayout>;
	using IDWriteTextLayout1Ref = TReferPtr<IDWriteTextLayout1>;
	using IDWriteTextLayout2Ref = TReferPtr<IDWriteTextLayout2>;
	using IDWriteTextLayout3Ref = TReferPtr<IDWriteTextLayout3>;

	using IDWriteFontFallbackRef = TReferPtr<IDWriteFontFallback>;

	using IDWriteTextAnalyzerRef = TReferPtr<IDWriteTextAnalyzer>;
	using IDWriteTextAnalyzer1Ref = TReferPtr<IDWriteTextAnalyzer1>;
	using IDWriteTextAnalyzer2Ref = TReferPtr<IDWriteTextAnalyzer2>;

	using IDWriteGdiInteropRef = TReferPtr<IDWriteGdiInterop>;

	using IDWriteBitmapRenderTargetRef = TReferPtr<IDWriteBitmapRenderTarget>;

	using IDWriteRenderingParamsRef = TReferPtr<IDWriteRenderingParams>;

	inline DWRITE_FONT_STYLE FromFontStyle(EFontStyle FontStyle)
	{
		DWRITE_FONT_STYLE DWriteFontStyle {};
		SetFlags(DWriteFontStyle, DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_ITALIC, FontStyle & EFontStyle::Italic);
		return DWriteFontStyle;
	}

	class FDWTextShaper;

	inline DWRITE_READING_DIRECTION FromReadingDirection(EReadingDirection ReadingDirection)
	{
		switch(ReadingDirection)
		{
		default:
		case EReadingDirection::LeftToRight: return DWRITE_READING_DIRECTION::DWRITE_READING_DIRECTION_LEFT_TO_RIGHT;
		case EReadingDirection::RightToLeft: return DWRITE_READING_DIRECTION::DWRITE_READING_DIRECTION_RIGHT_TO_LEFT;
		case EReadingDirection::TopToBottom: return DWRITE_READING_DIRECTION::DWRITE_READING_DIRECTION_TOP_TO_BOTTOM;
		case EReadingDirection::BottomToTop: return DWRITE_READING_DIRECTION::DWRITE_READING_DIRECTION_BOTTOM_TO_TOP;
		}
	}
}
