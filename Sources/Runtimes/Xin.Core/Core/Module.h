#pragma once

#include "Core.Types.h"

namespace Xin
{
    class CORE_API FModule
    {
    public:
        FModule();
        FModule(const FString & ModulePath);
        ~FModule();
        FModule(const FModule & That);

        bool Load(FStringV ModulePath, bool bUserDir = false);
        void Free();
        explicit operator bool() const { return !!Handle; }

        void * GetProc(const char * name);

        template<typename T>
        T * GetProc(const char * name)
        {
            void * ptr = GetProc(name);
            return static_cast<T *>(ptr);
        }

        template<typename T>
        T * GetProc(const std::u8string & name)
        {
            return GetProc<T *>(name.c_str());
        }

        void * GetHandle() const { return Handle; }

    public:
        void * Handle = nullptr;
        FString ModulePath;

    public:
        static FModule Current();
        static FModule Application();
    };
}

