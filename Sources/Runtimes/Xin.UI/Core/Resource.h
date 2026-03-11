#pragma once

#include "Xin.UI.Types.h"

namespace Xin::UI
{
	class UI_API IResource : public IUObject
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

	public:
		IResource() = default;
		IResource(FName Name) : IUObject(Name) { }
		~IResource() = default;
	};

	using IResourceRef = TReferPtr<IResource>;

	class UI_API FResourceTable : public IUObject
	{
	public:
		FResourceTable() = default;
		FResourceTable(TInitializerList<TReferPtr<FResourceTable>> ResourceLists);
		~FResourceTable() = default;

		IResourceRef FindResource(FName Name, const FType & ResourceType = FType::None) const;

		template<DerivedOfC<IResource> T>
		TReferPtr<T> FindResourceT(FName Name) const
		{
			return StaticCast<T>(FindResource(Name), Typeof<T>());
		}

	public:
		TList<IResourceRef> Resources;
		TList<TReferPtr<FResourceTable>> ResourceLists;

	public:
		static TReferPtr<FResourceTable> Default();
	};

	using FResourceTableRef = TReferPtr<FResourceTable>;
}
