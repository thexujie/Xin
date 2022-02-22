#pragma once

#include "Basic.Types.h"
#include "Formatter.h"
#include "Refer.h"

namespace Xin
{
	enum ELogLevel
	{
		LogLevel_Dbg = 0,
		LogLevel_Inf,
		LogLevel_War,
		LogLevel_Err,
	};
	constexpr TArray<const char8 *, 4> LogLevelText = { u8"Dbg", u8"Inf", u8"War", u8"Err" };

	class BASE_API ILoggerStream : public ISharedRefer
	{
	public:
		virtual ~ILoggerStream() = default;
		virtual EError Write(ELogLevel Level, FStringV Category, FStringV String) = 0;
		virtual EError WriteLine(ELogLevel Level, FStringV Category, FStringV String) = 0;
	};
	using ILoggerStreamRef = TReferPtr<ILoggerStream>;

	BASE_API void SetLogger(ILoggerStreamRef Logger);
	BASE_API ILoggerStream & Logger();

	void Log(ELogLevel LogLevel, FStringV String);

	template<typename ...ArgsT>
	void LogDbg(FStringV Formal, ArgsT &&...Args)
	{
#ifdef XIN_DEBUG
		Log(LogLevel_Dbg, Format(Formal, Forward<ArgsT>(Args)...));
#endif
	}

	inline void LogInfo(FStringV Content)
	{
		Log(LogLevel_Inf, Content);
	}

	template<typename ...ArgsT>
	void LogInfo(FStringV Formal, ArgsT &&...Args)
	{
		Log(LogLevel_Inf, Format(Formal, Forward<ArgsT>(Args)...));
	}

	inline void LogWarning(FStringV Content)
	{
		Log(LogLevel_War, Content);
	}

	template<typename ...ArgsT>
	void LogWarning(FStringV Formal, ArgsT &&...Args)
	{
		Log(LogLevel_War, Format(Formal, Forward<ArgsT>(Args)...));
	}

	inline void LogError(FStringV Content)
	{
		Log(LogLevel_Err, Content);
	}

	template<typename ...ArgsT>
	void LogError(FStringV Formal, ArgsT &&...Args)
	{
		Log(LogLevel_Err, Format(Formal, Forward<ArgsT>(Args)...));
	}
}
