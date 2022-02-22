#pragma once

#include "Core.Types.h"

namespace Xin
{
    class CORE_API FTicker
    {
    public:
        FTicker() = default;
        ~FTicker() = default;

        void Tick()
        {
            float64 CurrTime = FDateTime::HighResolution();
            DeltaTime = CurrTime - LastTime;
            LastTime = CurrTime;
        }

    public:
        float64 DeltaTime = 0.0f;

    private:
        float64 LastTime = FDateTime::HighResolution();
    };

    template<typename ValT, uintx NumValues>
    class FCounter
    {
    public:
        FCounter()
        {
            for (auto & Value : Values)
                Value = 0;
        }
    	
        FCounter(uint64 PeriodIn = 1000) : Period(PeriodIn)
        {
            for (auto & Value : Values)
                Value = 0;
        }

        void Accumulate(ValT Value)
        {
            uint64 Now = FDateTime::SteadyMilliseconds();
            if (Now - Last > Period)
            {
                Values[CurrIndex++ % NumValues] = CurrValue;
                CurrValue = 0;
                Last = Now;
            }
            CurrValue += Value;
        }
    	
        ValT Average() const
        {
            return std::accumulate(Values.begin(), Values.end(), static_cast<ValT>(0)) / Values.size();
        }

    private:
        uint64 Period;
        uint64 Last;
        std::array<ValT, NumValues> Values;

        int64_t CurrIndex = 0;
        ValT CurrValue = 0;
    };
}