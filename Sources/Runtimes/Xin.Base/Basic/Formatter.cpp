#include "PCH.h"
#include "Formatter.h"

#if XIN_USE_LIBFMT
#define FMT fmt::
#else
#define FMT std::
#endif

namespace Xin
{
	void Test()
	{
		//FMT buffered_context<char8> & bc = *(FMT buffered_context<char8> *)nullptr;
		//FMT formatter<char, char8>().format('a', bc);

		//int ival = 2432323;
		//auto ff = std::format(std::locale("en_US"), L"{:L}K", ival);

		//std::wstring str;
		//std::wstring wstr;
		//auto res = FMT format_to(std::back_inserter(str), "{:L}K", ival);

		//auto iter = std::back_inserter(wstr);
		//auto && buf = FMT detail::get_buffer<wchar_t>(iter);
		//FMT detail::vformat_to<wchar_t>(buf, L"{:L}K", FMT make_format_args<FMT buffered_context<wchar_t>>(ival), { });

		////return detail::get_iterator(buf, out);

		//auto fmtw = FMT vformat_to(std::back_inserter(wstr), "{:L}K", FMT make_format_args(ival));
		////auto fmtu = FMT vformat_to(std::back_inserter(wstr), L"{:L}K", 2334);

		//auto fmt2 = Format(L"{:L}K"V, 2334);
	}

	static constexpr uintx GStringBufferCount = 64;

	enum EFormatBuffer
	{
		EFormatBuffer_Ansi = 0,
		EFormatBuffer_Utf8,
		EFormatBuffer_Wide,
		EFormatBuffer_Count,
	};

	template<typename CharT>
	static int32 GFormatBufferIndex;
	
	template<typename CharT>
	static std::basic_string<CharT> GFormatBuffers[GStringBufferCount];
	
	template<typename CharT>
	std::basic_string<CharT> & GetThreadLocalFormatBuffer()
	{
		int32 Index = Atomics::IncFetch(GFormatBufferIndex<CharT>) - 1;
		auto & ThreadLocalFormatBuffer = GFormatBuffers<CharT>[Index % GStringBufferCount];
		ThreadLocalFormatBuffer.clear();
		return ThreadLocalFormatBuffer;
	}

	template BASE_API std::basic_string<char> & GetThreadLocalFormatBuffer<char>();
	template BASE_API std::basic_string<char8_t> & GetThreadLocalFormatBuffer<char8_t>();
	template BASE_API std::basic_string<wchar_t> & GetThreadLocalFormatBuffer<wchar_t>();
}
