#pragma once

#include "Xin.UI.Types.h"
#include "Element.h"

namespace Xin::UI
{
	class UI_API UImage : public UElement
	{
	public:
		static FType & StaticType();
		FType & Type() const override { return StaticType(); }

	public:
		UImage();
		~UImage() = default;

	public:
		void OnConstruct() override;
		void OnFinalize() override;
		FSize OnMeasure(FSize ConstrainedSize) override;
		void OnPaint(IPainter & Painter) override;

	public:
		IBitmapRef Bitmap;

	public:
		DependencyPropertyRW(IPixmapRef, Pixmap) = noval;
		DependencyPropertyRW(FString, ImageSource) = noval;

		DependencyPropertyRW(EStretch, Stretch) = EStretch::Uniform;
	};
	using UImageRef = TReferPtr<UImage>;
}
