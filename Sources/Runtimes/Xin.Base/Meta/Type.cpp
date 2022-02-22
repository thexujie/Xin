#include "PCH.h"
#include "Type.h"

namespace Xin
{
	FType FType::None;

	static TList<FType &> GTypes;

	FType::FType(FName Name, FTypeid Typeid, const FMetadata & Metadata) : FMeta(Name, Metadata), Typeid(Typeid)
	{
		GTypes.AddUnique(*this);
	}

	TView<FType &> FType::AllTypes()
	{
		return GTypes;
	}

	FType & FType::FromName(FName Name)
	{
		for (FType & Type : GTypes)
		{
			if (Type.Name == Name)
				return Type;
		}
		return FType::None;
	}

	FType & FType::FromTypeid(FTypeid Typeid)
	{
		for (FType & Type : GTypes)
		{
			if (Type.Typeid == Typeid)
				return Type;
		}
		return FType::None;
	}

	static TList<TUniquePtr<FPointerType>> GPointerTypes;

	FType & FPointerType::GetDecoratedType(FType & Type)
	{
		for (TUniquePtr<FPointerType> & PointerType : GPointerTypes)
		{
			if (PointerType->PointeeType == Type)
				return PointerType.Ref();
		}

		FName Name { u8"{} *"V.Format(Type.Name.Resolve()) };
		GPointerTypes.Add(MakeUnique<FPointerType>(Name, Type));
		return GPointerTypes.Back().Ref();
	}

	static TList<TUniquePtr<FReferPointerType>> GReferPointerTypes;
	FType & FReferPointerType::GetDecoratedType(const FType & Type)
	{
		for (TUniquePtr<FReferPointerType> & ReferPointerType : GReferPointerTypes)
		{
			if (ReferPointerType->PointeeType == Type)
				return ReferPointerType.Ref();
		}

		FName Name { u8"TReferPtr<{}>"V.Format(Type.Name.Resolve()) };
		GReferPointerTypes.Add(MakeUnique<FReferPointerType>(Name, ConstCast(Type)));
		return GReferPointerTypes.Back().Ref();
	}

	static TList<TUniquePtr<FListType>> GListTypes;
	FType & FListType::GetDecoratedType(const FType & Type)
	{
		for (TUniquePtr<FListType> & ListType : GListTypes)
		{
			if (ListType->PointeeType == Type)
				return ListType.Ref();
		}

		FName Name { u8"TList<{}>"V.Format(Type.Name.Resolve()) };
		GListTypes.Add(MakeUnique<FListType>(Name, ConstCast(Type)));
		return GListTypes.Back().Ref();
	}
}
