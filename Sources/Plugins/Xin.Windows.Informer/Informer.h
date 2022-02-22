#pragma once

#include "Xin.Windows.Informer.Types.h"

namespace Xin::Windows::Informer
{
	INFORMER_API FString GetCpuBrand();
	INFORMER_API TTuple<uintx, uintx> GetCpuMaxFrequencyMhz();
}
