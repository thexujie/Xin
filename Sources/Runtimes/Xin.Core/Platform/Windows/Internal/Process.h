#pragma once

#include "Internal.Types.h"

namespace Xin::Windows::Internal
{
	struct FSystemDynamicInfo
	{
		uintx CycleTime;
	};

	struct FProcessDynamicInfo
	{
		uintx ProcessId;
		uint64 WorkingSetSize;
		uint64 PagefileUsage;
		uint64 CycleTime;
	};

	class INTERNAL_API FProcess
	{
	public:
		FProcess() = default;
		FProcess(uint32 ProcessId, uint32 AccessMask);
		~FProcess();

		FProcessDynamicInfo QueryDynamicInfo() const;
		FWString QueryCommandLine() const;
		FWString QueryCurrentDirectory() const;

	public:
		static FWString QueryProcessImageFilePath(uint32 ProcessId);
		static TTuple<FSystemDynamicInfo, TList<FProcessDynamicInfo>> QueryProcessDynamicInfos();

	public:
		explicit operator bool() const { return !!Handle; }
		voidp Handle = nullptr;

	public:
		static constexpr uint32 IdelProcessId = 0;
		static constexpr uint32 SystemProcessId = 4;
	};
}
