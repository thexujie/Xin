#pragma once

#include "Basic.Types.h"

namespace Xin
{
	constexpr uintx NameHash(FAStringV NameString)
	{
		uintx HashCode = 0;
		for (char8 Ch : NameString)
		{
			HashCode ^= Ch;
			HashCode *= HashPrivate::FNVPrime;
		}
		return HashCode;
	}

	constexpr uintx NameHash(FStringV NameString)
	{
		uintx HashCode = 0;
		for (char8 Ch : NameString)
		{
			HashCode ^= Ch;
			HashCode *= HashPrivate::FNVPrime;
		}
		return HashCode;
	}

	struct BASE_API FNameid
	{
	public:
		constexpr FNameid() noexcept = default;
		constexpr FNameid(const FNameid & That) noexcept = default;
		constexpr FNameid(uintx Identifier) noexcept : Identifier(Identifier) {}

		FNameid(FStringV Name);

		constexpr FNameid & operator =(const FNameid & That) noexcept = default;
		constexpr bool operator ==(const FNameid & That) const noexcept = default;
		constexpr auto operator <=>(const FNameid & That) const noexcept = default;

		template<CharacterC CharT, uintx FixedSize>
		constexpr FNameid(const TFixedString<CharT, FixedSize> & FixedString) : Identifier(NameHash({ FixedString.Data, FixedSize })) { }

		template<uintx Size>
		constexpr FNameid(const char8 (& NameString)[Size]) : Identifier(NameHash({ NameString, Size })) { }

		constexpr bool Valid() const { return !!Identifier; }
		constexpr explicit operator bool() const { return Valid(); }

	public:
		uintx Identifier = 0;

	public:
		static FNameid None;
		friend uintx Hash(const FNameid & This) { return Hash(This.Identifier); }
	};

	class BASE_API INameEntry
	{
	public:
		virtual ~INameEntry() = default;
		virtual FStringV Resolve() const = 0;
	};

	struct BASE_API FName
	{
	public:
		constexpr FName() noexcept = default;
		constexpr FName(const FName & That) noexcept = default;
		constexpr FName(const char8 * String) : Nameid(NameHash(String)), NameEntry(String) {}

		FName(FStringV Name);
		
		constexpr FName & operator =(const FName & That) noexcept
		{
			Nameid = That.Nameid;
			NameEntry = That.NameEntry;
			return *this;
		}

		constexpr bool operator ==(const FName & That) const noexcept { return Nameid == That.Nameid; }
		constexpr auto operator <=>(const FName & That) const noexcept { return Nameid <=> That.Nameid; }

		template<uintx Size>
		FName(const char8 (& NameString)[Size]) : FName({ NameString, Size }) { }

		FStringV Resolve() const { return (uintx(NameEntry) & 1) ? ((const INameEntry *)(uintx(NameEntry) & ~1))->Resolve() : FStringV(NameEntry); }

		bool Valid() const { return !!Nameid; }
		explicit operator bool() const { return Valid(); }

	public:
		FNameid Nameid;
		const char8 * NameEntry = nullptr;

	public:
		static FName None;
		friend uintx Hash(const FName & This) { return Hash(This.Nameid); }
	};

	extern template BASE_API FType & Typeof<FName>();

	template<TFixedString FixedString>
	constexpr FName MakeFixedName()
	{
		FName Name;
		Name.Nameid = FNameid(FixedString);
		Name.NameEntry = FixedString.Data;
		return Name;
	}

	constexpr FName operator ""N(const char8_t * String, uintx Length)
	{
		return FName(String);
	}

	constexpr FName operator ""N(const char * String, uintx Length)
	{
		union
		{
			const char8_t * U8String;
			const char * AString;
		};
		AString = String;
		return FName(U8String);
	}

	template<>
	struct TFormatter<FName>
	{
		FStringV Format(const FName & Value, FStringV Formal = FStringV::None) const
		{
			return Value.Resolve();
		}
	};
}
