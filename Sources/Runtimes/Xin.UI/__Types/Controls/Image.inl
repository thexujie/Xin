// EE01B360E301A4CE2B9CF11EC70AE9E4 2025-03-24 18:06:19
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Controls/Image.h"

namespace Xin
{
	class UImage_Type : public TClass<UI::UImage>
	{
	public:
		using UImage = UI::UImage;

		// --- 0 Constructors
		static inline const TConstructor<UImage> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<UImage, FType &()> Method { u8"Type"N, &UImage::Type, FMetadata::None };
		};
		struct Method___void___OnConstruct
		{
			static inline const TMethod<UImage, void()> Method { u8"OnConstruct"N, &UImage::OnConstruct, FMetadata::None };
		};
		struct Method___void___OnFinalize
		{
			static inline const TMethod<UImage, void()> Method { u8"OnFinalize"N, &UImage::OnFinalize, FMetadata::None };
		};
		struct Method___TVec2__float64_____OnMeasure__TVec2__float64__
		{
			static inline const TMethod<UImage, FSize(FSize)> Method { u8"OnMeasure"N, &UImage::OnMeasure, FMetadata::None };
		};
		struct Method___void___OnPaint__IPainter_lref
		{
			static inline const TMethod<UImage, void(IPainter &)> Method { u8"OnPaint"N, &UImage::OnPaint, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___void___OnConstruct::Method,
			Method___void___OnFinalize::Method,
			Method___TVec2__float64_____OnMeasure__TVec2__float64__::Method,
			Method___void___OnPaint__IPainter_lref::Method,
		};

		// --- 2 Fields
		struct Field___Bitmap
		{
			static inline const TField Field { u8"Bitmap"N, &UImage::Bitmap, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FField> ReflectedFields[] =
		{
			Field___Bitmap::Field,
		};

		// --- 3 Properties
		struct Property___Pixmap
		{
			static inline const TDependencyProperty Property { u8"Pixmap"N, &UImage::__Pixmap, &UImage::__GetPixmap, &UImage::__SetPixmapValue, UImage::__Pixmap__, UImage::FPropertyMetadata::None };
		};
		struct Property___ImageSource
		{
			static inline const TDependencyProperty Property { u8"ImageSource"N, &UImage::__ImageSource, &UImage::__GetImageSource, &UImage::__SetImageSourceValue, UImage::__ImageSource__, UImage::FPropertyMetadata::None };
		};
		struct Property___Stretch
		{
			static inline const TDependencyProperty Property { u8"Stretch"N, &UImage::__Stretch, &UImage::__GetStretch, &UImage::__SetStretchValue, UImage::__Stretch__, UImage::FPropertyMetadata::None };
		};
		static inline const TReferenceWrapper<const FProperty> ReflectedProperties[] =
		{
			Property___Pixmap::Property,
			Property___ImageSource::Property,
			Property___Stretch::Property,
		};

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<UI::UElement>(),
		};

		// --- 5 Constructor
		UImage_Type(FName Name = u8"UImage"N) : TClass(Name, Classof<UI::UElement>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Fields = ReflectedFields;
			FClass::Methods = ReflectedMethods;
			FClass::Properties = ReflectedProperties;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	const TProperty<IPixmapRef> & UI::UImage::PixmapProperty() { return UImage_Type::Property___Pixmap::Property; }

	const TProperty<FString> & UI::UImage::ImageSourceProperty() { return UImage_Type::Property___ImageSource::Property; }

	const TProperty<UI::EStretch> & UI::UImage::StretchProperty() { return UImage_Type::Property___Stretch::Property; }

	template<> struct XIN_EXPORT TStaticType<UI::UImage>
	{
		static inline UImage_Type UImageType { u8"UImage"N };

		static FType & Type()
		{
			return UImageType;
		}
	};

	FType & UI::UImage::StaticType()
	{
		return TStaticType<UI::UImage>().Type();
	}

	static TTypeRegister<UI::UImage> UImageTypeRegister { TStaticType<UI::UImage>().Type() };
}

// Generated code end.
