#pragma once

#include "Meta.h"

namespace Xin
{
	struct BASE_API FField : FMeta
	{
	public:
		FField();
		FField(const FType & Type);
		FField(const FType & Type, FName Name, const FMetadata & Metadata);

		virtual FVariant GetValue(FVariant Instance) const { return nullptr; }
		virtual void SetValue(FVariant & Instance, FVariant Value) const { }

	public:
		const FType & Type;

	public:
		static FField None;
	};

	template<MemberFieldPointerC FieldT>
	struct TField : FField
	{
	public:
		using InstanceT = typename IsMemberFieldPointer<FieldT>::InstanceType;
		using ValueT = typename IsMemberFieldPointer<FieldT>::ValueType;
	public:

		TField() : FField(Typeof<ValueT>()), Field(nullptr) {}
		TField(FName Name, FieldT Field, const FMetadata & Metadata) : FField(Typeof<ValueT>(), Name, Metadata), Field(Field) { }

	public:
		FVariant GetValue(FVariant Instance) const override
		{
			InstanceT & This = Instance.Get<InstanceT &>();
			return This.*Field;
		}

		void SetValue(FVariant & Instance, FVariant Value) const override
		{
			InstanceT & This = Instance.Get<InstanceT &>();
			This.*Field = Value.Get<ValueT>();
		}

	public:
		FieldT Field;
	};
}
