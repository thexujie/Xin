// C433D85951D1F43197F8BD1E789240C0 2025-03-06 15:49:26
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Controls/Shapes/Shape.h"

namespace Xin
{
	class UShape_Type : public TClass<UI::UShape>
	{
	public:
		using UShape = UI::UShape;

		// --- 0 Constructors
		static inline const TConstructor<UShape> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TConstructor<UShape, FName> Constructor___FName { u8"!Constructor___FName"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
			Constructor___FName,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<UShape, FType &()> Method { u8"Type"N, &UShape::Type, FMetadata::None };
		};
		struct Method___void___OnConstruct
		{
			static inline const TMethod<UShape, void()> Method { u8"OnConstruct"N, &UShape::OnConstruct, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___void___OnConstruct::Method,
		};

		// --- 2 Fields

		// --- 3 Properties
		struct Property___Fill
		{
			struct FPropertyMetadata : UShape::FPropertyMetadata
			{
				FPropertyMetadata()
				{
					AffectsRender = true;
				}
			};
			static inline const FPropertyMetadata Metadata;
			static inline const TDependencyProperty Property { u8"Fill"N, &UShape::__Fill, &UShape::__GetFill, &UShape::__SetFillValue, UShape::__Fill__, Metadata };
		};
		struct Property___Stroke
		{
			struct FPropertyMetadata : UShape::FPropertyMetadata
			{
				FPropertyMetadata()
				{
					AffectsRender = true;
				}
			};
			static inline const FPropertyMetadata Metadata;
			static inline const TDependencyProperty Property { u8"Stroke"N, &UShape::__Stroke, &UShape::__GetStroke, &UShape::__SetStrokeValue, UShape::__Stroke__, Metadata };
		};
		struct Property___StrokeStyle
		{
			struct FPropertyMetadata : UShape::FPropertyMetadata
			{
				FPropertyMetadata()
				{
					AffectsRender = true;
				}
			};
			static inline const FPropertyMetadata Metadata;
			static inline const TDependencyProperty Property { u8"StrokeStyle"N, &UShape::__StrokeStyle, &UShape::__GetStrokeStyle, &UShape::__SetStrokeStyleValue, UShape::__StrokeStyle__, Metadata };
		};
		struct Property___StrokeThickness
		{
			struct FPropertyMetadata : UShape::FPropertyMetadata
			{
				FPropertyMetadata()
				{
					AffectsRender = true;
				}
			};
			static inline const FPropertyMetadata Metadata;
			static inline const TDependencyProperty Property { u8"StrokeThickness"N, &UShape::__StrokeThickness, &UShape::__GetStrokeThickness, &UShape::__SetStrokeThicknessValue, UShape::__StrokeThickness__, Metadata };
		};
		static inline const TReferenceWrapper<const FProperty> ReflectedProperties[] =
		{
			Property___Fill::Property,
			Property___Stroke::Property,
			Property___StrokeStyle::Property,
			Property___StrokeThickness::Property,
		};

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<UI::UElement>(),
		};

		// --- 5 Constructor
		UShape_Type(FName Name = u8"UShape"N) : TClass(Name, Classof<UI::UElement>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Methods = ReflectedMethods;
			FClass::Properties = ReflectedProperties;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	const TProperty<IBrushRef> & UI::UShape::FillProperty() { return UShape_Type::Property___Fill::Property; }

	const TProperty<IBrushRef> & UI::UShape::StrokeProperty() { return UShape_Type::Property___Stroke::Property; }

	const TProperty<FStrokeStyle> & UI::UShape::StrokeStyleProperty() { return UShape_Type::Property___StrokeStyle::Property; }

	const TProperty<float32> & UI::UShape::StrokeThicknessProperty() { return UShape_Type::Property___StrokeThickness::Property; }

	template<> struct XIN_EXPORT TStaticType<UI::UShape>
	{
		static inline UShape_Type UShapeType { u8"UShape"N };

		static FType & Type()
		{
			return UShapeType;
		}
	};

	FType & UI::UShape::StaticType()
	{
		return TStaticType<UI::UShape>().Type();
	}

	static TTypeRegister<UI::UShape> UShapeTypeRegister { TStaticType<UI::UShape>().Type() };
}

// Generated code end.
