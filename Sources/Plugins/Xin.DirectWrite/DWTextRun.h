#pragma once

#include "DWTypes.h"
#include "DWFontFace.h"

namespace Xin::DWrite
{
	struct FDWGlyphRun
	{
		FDWFontFace & FontFace;
		float32 FontSize = 0.0f;

		uint32 TextIndex = 0;
		uint32 TextLength = 0;

		uint32 Utf16Index = 0;
		uint32 Utf16Length = 0;

		uint32 GlyphIndex = 0;
		uint32 GlyphLength = 0;

		uint32 ClusterIndex = 0;
		uint32 ClusterLength = 0;

		EScriptCode ScriptCode = EScriptCode::None;
		DWRITE_SCRIPT_ANALYSIS ScriptAnalysis {};
		uint8 BidiLevel = 0;
		bool NumberSubstituted = false;

		bool Rotate90 = false;
		bool Invisible = false;

#if XIN_DWRITE_DEBUG
		FUStringV String;
#endif
	};

	forceinline bool IsTextIndexWithinTextRun(uint32 Utf16Index, const FShapeRun & ShapeRun)
	{
		return ShapeRun.Utf16Index <= Utf16Index && Utf16Index < ShapeRun.Utf16Index + ShapeRun.Utf16Length;
	}

}
