#include "PCH.h"
#include "Encoding.h"

#include "Platform/Platform.h"

namespace Xin
{
	extern template TString<chara> & GetThreadLocalStringBuffer<chara>();
	extern template TString<charw> & GetThreadLocalStringBuffer<charw>();
	extern template TString<char8> & GetThreadLocalStringBuffer<char8>();
	extern template TString<char16> & GetThreadLocalStringBuffer<char16>();
	extern template TString<char32> & GetThreadLocalStringBuffer<char32>();

	FEncoding FEncoding::ANSI { 0 };
	FEncoding FEncoding::UTF8 { 65001 };

	template<typename CharT>
	UINT GetCharCodePage()
	{
		if constexpr (IsSameV<CharT, chara>) return CP_ACP;
		if constexpr (IsSameV<CharT, char8>) return CP_UTF8;
		else return 0;
	}

	template<typename InT, typename OutT>
	TView<OutT> EncodeString(const InT * Source, uintx SourceLength)
	{
		if (SourceLength == NullIndex)
			SourceLength = Length(Source);

		if (!SourceLength)
			return {};

		if constexpr (IsSameV<InT, OutT>)
			return TView<OutT>(Source, SourceLength);

		TView<charw> WideString;
		if constexpr (IsSameV<InT, charw>)
			WideString = { Source, SourceLength };
		else
		{
			FWString & ConvertBufferWide = GetThreadLocalStringBuffer<charw>();
			UINT InputCodePage = GetCharCodePage<InT>();
			int32 NumWideChars = MultiByteToWideChar(InputCodePage, 0, (const chara *)(Source), int32(SourceLength), ConvertBufferWide.GetData(), (int32)ConvertBufferWide.GetCapacityWithoutNull());
			if (NumWideChars == 0)
			{
				if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
					return TView<OutT> {};

				NumWideChars = MultiByteToWideChar(InputCodePage, 0, (const chara *)(Source), int32(SourceLength), nullptr, 0);
				if (NumWideChars == 0)
					return TView<OutT> {};

				ConvertBufferWide.Resize(NumWideChars);
				int32 NumWideChars2 = MultiByteToWideChar(InputCodePage, 0, (const chara *)(Source), int32(SourceLength), ConvertBufferWide.GetData(), (int32)ConvertBufferWide.GetCapacityWithoutNull());
				assert(NumWideChars2 == NumWideChars);
			}
			else
				ConvertBufferWide.SetSize(NumWideChars);
			WideString = ConvertBufferWide;
		}

		if constexpr (IsSameV<OutT, charw>)
			return WideString;
		if constexpr (IsSameV<OutT, char16>)
			return { (const char16 *)WideString.Data, WideString.Size };
		else
		{
			TString<OutT> & ConvertBufferOut = GetThreadLocalStringBuffer<OutT>();
			UINT OutputCodePage = GetCharCodePage<OutT>();
			int32 NumOutChars = WideCharToMultiByte(OutputCodePage, 0, WideString.Data, static_cast<int32>(WideString.Size), reinterpret_cast<chara *>(ConvertBufferOut.Data), (int32)ConvertBufferOut.GetCapacityWithoutNull(), nullptr, nullptr);
			if (NumOutChars == 0)
			{
				if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
					return {};

				NumOutChars = WideCharToMultiByte(OutputCodePage, 0, WideString.Data, static_cast<int32>(WideString.Size), nullptr, 0, nullptr, nullptr);
				if (NumOutChars == 0)
					return {};

				ConvertBufferOut.Resize(NumOutChars);
				int32 NumOutChars2 = WideCharToMultiByte(OutputCodePage, 0, WideString.Data, static_cast<int32>(WideString.Size), reinterpret_cast<chara *>(ConvertBufferOut.Data), (int32)ConvertBufferOut.GetCapacityWithoutNull(), nullptr, nullptr);
				assert(NumOutChars2 == NumOutChars);
			}
			else
				ConvertBufferOut.SetSize(NumOutChars);
			return ConvertBufferOut;
		}
	}

	FAStringV FEncoding::ToAnsi(const charw * String, uintx StringLength)
	{
		return EncodeString<charw, chara>(String, StringLength);
	}

	FAStringV FEncoding::ToAnsi(const char8 * String, uintx StringLength)
	{
		return EncodeString<char8, chara>(String, StringLength);
	}

	FAStringV FEncoding::ToAnsi(const char16 * String, uintx StringLength)
	{
		return EncodeString<char16, chara>(String, StringLength);
	}

	FAStringV FEncoding::ToAnsi(const char32 * String, uintx StringLength)
	{
		return EncodeString<char32, chara>(String, StringLength);
	}

	FWStringV FEncoding::ToWide(const chara * String, uintx StringLength)
	{
		return EncodeString<chara, charw>(String, StringLength);
	}

	FWStringV FEncoding::ToWide(const char8 * String, uintx StringLength)
	{
		return EncodeString<char8, charw>(String, StringLength);
	}

	FWStringV FEncoding::ToWide(const char16 * String, uintx StringLength)
	{
		return EncodeString<char16, charw>(String, StringLength);
	}

	FWStringV FEncoding::ToWide(const char32 * String, uintx StringLength)
	{
		return EncodeString<char32, charw>(String, StringLength);
	}

	FUStringV FEncoding::ToUtf8(const chara * String, uintx StringLength)
	{
		return EncodeString<chara, char8>(String, StringLength);
	}

	FUStringV FEncoding::ToUtf8(const charw * String, uintx StringLength)
	{
		return EncodeString<charw, char8>(String, StringLength);
	}

	FUStringV FEncoding::ToUtf8(const char16 * String, uintx StringLength)
	{
		return EncodeString<char16, char8>(String, StringLength);
	}

	FUStringV FEncoding::ToUtf8(const char32 * String, uintx StringLength)
	{
		return EncodeString<char32, char8>(String, StringLength);
	}

	FU16StringV FEncoding::ToUtf16(const chara * String, uintx StringLength)
	{
		return EncodeString<chara, char16>(String, StringLength);
	}

	FU16StringV FEncoding::ToUtf16(const charw * String, uintx StringLength)
	{
		return EncodeString<charw, char16>(String, StringLength);
	}

	FU16StringV FEncoding::ToUtf16(const char8 * String, uintx StringLength)
	{
		return EncodeString<char8, char16>(String, StringLength);
	}

	FU16StringV FEncoding::ToUtf16(const char32 * String, uintx StringLength)
	{
		return EncodeString<char32, char16>(String, StringLength);
	}

	FU32StringV FEncoding::ToUtf32(const chara * String, uintx StringLength)
	{
		return EncodeString<chara, char32>(String, StringLength);
	}

	FU32StringV FEncoding::ToUtf32(const charw * String, uintx StringLength)
	{
		return EncodeString<charw, char32>(String, StringLength);
	}

	FU32StringV FEncoding::ToUtf32(const char8 * String, uintx StringLength)
	{
		return EncodeString<char8, char32>(String, StringLength);
	}

	FU32StringV FEncoding::ToUtf32(const char16 * String, uintx StringLength)
	{
		return EncodeString<char16, char32>(String, StringLength);
	}
}
