#pragma once

#include "Basic.Types.h"

namespace Xin
{
	struct BASE_API FTypeidEntry
	{
		uintx Identifier = 0;
#if XIN_DEBUG
		const void * DebugType = nullptr;
#endif

		FTypeidEntry() = default;
		FTypeidEntry(const FTypeidEntry &) = default;
		FTypeidEntry(FTypeidEntry &&) noexcept = default;
		FTypeidEntry & operator =(const FTypeidEntry &) noexcept = default;

		FTypeidEntry(const std::type_info & StdTypeInfo);
		bool operator ==(const FTypeidEntry & That) const noexcept { return Identifier == That.Identifier; }
		auto operator <=>(const FTypeidEntry & That) const noexcept { return Identifier <=> That.Identifier; }

		operator bool() const { return !!Identifier; }
	};

	struct BASE_API FTypeid
	{
	public:
		FTypeid() = default;
		FTypeid(const FTypeid &) = default;
		FTypeid(FTypeid &&) noexcept = default;
		FTypeid & operator =(const FTypeid &) noexcept = default;
		bool operator ==(const FTypeid &) const noexcept = default;
		auto operator <=>(const FTypeid &) const noexcept = default;
		~FTypeid() = default;

		FTypeid(const std::type_info & StdTypeInfo) : Entry(StdTypeInfo) { }
		operator bool() const { return !!Entry; }

	public:
		FTypeidEntry Entry {};

	public:
		static FTypeid None;

		static FStringV ExtractTypeName(const std::type_info & StdTypeInfo);
		static FStringV ExtractTypeName(FStringV Name);

		template<typename T>
		static FTypeid FromType() { return typeid(T); }
	};

	template<typename T>
	struct TTypeid : FTypeid
	{
		TTypeid() : FTypeid(typeid(T)) {}
	};
}
