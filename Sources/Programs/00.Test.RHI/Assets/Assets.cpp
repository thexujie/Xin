#include "pch.h"
#include "00.Test.RHI.Types.h"

FString GetProjectAssetsPath()
{
	return FPath::GetDirectory(FEncoding::ToUtf8(FWStringV(__FILEW__)));
}
