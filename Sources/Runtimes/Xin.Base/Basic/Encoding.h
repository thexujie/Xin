#pragma once

#include "Basic.Types.h"

#include "Strings.h"
#include "Tuple.h"

namespace Xin
{
	class BASE_API FEncoding
	{
	public:
		FEncoding() = default;
		FEncoding(uint32 CodePage) : CodePage(CodePage) {}
		bool operator ==(const FEncoding &) const noexcept = default;

		bool Valid() const { return CodePage != UInt32Max; }

	public:
		uint32 CodePage = UInt32Max;

	public:
		static FEncoding ANSI;
		static FEncoding UTF8;

	public:
		/**
		 * 
		 * @param Utf32 
		 * @param Utf8 
		 * @param Length 
		 * @return num of utf8s
		 */
		static uint32 Utf32ToUtf8(const char32 & Utf32, char8 * Utf8, uint32 Length)
		{
			if (Utf32 <= 0x7F && Length > 0)
			{
				Utf8[0] = (char)Utf32;
				return 1;
			}
			if (Utf32 <= 0x7FF && Length > 2)
			{
				Utf8[0] = char8(0xC0 | (Utf32 >> 6)); /* 110xxxxx */
				Utf8[1] = char8(0x80 | (Utf32 & 0x3F)); /* 10xxxxxx */
				return 2;
			}
			if (Utf32 <= 0xFFFF && Length > 3)
			{
				Utf8[0] = char8(0xE0 | (Utf32 >> 12)); /* 1110xxxx */
				Utf8[1] = char8(0x80 | ((Utf32 >> 6) & 0x3F)); /* 10xxxxxx */
				Utf8[2] = char8(0x80 | (Utf32 & 0x3F)); /* 10xxxxxx */
				return 3;
			}
			if (Utf32 <= 0x10FFFF && Length > 4)
			{
				Utf8[0] = char8(0xF0 | (Utf32 >> 18)); /* 11110xxx */
				Utf8[1] = char8(0x80 | ((Utf32 >> 12) & 0x3F)); /* 10xxxxxx */
				Utf8[2] = char8(0x80 | ((Utf32 >> 6) & 0x3F)); /* 10xxxxxx */
				Utf8[3] = char8(0x80 | (Utf32 & 0x3F)); /* 10xxxxxx */
				return 4;
			}
			return 0;
		}

		/**
		 * 
		 * @param Utf8 
		 * @param Length 
		 * @param Utf32 
		 * @return num of utf8s
		 */
		static uint32 Utf8ToUtf32(const char8 * Utf8, uint32 Length, char32 & Utf32)
		{
			if (!Length)
			{
				Utf32 = 0;
				return 0;
			}

			const uint8 * Start = (const uint8 *)Utf8;
			const uint8 * Curr = (const uint8 *)Utf8;
			uint32 Crtl = *Start;
			Utf32 = *Curr++;

			uint32 Mask = ~0x7Fu;
			if (Crtl & 0x80)
			{
				Mask = ~0x3Fu;
				Crtl <<= 1;
				do
				{
					if (Curr - Start > Length)
					{
						Utf32 = 0;
						return 0;
					}

					Utf32 <<= 6;
					Utf32 |= ((*Curr++) & 0x3F);
					Crtl <<= 1;

					Mask <<= 5;
				}
				while (Crtl & 0x80);
			}
			Utf32 &= ~Mask;
			return uint32(Curr - Start);
		}

		static TTuple<char32, uint32> Utf8ToUtf32(const char8 Utf8[], uint32 Length)
		{
			char32 Utf32;
			uint32 NumUtf8s = Utf8ToUtf32(Utf8, Length, Utf32);
			return { Utf32, NumUtf8s };
		}

		static uint32 Utf32ToUtf16(char32 Utf32, char16 Utf16[2], uint32 Length)
		{
			if (!Length)
				return 0;

			if (Utf32 >= 0x10000)
			{
				if (Length < 2)
					return 0;

				constexpr uint16 Mask10Bits = 0b1111111111;

				Utf16[0] = uint16(0xD800 | (((uint32(Utf32) - 0x10000) >> 10) & Mask10Bits));
				Utf16[1] = uint16(0xDC00 | (((uint32(Utf32) - 0x10000) >> 00) & Mask10Bits));
				return 2;
			}
			else
			{
				Utf16[0] = uint16(Utf32);
				return 1;
			}
		}

		static uint32 Utf16ToUtf32(const char16 Utf16[], uint32 Length, char32 & Utf32)
		{
			if (!Length)
				return 0;

			uint16 Ch1 = Utf16[0];
			if (Ch1 >= 0xD800 && Ch1 <= 0xDFFF)
			{
				if (Ch1 < 0xDC00 && Length > 1)
				{
					uint16 Ch2 = Utf16[1];
					if (Ch2 >= 0xDC00 && Ch2 <= 0xDFFF)
					{
						Utf32 = (Ch2 & 0x03FF) + (((Ch1 & 0x03FF) + 0x40) << 10);
						return 2;
					}
				}
				return 0;
			}
			else
			{
				Utf32 = Ch1;
				return 1;
			}
		}

		static TTuple<char32, uint32> Utf16ToUtf32(const char16 Utf16[], uint32 Length)
		{
			char32 Utf32;
			uint32 NumUtf16s = Utf16ToUtf32(Utf16, Length, Utf32);
			return { Utf32, NumUtf16s };
		}
	public:
		// -> FAStringV
		static FAStringV ToAnsi(const charw * String, uintx StringLength = NullIndex);
		static FAStringV ToAnsi(FWStringV String) { return ToAnsi(String.Data, String.Size); }
		static FAStringV ToAnsi(const std::wstring_view & String){ return ToAnsi(String.data(), String.size()); }

		static FAStringV ToAnsi(const char8 * String, uintx StringLength = NullIndex);
		static FAStringV ToAnsi(FUStringV String) { return ToAnsi(String.Data, String.Size); }
		static FAStringV ToAnsi(const std::u8string_view & String){ return ToAnsi(String.data(), String.size()); }

		static FAStringV ToAnsi(const char16 * String, uintx StringLength = NullIndex);
		static FAStringV ToAnsi(FU16StringV String) { return ToAnsi(String.Data, String.Size); }
		static FAStringV ToAnsi(const std::u16string_view & String){ return ToAnsi(String.data(), String.size()); }

		static FAStringV ToAnsi(const char32 * String, uintx StringLength = NullIndex);
		static FAStringV ToAnsi(FU32StringV String) { return ToAnsi(String.Data, String.Size); }
		static FAStringV ToAnsi(const std::u32string_view & String){ return ToAnsi(String.data(), String.size()); }

		// -> FWStringV
		static FWStringV ToWide(const chara * String, uintx StringLength = NullIndex);
		static FWStringV ToWide(FAStringV String) { return ToWide(String.Data, String.Size); }
		static FWStringV ToWide(const std::string_view & String){ return ToWide(String.data(), String.size()); }

		static FWStringV ToWide(const char8 * String, uintx StringLength = NullIndex);
		static FWStringV ToWide(FUStringV String) { return ToWide(String.Data, String.Size); }
		static FWStringV ToWide(const std::u8string_view & String){ return ToWide(String.data(), String.size()); }

		static FWStringV ToWide(const char16 * String, uintx StringLength = NullIndex);
		static FWStringV ToWide(FU16StringV String) { return ToWide(String.Data, String.Size); }
		static FWStringV ToWide(const std::u16string_view & String){ return ToWide(String.data(), String.size()); }

		static FWStringV ToWide(const char32 * String, uintx StringLength = NullIndex);
		static FWStringV ToWide(FU32StringV String) { return ToWide(String.Data, String.Size); }
		static FWStringV ToWide(const std::u32string_view & String){ return ToWide(String.data(), String.size()); }

		// -> FU8StringV
		static FUStringV ToUtf8(const chara * String, uintx StringLength = NullIndex);
		static FUStringV ToUtf8(FAStringV String) { return ToUtf8(String.Data, String.Size); }
		static FUStringV ToUtf8(const std::string_view & String){ return ToUtf8(String.data(), String.size()); }

		static FUStringV ToUtf8(const charw * String, uintx StringLength = NullIndex);
		static FUStringV ToUtf8(FWStringV String) { return ToUtf8(String.Data, String.Size); }
		static FUStringV ToUtf8(const std::wstring_view & String){ return ToUtf8(String.data(), String.size()); }

		static FUStringV ToUtf8(const char16 * String, uintx StringLength = NullIndex);
		static FUStringV ToUtf8(FU16StringV String) { return ToUtf8(String.Data, String.Size); }
		static FUStringV ToUtf8(const std::u16string_view & String){ return ToUtf8(String.data(), String.size()); }

		static FUStringV ToUtf8(const char32 * String, uintx StringLength = NullIndex);
		static FUStringV ToUtf8(FU32StringV String) { return ToUtf8(String.Data, String.Size); }
		static FUStringV ToUtf8(const std::u32string_view & String){ return ToUtf8(String.data(), String.size()); }

		// -> FU16StringV
		static FU16StringV ToUtf16(const chara * String, uintx StringLength = NullIndex);
		static FU16StringV ToUtf16(FAStringV String) { return ToUtf16(String.Data, String.Size); }
		static FU16StringV ToUtf16(const std::string_view & String){ return ToUtf16(String.data(), String.size()); }

		static FU16StringV ToUtf16(const charw * String, uintx StringLength = NullIndex);
		static FU16StringV ToUtf16(FWStringV String) { return ToUtf16(String.Data, String.Size); }
		static FU16StringV ToUtf16(const std::wstring_view & String){ return ToUtf16(String.data(), String.size()); }

		static FU16StringV ToUtf16(const char8 * String, uintx StringLength = NullIndex);
		static FU16StringV ToUtf16(FUStringV String) { return ToUtf16(String.Data, String.Size); }
		static FU16StringV ToUtf16(const std::u8string_view & String){ return ToUtf16(String.data(), String.size()); }

		static FU16StringV ToUtf16(const char32 * String, uintx StringLength = NullIndex);
		static FU16StringV ToUtf16(FU32StringV String) { return ToUtf16(String.Data, String.Size); }
		static FU16StringV ToUtf16(const std::u32string_view & String){ return ToUtf16(String.data(), String.size()); }

		// -> FU32StringV
		static FU32StringV ToUtf32(const chara * String, uintx StringLength = NullIndex);
		static FU32StringV ToUtf32(FAStringV String) { return ToUtf32(String.Data, String.Size); }
		static FU32StringV ToUtf32(const std::string_view & String){ return ToUtf32(String.data(), String.size()); }

		static FU32StringV ToUtf32(const charw * String, uintx StringLength = NullIndex);
		static FU32StringV ToUtf32(FWStringV String) { return ToUtf32(String.Data, String.Size); }
		static FU32StringV ToUtf32(const std::wstring_view & String){ return ToUtf32(String.data(), String.size()); }

		static FU32StringV ToUtf32(const char8 * String, uintx StringLength = NullIndex);
		static FU32StringV ToUtf32(FUStringV String) { return ToUtf32(String.Data, String.Size); }
		static FU32StringV ToUtf32(const std::u8string_view & String){ return ToUtf32(String.data(), String.size()); }

		static FU32StringV ToUtf32(const char16 * String, uintx StringLength = NullIndex);
		static FU32StringV ToUtf32(FU16StringV String) { return ToUtf32(String.Data, String.Size); }
		static FU32StringV ToUtf32(const std::u16string_view & String){ return ToUtf32(String.data(), String.size()); }


		static std::string_view AsStdString(FAStringV String) { return std::string_view(String.Data, String.Size); }
		static std::string_view AsStdString(FUStringV String) { return std::string_view((const chara *)String.Data, String.Size); }

		static std::wstring_view AsStdWString(FWStringV String) { return std::wstring_view(String.Data, String.Size); }
		static std::wstring_view AsStdWString(FU16StringV String) { return std::wstring_view((const charw *)String.Data, String.Size); }

		static FAStringV AsAnsi(const chara * String, uintx StringLength = NullIndex) { return { String, StringLength }; }
		static FAStringV AsAnsi(std::string_view String) { return { (const chara *)(String.data()), String.size() }; }
		static FAStringV AsAnsi(FUStringV String) { return { (const chara *)(String.Data), String.Size }; }

		static FWStringV AsWide(const char16 * String, uintx StringLength = NullIndex) { return { (const charw *)String, StringLength }; }
		static FWStringV AsWide(FU16StringV String) { return { (const charw *)String.Data, String.Size }; }
		static FWStringV AsWide(std::u8string_view String) { return { (const charw *)String.data(), String.size() }; }

		static FUStringV AsUtf8(const chara * String, uintx StringLength = NullIndex) { return { (const char8 *)String, StringLength }; }
		static FUStringV AsUtf8(std::string_view String) { return { (const char8 *)(String.data()), String.size() }; }

		static FU16StringV AsUtf16(const charw * String, uintx StringLength = NullIndex) { return { (const char16 *)String, StringLength }; }
		static FU16StringV AsUtf16(FWStringV String) { return AsUtf16(String.Data, String.Size ); }
		static FU16StringV AsUtf16(std::wstring_view String) { return AsUtf16(String.data(), String.size() ); }

		static std::string ToStdString(FAStringV String) { return std::string(AsStdString(String)); }
		static std::string ToStdString(FUStringV String) { return ToStdString(ToAnsi(String)); }
		static std::string ToStdString(FWStringV String) { return ToStdString(ToAnsi(String)); }

		static std::wstring ToStdWString(FAStringV String) { return ToStdWString(ToWide(String)); }
		static std::wstring ToStdWString(FWStringV String) { return std::wstring(String); }
		static std::wstring ToStdWString(FUStringV String) { return ToStdWString(ToWide(String)); }

		static std::string ToStdUString(FAStringV String) { return ToStdUString(ToUtf8(String)); }
		static std::string ToStdUString(FUStringV String) { return std::string(AsStdString(String)); }
		static std::string ToStdUString(FWStringV String) { return ToStdUString(ToUtf8(String)); }
	};
}
