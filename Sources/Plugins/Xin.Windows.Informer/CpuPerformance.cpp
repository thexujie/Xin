#include "PCH.h"
#include "CpuPerformance.h"

#include "Xin.Core/Platform/Windows/Internal/Private/InternalNT.h"

namespace Xin::Windows::Informer
{
    using namespace Xin::Windows::Internal;

    FCpuPerformance::FCpuPerformance()
    {
    	if (USER_SHARED_DATA->ActiveGroupCount == 1 && // optimization (dmex)
            USER_SHARED_DATA->ActiveProcessorCount > 0 &&
            USER_SHARED_DATA->ActiveProcessorCount <= MAXIMUM_PROC_PER_GROUP)
    	{
            SingleProcessorGroups = true;
            NumberOfProcessorGroups = 1;
    	}
        else
        {
            TList<byte> SystemInformationBytes { 0xFF, EInitializeMode::None };
            ULONG InfomationLength;
            LOGICAL_PROCESSOR_RELATIONSHIP RelationshipType = RelationGroup;
            NTSTATUS Status = NtQuerySystemInformationEx(
                SystemLogicalProcessorAndGroupInformation,
                &RelationshipType,
                sizeof(LOGICAL_PROCESSOR_RELATIONSHIP),
                SystemInformationBytes.Data,
                SystemInformationBytes.Size,
                &InfomationLength
            );

            if (Status == STATUS_INFO_LENGTH_MISMATCH)
            {
                SystemInformationBytes.Resize(InfomationLength, EInitializeMode::None);
                Status = NtQuerySystemInformationEx(
                    SystemLogicalProcessorAndGroupInformation,
                    &RelationshipType,
                    sizeof(LOGICAL_PROCESSOR_RELATIONSHIP),
                    SystemInformationBytes.Data,
                    SystemInformationBytes.Size,
                    &InfomationLength);
            }

            if (NT_SUCCESS(Status))
            {
                SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX & LogicalProcessorInformationEx =
                    InterpretCastRef<SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX>(SystemInformationBytes.Data);
                NumberOfProcessorGroups = LogicalProcessorInformationEx.Group.ActiveGroupCount;
                NumberOfProcessors = 0;
            	NumberOfActiveProcessors.Resize(NumberOfProcessorGroups, EInitializeMode::None);
                for (uintx GroupIndex = 0; GroupIndex < NumberOfProcessorGroups; ++GroupIndex)
                {
                    NumberOfProcessors += LogicalProcessorInformationEx.Group.GroupInfo[GroupIndex].ActiveProcessorCount;
                    NumberOfActiveProcessors[GroupIndex] = LogicalProcessorInformationEx.Group.GroupInfo[GroupIndex].ActiveProcessorCount;;
                }
            }
        }

        SYSTEM_INFO SystemInfo;
        GetNativeSystemInfo(&SystemInfo);
        NumberOfProcessors = SystemInfo.dwNumberOfProcessors;

        TList<PROCESSOR_POWER_INFORMATION> ProcessorPowerInformations_Dynamic;
        PROCESSOR_POWER_INFORMATION ProcessorPowerInformation_Static[256];
        if (SystemInfo.dwNumberOfProcessors > ArraySize(ProcessorPowerInformation_Static))
            ProcessorPowerInformations_Dynamic.Resize(SystemInfo.dwNumberOfProcessors, EInitializeMode::None);

        TSlice<PROCESSOR_POWER_INFORMATION> ProcessorPowerInformations
        {
            SystemInfo.dwNumberOfProcessors > ArraySize(ProcessorPowerInformation_Static) ? ProcessorPowerInformations_Dynamic.Data : ProcessorPowerInformation_Static,
            SystemInfo.dwNumberOfProcessors
        };

        ProcessorsMaxMhz.Resize(NumberOfProcessors, EInitializeMode::Zero);
        NTSTATUS Status = NtPowerInformation(POWER_INFORMATION_LEVEL(ProcessorInformation), NULL, 0, ProcessorPowerInformations.Data, sizeof(PROCESSOR_POWER_INFORMATION) * ProcessorPowerInformations.Size);
        if (NT_SUCCESS(Status))
        {
            for (uintx ProcessorIndex = 0; ProcessorIndex < NumberOfProcessors; ++ProcessorIndex)
            {
                const PROCESSOR_POWER_INFORMATION & ProcessorPowerInformation = ProcessorPowerInformations[ProcessorIndex];
                ProcessorsMaxMhz[ProcessorIndex] = ProcessorPowerInformation.MaxMhz;
                MaxMhz = Max(MaxMhz, ProcessorPowerInformation.MaxMhz);
                CurrentMhz = Max(MaxMhz, ProcessorPowerInformation.CurrentMhz);
            }
        }

        PerformanceInfos.Resize(NumberOfProcessors);
        PerformanceCounters.Resize(NumberOfProcessors * 2 * 2);
    }

    FCpuPerformance::~FCpuPerformance()
    {

    }

    void FCpuPerformance::Update()
	{
        UpdateUsage();
        UpdateFrequency();
	}

	void FCpuPerformance::UpdateUsage()
    {
	    //if (SingleProcessorGroups)
	    {
            TList<SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION> ProcessorPerformanceInformations(NumberOfProcessors, EInitializeMode::None);
            ULONG InfomationLength;
            NTSTATUS Status = NtQuerySystemInformation(SystemProcessorPerformanceInformation, 
                ProcessorPerformanceInformations.Data, sizeof(SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION) * ProcessorPerformanceInformations.Size, 
                &InfomationLength);
            if (Status == STATUS_INFO_LENGTH_MISMATCH)
            {
                ProcessorPerformanceInformations.Resize((InfomationLength + sizeof(SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION)) / sizeof(SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION), EInitializeMode::None);
                Status = NtQuerySystemInformation(SystemProcessorPerformanceDistribution, 
                    ProcessorPerformanceInformations.Data, sizeof(SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION) * ProcessorPerformanceInformations.Size, 
                    &InfomationLength);
            }

            if (NT_SUCCESS(Status))
            {
                float32 TotalUtilization = 0.0f;
                for (uintx ProcessorIndex = 0; ProcessorIndex < NumberOfProcessors; ++ProcessorIndex)
                {
                    SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION & ProcessorPerformanceInformation = ProcessorPerformanceInformations[ProcessorIndex];
					FPerformanceInfo & PerformanceInfo = PerformanceInfos[ProcessorIndex];

                    PerformanceInfo.IdleTime = ProcessorPerformanceInformation.IdleTime.QuadPart;
                    PerformanceInfo.KernelTime = ProcessorPerformanceInformation.KernelTime.QuadPart;
                    PerformanceInfo.UserTime = ProcessorPerformanceInformation.UserTime.QuadPart;
                    AssertExpr(PerformanceInfo.IdleTime.Delta <= PerformanceInfo.KernelTime.Delta + PerformanceInfo.UserTime.Delta);
                    uintx TotalDelta = PerformanceInfo.KernelTime.Delta + PerformanceInfo.UserTime.Delta;
                    if (TotalDelta > 0)
                        TotalUtilization += 1.0f - PerformanceInfo.IdleTime.Delta / float32(TotalDelta);
                }

                Utilization = TotalUtilization / NumberOfProcessors;
            }

            //Utilization = accurate_usage() / 100.0f;
	    }
    }

    void FCpuPerformance::UpdateFrequency()
    {
        // SYSTEM_PROCESSOR_PERFORMANCE_DISTRIBUTION
        uintx ExpectedLength = sizeof(ULONG) /*ProcessorCount*/ + sizeof(ULONG) * NumberOfProcessors /*Offsets*/;
        ExpectedLength = AlignWith(ExpectedLength, alignof(SYSTEM_PROCESSOR_PERFORMANCE_STATE_DISTRIBUTION_BASE));

        uintx StateDistributionLength;
        if (FWinversion::Current >= Winversion_8_1)
        {
            // SYSTEM_PROCESSOR_PERFORMANCE_STATE_DISTRIBUTION
            StateDistributionLength =
                sizeof(SYSTEM_PROCESSOR_PERFORMANCE_STATE_DISTRIBUTION::ProcessorNumber) +
                sizeof(SYSTEM_PROCESSOR_PERFORMANCE_STATE_DISTRIBUTION::StateCount) +
                2 * sizeof(SYSTEM_PROCESSOR_PERFORMANCE_HITCOUNT);
            StateDistributionLength = AlignWith(StateDistributionLength, alignof(SYSTEM_PROCESSOR_PERFORMANCE_STATE_DISTRIBUTION));
        }
        else
        {
            // SYSTEM_PROCESSOR_PERFORMANCE_STATE_DISTRIBUTION_WIN8
            StateDistributionLength =
                sizeof(SYSTEM_PROCESSOR_PERFORMANCE_STATE_DISTRIBUTION_WIN8::ProcessorNumber) +
                sizeof(SYSTEM_PROCESSOR_PERFORMANCE_STATE_DISTRIBUTION_WIN8::StateCount) +
                2 * sizeof(SYSTEM_PROCESSOR_PERFORMANCE_HITCOUNT_WIN8);
            StateDistributionLength = AlignWith(StateDistributionLength, alignof(SYSTEM_PROCESSOR_PERFORMANCE_STATE_DISTRIBUTION_WIN8));
        }

        ExpectedLength += StateDistributionLength * NumberOfProcessors;

        TList<byte> PerformanceDistributionBytes { ExpectedLength, EInitializeMode::None };
        ULONG InfomationLength;
        NTSTATUS Status = NtQuerySystemInformation(SystemProcessorPerformanceDistribution, PerformanceDistributionBytes.Data, PerformanceDistributionBytes.Size, &InfomationLength);

        uintx RetryIndex = 0;
        while (Status == STATUS_INFO_LENGTH_MISMATCH && RetryIndex < 8)
        {
            PerformanceDistributionBytes.Resize(InfomationLength, EInitializeMode::None);
            Status = NtQuerySystemInformation(SystemProcessorPerformanceDistribution, PerformanceDistributionBytes.Data, PerformanceDistributionBytes.Size, &InfomationLength);
            RetryIndex++;
        }

        if (NT_FAILED(Status))
            return;

        SYSTEM_PROCESSOR_PERFORMANCE_DISTRIBUTION & PerformanceDistribution = InterpretCastRef<SYSTEM_PROCESSOR_PERFORMANCE_DISTRIBUTION>(PerformanceDistributionBytes.Data);
        if (PerformanceDistribution.ProcessorCount != NumberOfProcessors)
            return;

        uintx TotalHits = 0;
        uintx TotalPercentFrequency = 0;

        for (uintx ProcessorIndex = 0; ProcessorIndex < NumberOfProcessors; ++ProcessorIndex)
        {
            SYSTEM_PROCESSOR_PERFORMANCE_STATE_DISTRIBUTION_BASE & StateDistributionBase =
                InterpretCastRef<SYSTEM_PROCESSOR_PERFORMANCE_STATE_DISTRIBUTION_BASE>(PerformanceDistributionBytes.Data + PerformanceDistribution.Offsets[ProcessorIndex]);

            if (StateDistributionBase.StateCount != 2)
                //continue;
                return;

            for (uintx StateIndex = 0; StateIndex < StateDistributionBase.StateCount; ++StateIndex)
            {
                FCpuPerformanceCounter & PerformanceCounter = PerformanceCounters[(UpdateIndex % 2) * (NumberOfProcessors * 2) + ProcessorIndex * 2 + StateIndex];
                PerformanceCounter = { };

                if (FWinversion::Current >= Winversion_8_1)
                {
                    SYSTEM_PROCESSOR_PERFORMANCE_STATE_DISTRIBUTION & StateDistribution =
                        InterpretCastRef<SYSTEM_PROCESSOR_PERFORMANCE_STATE_DISTRIBUTION>(PerformanceDistributionBytes.Data + PerformanceDistribution.Offsets[ProcessorIndex]);

                    SYSTEM_PROCESSOR_PERFORMANCE_HITCOUNT & PerformanceHitcount = StateDistribution.States[StateIndex];
                    PerformanceCounter.Hits = PerformanceHitcount.Hits;
                    PerformanceCounter.PercentFrequency = PerformanceHitcount.PercentFrequency;
                }
                else
                {
                    SYSTEM_PROCESSOR_PERFORMANCE_STATE_DISTRIBUTION_WIN8 & StateDistribution_Win8 =
                        InterpretCastRef<SYSTEM_PROCESSOR_PERFORMANCE_STATE_DISTRIBUTION_WIN8>(PerformanceDistributionBytes.Data + PerformanceDistribution.Offsets[ProcessorIndex]);

                    SYSTEM_PROCESSOR_PERFORMANCE_HITCOUNT_WIN8 & PerformanceHitcount = StateDistribution_Win8.States[StateIndex];
                    PerformanceCounter.Hits = PerformanceHitcount.Hits;
                    PerformanceCounter.PercentFrequency = PerformanceHitcount.PercentFrequency;
                }

                if (UpdateIndex > 0)
                {
                    FCpuPerformanceCounter & PerformanceCounterLast = PerformanceCounters[((UpdateIndex + 1) % 2) * (NumberOfProcessors * 2) + ProcessorIndex * 2 + StateIndex];

                    uintx Hits = PerformanceCounter.Hits - PerformanceCounterLast.Hits;
                    TotalHits += Hits;
                    TotalPercentFrequency += Hits * PerformanceCounter.PercentFrequency * ProcessorsMaxMhz[ProcessorIndex];
                }
            }
        }

        ++UpdateIndex;
        ActiveFrenquencyMhz = UpdateIndex > 1 ? (TotalPercentFrequency / TotalHits / 100) : MaxMhz;
    }
}
