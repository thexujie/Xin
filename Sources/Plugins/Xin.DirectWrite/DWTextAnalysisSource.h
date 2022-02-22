#pragma once

#include "DWTypes.h"

namespace Xin::DWrite
{
    struct FDWScriptRun
    {
        uint32 Utf16Index = 0;
        uint32 Utf16Length = 0;

        DWRITE_SCRIPT_ANALYSIS ScriptAnalysis { };

        EScriptCode ScriptCode = EScriptCode::None;
        EBiDiLevel BiDiLevel = EBiDiLevel::LTR;
        bool NumberSubstituted = false;

        uint32 NextRunIndex = NullIndex;

#if XIN_DWRITE_DEBUG
        FU16StringV U16String;
#endif

        bool ContainsTextPosition(UINT32 DesiredTextPosition) const noexcept
        {
            return DesiredTextPosition >= Utf16Index && DesiredTextPosition < Utf16Index + Utf16Length;
        }
    };

	class DWRITE_API FDWTextAnalysisSource : public TUnknown<IDWriteTextAnalysisSource>
	{
	public:
        FDWTextAnalysisSource(FDWTextShaper & Shaper, FU16StringV String, EReadingDirection ReadingDirection)
			: Shaper(Shaper), String(FEncoding::AsWide(String)), ReadingDirection(ReadingDirection) {}

	public:
        STDMETHOD(GetTextAtPosition)(
            UINT32 textPosition,
            _Outptr_result_buffer_(*textLength) WCHAR const ** textString,
            _Out_ UINT32 * textLength
            ) override
        {
            if (textPosition >= String.Size)
            {
                *textString = nullptr;
                *textLength = 0;
            }
            else
            {
                *textString = String.Data + textPosition;
                *textLength = uint32(String.Size - textPosition);
            }
            return S_OK;
        }

        STDMETHOD(GetTextBeforePosition)(
            UINT32 textPosition,
            _Outptr_result_buffer_(*textLength) WCHAR const ** textString,
            _Out_ UINT32 * textLength
            ) override
        {
            if (textPosition == 0 || textPosition > String.Size)
            {
                *textString = nullptr;
                *textLength = 0;
            }
            else
            {
                *textString = String.Data;
                *textLength = textPosition - 0;
            }
            return S_OK;
        }

        STDMETHOD_(DWRITE_READING_DIRECTION, GetParagraphReadingDirection)() override
        {
            return FromReadingDirection(ReadingDirection);
        }

        STDMETHOD(GetLocaleName)(
            UINT32 textPosition,
            _Out_ UINT32 * textLength,
            _Outptr_result_z_ WCHAR const ** localeName
            ) override;

        STDMETHOD(GetNumberSubstitution)(
            UINT32 textPosition,
            _Out_ UINT32 * textLength,
            _COM_Outptr_ IDWriteNumberSubstitution ** numberSubstitution
            ) override
        {
            *numberSubstitution = nullptr;
            *textLength = uint32(String.Size - textPosition);
            return S_OK;
        }

	public:
        FDWTextShaper & Shaper;
        FWStringV String;
        EReadingDirection ReadingDirection;
	};
    using FDWriteTextAnalysisSourceRef = TReferPtr<FDWTextAnalysisSource>;
}
