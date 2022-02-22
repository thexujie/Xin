#include "PCH.h"
#include "DWTextAnalysisSource.h"
#include "DWTextShaper.h"

namespace Xin::DWrite
{
	HRESULT FDWTextAnalysisSource::GetLocaleName(UINT32 textPosition, UINT32 * textLength, WCHAR const ** localeName) noexcept
	{
		*localeName = Shaper.DefaultLocalName.Data;
		*textLength = Shaper.DefaultLocalName.Length;
		return S_OK;
	}
}
