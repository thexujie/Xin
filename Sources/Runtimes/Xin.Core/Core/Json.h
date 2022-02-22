#pragma once

#include "../Xin.Core.Types.h"

namespace Xin
{
	class FJsonDocument_Inner;

	enum class EJsonObjectType
	{
		Object = 0,
		Array,
	};

	struct CORE_API [[Meta]] FJsonItem
	{
		FString Name;
		FVariant Value;

		bool IsArray() const
		{
			return Value.Kind == EVariantKind::ReferPointer/* && Value.NativeTypeValue*/;
		}

		template<typename T>
		T & operator[](uintx Index)
		{
			return Value.Get<T>();
		}

		FVariant & operator[](uintx Index);
		const FVariant & operator[](uintx Index) const;

		bool IsObject() const
		{
			return Value.Kind == EVariantKind::ReferPointer/* && Value.NativeTypeValue*/;
		}

		template<typename T>
		T GetValue()
		{
			return Value.Get<T>();
		}
		struct FJsonArray & GetArrayValue();
		const struct FJsonArray & GetArrayValue() const;
		struct FJsonObject & GetObjectValue();

		bool Valid() const { return !!Name.Length; }
		explicit operator bool() const { return !!Name.Length; }
	};

	struct CORE_API FJsonArray : public ISharedRefer
	{
		META_DECL

		TList<FVariant> Items;

		FVariant & operator[](uintx Index) { return Items[Index]; }
		const FVariant & operator[](uintx Index) const { return Items[Index]; }
	};

	inline FJsonArray & FJsonItem::GetArrayValue()
	{
		return *Value.Get<FJsonArray *>();
	}
	inline const FJsonArray & FJsonItem::GetArrayValue() const
	{
		return *Value.Get<FJsonArray *>();
	}

	inline FVariant & FJsonItem::operator[](uintx Index)
	{
		return GetArrayValue().Items[Index];
	}
	inline const FVariant & FJsonItem::operator[](uintx Index) const
	{
		return GetArrayValue().Items[Index];
	}

	struct CORE_API FJsonObject : ISharedRefer
	{
		TList<FJsonItem> Items;

		bool Contains(FStringV Name) const { return Find(Name).Valid(); }
		FJsonItem & Find(FStringV Name);
		const FJsonItem & Find(FStringV Name) const;
		FJsonItem & operator[](FStringV Name);
		const FJsonItem & operator[](FStringV Name) const;
	};

	inline FJsonObject & FJsonItem::GetObjectValue()
	{
		return *Value.Get<FJsonObject *>();
	}

	CORE_API IDataStream & operator << (IDataStream & Stream, const FJsonObject & JsonObject);
	CORE_API IDataStream & operator >> (IDataStream & Stream, FJsonObject & JsonObject);

	class CORE_API FJsonDocument : public FJsonObject
	{
	public:
		FJsonDocument();
		FJsonDocument(FStringV FilePath) { LoadFromFile(FilePath); }
		~FJsonDocument();

	public:
		bool LoadFromFile(FStringV FilePath);
		bool SaveToFile(FStringV FilePath);

	public:
#ifdef _DEBUG
		FString SourceFilePath;
#endif
	};
}
