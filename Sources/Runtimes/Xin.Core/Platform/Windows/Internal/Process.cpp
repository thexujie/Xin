#include "PCH.h"
#include "Process.h"

#include "Private/Path.h"

#define PTR_ADD_OFFSET(Pointer, Offset) ((PVOID)((ULONG_PTR)(Pointer) + (ULONG_PTR)(Offset)))
#define PTR_SUB_OFFSET(Pointer, Offset) ((PVOID)((ULONG_PTR)(Pointer) - (ULONG_PTR)(Offset)))

namespace Xin::Windows::Internal
{
	FProcess::FProcess(uint32 ProcessId, uint32 AccessMask)
	{
		OBJECT_ATTRIBUTES ObjectAttributes {};
		CLIENT_ID ClientId { HANDLE(ProcessId), NULL };

		InitializeObjectAttributes(&ObjectAttributes, NULL, 0, NULL, NULL);
		NTSTATUS Status = NtOpenProcess(&Handle, AccessMask, &ObjectAttributes, &ClientId);
	}

	FProcess::~FProcess()
	{
		if (Handle)
		{
			NtClose(Handle);
			Handle = NULL;
		}
	}

	FProcessDynamicInfo FProcess::QueryDynamicInfo() const
	{
		PROCESS_EXTENDED_BASIC_INFORMATION ExtendedBasicInformation { sizeof(PROCESS_EXTENDED_BASIC_INFORMATION) };

		return { };
	}

	FWString FProcess::QueryCommandLine() const
	{
		ULONG BufferLength = sizeof(UNICODE_STRING) + DOS_MAX_PATH_LENGTH;
		UNICODE_STRING * CommandLineBuffer = (UNICODE_STRING *)Malloc(BufferLength);

		ULONG RequiredLength = 0;
		NTSTATUS Status = NtQueryInformationProcess(Handle, ProcessCommandLineInformation, CommandLineBuffer, BufferLength, &RequiredLength);

		if (Status == STATUS_INFO_LENGTH_MISMATCH)
		{
			Free(CommandLineBuffer);
			BufferLength = RequiredLength;
			CommandLineBuffer = (UNICODE_STRING *)Malloc(BufferLength);

			Status = NtQueryInformationProcess( Handle, ProcessCommandLineInformation, CommandLineBuffer, BufferLength, &RequiredLength );
		}

		if (NT_SUCCESS(Status))
		{
			FWString CommandLine = FWStringV { (const charw *)CommandLineBuffer->Buffer, uintx(CommandLineBuffer->Length / 2) };
			Free(CommandLineBuffer);
			return CommandLine;
		}
		else
		{
			Free(CommandLineBuffer);
			return FWString::None;
		}
	}

	FWString FProcess::QueryCurrentDirectory() const
	{
		PROCESS_EXTENDED_BASIC_INFORMATION ProcessBasicInformation { sizeof(PROCESS_EXTENDED_BASIC_INFORMATION) };
		NTSTATUS Status = NtQueryInformationProcess( Handle,
			PROCESSINFOCLASS::ProcessBasicInformation, &ProcessBasicInformation, sizeof(PROCESS_EXTENDED_BASIC_INFORMATION), NULL );

		if (!NT_SUCCESS(Status) || !ProcessBasicInformation.BasicInfo.PebBaseAddress)
			return FWString::None;

		SIZE_T NumberOfBytesRead;

		if (ProcessBasicInformation.IsWow64Process)
		{
			ULONG_PTR PebBaseAddressWow64;
			Status = NtQueryInformationProcess(Handle, ProcessWow64Information, &PebBaseAddressWow64, sizeof(ULONG_PTR), NULL);
			if (!NT_SUCCESS(Status))
				return FWString::None;

			PEB32 ProcessPEB { };
			Status = NtReadVirtualMemory(Handle, (PVOID)PebBaseAddressWow64, &ProcessPEB, sizeof(PEB32), &NumberOfBytesRead);
			if (!NT_SUCCESS(Status))
				return FWString::None;

			RTL_USER_PROCESS_PARAMETERS32 UserProcessParameters { };
			// Read the address of the process parameters.
			Status = NtReadVirtualMemory(Handle, UlongToPtr(ProcessPEB.ProcessParameters), &UserProcessParameters, sizeof(RTL_USER_PROCESS_PARAMETERS32), &NumberOfBytesRead);
			if (!NT_SUCCESS(Status))
				return FWString::None;

			FWString CurrentDirectory(UserProcessParameters.CurrentDirectory.DosPath.Length / 2);
			Status = NtReadVirtualMemory(Handle, UlongToPtr(UserProcessParameters.CurrentDirectory.DosPath.Buffer), CurrentDirectory.Data, UserProcessParameters.CurrentDirectory.DosPath.Length, &NumberOfBytesRead);
			if (!NT_SUCCESS(Status))
				return FWString::None;
			return CurrentDirectory;
		}
		else
		{
			PEB ProcessPEB { };
			Status = NtReadVirtualMemory(Handle, ProcessBasicInformation.BasicInfo.PebBaseAddress, &ProcessPEB, sizeof(PEB), &NumberOfBytesRead);
			if (!NT_SUCCESS(Status))
				return FWString::None;

			RTL_USER_PROCESS_PARAMETERS UserProcessParameters { };
			// Read the address of the process parameters.
			Status = NtReadVirtualMemory(Handle, ProcessPEB.ProcessParameters, &UserProcessParameters, sizeof(RTL_USER_PROCESS_PARAMETERS), &NumberOfBytesRead);
			if (!NT_SUCCESS(Status))
				return FWString::None;

			FWString CurrentDirectory(UserProcessParameters.CurrentDirectory.DosPath.Length / 2);
			Status = NtReadVirtualMemory(Handle, UserProcessParameters.CurrentDirectory.DosPath.Buffer, CurrentDirectory.Data, UserProcessParameters.CurrentDirectory.DosPath.Length, &NumberOfBytesRead);
			if (!NT_SUCCESS(Status))
				return FWString::None;
			return CurrentDirectory;
		}

	}

	FWString FProcess::QueryProcessImageFilePath(uint32 ProcessId)
	{
		TList<byte> SystemInfomation { 1024, EInitializeMode::Zero };
		SYSTEM_PROCESS_ID_INFORMATION SystemProcessIdInformation { (HANDLE)ProcessId };
		SystemProcessIdInformation.ImageName.Buffer = PWSTR(SystemInfomation.Data);
		SystemProcessIdInformation.ImageName.MaximumLength = SystemInfomation.Size;
		SystemProcessIdInformation.ImageName.Length = 0;
		ULONG ReturnLength;
		NTSTATUS Status = NtQuerySystemInformation(SYSTEM_INFORMATION_CLASS::SystemProcessIdInformation,
			&SystemProcessIdInformation, sizeof(SystemProcessIdInformation),
			&ReturnLength);

		if (Status == STATUS_INFO_LENGTH_MISMATCH)
		{
			SystemInfomation.Resize(ReturnLength);
			Status = NtQuerySystemInformation(SYSTEM_INFORMATION_CLASS::SystemProcessIdInformation,
				SystemInfomation.Data, SystemInfomation.Size,
				&ReturnLength);
		}

		if (NT_SUCCESS(Status))
			return NtFilePathToDosFilePath(SystemProcessIdInformation.ImageName);
		else
			return FWString::None;
	}

	TTuple<FSystemDynamicInfo, TList<FProcessDynamicInfo>> FProcess::QueryProcessDynamicInfos()
	{
		TList<byte> SystemProcessInformationBytes(0x400);
		ULONG SizeNeeded = SystemProcessInformationBytes.Size;
		NTSTATUS Status = NtQuerySystemInformation(SYSTEM_INFORMATION_CLASS::SystemProcessInformation, SystemProcessInformationBytes.Data, SystemProcessInformationBytes.Size, &SizeNeeded);
		uintx RetryIndex = 0;
		while (Status == STATUS_BUFFER_TOO_SMALL || Status == STATUS_INFO_LENGTH_MISMATCH)
		{
			if (RetryIndex++ > 10)
				break;

			SystemProcessInformationBytes.Resize(SizeNeeded);
			Status = NtQuerySystemInformation(SYSTEM_INFORMATION_CLASS::SystemProcessInformation, SystemProcessInformationBytes.Data, SystemProcessInformationBytes.Size, &SizeNeeded);
		}

		if (!NT_SUCCESS(Status))
			return { };

		FSystemDynamicInfo SystemDynamicInfo {};
		TList<FProcessDynamicInfo> ProcessDynamicInfos;
		SYSTEM_PROCESS_INFORMATION * SystemProcessInformation = (SYSTEM_PROCESS_INFORMATION *)(SystemProcessInformationBytes.Data);
		while(SystemProcessInformation)
		{
			FProcessDynamicInfo & DynamicInfo = ProcessDynamicInfos.AddDefault();
			DynamicInfo.ProcessId = uintx(SystemProcessInformation->UniqueProcessId);
			DynamicInfo.WorkingSetSize = uint64(SystemProcessInformation->WorkingSetSize);
			DynamicInfo.PagefileUsage = uint64(SystemProcessInformation->PagefileUsage);
			DynamicInfo.CycleTime = uint64(SystemProcessInformation->CycleTime);

			SystemDynamicInfo.CycleTime += DynamicInfo.CycleTime;
			if (!SystemProcessInformation->NextEntryOffset)
				break;

			SystemProcessInformation = (SYSTEM_PROCESS_INFORMATION *)((byte *)SystemProcessInformation + SystemProcessInformation->NextEntryOffset);
		}
		return { SystemDynamicInfo, ProcessDynamicInfos };
	}
}
