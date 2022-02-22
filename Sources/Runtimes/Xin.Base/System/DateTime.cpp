#include "PCH.h"
#include "DateTime.h"

#define _CRT_NO_TIME_T
#include <time.h>
#include <sys/timeb.h>

namespace Xin
{
    //static_assert(sizeof(FDateTime) == sizeof(int) * 3);
    static int64_t _curr_timezone()
    {
        long timezone = 0;
        _get_timezone(&timezone);
        return static_cast<int64_t>(timezone);
    }

    FDateTime::FDateTime(uint64 Nanosecond_)
    {
        __time64_t Seconds = Nanosecond_ / 1'000'000'000;
        tm tmtime;
        _gmtime64_s(&tmtime, &Seconds);
        Year = (uint16)tmtime.tm_year + 1900;
        Month = (uint8)tmtime.tm_mon + 1;
        Day = (uint8)tmtime.tm_mday;
        Hour = (uint8)tmtime.tm_hour;
        Minute = (uint8)tmtime.tm_min;
        Second = (uint8)tmtime.tm_sec;
        Millisecond = (Nanosecond_ / 1'000'000) % 1'000;
        Microsecond = (Nanosecond_ / 1'000) % 1'000;
        Nanosecond = (Nanosecond_ / 1) % 1'000;
    }
     
    uint64 FDateTime::GetNanoseconds() const
    {
        tm tmtime = {};
        tmtime.tm_year = Year - 1900;
        tmtime.tm_mon = Month - 1;
        tmtime.tm_mday = Day;
        tmtime.tm_hour = Hour;
        tmtime.tm_min = Minute;
        tmtime.tm_sec = Second;
        int64_t Seconds = _mkgmtime64(&tmtime);
        return Seconds * 1'000'000'000 +
            Millisecond * 1'000'000 +
            Microsecond * 1'000 +
            Nanosecond;
    }
	
    FStringV FDateTime::Format(FStringV Formal) const
    {
        if (Formal.Empty())
            Formal = u8"yy-MM-dd HH:mm:ss fff"V;

        uintx Count = 0;
        char8 ChLast = 0;

        FString & String = Xin::GetThreadLocalStringBuffer<char8>();
        auto FunFlushInner = [&]()
        {
            if (Count == 1)
                return Xin::Format(u8"{}"V, ChLast);

            switch (ChLast)
            {
            case 'y':
                if (Count == 2)
                    return Xin::Format(u8"{:02}"V, Year);
                if (Count == 4)
                    return Xin::Format(u8"{:04}"V, Year);
                break;
            case 'M':
                if (Count == 2)
                    return Xin::Format(u8"{:02}"V, Month);
                break;
            case 'd':
                if (Count == 2)
                    return Xin::Format(u8"{:02}"V, Day);
                break;
            case 'h':
                if (Count == 2)
                    return Xin::Format(u8"{:02}"V, Hour % 12);
                break;
            case 'H':
                if (Count == 2)
                    return Xin::Format(u8"{:02}"V, Hour);
                break;
            case 'm':
                if (Count == 2)
                    return Xin::Format(u8"{:02}"V, Minute);
                break;
            case 's':
                if (Count == 2)
                    return Xin::Format(u8"{:02}"V, Second);
                break;
            case 'f':
                if (Count == 1)
                    return Xin::Format(u8"{:1}"V, Millisecond / 100);
                if (Count == 2)
                    return Xin::Format(u8"{:02}"V, Millisecond / 10);
                if (Count == 3)
                    return Xin::Format(u8"{:03}"V, Millisecond);
                break;
            default:
                break;
            }
            return FStringV::None;
        };

        auto FunFlush = [&]()
        {
            String.Append(FunFlushInner());
        };

        for (uintx Index = 0; Index < Formal.Size; ++Index)
        {
            char8 Ch = Formal[Index];
            if (!ChLast)
                ChLast = Ch;

            if (Ch != ChLast)
            {
                FunFlush();
                ChLast = Ch;
                Count = 1;
            }
            else if (Count == 3)
            {
                ++Count;
                FunFlush();
                ChLast = 0;
                Count = 0;
            }
            else
                ++Count;
        }

        if (Count > 0)
            FunFlush();
        return String;
    }

    bool FDateTime::IsLeapYear(uint32 Year)
    {
        return (Year % 4 == 0 && Year % 100 != 0) || (Year % 400 == 0);
    }

    EDayWeek FDateTime::DayWeek(uint32 Year, uint32 Month, uint32 Day)
    {
        if (Month < 1 || 12 < Month)
            return EDayWeek::Monday;

        static uint32 DAY[] = { 6, 2, 1, 4, 6, 2, 4, 0, 3, 5, 1, 3 };
        Year -= Month < 3;
        return static_cast<EDayWeek>((Year + Year / 4 - Year / 100 + Year / 400 + DAY[Month - 1] + Day) % 7);
    }

    uint32 FDateTime::DayYear(uint32 Year, uint32 Month, uint32 Day)
    {
        if (Month < 1 || 12 < Month)
            return 0;

        static uint32 DAY[] = { 1, 0, 2, 0, 3, 0, 4, 5, 0, 6, 0, 7 };
        uint32 Temp = (Month - 1) * 30 + DAY[Month - 1] + Day;
        if (Month > 2)
            Temp -= IsLeapYear(Year) ? 1 : 2;
        return Temp;
    }

    FDateTime FDateTime::Current()
    {
        return FDateTime((uint64)((int64_t)SystemNanoseconds() + _curr_timezone() * 1'000'000'000));
    }

    float64 FDateTime::System()
    {
        return SystemNanoseconds() / 1'000'000'000.0;
    }

    uint64 FDateTime::SystemMilliseconds()
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    }

    uint64 FDateTime::SystemMicroseconds()
    {
        return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    }

    uint64 FDateTime::SystemNanoseconds()
    {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    }

    float64 FDateTime::Steady()
    {
        return SteadyNanoseconds() / 1'000'000'000.0;
    }

	uint32 FDateTime::SteadyMilliseconds()
    {
        return (uint32)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
    }

    uint64 FDateTime::SteadyMicroseconds()
    {
        return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
    }

    uint64 FDateTime::SteadyNanoseconds()
    {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
    }

    float64 FDateTime::HighResolution()
    {
        return HighResolutionNanoseconds() / 1'000'000'000.0;
    }

    uint64 FDateTime::HighResolutionMilliseconds()
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    }
	
    uint64 FDateTime::HighResolutionMicroseconds()
    {
        return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    }
	
    uint64 FDateTime::HighResolutionNanoseconds()
    {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    }
}
