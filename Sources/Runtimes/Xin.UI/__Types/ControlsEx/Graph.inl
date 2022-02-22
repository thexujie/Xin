// E753BAA4C69CDAE8E4A20C6455503881 2025-03-24 18:07:07
// Generated code begin.

#pragma once
#include "PCH.h"
#include "ControlsEx/Graph.h"

namespace Xin
{
	class UGraph_Type : public TClass<UI::UGraph>
	{
	public:
		using UGraph = UI::UGraph;

		// --- 0 Constructors
		static inline const TConstructor<UGraph> Constructor { u8"!Constructor"N, FMetadata::None };
		static inline const TReferenceWrapper<const FConstructor> ReflectedConstructors[] =
		{
			Constructor,
		};

		// --- 1 Methods
		struct Method___FType_lref___Type____const
		{
			static inline const TMethod<UGraph, FType &()> Method { u8"Type"N, &UGraph::Type, FMetadata::None };
		};
		struct Method___void___OnInitialize
		{
			static inline const TMethod<UGraph, void()> Method { u8"OnInitialize"N, &UGraph::OnInitialize, FMetadata::None };
		};
		struct Method___void___OnFinalize
		{
			static inline const TMethod<UGraph, void()> Method { u8"OnFinalize"N, &UGraph::OnFinalize, FMetadata::None };
		};
		struct Method___TVec2__float64_____OnMeasure__TVec2__float64__
		{
			static inline const TMethod<UGraph, FSize(FSize)> Method { u8"OnMeasure"N, &UGraph::OnMeasure, FMetadata::None };
		};
		struct Method___void___OnPaint__IPainter_lref
		{
			static inline const TMethod<UGraph, void(IPainter &)> Method { u8"OnPaint"N, &UGraph::OnPaint, FMetadata::None };
		};
		struct Method___void___LoadSvg__TView__char8__
		{
			static inline const TMethod<UGraph, void(FStringV)> Method { u8"LoadSvg"N, &UGraph::LoadSvg, FMetadata::None };
		};
		static inline const TReferenceWrapper<const FMethod> ReflectedMethods[] =
		{
			Method___FType_lref___Type____const::Method,
			Method___void___OnInitialize::Method,
			Method___void___OnFinalize::Method,
			Method___TVec2__float64_____OnMeasure__TVec2__float64__::Method,
			Method___void___OnPaint__IPainter_lref::Method,
			Method___void___LoadSvg__TView__char8__::Method,
		};

		// --- 2 Fields

		// --- 3 Properties
		struct Property___SvgFilePath
		{
			static inline const TProxyProperty Property { u8"SvgFilePath"N, &UGraph::__SvgFilePath, &UGraph::__GetSvgFilePath, &UGraph::__SetSvgFilePath, UGraph::FPropertyMetadata::None };
		};
		struct Property___SvgSize
		{
			static inline const TProxyProperty Property { u8"SvgSize"N, &UGraph::__SvgSize, &UGraph::__GetSvgSize, &UGraph::__SetSvgSize, UGraph::FPropertyMetadata::None };
		};
		struct Property___SvgBounds
		{
			static inline const TProxyProperty Property { u8"SvgBounds"N, &UGraph::__SvgBounds, &UGraph::__GetSvgBounds, &UGraph::__SetSvgBounds, UGraph::FPropertyMetadata::None };
		};
		struct Property___SvgScale
		{
			static inline const TProxyProperty Property { u8"SvgScale"N, &UGraph::__SvgScale, &UGraph::__GetSvgScale, &UGraph::__SetSvgScale, UGraph::FPropertyMetadata::None };
		};
		static inline const TReferenceWrapper<const FProperty> ReflectedProperties[] =
		{
			Property___SvgFilePath::Property,
			Property___SvgSize::Property,
			Property___SvgBounds::Property,
			Property___SvgScale::Property,
		};

		// --- 4 Base Types
		static inline const TReferenceWrapper<const FClass> ReflectedBaseTypes[] =
		{
			Classof<UI::UElement>(),
		};

		// --- 5 Constructor
		UGraph_Type(FName Name = u8"UGraph"N) : TClass(Name, Classof<UI::UElement>())
		{
			FClass::Constructors = ReflectedConstructors;
			FClass::Methods = ReflectedMethods;
			FClass::Properties = ReflectedProperties;
			FClass::BaseTypes = ReflectedBaseTypes;
		}
	};

	const TProperty<FString> & UI::UGraph::SvgFilePathProperty() { return UGraph_Type::Property___SvgFilePath::Property; }

	const TProperty<FSize> & UI::UGraph::SvgSizeProperty() { return UGraph_Type::Property___SvgSize::Property; }

	const TProperty<Vec4F> & UI::UGraph::SvgBoundsProperty() { return UGraph_Type::Property___SvgBounds::Property; }

	const TProperty<Vec2F> & UI::UGraph::SvgScaleProperty() { return UGraph_Type::Property___SvgScale::Property; }

	template<> struct XIN_EXPORT TStaticType<UI::UGraph>
	{
		static inline UGraph_Type UGraphType { u8"UGraph"N };

		static FType & Type()
		{
			return UGraphType;
		}
	};

	FType & UI::UGraph::StaticType()
	{
		return TStaticType<UI::UGraph>().Type();
	}

	static TTypeRegister<UI::UGraph> UGraphTypeRegister { TStaticType<UI::UGraph>().Type() };
}

// Generated code end.
