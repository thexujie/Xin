// 602E0BB7F82F21E4638B1282E6FFB3FB 2025-03-26 16:24:48
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Controls/TextBlock.h"

namespace Xin
{
	class UTextBlock_Type : public TClass<UI::UTextBlock>
	{
	public:
		using UTextBlock = UI::UTextBlock;

		// --- 0 Constructors
		static inline const TConstructor<UTextBlock> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TConstructor<UTextBlock, FText> Constructor___FText { u8"!Constructor___FText"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
			Constructor___FText,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<UTextBlock, FType &()> Method { u8"Type"N, &UTextBlock::Type, FMetadata::None };
		};
		struct Method___TView__char8_____Describe____const
		{
			static inline const TMethod<UTextBlock, FStringV()> Method { u8"Describe"N, &UTextBlock::Describe, FMetadata::None };
		};
		struct Method___TView__char8_____ToString__TView__char8______const
		{
			static inline const TMethod<UTextBlock, FStringV(FStringV)> Method { u8"ToString"N, &UTextBlock::ToString, FMetadata::None };
		};
		struct Method___void___OnConstruct
		{
			static inline const TMethod<UTextBlock, void()> Method { u8"OnConstruct"N, &UTextBlock::OnConstruct, FMetadata::None };
		};
		struct Method___void___OnInitialize
		{
			static inline const TMethod<UTextBlock, void()> Method { u8"OnInitialize"N, &UTextBlock::OnInitialize, FMetadata::None };
		};
		struct Method___void___OnFinalize
		{
			static inline const TMethod<UTextBlock, void()> Method { u8"OnFinalize"N, &UTextBlock::OnFinalize, FMetadata::None };
		};
		struct Method___TVec2__float64_____OnMeasure__TVec2__float64__
		{
			static inline const TMethod<UTextBlock, FSize(FSize)> Method { u8"OnMeasure"N, &UTextBlock::OnMeasure, FMetadata::None };
		};
		struct Method___void___OnArrange
		{
			static inline const TMethod<UTextBlock, void()> Method { u8"OnArrange"N, &UTextBlock::OnArrange, FMetadata::None };
		};
		struct Method___void___OnPaint__IPainter_lref
		{
			static inline const TMethod<UTextBlock, void(IPainter &)> Method { u8"OnPaint"N, &UTextBlock::OnPaint, FMetadata::None };
		};
		struct Method___void___OnTextChanged__FText
		{
			static inline const TMethod<UTextBlock, void(FText)> Method { u8"OnTextChanged"N, &UTextBlock::OnTextChanged, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___TView__char8_____Describe____const::Method,
			Method___TView__char8_____ToString__TView__char8______const::Method,
			Method___void___OnConstruct::Method,
			Method___void___OnInitialize::Method,
			Method___void___OnFinalize::Method,
			Method___TVec2__float64_____OnMeasure__TVec2__float64__::Method,
			Method___void___OnArrange::Method,
			Method___void___OnPaint__IPainter_lref::Method,
			Method___void___OnTextChanged__FText::Method,
		};

		// --- 2 Fields
		struct Field___Color
		{
			static inline const TField Field { u8"Color"N, &UTextBlock::Color, FMetadata::None };
		};
		struct Field___Multiline
		{
			static inline const TField Field { u8"Multiline"N, &UTextBlock::Multiline, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FField> ReflectedFields[] =
		{
			Field___Color::Field,
			Field___Multiline::Field,
		};

		// --- 3 Properties
		struct Property___Text
		{
			static inline const TDependencyProperty Property { u8"Text"N, &UTextBlock::__Text, &UTextBlock::__GetText, &UTextBlock::__SetTextValue, UTextBlock::__Text__, UTextBlock::FPropertyMetadata::None };
		};
		static inline const TReferenceWrapper<const FProperty> ReflectedProperties[] =
		{
			Property___Text::Property,
		};

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<UI::UElement>(),
		};

		// --- 5 Constructor
		UTextBlock_Type(FName Name = u8"UTextBlock"N) : TClass(Name, Classof<UI::UElement>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Fields = ReflectedFields;
			FClass::Methods = ReflectedMethods;
			FClass::Properties = ReflectedProperties;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	const TProperty<FText> & UI::UTextBlock::TextProperty() { return UTextBlock_Type::Property___Text::Property; }

	template<> struct XIN_EXPORT TStaticType<UI::UTextBlock>
	{
		static inline UTextBlock_Type UTextBlockType { u8"UTextBlock"N };

		static FType & Type()
		{
			return UTextBlockType;
		}
	};

	FType & UI::UTextBlock::StaticType()
	{
		return TStaticType<UI::UTextBlock>().Type();
	}

	static TTypeRegister<UI::UTextBlock> UTextBlockTypeRegister { TStaticType<UI::UTextBlock>().Type() };
}

// Generated code end.
