#include "PCH.h"
#include "Logger.h"

#include <iostream>

#include "Threading/Lock.h"

#ifdef XIN_WINDOWS
#include "Platform/Windows/WindowsPrivate.h"
#endif

namespace Xin
{
    FLogger::FLogger()
    {
        NeedDebugOutput = ::IsDebuggerPresent();
        if (::GetConsoleWindow())
            NeedConsoleOutput = true;

        static_assert(std::is_invocable_v<decltype(&FLogger::Thread_Write), FLogger *>);
        WriteThread = FThread(&FLogger::Thread_Write, this);
    }

    FLogger::FLogger(FStringV FileName, ELogLevel LogLevelIn) : LogLevel(LogLevelIn)
    {
#ifdef XIN_DEBUG
        NeedDebugOutput = true;
#ifdef XIN_WINDOWS
        if (::GetConsoleWindow())
            NeedConsoleOutput = true;
#endif
#endif
        Open(FileName);
        WriteThread = FThread(&FLogger::Thread_Write, this);
    }

    FLogger::~FLogger()
    {
        Close();
    }

    EError FLogger::Open(FStringV FileName)
    {
        if (FileName.Empty())
            return EError::Args;

        FileStream.Open(FileName, EFileModes::Output);
        FileStream.Seek(EStreamSeek::End, 0);
        return FileStream.Valid() ? EError::OK : EError::IO;
    }

    void FLogger::Close()
    {
        if (WriteThread.Joinable())
        {
            WriteExiting = true;
            WriteCondition.NotifyAll();
            WriteThread.Join();
        }
        FileStream.Flush();
        FileStream.Close();
    }

    void FLogger::Flush()
    {
        FileStream.Flush();
    }

    EError FLogger::LogToBuffer(uint32 ProcessId, uint32 ThreadId, ELogLevel LogLevelThis, FStringV Category, FStringV String)
    {
        if (LineCurrent == Line)
            return EError::OK;

        if (!WriteThread.Joinable())
            return EError::OK;

        LineCurrent = Line;
        if (ProcessId == NullIndex)
            ProcessId = FSystem::ProcessId();

        if (ThreadId == NullIndex)
            ThreadId = FSystem::ThreadId();

        if (LogLevelThis >= LogLevel)
        {
			uint64 TimeInMs = FDateTime::SystemNanoseconds() / 1000 / 1000;
            {
                TUniqueLock Lock(WriteMutex);
                LogItems.Emplace(LogItem{ LogLevelThis, uint64(TimeInMs), ProcessId, ThreadId, Category, String });
            }

#ifdef XIN_DEBUG
            {
                TUniqueLock Lock(SyncMutex);
                WriteCondition.NotifyAll();
                SyncCondition.Wait(Lock);
            }
#else
            WriteCondition.NotifyAll();
#endif
        }
        return EError::OK;
    }

    EError FLogger::Write(ELogLevel Level, FStringV Category, FStringV String)
    {
        EError err = LogToBuffer(NullIndex, NullIndex, Level, Category, String);
        ++Line;
        return err;
    }

    EError FLogger::WriteLine(ELogLevel Level, FStringV Category, FStringV String)
    {
        EError err = LogToBuffer(NullIndex, NullIndex, Level, Category, String);
        ++Line;
        return err;
    }

    void FLogger::Thread_Write()
    {
        FSystem::SetThreadDescription(u8"Logger_Write"V);
        while (!WriteExiting)
        {
            TList<LogItem> LogItemsWriting;
        	{
				TUniqueLock Lock(WriteMutex);
                WriteCondition.Wait(Lock, [this]() { return WriteExiting || !LogItems.Empty(); });
                LogItemsWriting = Move(LogItems);
        	}
        	
            if (LogItemsWriting.Empty())
                break;

            for (auto & LogItem : LogItemsWriting)
            {
                if (LoggerProxy)
                    LoggerProxy(LoggerProxyOpaque, LogItem.LogLevel, LogItem.TimeInMs, LogItem.ProcessId, LogItem.ThreadId, LogItem.String.Data, uint32(LogItem.String.Length));
                else
                {
                    time_t ms = time_t(LogItem.TimeInMs / 1000);
                    tm tm;
                    localtime_s(&tm, &ms);

                    char temp[128];
                    int32 nchars = _snprintf_s(temp, std::size(temp), "[%04d-%02d-%02d %02d:%02d:%02d %03d][PID:%-6d][TID:%-6d][%s][%s] ",
                        tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
                        tm.tm_hour, tm.tm_min, tm.tm_sec, uint32(LogItem.TimeInMs % 1000),
                        LogItem.ProcessId, LogItem.ThreadId,
                        (const char *)LogItem.Category.Data,
                        (const char *)LogLevelText[Clamp((int32)LogItem.LogLevel, 0, (int32)LogLevelText.Size)]);

                    FString log_text((const char8_t *)temp, nchars);
                    log_text += LogItem.String;

                    if (NeedDebugOutput)
                        FSystem::DebugOutput(log_text);

                    if (log_text.Empty() || log_text[log_text.GetSize() - 1] != u8'\n')
                        log_text += LineTag;

                    if (NeedConsoleOutput)
                        std::wcout << FEncoding::ToStdWString(/*LogItem.String*/log_text);
                	
                    FileStream.WriteData(log_text.Data, log_text.Length);
                }
            }
            FileStream.Flush();
#ifdef XIN_DEBUG
            {
                TUniqueLock Lock(SyncMutex);
                SyncCondition.NotifyAll();
            }
#endif
        }
    }

    static TReferPtr<FLogger> GGlobalLogger;

    FGlobalLogger::FGlobalLogger()
    {
        if (!GGlobalLogger)
            GGlobalLogger = new FLogger();
        SetLogger(GGlobalLogger);
    }

    FGlobalLogger::FGlobalLogger(FStringV FileName, ELogLevel LogLevel)
    {
        FStringV LogFileDirectory = FPath::GetDirectory(FileName);
        if (!FDirectory::Exists(LogFileDirectory))
            FDirectory::Create(FPath::Combine(FPath::GetWorkingDirectory(), LogFileDirectory));

        if (!GGlobalLogger)
            GGlobalLogger = new FLogger(FileName, LogLevel);
        SetLogger(GGlobalLogger);
    }

    FGlobalLogger::~FGlobalLogger()
    {
        SetLogger(nullptr);
        GGlobalLogger = nullptr;
    }

    void FGlobalLogger::SetProxy(void * Opaque, FunLoggerProxy LoggerProxy)
    {
        if (GGlobalLogger)
            GGlobalLogger->SetProxy(Opaque, LoggerProxy);
    }
}
