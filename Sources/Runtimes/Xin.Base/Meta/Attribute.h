#pragma once

#include "Meta.h"

namespace Xin
{
	struct BASE_API FAttribute : FMeta
	{
	public:
		FAttribute() = default;
		FAttribute(FName Name, const FMetadata & Metadata = FMetadata::None) : FMeta(Name, Metadata) {}

	public:
		static FAttribute None;
	};

	template<typename T>
	struct TAttribute : FAttribute
	{
	public:
		TAttribute() = default;
		TAttribute(FName Name, T Value, const FMetadata & Metadata = FMetadata::None) : FAttribute(Name, Metadata), Value(Value) {}

	public:
		T Value;

	public:
		static TAttribute<T> None;
	};

	template<typename T>
	TAttribute<T> TAttribute<T>::None;

	struct FAncestorTypeAttribute : public FAttribute
	{
		FAncestorTypeAttribute() : FAttribute(u8"AncestorType"N) {}
	};

	struct FFriendlyNameAttribute : public TAttribute<FName>
	{
		FFriendlyNameAttribute() = default;
		FFriendlyNameAttribute(FName FriendlyName, const FMetadata & Metadata = FMetadata::None) : TAttribute(u8"FriendlyName"N, FriendlyName, Metadata) {}
	};
}
