#include "PCH.h"
#include "DWTextAnalyzer.h"

#include "DWTextShaper.h"

namespace Xin::DWrite
{
	FDWTextAnalyzer::FDWTextAnalyzer(FDWTextShaper & Shaper, FDWTextAnalysisSource & AnalysisSource, TList<FDWScriptRun> & ScriptRuns, TList<DWRITE_LINE_BREAKPOINT> & TextBreakpoints)
		: Shaper(Shaper), AnalysisSource(AnalysisSource), ScriptRuns(ScriptRuns), TextBreakpoints(TextBreakpoints)
	{
		
	}

	void FDWTextAnalyzer::Analyze()
	{
		if (!AnalysisSource.String.Size)
			return;

		{
			// First run.
			FDWScriptRun & AnalyzedRun = ScriptRuns.AddDefault();
			AnalyzedRun.NextRunIndex = NullIndex;
			AnalyzedRun.Utf16Index = 0;
			AnalyzedRun.Utf16Length = uint32(AnalysisSource.String.Size);
		}

		TextBreakpoints.Resize(AnalysisSource.String.Size, EInitializeMode::Zero);
		uint32 TextLength = AnalysisSource.String.Size;
		HRESULT Result;
		if (SUCCEEDED(Result = Shaper.dwTextAnalyzer->AnalyzeLineBreakpoints(&AnalysisSource, 0, TextLength, this))
			&& SUCCEEDED(Result = Shaper.dwTextAnalyzer->AnalyzeBidi(&AnalysisSource, 0, TextLength, this))
			&& SUCCEEDED(Result = Shaper.dwTextAnalyzer->AnalyzeScript(&AnalysisSource, 0, TextLength, this))
			&& SUCCEEDED(Result = Shaper.dwTextAnalyzer->AnalyzeNumberSubstitution(&AnalysisSource, 0, TextLength, this)))
		{
#if XIN_DWRITE_DEBUG
			for (FDWScriptRun & AnalyzedRun : ScriptRuns)
			{
				AnalyzedRun.U16String = FEncoding::AsUtf16(AnalysisSource.String.SubView(AnalyzedRun.Utf16Index, AnalyzedRun.Utf16Length));
			}
#endif
		}
	}

	HRESULT FDWTextAnalyzer::SetLineBreakpoints(UINT32 textPosition, UINT32 textLength, DWRITE_LINE_BREAKPOINT const * lineBreakpoints) noexcept
	{
		ArrayCopy(TextBreakpoints.Data + textPosition, lineBreakpoints, textLength);
		return S_OK;
	}

	HRESULT FDWTextAnalyzer::SetScriptAnalysis(UINT32 textPosition, UINT32 textLength, DWRITE_SCRIPT_ANALYSIS const * scriptAnalysis) noexcept
	{
		SetCurrentRun(textPosition);
		SplitCurrentRun(textPosition);

		while (textLength > 0)
		{
			uint32 RunIndex = CurrRunIndex;
			textLength = FetchNextRun(textLength);
			ScriptRuns[RunIndex].ScriptAnalysis = *scriptAnalysis;
			ScriptRuns[RunIndex].ScriptCode = Shaper.FindIsoScriptCode(scriptAnalysis->script);
		}
		return S_OK;
	}

	HRESULT FDWTextAnalyzer::SetBidiLevel(UINT32 textPosition, UINT32 textLength, UINT8 explicitLevel, UINT8 resolvedLevel) noexcept
	{
		SetCurrentRun(textPosition);
		SplitCurrentRun(textPosition);

		while (textLength > 0)
		{
			uint32 RunIndex = CurrRunIndex;
			textLength = FetchNextRun(textLength);
			ScriptRuns[RunIndex].BiDiLevel = EBiDiLevel(resolvedLevel);
		}
		return S_OK;
	}

	HRESULT FDWTextAnalyzer::SetNumberSubstitution(UINT32 textPosition, UINT32 textLength, IDWriteNumberSubstitution * numberSubstitution) noexcept
	{
		SetCurrentRun(textPosition);
		SplitCurrentRun(textPosition);

		while (textLength > 0)
		{
			uint32 RunIndex = CurrRunIndex;
			textLength = FetchNextRun(textLength);
			ScriptRuns[RunIndex].NumberSubstituted = numberSubstitution != nullptr;
		}
		return S_OK;
	}

	void FDWTextAnalyzer::SetCurrentRun(uint32 TextPosition)
	{
		if (CurrRunIndex >= ScriptRuns.Size || !ScriptRuns[CurrRunIndex].ContainsTextPosition(TextPosition))
		{
			CurrRunIndex = FindIf(ScriptRuns.Begin(), ScriptRuns.End(), [&](const FDWScriptRun & Run) { return Run.ContainsTextPosition(TextPosition); }) - ScriptRuns.Begin();
		}
	};

	void FDWTextAnalyzer::SplitCurrentRun(uint32 SplitPosition)
	{
		if (SplitPosition > ScriptRuns[CurrRunIndex].Utf16Index)
		{
			FDWScriptRun & BackHalf = ScriptRuns.AddZeroed();
			FDWScriptRun & FrontHalf = ScriptRuns[CurrRunIndex];
			BackHalf = FrontHalf;

			uint32 FrontLength = SplitPosition - FrontHalf.Utf16Index;

			BackHalf.Utf16Index = SplitPosition;
			BackHalf.Utf16Length = BackHalf.Utf16Length - FrontLength;

			//FrontHalf.TextStart = FrontHalf.TextStart;
			FrontHalf.Utf16Length = FrontLength;
			FrontHalf.NextRunIndex = CurrRunIndex = uint32(ScriptRuns.Size) - 1;
		}
	};

	uint32 FDWTextAnalyzer::FetchNextRun(uint32 TextLength)
	{
		uint32 CurrRunTextLength = ScriptRuns[CurrRunIndex].Utf16Length;
		if (TextLength < CurrRunTextLength)
		{
			SplitCurrentRun(ScriptRuns[CurrRunIndex].Utf16Index + TextLength);
			return 0;
		}
		else
		{
			CurrRunIndex = ScriptRuns[CurrRunIndex].NextRunIndex;
			return TextLength - CurrRunTextLength;
		}
	};
}

