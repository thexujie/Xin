#include "PCH.h"
#include "Log.h"
#include "System.h"

namespace Xin
{
    class FNullLogger : public ILoggerStream
    {
    public:
        EError Write(ELogLevel Level, FStringV Category, FStringV String) override
        {
#if !XIN_PUBLISH
            FSystem::DebugOutput(Format(u8"[{}][{}] {}"V, LogLevelText[Level], Category, String));
#endif
            return EError::OK;
        }
        EError WriteLine(ELogLevel Level, FStringV Category, FStringV String) override
        {
#if !XIN_PUBLISH
            FSystem::DebugOutput(Format(u8"[{}][{}] {}"V, LogLevelText[Level], Category, String));
#endif
            return EError::OK;
        }
    };

    static ILoggerStreamRef GLogger;
    static FNullLogger GNullLogger;

	void SetLogger(ILoggerStreamRef Logger)
	{
		GLogger = Logger;
	}

	ILoggerStream & Logger()
	{
		return GLogger ? GLogger.Ref() : GNullLogger;
	}
}
