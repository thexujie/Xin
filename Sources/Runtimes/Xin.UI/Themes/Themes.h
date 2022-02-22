#pragma once
#include "../Xin.UI.Types.h"

#include "Controls/Controls.h"
#include "Animations/Animations.h"

namespace Xin::UI::Themes
{
	class UI_API FDefaultInt64DataTemplate : public IDataTemplate
	{
	public:
		FDefaultInt64DataTemplate() : IDataTemplate(Typeof<int64>()) { }
		UElementRef Instantiate(UControl & Control, const FVariant & Variant) override;
	};

	class UI_API FDefaultUInt64DataTemplate : public IDataTemplate
	{
	public:
		FDefaultUInt64DataTemplate() : IDataTemplate(Typeof<uint64>()) { }
		UElementRef Instantiate(UControl & Control, const FVariant & Variant) override;
	};

	class UI_API FDefaultUInt32DataTemplate : public IDataTemplate
	{
	public:
		FDefaultUInt32DataTemplate() : IDataTemplate(Typeof<uint32>()) { }
		UElementRef Instantiate(UControl & Control, const FVariant & Variant) override;
	};

	class UI_API FDefaultStringDataTemplate : public IDataTemplate
	{
	public:
		FDefaultStringDataTemplate() : IDataTemplate(Typeof<FString>()) { }
		UElementRef Instantiate(UControl & Control, const FVariant & Variant) override;
	};

	class UI_API FDefaultTextDataTemplate : public IDataTemplate
	{
	public:
		FDefaultTextDataTemplate() : IDataTemplate(Typeof<FText>()) { }
		UElementRef Instantiate(UControl & Control, const FVariant & Variant) override;
	};

	class UI_API FDefaultPixmapTemplate : public IDataTemplate
	{
	public:
		FDefaultPixmapTemplate() : IDataTemplate(Typeof<IPixmap>()) { }
		UElementRef Instantiate(UControl & Control, const FVariant & Variant) override;
	};

	class UI_API FDefaultBitmapTemplate : public IDataTemplate
	{
	public:
		FDefaultBitmapTemplate() : IDataTemplate(Typeof<IBitmap>()) { }
		UElementRef Instantiate(UControl & Control, const FVariant & Variant) override;
	};

	class UI_API FDefaultWindowTemplate : public TControlTemplate<UWindow>
	{
	public:
		FDefaultWindowTemplate() = default;
		UElementRef LoadTemplateT(UWindow & Window) override;
	};
}

