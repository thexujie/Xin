#pragma once

#include "Xin.UI.Types.h"
#include "Element.h"

namespace Xin::UI
{
	class UI_API UTextBlock : public UElement
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

	public:
		UTextBlock() = default;
		UTextBlock(FText Text);
		~UTextBlock() = default;
		FStringV Describe() const override { return Text.Resolve(); }
		FStringV ToString(FStringV Formal = FStringV::None) const override;

	public:
		void OnConstruct() override;
		void OnInitialize() override;
		void OnFinalize() override;
		FSize OnMeasure(FSize ConstrainedSize) override;
		void OnArrange() override;
		void OnPaint(IPainter & Painter) override;

	public:
		void OnTextChanged(FText TextBefore);
		DependencyPropertyRW(FText, Text);

		FColor Color = Colors::Black;
		bool Multiline = false;

	private:
		ITextBlobRef TextBlob;
	};
	using UTextBlockRef = TReferPtr<UTextBlock>;
}
