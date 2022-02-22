#pragma once

#include "Core.Types.h"
#include "Thread.h"
#include "Threading/ConditionalVariable.h"
#include "Threading/Mutex.h"

namespace Xin
{
    using FunLoggerProxy = TFunction<void(void * Opaque, uint32 LogLevel, uint64 TimeInMs, uint32 ProcessId, uint32 ThreadId, const char8 * Data, uint32 Length)>;
	
    class CORE_API FLogger : public ILoggerStream
    {
    public:
        FLogger();
        FLogger(FStringV FileName, ELogLevel LogLevelIn);
        FLogger(const FLogger &) = delete;
        FLogger(FLogger && Another) = delete;
        ~FLogger();

        void SetLogLevel(ELogLevel LogLevelIn) { LogLevel = LogLevelIn; }
        ELogLevel GetLogLevel() const { return LogLevel; }
        void SetProxy(void * Opaque, FunLoggerProxy LoggerProxyIn)
        {
	        LoggerProxyOpaque = Opaque;
        	LoggerProxy = LoggerProxyIn;
        }
        EError Open(FStringV FileName);
        void Close();
        void Flush();

        EError LogToBuffer(uint32 ProcessId, uint32 ThreadId, ELogLevel LogLevelThis, FStringV Category, FStringV String);

        EError Write(ELogLevel Level, FStringV Category, FStringV String);
        EError WriteLine(ELogLevel Level, FStringV Category, FStringV String);

    private:
        void Thread_Write();

    private:
        FMutex WriteMutex;
        FThread WriteThread;
        FConditionalVariable WriteCondition;
        std::atomic<bool> WriteExiting = false;
    	
#ifdef XIN_DEBUG
        // write log synchronic on debug
        FMutex SyncMutex;
        FConditionalVariable SyncCondition;
#endif

        FAtomicBool NeedConsoleOutput = false;
        FAtomicBool NeedDebugOutput = false;

        ELogLevel LogLevel = ELogLevel::LogLevel_Dbg;
        FString LineTag = u8"\r\n"V;
        FFileStream FileStream;
        void * LoggerProxyOpaque = nullptr;
        FunLoggerProxy LoggerProxy;
        struct LogItem
        {
            ELogLevel LogLevel;
            uint64 TimeInMs;
            uint32 ProcessId;
            uint32 ThreadId;
            FString Category;
            FString String;
        };
        TList<LogItem> LogItems;
        int64_t Line = 0;
        int64_t LineCurrent = -1;
    };

    class CORE_API FGlobalLogger
    {
    public:
        FGlobalLogger();
        FGlobalLogger(FStringV FileName, ELogLevel LogLevel);
        ~FGlobalLogger();

        void SetProxy(void * Opaque, FunLoggerProxy LoggerProxy);
    };
}
