#pragma once

#include "Xin.UI.Types.h"
#include "Core/Resource.h"

namespace Xin::UI
{
	class UContentControl;

	class UI_API IDataTemplate : public IResource
	{
	public:
		IDataTemplate() = default;
		IDataTemplate(const FType & DataType) : IResource(DataType.Name), DataType(DataType) { }
		~IDataTemplate() = default;

		virtual UElementRef Instantiate(UControl & Control, const FVariant & Variant) = 0;

	public:
		const FType & DataType = FType::None;
	};
	using IDataTemplateRef = TReferPtr<IDataTemplate>;
}
