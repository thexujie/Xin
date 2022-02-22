// E1EA4167D4924BB78D946C792D3FC9F4 2025-03-06 15:45:43
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Core/Json.h"

namespace Xin
{
	class FJsonItem_Type : public TClass<FJsonItem>
	{
	public:
		// --- 0 Constructors
		static inline const TConstructor<FJsonItem, FJsonItem const &> Constructor___FJsonItem_const_lref { u8"!Constructor___FJsonItem_const_lref"N, FMetadata::None };
		static inline const TConstructor<FJsonItem, FJsonItem &&> Constructor___FJsonItem_ref { u8"!Constructor___FJsonItem_ref"N, FMetadata::None };
		static inline const TConstructor<FJsonItem> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor___FJsonItem_const_lref,
			Constructor___FJsonItem_ref,
			Constructor,
		};

		// --- 1 Methods
		struct Method___bool___IsArray____const
		{
			static inline const TMethod<FJsonItem, bool()> Method { u8"IsArray"N, &FJsonItem::IsArray, FMetadata::None };
		};
		struct Method___bool___IsObject____const
		{
			static inline const TMethod<FJsonItem, bool()> Method { u8"IsObject"N, &FJsonItem::IsObject, FMetadata::None };
		};
		struct Method___FJsonArray_lref___GetArrayValue
		{
			static inline const TMethod<FJsonItem, FJsonArray &()> Method { u8"GetArrayValue"N, &FJsonItem::GetArrayValue, FMetadata::None };
		};
		struct Method___FJsonArray_const_lref___GetArrayValue____const
		{
			static inline const TMethod<FJsonItem, FJsonArray const &()> Method { u8"GetArrayValue"N, &FJsonItem::GetArrayValue, FMetadata::None };
		};
		struct Method___FJsonObject_lref___GetObjectValue
		{
			static inline const TMethod<FJsonItem, FJsonObject &()> Method { u8"GetObjectValue"N, &FJsonItem::GetObjectValue, FMetadata::None };
		};
		struct Method___bool___Valid____const
		{
			static inline const TMethod<FJsonItem, bool()> Method { u8"Valid"N, &FJsonItem::Valid, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___bool___IsArray____const::Method,
			Method___bool___IsObject____const::Method,
			Method___FJsonArray_lref___GetArrayValue::Method,
			Method___FJsonArray_const_lref___GetArrayValue____const::Method,
			Method___FJsonObject_lref___GetObjectValue::Method,
			Method___bool___Valid____const::Method,
		};

		// --- 2 Fields
		struct Field___Name
		{
			static inline const TField Field { u8"Name"N, &FJsonItem::Name, FMetadata::None };
		};
		struct Field___Value
		{
			static inline const TField Field { u8"Value"N, &FJsonItem::Value, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FField> ReflectedFields[] =
		{
			Field___Name::Field,
			Field___Value::Field,
		};

		// --- 3 Properties

		// --- 4 Base Types

		// --- 5 Constructor
		FJsonItem_Type(FName Name = u8"FJsonItem"N) : TClass(Name, FClass::None)
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Fields = ReflectedFields;
			FClass::Methods = ReflectedMethods;
		}
	};

	template<> struct XIN_EXPORT TStaticType<FJsonItem>
	{
		static inline FJsonItem_Type FJsonItemType { u8"FJsonItem"N };

		static FType & Type()
		{
			return FJsonItemType;
		}
	};

	template<> XIN_EXPORT inline FType & Typeof<FJsonItem>()
	{
		return TStaticType<FJsonItem>().Type();
	}

	static TTypeRegister<FJsonItem> FJsonItemTypeRegister { TStaticType<FJsonItem>().Type() };

	class FJsonArray_Type : public TClass<FJsonArray>
	{
	public:
		// --- 0 Constructors
		static inline const TConstructor<FJsonArray, FJsonArray const &> Constructor___FJsonArray_const_lref { u8"!Constructor___FJsonArray_const_lref"N, FMetadata::None };
		static inline const TConstructor<FJsonArray, FJsonArray &&> Constructor___FJsonArray_ref { u8"!Constructor___FJsonArray_ref"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor___FJsonArray_const_lref,
			Constructor___FJsonArray_ref,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<FJsonArray, FType &()> Method { u8"Type"N, &FJsonArray::Type, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
		};

		// --- 2 Fields
		struct Field___Items
		{
			static inline const TField Field { u8"Items"N, &FJsonArray::Items, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FField> ReflectedFields[] =
		{
			Field___Items::Field,
		};

		// --- 3 Properties

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<ISharedRefer>(),
		};

		// --- 5 Constructor
		FJsonArray_Type(FName Name = u8"FJsonArray"N) : TClass(Name, Classof<ISharedRefer>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Fields = ReflectedFields;
			FClass::Methods = ReflectedMethods;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	template<> struct XIN_EXPORT TStaticType<FJsonArray>
	{
		static inline FJsonArray_Type FJsonArrayType { u8"FJsonArray"N };

		static FType & Type()
		{
			return FJsonArrayType;
		}
	};

	FType & FJsonArray::StaticType()
	{
		return TStaticType<FJsonArray>().Type();
	}

	static TTypeRegister<FJsonArray> FJsonArrayTypeRegister { TStaticType<FJsonArray>().Type() };
}

// Generated code end.
