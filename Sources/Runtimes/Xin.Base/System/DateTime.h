#pragma once

#include "../Basic/Basic.h"

namespace Xin
{
    enum class EDayWeek
    {
        Monday = 0,
        Tuesday,
        Wednesday,
        Thursday,
        Friday,
        Saturday,
        Sunday
    };

    class BASE_API FDateTime
    {
    public:
        static FType & StaticType();

        FDateTime() = default;
        FDateTime(const FDateTime &) = default;
        FDateTime & operator =(const FDateTime & Another) = default;
        friend auto operator <=> (const FDateTime &, const FDateTime &) = default;

        FDateTime(uint64 Nanosecond);
        FDateTime(uint32 Year, uint32 Month, uint32 Day, uint32 Hour, uint32 Minute, uint32 Second,
            uint32 Millisecond = 0, uint32 Microsecond = 0, uint32 Nanosecond = 0)
            : Year(Year), Month(Month), Day(Day), Hour(Hour), Minute(Minute), Second(Second)
            , Millisecond(Millisecond), Microsecond(Microsecond), Nanosecond(Nanosecond)
        {
        }

    	// https://docs.microsoft.com/en-us/dotnet/standard/base-types/custom-date-and-time-format-strings
        FStringV Format(FStringV Formal = FStringV::None) const;

        uint64 GetNanoseconds() const;

    public:
        //! Year [-, -]
        uint32 Year = 1990;
        //! Month [1, 12]
        uint32 Month  = 1;
        //! Day [1, 31]
        uint32 Day = 1;
    	
        //! Hour [0, 23]
        uint32 Hour = 0;
        //! Minute [0, 59]
        uint32 Minute = 0;
        //! Second [0, 59]
        uint32 Second = 0;
        //! Millisecond [0, 999]
        uint32 Millisecond = 0;
        //! Microsecond [0, 999]
        uint32 Microsecond = 0;
        //! Nanosecond [0, 999]
        uint32 Nanosecond = 0;
    	
    public:
        static bool IsLeapYear(uint32 Year);
        static EDayWeek DayWeek(uint32 Year, uint32 Month, uint32 Day);
        static uint32 DayYear(uint32 Year, uint32 Month, uint32 Day);

        static FDateTime Current();

    public:
        static float64 System();
        static uint64 SystemMilliseconds();
        static uint64 SystemMicroseconds();
        static uint64 SystemNanoseconds();
    	
        static float64 Steady();
        static uint32 SteadyMilliseconds();
        static uint64 SteadyMicroseconds();
        static uint64 SteadyNanoseconds();
    	
        static float64 HighResolution();
        static uint64 HighResolutionMilliseconds();
        static uint64 HighResolutionMicroseconds();
    	
        static uint64 HighResolutionNanoseconds();
    };
}
