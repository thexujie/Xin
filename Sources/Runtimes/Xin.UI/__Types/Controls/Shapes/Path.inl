// EA573045843E025B007FDA2184AE9CE5 2025-03-24 18:07:00
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Controls/Shapes/Path.h"

namespace Xin
{
	class UPath_Type : public TClass<UI::UPath>
	{
	public:
		using UPath = UI::UPath;

		// --- 0 Constructors
		static inline const TConstructor<UPath> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<UPath, FType &()> Method { u8"Type"N, &UPath::Type, FMetadata::None };
		};
		struct Method___void___OnInitialize
		{
			static inline const TMethod<UPath, void()> Method { u8"OnInitialize"N, &UPath::OnInitialize, FMetadata::None };
		};
		struct Method___void___OnFinalize
		{
			static inline const TMethod<UPath, void()> Method { u8"OnFinalize"N, &UPath::OnFinalize, FMetadata::None };
		};
		struct Method___TVec2__float64_____OnMeasure__TVec2__float64__
		{
			static inline const TMethod<UPath, FSize(FSize)> Method { u8"OnMeasure"N, &UPath::OnMeasure, FMetadata::None };
		};
		struct Method___void___OnArranged
		{
			static inline const TMethod<UPath, void()> Method { u8"OnArranged"N, &UPath::OnArranged, FMetadata::None };
		};
		struct Method___void___OnPaint__IPainter_lref
		{
			static inline const TMethod<UPath, void(IPainter &)> Method { u8"OnPaint"N, &UPath::OnPaint, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___void___OnInitialize::Method,
			Method___void___OnFinalize::Method,
			Method___TVec2__float64_____OnMeasure__TVec2__float64__::Method,
			Method___void___OnArranged::Method,
			Method___void___OnPaint__IPainter_lref::Method,
		};

		// --- 2 Fields

		// --- 3 Properties
		struct Property___Command
		{
			struct FPropertyMetadata : UPath::FPropertyMetadata
			{
				FPropertyMetadata()
				{
					AffectsMeasure = true;
					AffectsRender = true;
				}
			};
			static inline const FPropertyMetadata Metadata;
			static inline const TDependencyProperty Property { u8"Command"N, &UPath::__Command, &UPath::__GetCommand, &UPath::__SetCommandValue, UPath::__Command__, Metadata };
		};
		struct Property___PathTransform
		{
			struct FPropertyMetadata : UPath::FPropertyMetadata
			{
				FPropertyMetadata()
				{
					AffectsMeasure = true;
					AffectsRender = true;
				}
			};
			static inline const FPropertyMetadata Metadata;
			static inline const TDependencyProperty Property { u8"PathTransform"N, &UPath::__PathTransform, &UPath::__GetPathTransform, &UPath::__SetPathTransformValue, UPath::__PathTransform__, Metadata };
		};
		struct Property___Stretch
		{
			struct FPropertyMetadata : UPath::FPropertyMetadata
			{
				FPropertyMetadata()
				{
					AffectsRender = true;
				}
			};
			static inline const FPropertyMetadata Metadata;
			static inline const TDependencyProperty Property { u8"Stretch"N, &UPath::__Stretch, &UPath::__GetStretch, &UPath::__SetStretchValue, UPath::__Stretch__, Metadata };
		};
		static inline const TReferenceWrapper<const FProperty> ReflectedProperties[] =
		{
			Property___Command::Property,
			Property___PathTransform::Property,
			Property___Stretch::Property,
		};

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<UI::UShape>(),
		};

		// --- 5 Constructor
		UPath_Type(FName Name = u8"UPath"N) : TClass(Name, Classof<UI::UShape>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Methods = ReflectedMethods;
			FClass::Properties = ReflectedProperties;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	const TProperty<FString> & UI::UPath::CommandProperty() { return UPath_Type::Property___Command::Property; }

	const TProperty<FTransform3x2F> & UI::UPath::PathTransformProperty() { return UPath_Type::Property___PathTransform::Property; }

	const TProperty<UI::EStretch> & UI::UPath::StretchProperty() { return UPath_Type::Property___Stretch::Property; }

	template<> struct XIN_EXPORT TStaticType<UI::UPath>
	{
		static inline UPath_Type UPathType { u8"UPath"N };

		static FType & Type()
		{
			return UPathType;
		}
	};

	FType & UI::UPath::StaticType()
	{
		return TStaticType<UI::UPath>().Type();
	}

	static TTypeRegister<UI::UPath> UPathTypeRegister { TStaticType<UI::UPath>().Type() };
}

// Generated code end.
