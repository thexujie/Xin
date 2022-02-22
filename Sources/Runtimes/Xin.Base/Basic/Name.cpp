#include "PCH.h"
#include "Name.h"
#include "Object.h"
#include "Strings.h"
#include "Map.h"
#include "Meta/Type.h"

namespace Xin
{
	FNameid FNameid::None;
	FName FName::None;

	class FNameEntry : public INameEntry, public ISharedRefer
	{
	public:
		FNameEntry(FStringV Name) : Name(Name) {}
		FStringV Resolve() const override { return Name ; }

	public:
		FString Name;
	};

	static TMap<uintx, TReferPtr<FNameEntry>> GNameEntries;

	FName::FName(FStringV Name) : Nameid(NameHash(Name))
	{
		if (auto Iter = GNameEntries.Find(Nameid.Identifier); Iter != GNameEntries.End())
			NameEntry = (const char8 *)(uintx(Iter->Second.Get()) | 1);
		else
		{
			TReferPtr<FNameEntry> NewNameEntry = new FNameEntry(Name);
			GNameEntries.Insert(MakePair(Nameid.Identifier, NewNameEntry));
			NameEntry = (const char8 *)(uintx(NewNameEntry.Get()) | 1);
		}
	}
}
