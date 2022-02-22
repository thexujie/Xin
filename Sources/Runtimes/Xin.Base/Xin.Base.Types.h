#pragma once

#pragma warning(disable: 4251 4455 5030 5222)

// https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-2-c4275
#pragma warning(disable: 4275)

// struct_name' : structure was padded due to __declspec(align())
#pragma warning(disable: 4324)

// https://learn.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-1-c4558
#pragma warning(disable: 4458)

// unreachable code
// https://learn.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-4-c4702
#pragma warning(disable: 4702)

// occurs when you compile an ANSI source file on a system using a codepage that can't represent all characters in the file.
#pragma warning(disable: 4819)

// 'std::is_constant_evaluated' always evaluates to true in manifestly constant-evaluated expressions
#pragma warning(disable: 5063)

#define XIN_CORE 1

#if defined(__Xin_Debug__)
#define XIN_DEBUG 1
#define XIN_DEVELOPMENT 0
#define XIN_RELEASE 0
#define XIN_PUBLISH 0
#define XIN_TEST 1
#define XIN_CONFIG "Debug"
#elif defined(__Xin_Development__)
#define XIN_CONFIG "Development"
#define XIN_DEBUG 0
#define XIN_DEVELOPMENT 1
#define XIN_RELEASE 0
#define XIN_PUBLISH 0
#define XIN_TEST 1
#endif

#ifdef _WIN32
#define XIN_WINDOWS 1

#ifdef _WINDLL
#define XIN_LIB_STATIC 0
#define XIN_LIB_DYNAMIC 1
#elif _LIB
#define XIN_LIB_STATIC 1
#define XIN_LIB_DYNAMIC 0
#else
#define XIN_LIB_STATIC 0
#define XIN_LIB_DYNAMIC 0
#endif

#endif

#if XIN_WINDOWS
#define XIN_EXPORT __declspec(dllexport)
#define XIN_IMPORT __declspec(dllimport)
#else
#define XIN_EXPORT
#define XIN_IMPORT
#endif

#define NONE_EXPORT
#define NONE_API NONE_EXPORT

#ifdef _M_X64
#define XIN_X64 1
#define XIN_BITS 64
#define XIN_PLATFORM "x64"
#else
#define XIN_X86 1
#define XIN_BITS 32
#define XIN_PLATFORM "x86"
#endif

#define forceinline __forceinline
#define forcenoinline __declspec(noinline)

#ifdef _Xin_Base_Module_
#define BASE_API XIN_EXPORT
#define BASE_VAR XIN_EXPORT
#else
#define BASE_API XIN_IMPORT
#define BASE_VAR XIN_IMPORT
#endif

#define ENUM_STRING_BEGIN(Value) switch(Value) {
#define ENUM_STRING_CASE(Enum, Name) case Enum::Name: return u8"" #Name ##V;
#define ENUM_STRING_END default: return {}; }

#define MACRO_STRING_HELPER(M) #M
#define MACRO_STRING(M) MACRO_STRING_HELPER(M)


#define PropertyRW(Type, Reader, Writer) __declspec(property(get = Reader, put = Writer)) Type
#define PropertyR(Type, Reader) __declspec(property(get = Reader)) Type
#define PropertyW(Type, Writer) __declspec(property(put = Writer)) Type

#define XinAttributes(...) static_assert(true)
#define XinType(...) static FType & StaticType()
#define PropertyMetadata(...) static_assert(true)

#define XinFunction(...) static_assert(true)
#define XinProperty(...) static_assert(true)

#define overwrite

#pragma warning(disable: 4201)
#pragma warning(disable: 4100)

#define NOMINMAX 1

//#ifdef _MSC_VER
//#define _CRTIMP2_PURE_IMPORT
//#endif

#include <memory.h>
#include <assert.h>
#include <stdint.h>
#include <stdarg.h>
#include <intrin.h>

//#include <cstring>
//#include <cmath>
//#include <climits>
//#include <cctype>

#include <stdexcept>
#include <type_traits>
#include <cstddef>
#include <locale>
#include <string>
#include <format>
#include <sstream>
#include <iomanip>
#include <type_traits>
#include <limits>
#include <exception>
#include <array>
#include <future>
#include <fstream>
#include <compare>

#include <numeric>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <bitset>

#include <random>
#include <filesystem>
#include <execution>
#include <source_location>

namespace Xin
{
	template<typename T>
	using TInitializerList = std::initializer_list<T>;
}

