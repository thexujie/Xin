#include "PCH.h"
#include "Strings.h"

#include "Platform/Platform.h"

namespace Xin
{
	static const uintx GStringBufferCount = 64;
	static const int GStringBufferMinLength = 4;

	template<typename CharT>
	static int32 GStringBufferIndex;

	template<typename CharT>
	static TString<CharT> GStringBuffers[GStringBufferCount];

	template<typename CharT>
	TString<CharT> & GetThreadLocalStringBuffer()
	{
		int32 Index = Atomics::IncFetch(GStringBufferIndex<CharT>) - 1;
		auto & ThreadLocalStringBuffer = GStringBuffers<CharT>[Index % GStringBufferCount];
		if (ThreadLocalStringBuffer.Capacity < GStringBufferMinLength)
			ThreadLocalStringBuffer.Resize(0, GStringBufferMinLength);
		else
			ThreadLocalStringBuffer.Resize(0);
		return ThreadLocalStringBuffer;
	}

	template BASE_API TString<chara> & GetThreadLocalStringBuffer<chara>();
	template BASE_API TString<charw> & GetThreadLocalStringBuffer<charw>();
	template BASE_API TString<char8> & GetThreadLocalStringBuffer<char8>();
	template BASE_API TString<char16> & GetThreadLocalStringBuffer<char16>();
	template BASE_API TString<char32> & GetThreadLocalStringBuffer<char32>();


	namespace Strings
	{
		FAStringV Printf(const char * Format, ...)
		{
			TString<char> & GConvertBufferAnsi = GetThreadLocalStringBuffer<char>();

			va_list ArgList;
			va_start(ArgList, Format);
			int32 NumAnsiChars = std::vsnprintf(GConvertBufferAnsi.GetData(), GConvertBufferAnsi.GetCapacityWithoutNull(), Format, ArgList);
			// error happend
			if (NumAnsiChars == -1)
				return FAStringV();

			if (NumAnsiChars > GConvertBufferAnsi.GetCapacityWithoutNull())
			{
				GConvertBufferAnsi.Resize(0, NumAnsiChars + 1);
				uintx NumAnsiCharsExpect = NumAnsiChars;
				NumAnsiChars = std::vsnprintf(GConvertBufferAnsi.GetData(), GConvertBufferAnsi.GetCapacityWithoutNull(), Format, ArgList);
				assert(NumAnsiCharsExpect == NumAnsiChars);
			}

			return FAStringV(GConvertBufferAnsi.GetData(), NumAnsiChars);
		}

		FUStringV Printf(const char8_t * Format, ...)
		{
			TString<char8_t> & ConvertBufferUtf8 = GetThreadLocalStringBuffer<char8_t>();

			va_list ArgList;
			va_start(ArgList, Format);
			int32 NumUtf8Chars = std::vsnprintf((char *)ConvertBufferUtf8.GetData(), ConvertBufferUtf8.GetCapacityWithoutNull(), (const char *)Format, ArgList);
			// error happend
			if (NumUtf8Chars == -1)
				return FUStringV();

			if (NumUtf8Chars > ConvertBufferUtf8.GetCapacityWithoutNull())
			{
				ConvertBufferUtf8.Resize(0, NumUtf8Chars + 1);
				uintx NumUtf8CharsExpect = NumUtf8Chars;
				NumUtf8Chars = std::vsnprintf((char *)ConvertBufferUtf8.GetData(), ConvertBufferUtf8.GetCapacityWithoutNull(), (const char *)Format, ArgList);
				assert(NumUtf8CharsExpect == NumUtf8Chars);
			}

			return FUStringV(ConvertBufferUtf8.GetData(), NumUtf8Chars);
		}

		FWStringV Printf(const wchar_t * Format, ...)
		{
			TString<wchar_t> & ConvertBufferWide = GetThreadLocalStringBuffer<wchar_t>();

			va_list ArgList;
			va_start(ArgList, Format);
			int32 NumWideChars = std::vswprintf(ConvertBufferWide.GetData(), ConvertBufferWide.GetCapacityWithoutNull(), Format, ArgList);
			// error happend
			if (NumWideChars == -1)
				return FWStringV();

			if (NumWideChars > ConvertBufferWide.GetCapacityWithoutNull())
			{
				ConvertBufferWide.Resize(0, NumWideChars + 1);
				uintx NumWideCharsExpect = NumWideChars;
				NumWideChars = std::vswprintf(ConvertBufferWide.GetData(), ConvertBufferWide.GetCapacityWithoutNull(), Format, ArgList);
				assert(NumWideCharsExpect == NumWideChars);
			}

			return FWStringV(ConvertBufferWide.GetData(), NumWideChars);
		}

		int32 ToBool(FStringV String)
		{
			return TStringToBoolean(String.Data, String.Size);
		}

		int32 ToInt32(FStringV String, uint32 Radix)
		{
			return ToInterger<char8, int32>(String.Data, String.Size);
		}

		uint32 ToUInt32(FStringV String, uint32 Radix)
		{
			return ToInterger<char8, uint32>(String.Data, String.Size);
		}

		int64_t ToInt64(FStringV String, uint32 Radix)
		{
			return ToInterger<char8, int32>(String.Data, String.Size);
		}

		uint64 ToUInt64(FStringV String, uint32 Radix)
		{
			return ToInterger<char8, uint64>(String.Data, String.Size);
		}

		intx ToIntX(FStringV String, uint32 Radix)
		{
			return ToInterger<char8, intx>(String.Data, String.Size);
		}

		uintx ToUIntX(FStringV String, uint32 Radix)
		{
			return ToInterger<char8, uintx>(String.Data, String.Size);
		}

		float32 ToFloat32(FStringV String)
		{
			return ToFloatingPoint<char8, float32>(String.Data, String.Size);
		}

		float64 ToFloat64(FStringV String)
		{
			return ToFloatingPoint<char8, float64>(String.Data, String.Size);
		}
	}
}
