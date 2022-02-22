#include "PCH.h"
#include "Text.h"

#include "Object.h"
#include "Strings.h"
#include "Map.h"
#include "Meta/Type.h"

namespace Xin
{
	FText FText::None;

	class FTextEntry : public ITextEntry, public ISharedRefer
	{
	public:
		FTextEntry(FStringV Name) : Text(Name) {}
		FStringV Resolve() const override { return Text; }

	public:
		FString Text;
	};

	static TMap<uintx, TReferPtr<FTextEntry>> GTextEntries;
	FText::FText(FStringV String) : Nameid(NameHash(String))
	{
		if (auto Iter = GTextEntries.Find(Nameid.Identifier); Iter != GTextEntries.End())
			TextEntry = Iter->Second.Get();
		else
		{
			TReferPtr<FTextEntry> NewNameEntry = new FTextEntry(String);
			GTextEntries.Insert(MakePair(Nameid.Identifier, NewNameEntry));
			TextEntry = NewNameEntry.Get();
		}
	}
}
