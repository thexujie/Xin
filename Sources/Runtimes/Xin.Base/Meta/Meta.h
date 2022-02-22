#pragma once

#include "../Basic/Basic.h"

#define STATIC_TYPE \
	public: \
		static FType & StaticType();

#define META_DECL \
	public: \
		static FType & StaticType(); \
		FType & Type() const override { return StaticType(); }

#define STRUCT_DECL CLASS_DECL

namespace Xin
{
	struct FAttribute;
	struct FType;

	template<typename T>
	struct TAttribute;

	enum class EMetaKind
	{
		None = 0,
		Attribute,
		Method,
		Property,
		Type,
	};

	struct FMetaItem
	{
		auto operator<=>(const FMetaItem &) const = default;

		FName Name;
		FVariant Value;
	};

	struct BASE_API FMetadata
	{
	public:
		FMetadata() = default;
		FMetadata(const FMetadata &) = default;

		FMetadata(TView<FMetaItem> MetaItems) : Items(MetaItems) {}
		virtual ~FMetadata() = default;

		explicit operator bool() const { return this != &None; }

		const FVariant & operator [](FName Name) const
		{
			for (const FMetaItem & MetaItem : Items)
			{
				if (MetaItem.Name == Name)
					return MetaItem.Value;
			}
			return FVariant::None;
		}
		//public:
		//	EMetaKind Kind = EMetaKind::None;

	public:
		FStringV FriendlyName;
		TList<FMetaItem> Items;

		bool Hierarchical = false;

	public:
		static FMetadata None;
	};

	struct BASE_API FMeta
	{
	public: // discoypable
		FMeta(const FMeta &) = delete;
		FMeta(FMeta &&) = delete;
		FMeta & operator =(const FMeta &) = delete;
		FMeta & operator =(FMeta &&) = delete;
		bool operator ==(const FMeta & That) const noexcept { return Name.Nameid && this == &That; }
		auto operator <=>(const FMeta & That) const noexcept { return Compare(Name, That.Name, this, &That); }

	public:
		FMeta() = default;
		FMeta(FName Name, const FMetadata & Metadata) : Name(Name), Metadata(Metadata) {}
		virtual ~FMeta() = default;

		bool Valid() const { return Name.Valid(); }
		explicit operator bool() const { return Name.Valid(); }

	public:
		FName Name;
		const FMetadata & Metadata = FMetadata::None;
	};
}
