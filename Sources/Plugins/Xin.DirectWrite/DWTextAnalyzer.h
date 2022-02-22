#pragma once

#include "DWTypes.h"
#include "DWTextRun.h"
#include "DWTextAnalysisSource.h"

namespace Xin::DWrite
{
	class FDWTextAnalyzer : public TUnknown<IDWriteTextAnalysisSink>
	{
	public:
		FDWTextAnalyzer(FDWTextShaper & Shaper, FDWTextAnalysisSource & AnalysisSource, TList<FDWScriptRun> & ScriptRuns, TList<DWRITE_LINE_BREAKPOINT> & TextBreakpoints);

		void Analyze();

	public: // IDWriteTextAnalysisSink
		HRESULT SetLineBreakpoints(UINT32 textPosition, UINT32 textLength, DWRITE_LINE_BREAKPOINT const * lineBreakpoints) noexcept override;
		HRESULT SetScriptAnalysis(UINT32 textPosition, UINT32 textLength, DWRITE_SCRIPT_ANALYSIS const * scriptAnalysis) noexcept override;
		HRESULT SetBidiLevel(UINT32 textPosition, UINT32 textLength, UINT8 explicitLevel, UINT8 resolvedLevel) noexcept override;
		HRESULT SetNumberSubstitution(UINT32 textPosition, UINT32 textLength, IDWriteNumberSubstitution * numberSubstitution) noexcept override;

	private:
		void SetCurrentRun(uint32 DesiredTextPosition);
		void SplitCurrentRun(uint32 SplitPosition);
		uint32 FetchNextRun(uint32 CurrTextLength);

	public:
		FDWTextShaper & Shaper;
		FDWTextAnalysisSource & AnalysisSource;

		TList<FDWScriptRun> & ScriptRuns;
		TList<DWRITE_LINE_BREAKPOINT> & TextBreakpoints;

		uint32 CurrRunIndex = 0;
		IDWriteNumberSubstitution * DWriteNumberSubstitution = nullptr;
	};
}
