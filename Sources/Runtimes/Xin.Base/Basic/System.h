#pragma once

#include "Basic.Types.h"

namespace Xin
{
	struct BASE_API FSystemParameterInfo
	{
	public:
		FSystemParameterInfo();

	public:
		uint32 KeyboardDelay;
		uint32 KeyboardSpeed;
	};

	struct BASE_API FSystem
	{
		FSystem() = delete;
		
		static voidp Instance();
		static uint32 ThreadId();
		static uint32 ProcessId();
		static void SetThreadDescription(FStringV Description);
		static void SetThreadDescription(uint32 ThreadId, FStringV Description);
		static void DebugOutput(FStringV String);
		static void DebugOutput(FWStringV String);
		static void MessageBox(FStringV Title, FStringV Content);

	public:
		static FSystemParameterInfo ParameterInfo;
	};
}
