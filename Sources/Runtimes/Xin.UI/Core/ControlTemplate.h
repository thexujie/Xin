#pragma once

#include "Xin.UI.Types.h"
#include "Core/Resource.h"

namespace Xin::UI
{
	class UI_API IControlTemplate : public IResource
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

	public:
		IControlTemplate() = default;
		IControlTemplate(const FType & TargetType) : IResource(TargetType.Name), TargetType(TargetType) { }
		~IControlTemplate() = default;

		virtual UElementRef LoadTemplate(UControl & Control) = 0;

	public:
		const FType & TargetType = FType::None;
	};
	using IControlTemplateRef = TReferPtr<IControlTemplate>;


	template<typename ControlT>
	class TControlTemplate : public IControlTemplate
	{
	public:
		TControlTemplate() : IControlTemplate(Typeof<ControlT>()) {}

		UElementRef LoadTemplate(UControl & Control) override { return LoadTemplateT(StaticCast<ControlT>(Control)); }

	public:
		virtual UElementRef LoadTemplateT(ControlT & Control) = 0;
	};
}
