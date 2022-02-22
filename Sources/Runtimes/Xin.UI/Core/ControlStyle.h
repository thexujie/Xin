#pragma once

#include "Xin.UI.Types.h"
#include "Resource.h"

namespace Xin::UI
{
	class UI_API IControlStyle : public IResource
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

	public:
		IControlStyle(const FType & Type) : IResource(Type.Name) {}

		virtual void LoadStyle(class UObject & Object) {}
	};
	using IControlStyleRef = TReferPtr<IControlStyle>;

	template<typename ControlT>
	class TControlStyle : public IControlStyle
	{
	public:
		TControlStyle() : IControlStyle(Typeof<ControlT>()) {}

	private:
		void LoadStyle(UObject & Object) final { LoadStyleT(StaticCast<ControlT>(Object)); }

	public:
		virtual void LoadStyleT(ControlT & Control) = 0;
	};
}
