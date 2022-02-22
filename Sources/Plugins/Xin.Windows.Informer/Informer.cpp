#include "PCH.h"
#include "Informer.h"

#include "Xin.Core/Platform/Windows/Internal/Private/InternalNT.h"

namespace Xin::Windows::Informer
{
	FString GetCpuBrand()
	{
		CHAR BrandString[256];
        ULONG ReturnLength;
        if (NT_SUCCESS(NtQuerySystemInformation(SystemProcessorBrandString, BrandString, sizeof(BrandString), &ReturnLength)))
        {
            uintx BrandLength = Length(BrandString);
            AssertExpr(BrandLength <= ReturnLength);
            return FString((const char8 *)BrandString, BrandLength);
        }
        return FString::None;
	}

    TTuple<uintx, uintx> GetCpuMaxFrequencyMhz()
	{
        SYSTEM_INFO SystemInfo;
        GetNativeSystemInfo(&SystemInfo);

        TList<PROCESSOR_POWER_INFORMATION> ProcessorPowerInformations_Dynamic;
        PROCESSOR_POWER_INFORMATION ProcessorPowerInformation_Static[256];
        if (SystemInfo.dwNumberOfProcessors > ArraySize(ProcessorPowerInformation_Static))
            ProcessorPowerInformations_Dynamic.Resize(SystemInfo.dwNumberOfProcessors, EInitializeMode::None);

        TSlice<PROCESSOR_POWER_INFORMATION> ProcessorPowerInformations
		{
            SystemInfo.dwNumberOfProcessors > ArraySize(ProcessorPowerInformation_Static) ? ProcessorPowerInformations_Dynamic.Data : ProcessorPowerInformation_Static,
            SystemInfo.dwNumberOfProcessors
        };

        NTSTATUS Status = NtPowerInformation(POWER_INFORMATION_LEVEL(ProcessorInformation), NULL, 0, ProcessorPowerInformations.Data, sizeof(PROCESSOR_POWER_INFORMATION) * ProcessorPowerInformations.Size);
        if (NT_SUCCESS(Status))
        {
            uintx CurrentMhz = 0;
            uintx MaxMhz = 0;
	        for (const PROCESSOR_POWER_INFORMATION & ProcessorPowerInformation : ProcessorPowerInformations)
	        {
                MaxMhz = Max(MaxMhz, ProcessorPowerInformation.MaxMhz);
                CurrentMhz = Max(MaxMhz, ProcessorPowerInformation.CurrentMhz);
	        }
            return { CurrentMhz, MaxMhz };
        }
        return { 0, 0 };
	}
}
