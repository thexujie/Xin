#pragma once

#include "Basic.Types.h"
#include "Name.h"

namespace Xin
{
	class BASE_API ITextEntry
	{
	public:
		virtual ~ITextEntry() = default;
		virtual FStringV Resolve() const = 0;
	};

	struct BASE_API FText
	{
	public:
		FText() noexcept = default;
		FText(const FText & That) noexcept = default;
		FText(FStringV String);

		FText & operator =(const FText & That) noexcept = default;
		bool operator ==(const FText & That) const noexcept { return Nameid == That.Nameid; }
		auto operator <=>(const FText & That) const noexcept { return Nameid <=> That.Nameid; }

		template<uintx Size>
		FText(const char8 (& NameString)[Size]) : FText({ NameString, Size }) { }

		FStringV Resolve() const { return TextEntry ? TextEntry->Resolve() : FStringV::None; }

		bool Valid() const { return !!Nameid; }
		explicit operator bool() const { return Valid(); }

	public:
		FNameid Nameid;

	protected:
		const ITextEntry * TextEntry = nullptr;

	public:
		static FText None;
	};

	extern template BASE_API FType & Typeof<FText>();

	inline FText operator ""T(const char8_t * String, uintx Length)
	{
		return FText({ String, Length });
	}

	inline FText operator ""T(const char * String, uintx Length)
	{
		return FText({ (const char8_t *)String, Length });
	}

	template<>
	struct TFormatter<FText>
	{
		FStringV Format(const FText & Value, FStringV Formal = FStringV::None) const
		{
			return Value.Resolve();
		}
	};
}
