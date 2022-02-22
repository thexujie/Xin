#pragma once

#include "Xin.UI.Types.h"
#include "Dimen.h"
#include "ControlStyle.h"
#include "Trigger.h"
#include "Animations/Storyboard.h"

namespace Xin::UI
{
	class UObject;
	class UI_API UObject : public IDependency
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

	public:
		UObject() = default;
		UObject(FName Name);
		~UObject();

	public:
		virtual void Construct();
		virtual void OnConstruct();
		virtual void OnConstructed();

		virtual void Initialize();
		virtual void OnInitialize();
		virtual void OnInitialized();

		virtual void Finalize();
		virtual void OnFinalize();
		virtual void OnFinalized();

		bool Constructing = false;
		bool Constructed = false;
		bool Initializing = false;
		bool Finalizing = false;
		bool Initialized = false;

	public:
		TList<IBindingRef> Bindings;

	public:
		TList<FTriggerRef> Triggers;
	};
	using UObjectRef = TReferPtr<UObject>;
	using UEntryPtr = TEntryPtr<UObject>;
}
