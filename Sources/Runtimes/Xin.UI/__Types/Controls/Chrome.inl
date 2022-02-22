// F4493F445D47B10F1768C8C131D7F1CE 2025-03-24 18:06:08
// Generated code begin.

#pragma once
#include "PCH.h"
#include "Controls/Chrome.h"

namespace Xin
{
	class UChromeButton_Type : public TClass<UI::UChromeButton>
	{
	public:
		using UChromeButton = UI::UChromeButton;

		// --- 0 Constructors
		static inline const TConstructor<UChromeButton, UI::UChrome &> Constructor___UChrome_lref { u8"!Constructor___UChrome_lref"N, FMetadata::None };
		static inline const TConstructor<UChromeButton, UI::UChrome &, FName> Constructor___UChrome_lref__FName { u8"!Constructor___UChrome_lref__FName"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor___UChrome_lref,
			Constructor___UChrome_lref__FName,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<UChromeButton, FType &()> Method { u8"Type"N, &UChromeButton::Type, FMetadata::None };
		};
		struct Method___void___OnMouseDown__FMouseButtonEventArgs_lref
		{
			static inline const TMethod<UChromeButton, void(UI::FMouseButtonEventArgs &)> Method { u8"OnMouseDown"N, &UChromeButton::OnMouseDown, FMetadata::None };
		};
		struct Method___void___OnClick__FRoutedEventArgs_lref
		{
			static inline const TMethod<UChromeButton, void(UI::FRoutedEventArgs &)> Method { u8"OnClick"N, &UChromeButton::OnClick, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___void___OnMouseDown__FMouseButtonEventArgs_lref::Method,
			Method___void___OnClick__FRoutedEventArgs_lref::Method,
		};

		// --- 2 Fields

		// --- 3 Properties
		struct Property___Command
		{
			static inline const TDependencyProperty Property { u8"Command"N, &UChromeButton::__Command, &UChromeButton::__GetCommand, &UChromeButton::__SetCommandValue, UChromeButton::__Command__, UChromeButton::FPropertyMetadata::None };
		};
		struct Property___PathCommand
		{
			static inline const TDependencyProperty Property { u8"PathCommand"N, &UChromeButton::__PathCommand, &UChromeButton::__GetPathCommand, &UChromeButton::__SetPathCommandValue, UChromeButton::__PathCommand__, UChromeButton::FPropertyMetadata::None };
		};
		struct Property___PathFill
		{
			static inline const TDependencyProperty Property { u8"PathFill"N, &UChromeButton::__PathFill, &UChromeButton::__GetPathFill, &UChromeButton::__SetPathFillValue, UChromeButton::__PathFill__, UChromeButton::FPropertyMetadata::None };
		};
		struct Property___PathStroke
		{
			static inline const TDependencyProperty Property { u8"PathStroke"N, &UChromeButton::__PathStroke, &UChromeButton::__GetPathStroke, &UChromeButton::__SetPathStrokeValue, UChromeButton::__PathStroke__, UChromeButton::FPropertyMetadata::None };
		};
		struct Property___PathStrokeThickness
		{
			static inline const TDependencyProperty Property { u8"PathStrokeThickness"N, &UChromeButton::__PathStrokeThickness, &UChromeButton::__GetPathStrokeThickness, &UChromeButton::__SetPathStrokeThicknessValue, UChromeButton::__PathStrokeThickness__, UChromeButton::FPropertyMetadata::None };
		};
		static inline const TReferenceWrapper<const FProperty> ReflectedProperties[] =
		{
			Property___Command::Property,
			Property___PathCommand::Property,
			Property___PathFill::Property,
			Property___PathStroke::Property,
			Property___PathStrokeThickness::Property,
		};

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<UI::UButtonControl>(),
		};

		// --- 5 Constructor
		UChromeButton_Type(FName Name = u8"UChromeButton"N) : TClass(Name, Classof<UI::UButtonControl>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Methods = ReflectedMethods;
			FClass::Properties = ReflectedProperties;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	const TProperty<UI::EChromeCommand> & UI::UChromeButton::CommandProperty() { return UChromeButton_Type::Property___Command::Property; }

	const TProperty<FString> & UI::UChromeButton::PathCommandProperty() { return UChromeButton_Type::Property___PathCommand::Property; }

	const TProperty<IBrushRef> & UI::UChromeButton::PathFillProperty() { return UChromeButton_Type::Property___PathFill::Property; }

	const TProperty<IBrushRef> & UI::UChromeButton::PathStrokeProperty() { return UChromeButton_Type::Property___PathStroke::Property; }

	const TProperty<float32> & UI::UChromeButton::PathStrokeThicknessProperty() { return UChromeButton_Type::Property___PathStrokeThickness::Property; }

	template<> struct XIN_EXPORT TStaticType<UI::UChromeButton>
	{
		static inline UChromeButton_Type UChromeButtonType { u8"UChromeButton"N };

		static FType & Type()
		{
			return UChromeButtonType;
		}
	};

	FType & UI::UChromeButton::StaticType()
	{
		return TStaticType<UI::UChromeButton>().Type();
	}

	static TTypeRegister<UI::UChromeButton> UChromeButtonTypeRegister { TStaticType<UI::UChromeButton>().Type() };

	class UChromeBorder_Type : public TClass<UI::UChromeBorder>
	{
	public:
		using UChromeBorder = UI::UChromeBorder;

		// --- 0 Constructors
		static inline const TConstructor<UChromeBorder, UI::UChrome &> Constructor___UChrome_lref { u8"!Constructor___UChrome_lref"N, FMetadata::None };
		static inline const TConstructor<UChromeBorder, UI::UChrome &, FName> Constructor___UChrome_lref__FName { u8"!Constructor___UChrome_lref__FName"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor___UChrome_lref,
			Constructor___UChrome_lref__FName,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<UChromeBorder, FType &()> Method { u8"Type"N, &UChromeBorder::Type, FMetadata::None };
		};
		struct Method___void___OnConstruct
		{
			static inline const TMethod<UChromeBorder, void()> Method { u8"OnConstruct"N, &UChromeBorder::OnConstruct, FMetadata::None };
		};
		struct Method___void___OnMouseDown__FMouseButtonEventArgs_lref
		{
			static inline const TMethod<UChromeBorder, void(UI::FMouseButtonEventArgs &)> Method { u8"OnMouseDown"N, &UChromeBorder::OnMouseDown, FMetadata::None };
		};
		struct Method___EHitResult___HitTest__TVec2__float64______const
		{
			static inline const TMethod<UChromeBorder, EHitResult(FPoint)> Method { u8"HitTest"N, &UChromeBorder::HitTest, FMetadata::None };
		};
		struct Method___ECursor___QueryCursor__TVec2__float64______const
		{
			static inline const TMethod<UChromeBorder, ECursor(FPoint)> Method { u8"QueryCursor"N, &UChromeBorder::QueryCursor, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___void___OnConstruct::Method,
			Method___void___OnMouseDown__FMouseButtonEventArgs_lref::Method,
			Method___EHitResult___HitTest__TVec2__float64______const::Method,
			Method___ECursor___QueryCursor__TVec2__float64______const::Method,
		};

		// --- 2 Fields

		// --- 3 Properties
		struct Property___ChromeThickness
		{
			struct FPropertyMetadata : UChromeBorder::FPropertyMetadata
			{
				FPropertyMetadata()
				{
					AffectsMeasure = true;
					FriendlyName = u8"Border Thickness"V;
					AffectsRender = true;
				}
			};
			static inline const FPropertyMetadata Metadata;
			static inline const TDependencyProperty Property { u8"ChromeThickness"N, &UChromeBorder::__ChromeThickness, &UChromeBorder::__GetChromeThickness, &UChromeBorder::__SetChromeThicknessValue, UChromeBorder::__ChromeThickness__, Metadata };
		};
		static inline const TReferenceWrapper<const FProperty> ReflectedProperties[] =
		{
			Property___ChromeThickness::Property,
		};

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<UI::UBorder>(),
		};

		// --- 5 Constructor
		UChromeBorder_Type(FName Name = u8"UChromeBorder"N) : TClass(Name, Classof<UI::UBorder>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Methods = ReflectedMethods;
			FClass::Properties = ReflectedProperties;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	const TProperty<FThickness> & UI::UChromeBorder::ChromeThicknessProperty() { return UChromeBorder_Type::Property___ChromeThickness::Property; }

	template<> struct XIN_EXPORT TStaticType<UI::UChromeBorder>
	{
		static inline UChromeBorder_Type UChromeBorderType { u8"UChromeBorder"N };

		static FType & Type()
		{
			return UChromeBorderType;
		}
	};

	FType & UI::UChromeBorder::StaticType()
	{
		return TStaticType<UI::UChromeBorder>().Type();
	}

	static TTypeRegister<UI::UChromeBorder> UChromeBorderTypeRegister { TStaticType<UI::UChromeBorder>().Type() };

	class UChromeTitle_Type : public TClass<UI::UChromeTitle>
	{
	public:
		using UChromeTitle = UI::UChromeTitle;

		// --- 0 Constructors
		static inline const TConstructor<UChromeTitle, UI::UChrome &> Constructor___UChrome_lref { u8"!Constructor___UChrome_lref"N, FMetadata::None };
		static inline const TConstructor<UChromeTitle, UI::UChrome &, FName> Constructor___UChrome_lref__FName { u8"!Constructor___UChrome_lref__FName"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor___UChrome_lref,
			Constructor___UChrome_lref__FName,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<UChromeTitle, FType &()> Method { u8"Type"N, &UChromeTitle::Type, FMetadata::None };
		};
		struct Method___void___OnConstruct
		{
			static inline const TMethod<UChromeTitle, void()> Method { u8"OnConstruct"N, &UChromeTitle::OnConstruct, FMetadata::None };
		};
		struct Method___void___OnMouseDown__FMouseButtonEventArgs_lref
		{
			static inline const TMethod<UChromeTitle, void(UI::FMouseButtonEventArgs &)> Method { u8"OnMouseDown"N, &UChromeTitle::OnMouseDown, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___void___OnConstruct::Method,
			Method___void___OnMouseDown__FMouseButtonEventArgs_lref::Method,
		};

		// --- 2 Fields

		// --- 3 Properties
		struct Property___Title
		{
			static inline const TDependencyProperty Property { u8"Title"N, &UChromeTitle::__Title, &UChromeTitle::__GetTitle, &UChromeTitle::__SetTitleValue, UChromeTitle::__Title__, UChromeTitle::FPropertyMetadata::None };
		};
		static inline const TReferenceWrapper<const FProperty> ReflectedProperties[] =
		{
			Property___Title::Property,
		};

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<UI::UControl>(),
		};

		// --- 5 Constructor
		UChromeTitle_Type(FName Name = u8"UChromeTitle"N) : TClass(Name, Classof<UI::UControl>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Methods = ReflectedMethods;
			FClass::Properties = ReflectedProperties;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	const TProperty<FText> & UI::UChromeTitle::TitleProperty() { return UChromeTitle_Type::Property___Title::Property; }

	template<> struct XIN_EXPORT TStaticType<UI::UChromeTitle>
	{
		static inline UChromeTitle_Type UChromeTitleType { u8"UChromeTitle"N };

		static FType & Type()
		{
			return UChromeTitleType;
		}
	};

	FType & UI::UChromeTitle::StaticType()
	{
		return TStaticType<UI::UChromeTitle>().Type();
	}

	static TTypeRegister<UI::UChromeTitle> UChromeTitleTypeRegister { TStaticType<UI::UChromeTitle>().Type() };

	class UChrome_Type : public TClass<UI::UChrome>
	{
	public:
		using UChrome = UI::UChrome;

		// --- 0 Constructors
		static inline const TConstructor<UChrome, UI::UChromePanel &> Constructor___UChromePanel_lref { u8"!Constructor___UChromePanel_lref"N, FMetadata::None };
		static inline const TConstructor<UChrome, UI::UChromePanel &, FName> Constructor___UChromePanel_lref__FName { u8"!Constructor___UChromePanel_lref__FName"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor___UChromePanel_lref,
			Constructor___UChromePanel_lref__FName,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<UChrome, FType &()> Method { u8"Type"N, &UChrome::Type, FMetadata::None };
		};
		struct Method___void___OnConstruct
		{
			static inline const TMethod<UChrome, void()> Method { u8"OnConstruct"N, &UChrome::OnConstruct, FMetadata::None };
		};
		struct Method___void___OnInitialize
		{
			static inline const TMethod<UChrome, void()> Method { u8"OnInitialize"N, &UChrome::OnInitialize, FMetadata::None };
		};
		struct Method___TVec2__float64_____OnMeasure__TVec2__float64__
		{
			static inline const TMethod<UChrome, FSize(FSize)> Method { u8"OnMeasure"N, &UChrome::OnMeasure, FMetadata::None };
		};
		struct Method___void___OnArrange
		{
			static inline const TMethod<UChrome, void()> Method { u8"OnArrange"N, &UChrome::OnArrange, FMetadata::None };
		};
		struct Method___void___OnMouseDown__FMouseButtonEventArgs_lref
		{
			static inline const TMethod<UChrome, void(UI::FMouseButtonEventArgs &)> Method { u8"OnMouseDown"N, &UChrome::OnMouseDown, FMetadata::None };
		};
		struct Method___void___OnLocationChanged__TVec2__float64__
		{
			static inline const TMethod<UChrome, void(FPoint)> Method { u8"OnLocationChanged"N, &UChrome::OnLocationChanged, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___void___OnConstruct::Method,
			Method___void___OnInitialize::Method,
			Method___TVec2__float64_____OnMeasure__TVec2__float64__::Method,
			Method___void___OnArrange::Method,
			Method___void___OnMouseDown__FMouseButtonEventArgs_lref::Method,
			Method___void___OnLocationChanged__TVec2__float64__::Method,
		};

		// --- 2 Fields
		struct Field___ChromeDepth
		{
			static inline const TField Field { u8"ChromeDepth"N, &UChrome::ChromeDepth, FMetadata::None };
		};
		struct Field___Closed
		{
			static inline const TField Field { u8"Closed"N, &UChrome::Closed, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FField> ReflectedFields[] =
		{
			Field___ChromeDepth::Field,
			Field___Closed::Field,
		};

		// --- 3 Properties
		struct Property___Title
		{
			static inline const TDependencyProperty Property { u8"Title"N, &UChrome::__Title, &UChrome::__GetTitle, &UChrome::__SetTitleValue, UChrome::__Title__, UChrome::FPropertyMetadata::None };
		};
		struct Property___Location
		{
			static inline const TDependencyProperty Property { u8"Location"N, &UChrome::__Location, &UChrome::__GetLocation, &UChrome::__SetLocationValue, UChrome::__Location__, UChrome::FPropertyMetadata::None };
		};
		static inline const TReferenceWrapper<const FProperty> ReflectedProperties[] =
		{
			Property___Title::Property,
			Property___Location::Property,
		};

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<UI::UContentControl>(),
		};

		// --- 5 Constructor
		UChrome_Type(FName Name = u8"UChrome"N) : TClass(Name, Classof<UI::UContentControl>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Fields = ReflectedFields;
			FClass::Methods = ReflectedMethods;
			FClass::Properties = ReflectedProperties;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	const TProperty<FText> & UI::UChrome::TitleProperty() { return UChrome_Type::Property___Title::Property; }

	const TProperty<FPoint> & UI::UChrome::LocationProperty() { return UChrome_Type::Property___Location::Property; }

	template<> struct XIN_EXPORT TStaticType<UI::UChrome>
	{
		static inline UChrome_Type UChromeType { u8"UChrome"N };

		static FType & Type()
		{
			return UChromeType;
		}
	};

	FType & UI::UChrome::StaticType()
	{
		return TStaticType<UI::UChrome>().Type();
	}

	static TTypeRegister<UI::UChrome> UChromeTypeRegister { TStaticType<UI::UChrome>().Type() };
}

// Generated code end.
