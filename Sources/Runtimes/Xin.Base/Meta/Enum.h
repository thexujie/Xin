#pragma once

#include "Type.h"

namespace Xin
{
	struct BASE_API FEnumItem : FMeta
	{
	public:
		FEnumItem() = default;
		FEnumItem(FName Name, const FMetadata & Metadata) : FMeta(Name, Metadata) {}

		virtual uint64 GetValue() const { return 0; }
	};

	template<EnumC EnumT>
	struct TEnumItem : FEnumItem
	{
	public:
		TEnumItem() = default;
		TEnumItem(FName Name, EnumT Enum, const FMetadata & Metadata) : FEnumItem(Name, Metadata), Enum(Enum) { }

	public:
		uint64 GetValue() const override { return (uint64)(UnderlyingTypeT<EnumT>)Enum; }

	public:
		EnumT Enum { };

	public:
		static TEnumItem None;
	};

	template<EnumC EnumT>
	inline TEnumItem<EnumT> TEnumItem<EnumT>::None;

	struct BASE_API FEnum : FType
	{
	public:
		FEnum();
		FEnum(const FType & UnderlyingType, FName Name, FTypeid Typeid, const FMetadata & Metadata);

	public:
		const FType & UnderlyingType;

	public:
		TView<const FEnumItem &> Items;
		static FEnum None;
	};

	template<EnumC EnumT>
	struct TEnum : FEnum
	{
	public:
		TEnum() : FEnum(Typeof<UnderlyingTypeT<EnumT>>()) {}
		TEnum(FName Name, const FMetadata & Metadata) : FEnum(Typeof<UnderlyingTypeT<EnumT>>(), Name, typeid(EnumT), Metadata) { }

		const TEnumItem<EnumT> & operator [](EnumT Value) const
		{
			for (const FEnumItem & EnumItem : Items)
			{
				const TEnumItem<EnumT> & Item = StaticCast<TEnumItem<EnumT>>(EnumItem);
				if (Item.Enum == Value)
					return Item;
			}
			return TEnumItem<EnumT>::None;
		}

	public:
		static TEnum None;
	};

	template<EnumC EnumT>
	inline TEnum<EnumT> TEnum<EnumT>::None;

	template<EnumC T>
	TEnum<T> & Enumof() { return static_cast<TEnum<T> &>(Typeof<T>()); }

	template<EnumC T>
	const TEnumItem<T> & EnumItemof(T Value) { return static_cast<TEnum<T> &>(Typeof<T>())[Value]; }

	template<EnumC T>
	FName EnumNameof(T Value) { return static_cast<TEnum<T> &>(Typeof<T>())[Value].Name; }
}
