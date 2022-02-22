#pragma once

#include "Xin.UI.Types.h"

namespace Xin::UI
{
	class UI_API ICommand : public IUObject
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

	public:
		ICommand() = default;
		ICommand(FName Name) : IUObject(Name) { }
		~ICommand() = default;
	};

	using ICommandRef = TReferPtr<ICommand>;
}
