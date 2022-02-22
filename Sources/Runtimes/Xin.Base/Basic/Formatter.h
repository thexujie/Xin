#pragma once

#include "Strings.h"
#include "Locale.h"


#define XIN_USE_LIBFMT 1

#if XIN_USE_LIBFMT
#define FMT_HEADER_ONLY
#define FMT_UNICODE 1
#include <fmt/format.h>
#include <fmt/os.h>

namespace fmt
{
	//---------------------------------------------------------------- char8_t
	template <> struct is_char<char8_t> : std::true_type {};

	template<typename ...ArgsT>
	using u8format_string = basic_format_string<char8_t, ArgsT...>;

	using u8string_view = basic_string_view<char8_t>;
	using u8format_args = basic_format_args<buffered_context<char8_t>>;

	template <typename OutputIt, FMT_ENABLE_IF(detail::is_output_iterator<remove_cvref_t<OutputIt>, char8_t>::value)>
	auto vformat_to(OutputIt && out, u8string_view fmt, u8format_args args) -> remove_cvref_t<OutputIt>
	{
		auto && buf = detail::get_buffer<char8_t>(out);
		detail::vformat_to<char8_t>(buf, fmt, args, { });
		return detail::get_iterator(buf, out);
	}

	template <typename OutputIt, FMT_ENABLE_IF(detail::is_output_iterator<OutputIt, char8_t>::value)>
	auto vformat_to(OutputIt out, const std::locale & locale, u8string_view fmt, u8format_args args) -> remove_cvref_t<OutputIt>
	{
		auto && buf = detail::get_buffer<char8_t>(out);
		detail::vformat_to<char8_t>(buf, fmt, args, detail::locale_ref(locale));
		return detail::get_iterator(buf, out);
	}


	//---------------------------------------------------------------- wchar_t
	template <> struct is_char<wchar_t> : std::true_type {};

	template<typename ...ArgsT>
	using wformat_string = basic_format_string<wchar_t, ArgsT...>;

	using wstring_view = basic_string_view<wchar_t>;
	using wformat_args = basic_format_args<buffered_context<wchar_t>>;

	template <typename OutputIt,
		FMT_ENABLE_IF(detail::is_output_iterator<OutputIt, wchar_t>::value)>
	auto vformat_to(OutputIt out, wstring_view fmt, wformat_args args) -> OutputIt
	{
		auto && buf = detail::get_buffer<wchar_t>(out);
		detail::vformat_to<wchar_t>(buf, fmt, args, { });
		return detail::get_iterator(buf, out);
	}

	template <typename OutputIt, FMT_ENABLE_IF(detail::is_output_iterator<OutputIt, wchar_t>::value)>
	auto vformat_to(OutputIt out, const std::locale & locale, wstring_view fmt, wformat_args args) -> remove_cvref_t<OutputIt>
	{
		auto && buf = detail::get_buffer<wchar_t>(out);
		detail::vformat_to<wchar_t>(buf, fmt, args, detail::locale_ref(locale));
		return detail::get_iterator(buf, out);
	}
}
#else
namespace std
{
	using _Fmt_u8it = back_insert_iterator<_Fmt_buffer<char8_t>>;
	using u8format_context = basic_format_context<_Fmt_u8it, char8_t>;
	using u8format_args = basic_format_args<u8format_context>;

	template <class... _Args>
	using u8format_string = basic_format_string<char8_t, type_identity_t<_Args>...>;

	template <class... _Args>
	_NODISCARD auto make_u8format_args(_Args&&... _Vals) {
		return _Format_arg_store<u8format_context, _Args...>{_Vals...};
	}

	//template<output_iterator<const char8_t &> _OutputIt>
	//_OutputIt vformat_to(_OutputIt _Out, const u8string_view _Fmt, const u8format_args _Args) {
	//	if constexpr (is_same_v<_OutputIt, _Fmt_u8it>) {
	//		_Format_handler<char8_t> _Handler(_Out, _Fmt, _Args);
	//		_Parse_format_string(_Fmt, _Handler);
	//		return _Out;
	//	}
	//	else {
	//		_Fmt_iterator_buffer<_OutputIt, char8_t> _Buf(_STD move(_Out));
	//		_Format_handler<char8_t> _Handler(_Fmt_u8it { _Buf }, _Fmt, _Args);
	//		_Parse_format_string(_Fmt, _Handler);
	//		return _Buf._Out();
	//	}
	//}
	_EXPORT_STD template <output_iterator<const char8_t&> _OutputIt>
		_OutputIt vformat_to(_OutputIt _Out, const u8string_view _Fmt, const wformat_args _Args) {
		return _Format_to_it(_STD move(_Out), _Fmt, _Args, _Lazy_locale{});
	}

	_EXPORT_STD template <output_iterator<const char8_t&> _OutputIt, class... _Types>
		_OutputIt format_to(_OutputIt _Out, const /*u8format_string<_Types...>*/u8string_view _Fmt, _Types&&... _Args) {
		return _STD vformat_to(_STD move(_Out), _Fmt, _STD make_format_args(_Args...));
		//return _Format_to_it(_STD move(_Out), _Fmt, _Args, _Lazy_locale{});
	}

	//template <bool _Statically_Utf8>
	//class _Fmt_codec<char8_t, _Statically_Utf8> {
	//public:
	//	_NODISCARD constexpr const char8_t * _Find_encoded(
	//		const char8_t * const _First, const char8_t * const _Last, const char8_t _Val) const {
	//		return _Find_unchecked(_First, _Last, _Val);
	//	}
	//	_NODISCARD constexpr int _Units_in_next_character(
	//		const char8_t * _First, const char8_t * const _Last) const noexcept {
	//		char32_t _Ch;
	//		const auto _Next = _Decode_utf((const char *)_First, (const char *)_Last, _Ch);
	//		_STL_INTERNAL_CHECK(_Next - _First <= 4);
	//		return static_cast<int>((const char8_t *)_Next - _First);
	//	}
	//};
}
#endif

namespace Xin
{
	template<IntegerC T>
	struct TConverter<T>
	{
		T Convert(FStringV String, int32 Radis = 0) const
		{
			if constexpr (IsCharacterV<T>)
				return T(String[0]);
			else
				return ToInterger<char8, T>(String.Data, String.Size, Radis);
		}
	};

	template<FloatingPointC T>
	struct TConverter<T>
	{
		T Convert(FStringV String) const
		{
			return ToFloatingPoint<char8, T>(String.Data, String.Size);
		}
	};

	template<typename CharT>
	struct TConverter<TView<CharT>>
	{
		TView<CharT> Convert(TView<CharT> String) const
		{
			return String;
		}
	};

	template<>
	struct TConverter<FString>
	{
		FString Convert(FStringV String) const
		{
			return String;
		}
	};

	template<>
	struct TFormatter<FString>
	{
		FStringV Format(const FStringV & Value, FStringV) const
		{
			return Value;
		}
	};
}

namespace Xin
{
	template<typename T>
	constexpr TView<T> DefaultFormal;

	template<>
	constexpr TView<chara> DefaultFormal<chara> = "{}"V;

	template<>
	constexpr TView<charw> DefaultFormal<charw> = L"{}"V;

	template<>
	constexpr TView<char8> DefaultFormal<char8> = u8"{}"V;

	template<>
	constexpr TView<char16> DefaultFormal<char16> = u"{}"V;

	template<>
	constexpr TView<char32> DefaultFormal<char32> = U"{}"V;


	template<typename T, typename CharT = char8>
	concept SpecializedFormatAbleC = requires(const T & Instance, TView<CharT> Formal)
	{
		{ TFormatter<T>().Format(Instance, Formal) } -> ConvertibleToC<TView<CharT>>;
	};
	template<typename T, CharacterC CharT = char8>
	inline constexpr bool IsSpecializedFormatAbleV = SpecializedFormatAbleC<T, CharT>;

	template<typename T, typename CharT = char8>
	concept FunctionFormatAbleC = requires(const T & Instance, TView<CharT> Formal)
	{
		Instance.Format(Formal);
	};
	template<typename T, CharacterC CharT = char8>
	inline constexpr bool IsFunctionFormatAbleV = FunctionFormatAbleC<T, CharT>;

	template<typename T, CharacterC CharT = char8>
	inline constexpr bool IsAdapterFormatAbleV = AdaptableC<T, TView<CharT>>;

	template<typename T, typename CharT = char8>
	concept AdapterFormatAbleC = IsAdapterFormatAbleV<T, CharT>;

	template<typename T>
	inline constexpr bool IsSystemFormatAbleV = IsIntegerV<T> || IsFloatingPointV<T> || IsAnyOfV<T, void *, const void *
		, std::monostate
#if XIN_USE_LIBFMT
		, fmt::monostate
#endif
	>;
	template<CharacterC T>
	inline constexpr bool IsSystemFormatAbleV<T *> = true;

	template<CharacterC T>
	inline constexpr bool IsSystemFormatAbleV<const T *> = true;

	template<CharacterC T, uintx Length>
	inline constexpr bool IsSystemFormatAbleV<T[Length]> = true;

	template<CharacterC T>
	inline constexpr bool IsSystemFormatAbleV<TView<T>> = true;

	template<CharacterC T>
	inline constexpr bool IsSystemFormatAbleV<std::basic_string_view<T>> = true;

#if XIN_USE_LIBFMT
	template<CharacterC T>
	inline constexpr bool IsSystemFormatAbleV<fmt::basic_string_view<T>> = true;
#endif

	template<typename T>
	inline constexpr bool IsNoneSystemFormatAbleV = !IsSystemFormatAbleV<T>;

	template<typename T>
	concept NoneSystemFormatAbleC = IsNoneSystemFormatAbleV<T>;

	template<typename T, CharacterC CharT = char8>
	inline constexpr bool IsFormatAbleV = IsSystemFormatAbleV<T> || IsSpecializedFormatAbleV<T, CharT> || IsFunctionFormatAbleV<T, CharT> || IsAdapterFormatAbleV<T, CharT> || IsEnumV<T>;

	template<typename T, CharacterC CharT = char8> struct IsFormatAble : BoolConstant<IsFormatAbleV<T, CharT>> {};

	template<typename T, typename CharT = char8> concept FormatAbleC = IsFormatAbleV<T, CharT>;

	template<typename CharT>
	std::basic_string<CharT> & GetThreadLocalFormatBuffer();
}

#if XIN_USE_LIBFMT
namespace fmt
#else
namespace std
#endif
{
	template<>
	struct formatter<char, wchar_t> : formatter<wchar_t, wchar_t>
	{
		using basic_formatter = formatter<wchar_t, wchar_t>;

		template<typename FormatContext>
		auto format(char Ch, FormatContext & Context) const
		{
			return basic_formatter::format(wchar_t(Ch), Context);
		}
	};

	template<>
	struct formatter<char8_t, wchar_t> : formatter<wchar_t, wchar_t>
	{
		using basic_formatter = formatter<wchar_t, wchar_t>;

		template<typename FormatContext>
		auto format(char8_t Ch, FormatContext & Context) const
		{
			return basic_formatter::format(wchar_t(Ch), Context);
		}
	};

	template<>
	struct formatter<char, char8_t> : formatter<char8_t, char8_t>
	{
		using basic_formatter = formatter<char8_t, char8_t>;

		template<typename FormatContext>
		auto format(char Ch, FormatContext & Context) const
		{
			return basic_formatter::format(char8_t(Ch), Context);
		}
	};

	template<>
	struct formatter<const char *, char8_t> : formatter<std::basic_string_view<char8_t>, char8_t>
	{
		using basic_formatter = formatter<std::basic_string_view<char8_t>, char8_t>;

		template<typename FormatContext>
		auto format(const char * String, FormatContext & Context) const
		{
			return basic_formatter::format(std::basic_string_view<char8_t>((const char8_t *)String), Context);
		}
	};

	template<>
	struct formatter<std::basic_string_view<char>, char8_t> : formatter<std::basic_string_view<char8_t>, char8_t>
	{
		using basic_formatter = formatter<std::basic_string_view<char8_t>, char8_t>;

		template<typename FormatContext>
		auto format(std::basic_string_view<char> String, FormatContext & Context) const
		{
			return basic_formatter::format(std::basic_string_view<char8_t>((const char8_t *)String.data(), String.size()), Context);
		}
	};

	template<>
	struct formatter<Xin::TView<char>, char8_t> : formatter<std::basic_string_view<char8_t>, char8_t>
	{
		using basic_formatter = formatter<std::basic_string_view<char8_t>, char8_t>;

		template<typename FormatContext>
		auto format(Xin::TView<char> String, FormatContext & Context) const
		{
			return basic_formatter::format(std::basic_string_view<char8_t>((const char8_t *)String.Data, String.Size), Context);
		}
	};

	template<Xin::EnumC T, typename CharT>
	struct formatter<T, CharT> : formatter<std::underlying_type_t<T>, CharT>
	{
		using basic_formatter = formatter<std::underlying_type_t<T>, CharT>;

		template<typename FormatContext>
		auto format(const T & Value, FormatContext & Context) const
		{
			return basic_formatter::format(std::underlying_type_t<T>(Value), Context);
		}
	};

	template<typename CharT>
	struct formatter<Xin::TView<CharT>, CharT> : formatter<std::basic_string_view<CharT>, CharT>
	{
		using basic_formatter = formatter<std::basic_string_view<CharT>, CharT>;

		template<typename FormatContext>
		auto format(Xin::TView<CharT> String, FormatContext & Context) const
		{
			return basic_formatter::format(std::basic_string_view<CharT>(String.Data, String.Size), Context);
		}
	};

	template<typename T, typename CharT> requires Xin::IsFunctionFormatAbleV<T, CharT>
	struct formatter<T, CharT> : formatter<std::basic_string_view<CharT>, CharT>
	{
		using string_view_type = std::basic_string_view<CharT>;
		using basic_formatter = formatter<std::basic_string_view<CharT>, CharT>;

		Xin::TView<CharT> Formal = Xin::DefaultFormal<CharT>;

		constexpr auto parse(basic_format_parse_context<CharT> & Context) 
		{
			size_t IterDepth = 0;
			auto Iter = Context.begin(), IterEnd = Context.end();
			while (Iter != IterEnd)
			{
				if (*Iter == u8'{')
				{
					if (Iter > Context.begin() && *(Iter - 1) != u8'{')
						++IterDepth;
				}
				else if (*Iter == u8'}')
				{
					if (Iter > Context.begin() && *(Iter - 1) != u8'}')
					{
						if (IterDepth == 0)
							break;
						--IterDepth;
					}
				}
				else { }
				++Iter;
			}

			if (Iter != Context.begin())
			{
#if XIN_USE_LIBFMT
				Formal = Xin::TView<CharT>(Context.begin(), size_t(Iter - Context.begin()));
#else
				Formal = Xin::TView<CharT>(Context.begin().operator->(), std::size_t(Iter - Context.begin()));
#endif
			}
			return Iter;
		}

		template<typename FormatContext>
		auto format(const T & Value, FormatContext & Context) const
		{
			return basic_formatter::format(static_cast<string_view_type>(Value.Format(Formal)), Context);
		}
	};


	template<typename T, typename CharT> requires Xin::IsSpecializedFormatAbleV<T, CharT>
	struct formatter<T, CharT> : formatter<std::basic_string_view<CharT>, CharT>
	{
		using string_view_type = std::basic_string_view<CharT>;
		using basic_formatter = formatter<std::basic_string_view<CharT>, CharT>;

		Xin::TView<CharT> Formal = Xin::DefaultFormal<CharT>;

		constexpr auto parse(basic_format_parse_context<CharT> & Context) 
		{
			size_t IterDepth = 0;
			auto Iter = Context.begin(), IterEnd = Context.end();
			while (Iter != IterEnd)
			{
				if (*Iter == u8'{')
				{
					if (Iter > Context.begin() && *(Iter - 1) != u8'{')
						++IterDepth;
				}
				else if (*Iter == u8'}')
				{
					if (Iter > Context.begin() && *(Iter - 1) != u8'}')
					{
						if (IterDepth == 0)
							break;
						--IterDepth;
					}
				}
				else { }
				++Iter;
			}

			if (Iter != Context.begin())
			{
#if XIN_USE_LIBFMT
				Formal = Xin::TView<CharT>(Context.begin(), size_t(Iter - Context.begin()));
#else
				Formal = Xin::TView<CharT>(Context.begin().operator->(), std::size_t(Iter - Context.begin()));
#endif
			}
			return Iter;
		}

		template<typename FormatContext>
		auto format(const T & Value, FormatContext & Context) const
		{
			return basic_formatter::format(static_cast<string_view_type>(Xin::TFormatter<T>().Format(Value, Formal)), Context);
		}
	};

	template<typename T, typename CharT> requires Xin::IsAdapterFormatAbleV<T, CharT>
	struct formatter<T, CharT> : formatter<std::basic_string_view<CharT>, CharT>
	{
		using string_view_type = std::basic_string_view<CharT>;
		using basic_formatter = formatter<std::basic_string_view<CharT>, CharT>;

		Xin::TView<CharT> Formal = Xin::DefaultFormal<CharT>;

		constexpr auto parse(basic_format_parse_context<CharT> & Context) 
		{
			size_t IterDepth = 0;
			auto Iter = Context.begin(), IterEnd = Context.end();
			while (Iter != IterEnd)
			{
				if (*Iter == u8'{')
				{
					if (Iter > Context.begin() && *(Iter - 1) != u8'{')
						++IterDepth;
				}
				else if (*Iter == u8'}')
				{
					if (Iter > Context.begin() && *(Iter - 1) != u8'}')
					{
						if (IterDepth == 0)
							break;
						--IterDepth;
					}
				}
				else { }
				++Iter;
			}

			if (Iter != Context.begin())
			{
#if XIN_USE_LIBFMT
				Formal = Xin::TView<CharT>(Context.begin(), size_t(Iter - Context.begin()));
#else
				Formal = Xin::TView<CharT>(Context.begin().operator->(), std::size_t(Iter - Context.begin()));
#endif
			}
			return Iter;
		}

		template<typename FormatContext>
		auto format(const T & Value, FormatContext & Context) const
		{
			return basic_formatter::format(static_cast<string_view_type>(Xin::TAdapter<T, Xin::FStringV>()(Value)), Context);
		}
	};

}

namespace Xin
{
	template<typename CharT, typename ...ArgTs>
	TView<CharT> Format(TView<CharT> Formal, ArgTs &&...Args)
	{
		static_assert((IsFormatAbleV<RemoveCVRefT<ArgTs>, CharT> && ...), " Custom type need a TFormatter<T> or ToString() member.");

		std::basic_string<CharT> & FormatBuffer = GetThreadLocalFormatBuffer<CharT>();
#if XIN_USE_LIBFMT
		fmt::vformat_to(std::back_inserter(FormatBuffer),
			fmt::basic_format_string<CharT, ArgTs...>(fmt::runtime_format_string<CharT>(fmt::basic_string_view<CharT>(Formal.Data, Formal.Size))),
			fmt::make_format_args<fmt::buffered_context<CharT>>(Args...));
#else
		std::format_to(std::back_inserter(FormatBuffer), std::basic_string_view<CharT>(Formal.Data, Formal.Size), std::make_wformat_args(Args...));
#endif
		return TView<CharT>(FormatBuffer.data(), FormatBuffer.size());
	}

	template<typename CharT, typename ...ArgTs>
	TView<CharT> Format(const FLocale & Local, TView<CharT> Formal, const ArgTs &...Args)
	{
		static_assert((IsFormatAbleV<RemoveCVRefT<ArgTs>, CharT> && ...), " Custom type need a TFormatter<T> or ToString() member.");

		std::basic_string<CharT> & FormatBuffer = GetThreadLocalFormatBuffer<CharT>();
#if XIN_USE_LIBFMT
		fmt::vformat_to(std::back_inserter(FormatBuffer), Local.Locale,
			fmt::basic_string_view<CharT>(Formal.Data, Formal.Size),
			fmt::make_format_args<fmt::buffered_context<CharT>>(Args...));
#else
		std::format_to(std::back_inserter(FormatBuffer), Local.Locale, std::basic_string_view<CharT>(Formal.Data, Formal.Size), std::make_wformat_args(Args...));
#endif
		return TView<CharT>(FormatBuffer.data(), FormatBuffer.size());
	}

	template<typename T>
	template<typename ... ArgsT>
	TView<T> TView<T>::Format(const ArgsT &... Args) requires CharacterC<T>
	{
		return Xin::Format(*this, Args...);
	}

	template<typename T>
	template<typename ... ArgsT>
	TView<T> TSlice<T>::Format(const ArgsT &... Args) requires CharacterC<T>
	{
		return Xin::Format(*this, Args...);
	}
}
